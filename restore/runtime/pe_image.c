/* Loader for the exact, fixed-base, user-owned Pharaoh 2.1 image. */
#include "pe_image.h"
#include <bcrypt.h>
#include <stdint.h>
#include <string.h>

static const BYTE expected_sha256[32] = {
    0xfd,0xe7,0xcd,0x65,0x42,0xab,0x00,0xe3,0x95,0x2d,0x03,0x66,0xc0,0xc5,0xc5,0x2d,
    0xde,0x59,0xa7,0x01,0x60,0x28,0x0f,0x17,0x2e,0x24,0xd4,0x14,0x06,0xd3,0x3c,0x8d
};

static int authenticated(const BYTE *data, DWORD size)
{
    BCRYPT_ALG_HANDLE algorithm = NULL;
    BCRYPT_HASH_HANDLE hash = NULL;
    BYTE digest[32];
    int valid=0;
    if (BCryptOpenAlgorithmProvider(&algorithm,BCRYPT_SHA256_ALGORITHM,NULL,0)<0) goto done;
    if (BCryptCreateHash(algorithm,&hash,NULL,0,NULL,0,0)<0) goto done;
    if (BCryptHashData(hash,(PUCHAR)data,size,0)<0) goto done;
    if (BCryptFinishHash(hash,digest,sizeof(digest),0)<0) goto done;
    valid=memcmp(digest,expected_sha256,sizeof(digest))==0;
done:
    if (hash) BCryptDestroyHash(hash);
    if (algorithm) BCryptCloseAlgorithmProvider(algorithm,0);
    return valid;
}

static int within(DWORD offset, SIZE_T bytes, SIZE_T limit)
{
    return offset<=limit && bytes<=limit-offset;
}

static void *rva(RestoreImage *image,DWORD offset,SIZE_T size)
{
    return within(offset,size,image->size) ? image->base+offset : NULL;
}

static const char *image_string(RestoreImage *image,DWORD offset)
{
    const char *text=rva(image,offset,1);
    return text && memchr(text,0,image->size-offset) ? text : NULL;
}

/* Adopt the image mapped by Windows before any CRT or heap allocation. */
int restore_image_adopt(RestoreImage *image,const char *filename,RestoreImportHook hook,FILE *log)
{
    HANDLE file=INVALID_HANDLE_VALUE;
    BYTE *data=NULL;
    DWORD size,received,i;
    IMAGE_DOS_HEADER *dos;
    IMAGE_IMPORT_DESCRIPTOR *imports;
    IMAGE_DATA_DIRECTORY directory;
    int ok=0;
    memset(image,0,sizeof(*image));
    file=CreateFileA(filename,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
    if (file==INVALID_HANDLE_VALUE) goto done;
    size=GetFileSize(file,NULL);
    if (size!=2142208) goto done;
    data=HeapAlloc(GetProcessHeap(),0,size);
    if (!data || !ReadFile(file,data,size,&received,NULL) || received!=size || !authenticated(data,size)) {
        fprintf(log,"Input does not match the canonical executable SHA-256.\n"); goto done;
    }
    image->base=(BYTE *)GetModuleHandleA(NULL);
    dos=(IMAGE_DOS_HEADER *)image->base;
    if (image->base!=(BYTE *)0x400000 || dos->e_magic!=IMAGE_DOS_SIGNATURE) goto done;
    image->nt=(IMAGE_NT_HEADERS32 *)(image->base+dos->e_lfanew);
    if (image->nt->Signature!=IMAGE_NT_SIGNATURE || image->nt->FileHeader.Machine!=IMAGE_FILE_MACHINE_I386) goto done;
    image->size=image->nt->OptionalHeader.SizeOfImage;
    image->entry_rva=0x162fea;
    strcpy_s(image->filename,sizeof(image->filename),filename);
    directory=image->nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
    imports=rva(image,directory.VirtualAddress,directory.Size);
    if (!imports || directory.Size<sizeof(*imports)) goto done;
    for (i=0;i<directory.Size/sizeof(*imports) && imports[i].Name;i++) {
        const char *name=image_string(image,imports[i].Name);
        DWORD index;
        if (!name) goto done;
        fprintf(log,"Native-loaded DLL: %s\n",name);
        image->imported_modules++;
        for (index=0;;index++) {
            DWORD source=imports[i].OriginalFirstThunk;
            IMAGE_THUNK_DATA32 *lookup=rva(image,source+index*sizeof(*lookup),sizeof(*lookup));
            IMAGE_THUNK_DATA32 *target=rva(image,imports[i].FirstThunk+index*sizeof(*target),sizeof(*target));
            FARPROC replacement;
            const char *symbol=NULL;
            DWORD previous,ignored;
            if (!source || !lookup || !target) goto done;
            if (!lookup->u1.AddressOfData) break;
            if (!IMAGE_SNAP_BY_ORDINAL32(lookup->u1.Ordinal)) symbol=image_string(image,lookup->u1.AddressOfData+sizeof(WORD));
            replacement=(FARPROC)(uintptr_t)target->u1.Function;
            if (!replacement) goto done;
            if (hook && symbol) replacement=hook(name,symbol,replacement);
            if ((DWORD)(uintptr_t)replacement!=target->u1.Function) {
                if (!VirtualProtect(target,sizeof(*target),PAGE_READWRITE,&previous)) goto done;
                target->u1.Function=(DWORD)(uintptr_t)replacement;
                if (!VirtualProtect(target,sizeof(*target),previous,&ignored)) goto done;
            }
            image->imported_functions++;
        }
    }
    fprintf(log,"Native image adopted: base=%p original-entry=%08lx DLLs=%lu imports=%lu\n",image->base,
        0x400000+image->entry_rva,image->imported_modules,image->imported_functions);
    ok=1;
done:
    if (!ok) { fprintf(log,"Image adoption failed; Win32 error %lu.\n",GetLastError()); restore_image_close(image); }
    if (data) HeapFree(GetProcessHeap(),0,data);
    if (file!=INVALID_HANDLE_VALUE) CloseHandle(file);
    fflush(log);
    return ok;
}

int restore_image_jump(RestoreImage *image,DWORD original_va,const void *replacement)
{
    BYTE *destination;
    DWORD previous,ignored;
    int32_t delta;
    if (original_va<0x400000 || !(destination=rva(image,original_va-0x400000,5))) return 0;
    delta=(int32_t)((uintptr_t)replacement-(uintptr_t)destination-5);
    if (!VirtualProtect(destination,5,PAGE_EXECUTE_READWRITE,&previous)) return 0;
    destination[0]=0xe9;
    memcpy(destination+1,&delta,sizeof(delta));
    FlushInstructionCache(GetCurrentProcess(),destination,5);
    return VirtualProtect(destination,5,previous,&ignored)!=0;
}

void restore_image_close(RestoreImage *image)
{
    /* The main module belongs to the Windows loader. */
    memset(image,0,sizeof(*image));
}
