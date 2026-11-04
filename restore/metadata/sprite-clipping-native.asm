
Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  004CDAD0: mov         cx,word ptr ds:[00D3B5BEh]
  004CDAD7: push        esi
  004CDAD8: movsx       eax,cx
  004CDADB: push        edi
  004CDADC: xor         esi,esi
  004CDADE: mov         edi,eax
  004CDAE0: mov         byte ptr ds:[00D3B531h],0
  004CDAE7: test        cx,cx
  004CDAEA: mov         dword ptr ds:[00D3B54Ch],esi
  004CDAF0: mov         dword ptr ds:[00D3B548h],edi
  004CDAF6: jle         004CDB6F
  004CDAF8: mov         ecx,dword ptr ds:[00D3B5E0h]
  004CDAFE: mov         edx,dword ptr ds:[00D3B55Ch]
  004CDB04: cmp         ecx,edx
  004CDB06: jge         004CDB48
  004CDB08: lea         esi,[eax+ecx]
  004CDB0B: cmp         esi,edx
  004CDB0D: jle         004CDB6F
  004CDB0F: mov         esi,edx
  004CDB11: mov         byte ptr ds:[00D3B531h],1
  004CDB18: sub         esi,ecx
  004CDB1A: mov         dword ptr ds:[00D3B5E0h],edx
  004CDB20: mov         dword ptr ds:[00D3B54Ch],esi
  004CDB26: sub         edi,esi
  004CDB28: sub         eax,edi
  004CDB2A: mov         dword ptr ds:[00D3B544h],edi
  004CDB30: shl         eax,1
  004CDB32: mov         dword ptr ds:[00D3B540h],eax
  004CDB37: mov         eax,dword ptr ds:[00E38D9Ch]
  004CDB3C: sub         eax,edi
  004CDB3E: pop         edi
  004CDB3F: shl         eax,1
  004CDB41: mov         dword ptr ds:[00D3B53Ch],eax
  004CDB46: pop         esi
  004CDB47: ret
  004CDB48: mov         edx,dword ptr ds:[00D3B558h]
  004CDB4E: push        ebx
  004CDB4F: mov         ebx,edx
  004CDB51: sub         ebx,eax
  004CDB53: cmp         ecx,ebx
  004CDB55: pop         ebx
  004CDB56: jle         004CDB26
  004CDB58: cmp         ecx,edx
  004CDB5A: jge         004CDB6F
  004CDB5C: sub         edx,ecx
  004CDB5E: mov         byte ptr ds:[00D3B531h],2
  004CDB65: mov         edi,edx
  004CDB67: mov         dword ptr ds:[00D3B548h],edi
  004CDB6D: jmp         004CDB26
  004CDB6F: mov         byte ptr ds:[00D3B531h],5
  004CDB76: xor         edi,edi
  004CDB78: jmp         004CDB28
  004CDB7A: nop
  004CDB7B: nop
  004CDB7C: nop
  004CDB7D: nop
  004CDB7E: nop
  004CDB7F: nop
  004CDB80: mov         cx,word ptr ds:[00D3B5BCh]
  004CDB87: xor         eax,eax
  004CDB89: movsx       edx,cx
  004CDB8C: push        ebx
  004CDB8D: push        esi
  004CDB8E: cmp         cx,ax
  004CDB91: push        edi
  004CDB92: mov         byte ptr ds:[00D3B530h],al
  004CDB97: mov         dword ptr ds:[00D3B538h],eax
  004CDB9C: mov         dword ptr ds:[00D3B534h],edx
  004CDBA2: mov         bl,5
  004CDBA4: jle         004CDC21
  004CDBA6: mov         esi,dword ptr ds:[00D3B5DCh]
  004CDBAC: mov         ecx,dword ptr ds:[00D3B554h]
  004CDBB2: cmp         esi,ecx
  004CDBB4: jge         004CDC04
  004CDBB6: lea         edi,[edx+esi]
  004CDBB9: cmp         edi,ecx
  004CDBBB: jle         004CDC21
  004CDBBD: mov         eax,ecx
  004CDBBF: mov         dword ptr ds:[00D3B5DCh],ecx
  004CDBC5: sub         eax,esi
  004CDBC7: mov         esi,dword ptr ds:[00D3B550h]
  004CDBCD: sub         esi,ecx
  004CDBCF: mov         byte ptr ds:[00D3B530h],3
  004CDBD6: mov         dword ptr ds:[00D3B538h],eax
  004CDBDB: lea         ecx,[esi+eax]
  004CDBDE: cmp         edx,ecx
  004CDBE0: jle         004CDBEA
  004CDBE2: mov         edx,ecx
  004CDBE4: mov         dword ptr ds:[00D3B534h],edx
  004CDBEA: sub         edx,eax
  004CDBEC: mov         dword ptr ds:[00D3B5D4h],edx
  004CDBF2: cmp         byte ptr ds:[00D3B531h],bl
  004CDBF8: jne         004CDC00
  004CDBFA: mov         byte ptr ds:[00D3B530h],bl
  004CDC00: pop         edi
  004CDC01: pop         esi
  004CDC02: pop         ebx
  004CDC03: ret
  004CDC04: mov         ecx,dword ptr ds:[00D3B550h]
  004CDC0A: mov         edi,ecx
  004CDC0C: sub         edi,edx
  004CDC0E: cmp         esi,edi
  004CDC10: jle         004CDBEA
  004CDC12: cmp         esi,ecx
  004CDC14: jge         004CDC21
  004CDC16: mov         byte ptr ds:[00D3B530h],4
  004CDC1D: sub         ecx,esi
  004CDC1F: jmp         004CDBE2
  004CDC21: mov         byte ptr ds:[00D3B530h],bl
  004CDC27: mov         dword ptr ds:[00D3B5D4h],eax
  004CDC2C: jmp         004CDBF2
  004CDC2E: nop
  004CDC2F: nop
  004CDC30: mov         eax,dword ptr [esp+4]
  004CDC34: mov         ecx,dword ptr ds:[00D3B55Ch]
  004CDC3A: cmp         eax,ecx
  004CDC3C: jge         004CDC41
  004CDC3E: xor         eax,eax
  004CDC40: ret
  004CDC41: cmp         eax,dword ptr ds:[00D3B558h]
  004CDC47: jle         004CDC4C
  004CDC49: xor         eax,eax
  004CDC4B: ret
  004CDC4C: mov         eax,dword ptr [esp+8]
  004CDC50: mov         ecx,dword ptr ds:[00D3B554h]
  004CDC56: cmp         eax,ecx
  004CDC58: jge         004CDC5D
  004CDC5A: xor         eax,eax
  004CDC5C: ret
  004CDC5D: mov         edx,dword ptr ds:[00D3B550h]
  004CDC63: xor         ecx,ecx
  004CDC65: cmp         eax,edx
  004CDC67: setle       cl
  004CDC6A: mov         eax,ecx
  004CDC6C: ret
  004CDC6D: nop
  004CDC6E: nop
  004CDC6F: nop
  004CDC70: mov         eax,dword ptr ds:[00D3B430h]
  004CDC75: xor         ecx,ecx
  004CDC77: inc         eax
  004CDC78: cmp         eax,4
  004CDC7B: mov         dword ptr ds:[00D3B430h],eax
  004CDC80: jl          004CDC88
  004CDC82: mov         dword ptr ds:[00D3B430h],ecx
  004CDC88: mov         eax,dword ptr ds:[00D3B42Ch]
  004CDC8D: inc         eax
  004CDC8E: cmp         eax,8
  004CDC91: mov         dword ptr ds:[00D3B42Ch],eax
  004CDC96: jl          004CDC9E
  004CDC98: mov         dword ptr ds:[00D3B42Ch],ecx
  004CDC9E: mov         eax,dword ptr ds:[00D3B428h]
  004CDCA3: inc         eax
  004CDCA4: cmp         eax,10h
  004CDCA7: mov         dword ptr ds:[00D3B428h],eax
  004CDCAC: jl          004CDCB4
  004CDCAE: mov         dword ptr ds:[00D3B428h],ecx
  004CDCB4: mov         eax,dword ptr ds:[00D3B424h]
  004CDCB9: inc         eax
  004CDCBA: cmp         eax,18h
  004CDCBD: mov         dword ptr ds:[00D3B424h],eax
  004CDCC2: jl          004CDCCA
  004CDCC4: mov         dword ptr ds:[00D3B424h],ecx
  004CDCCA: mov         eax,dword ptr ds:[00D3B420h]
  004CDCCF: inc         eax
  004CDCD0: cmp         eax,20h
  004CDCD3: mov         dword ptr ds:[00D3B420h],eax
  004CDCD8: jl          004CDCE0
  004CDCDA: mov         dword ptr ds:[00D3B420h],ecx
  004CDCE0: mov         eax,dword ptr ds:[00D3B41Ch]
  004CDCE5: inc         eax
  004CDCE6: cmp         eax,40h
  004CDCE9: mov         dword ptr ds:[00D3B41Ch],eax
  004CDCEE: jl          004CDCF6
  004CDCF0: mov         dword ptr ds:[00D3B41Ch],ecx
  004CDCF6: mov         eax,dword ptr ds:[00D3B418h]
  004CDCFB: inc         eax
  004CDCFC: cmp         eax,80h

  Summary

      16E000 .text
