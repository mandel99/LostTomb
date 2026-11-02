"""Exercise actual compiled media loader against controlled Windows APIs.

The callback, path handling and directory registration execute in Unicorn.
No game or DLL is launched. This checks the C support runtime, not equivalence
to original engine routines. DLL resolution retains its native binding layer.
"""
from verify_options_startup import *

c,u,state=machines[1]
u.mem_map(0x2fe00000,0x200000)
MODULE=S+0x2000

def wide(cpu,p):
    out=bytearray()
    for i in range(32768):
        b=bytes(cpu.mem_read(p+i*2,2))
        if b==b'\0\0':return out.decode('utf-16le')
        out.extend(b)
    raise AssertionError('unterminated wide path')

def execute(name,args=(),cleanup=0):
    return run(0,u,sym(name),args,cleanup=cleanup)

def register(directory):
    execute('restore_media_set_game_directory',[0])
    if directory is None:return 1
    u.mem_write(S+0x10000,directory.encode('utf-16le')+b'\0\0')
    return execute('restore_media_set_game_directory',[S+0x10000])

def case(dll='mss32.dll',exe=r'C:\build\Preview\PharaohRestoreC.exe',
         directory=r'D:\Game',success='game',error=126):
    reset(u,state);calls=[]
    assert register(directory)==1
    u.mem_write(S+0x400,dll.encode()+b'\0')
    u.mem_write(S+0x800,dll.encode('utf-16le')+b'\0\0')
    u.mem_write(MODULE,struct.pack('<5I',S+0x400,S+0x800,0,0,0))
    primary=exe[:exe.rfind('\\')+1]+dll if isinstance(exe,str) and '\\' in exe else None
    fallback=directory.rstrip('\\/')+'\\'+dll if directory else None
    if directory and directory.endswith('/'):fallback=directory+dll
    def module_filename(cpu,args):
        handle,p,capacity=args;assert handle==0 and capacity==32768
        if exe is None:return 0
        if exe=='<truncated>':return 32768
        data=exe.encode('utf-16le');cpu.mem_write(p,data+b'\0\0');return len(data)//2
    def load(cpu,args):
        p,handle,flags=args;path=wide(cpu,p)
        assert handle==0 and flags==0x900
        calls.append(path)
        return 0x12345000 if path==(primary if success=='exe' else fallback if success=='game' else None) else 0
    for i,(name,count,cleanup,fn) in enumerate([
        ('_imp__GetModuleFileNameW@12',3,12,module_filename),
        ('_imp__LoadLibraryExW@12',3,12,load),
        ('_imp__GetLastError@0',0,0,lambda cpu,args:error)]):
        put(u,sym(name),S+0x49000+i*16);state['hooks'][S+0x49000+i*16]=(count,cleanup,fn)
    assert execute('load_module@12',[0,MODULE,0],12)==1
    expected=[]
    if primary and exe!='<truncated>':expected.append(primary)
    if success!='exe' and fallback:expected.append(fallback)
    assert calls==expected,(exe,directory,success,dll,calls,expected)
    handle=get(u,MODULE+12);last=get(u,MODULE+16)
    succeeded=success=='exe' and primary is not None or success=='game' and fallback is not None
    assert handle==(0x12345000 if succeeded else 0)
    if succeeded:assert last==0
    elif calls:assert last==error

def cases():
    n=0
    for dll in ['mss32.dll','binkw32.dll','smackw32.dll']:
        for directory in [None,r'D:\Game',r'D:\Game'+chr(92),'D:/Game/',r'D:\Hra Život']:
            for success in ['exe','game','none']:
                case(dll=dll,directory=directory,success=success);n+=1
        for exe in [None,'<truncated>','PharaohRestoreC.exe',r'\\server\share\Preview\PharaohRestoreC.exe']:
            case(dll=dll,exe=exe);n+=1
        for error in [126,193,5,127]:case(dll=dll,success='none',error=error);n+=1
    for directory in ['',r'Game',r'..\Game','C:Game','C:/'+'X'*32760]:
        reset(u,state);assert register(directory)==0;n+=1
    print('PASS compiled media loader directory selection, DLL precedence, Unicode and error paths:',n,flush=True)
    print('target SHA256',target_sha,flush=True)

if __name__=='__main__':cases()
