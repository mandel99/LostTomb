
Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  00410B10: push        ebp
  00410B11: mov         ebp,esp
  00410B13: mov         eax,dword ptr [ebp+8]
  00410B16: push        ebx
  00410B17: push        esi
  00410B18: push        edi
  00410B19: push        eax
  00410B1A: call        00410F20
  00410B1F: add         esp,4
  00410B22: mov         dword ptr [ebp+8],eax
  00410B25: mov         esi,dword ptr [ebp+8]
  00410B28: mov         edi,dword ptr ds:[00E92E18h]
  00410B2E: mov         eax,dword ptr ds:[00D3B5E0h]
  00410B33: add         eax,eax
  00410B35: add         edi,eax
  00410B37: mov         eax,dword ptr ds:[00D3B5DCh]
  00410B3C: mov         edx,dword ptr ds:[00E38D94h]
  00410B42: mul         eax,edx
  00410B44: add         edi,eax
  00410B46: mov         ax,word ptr ds:[00D3B51Ch]
  00410B4C: shl         eax,10h
  00410B4F: mov         ax,word ptr ds:[00D3B51Ch]
  00410B55: mov         edx,dword ptr ds:[00D3B5D4h]
  00410B5B: movsx       ecx,word ptr ds:[00D3B5BEh]
  00410B62: cmp         ecx,0
  00410B65: jle         00410DC5
  00410B6B: mov         bl,byte ptr [esi]
  00410B6D: cmp         bl,0FFh
  00410B70: je          00410DB2
  00410B76: cmp         bl,1
  00410B79: je          00410C02
  00410B7F: cmp         bl,2
  00410B82: je          00410C13
  00410B88: cmp         bl,3
  00410B8B: je          00410C23
  00410B91: cmp         bl,4
  00410B94: je          00410C37
  00410B9A: cmp         bl,10h
  00410B9D: je          00410D8D
  00410BA3: cmp         bl,5
  00410BA6: je          00410C4A
  00410BAC: cmp         bl,6
  00410BAF: je          00410C61
  00410BB5: cmp         bl,7
  00410BB8: je          00410C77
  00410BBE: cmp         bl,8
  00410BC1: je          00410C91
  00410BC7: cmp         bl,9
  00410BCA: je          00410CAA
  00410BD0: cmp         bl,0Ah
  00410BD3: je          00410CC7
  00410BD9: cmp         bl,0Bh
  00410BDC: je          00410CE3
  00410BE2: cmp         bl,0Ch
  00410BE5: je          00410D03
  00410BEB: cmp         bl,0Dh
  00410BEE: je          00410D22
  00410BF4: cmp         bl,0Eh
  00410BF7: je          00410D45
  00410BFD: jmp         00410D67
  00410C02: mov         word ptr [edi],ax
  00410C05: add         esi,3
  00410C08: add         edi,2
  00410C0B: sub         ecx,1
  00410C0E: jmp         00410B62
  00410C13: mov         dword ptr [edi],eax
  00410C15: add         esi,5
  00410C18: add         edi,4
  00410C1B: sub         ecx,2
  00410C1E: jmp         00410B62
  00410C23: mov         dword ptr [edi],eax
  00410C25: mov         word ptr [edi+4],ax
  00410C29: add         esi,7
  00410C2C: add         edi,6
  00410C2F: sub         ecx,3
  00410C32: jmp         00410B62
  00410C37: mov         dword ptr [edi],eax
  00410C39: mov         dword ptr [edi+4],eax
  00410C3C: add         esi,9
  00410C3F: add         edi,8
  00410C42: sub         ecx,4
  00410C45: jmp         00410B62
  00410C4A: mov         dword ptr [edi],eax
  00410C4C: mov         dword ptr [edi+4],eax
  00410C4F: mov         word ptr [edi+8],ax
  00410C53: add         esi,0Bh
  00410C56: add         edi,0Ah
  00410C59: sub         ecx,5
  00410C5C: jmp         00410B62
  00410C61: mov         dword ptr [edi],eax
  00410C63: mov         dword ptr [edi+4],eax
  00410C66: mov         dword ptr [edi+8],eax
  00410C69: add         esi,0Dh
  00410C6C: add         edi,0Ch
  00410C6F: sub         ecx,6
  00410C72: jmp         00410B62
  00410C77: mov         dword ptr [edi],eax
  00410C79: mov         dword ptr [edi+4],eax
  00410C7C: mov         dword ptr [edi+8],eax
  00410C7F: mov         word ptr [edi+0Ch],ax
  00410C83: add         esi,0Fh
  00410C86: add         edi,0Eh
  00410C89: sub         ecx,7
  00410C8C: jmp         00410B62
  00410C91: mov         dword ptr [edi],eax
  00410C93: mov         dword ptr [edi+4],eax
  00410C96: mov         dword ptr [edi+8],eax
  00410C99: mov         dword ptr [edi+0Ch],eax
  00410C9C: add         esi,11h
  00410C9F: add         edi,10h
  00410CA2: sub         ecx,8
  00410CA5: jmp         00410B62
  00410CAA: mov         dword ptr [edi],eax
  00410CAC: mov         dword ptr [edi+4],eax
  00410CAF: mov         dword ptr [edi+8],eax
  00410CB2: mov         dword ptr [edi+0Ch],eax
  00410CB5: mov         word ptr [edi+10h],ax
  00410CB9: add         esi,13h
  00410CBC: add         edi,12h
  00410CBF: sub         ecx,9
  00410CC2: jmp         00410B62
  00410CC7: mov         dword ptr [edi],eax
  00410CC9: mov         dword ptr [edi+4],eax
  00410CCC: mov         dword ptr [edi+8],eax
  00410CCF: mov         dword ptr [edi+0Ch],eax
  00410CD2: mov         dword ptr [edi+10h],eax
  00410CD5: add         esi,15h
  00410CD8: add         edi,14h
  00410CDB: sub         ecx,0Ah
  00410CDE: jmp         00410B62
  00410CE3: mov         dword ptr [edi],eax
  00410CE5: mov         dword ptr [edi+4],eax
  00410CE8: mov         dword ptr [edi+8],eax
  00410CEB: mov         dword ptr [edi+0Ch],eax
  00410CEE: mov         dword ptr [edi+10h],eax
  00410CF1: mov         word ptr [edi+14h],ax
  00410CF5: add         esi,17h
  00410CF8: add         edi,16h
  00410CFB: sub         ecx,0Bh
  00410CFE: jmp         00410B62
  00410D03: mov         dword ptr [edi],eax
  00410D05: mov         dword ptr [edi+4],eax
  00410D08: mov         dword ptr [edi+8],eax
  00410D0B: mov         dword ptr [edi+0Ch],eax
  00410D0E: mov         dword ptr [edi+10h],eax
  00410D11: mov         dword ptr [edi+14h],eax
  00410D14: add         esi,19h
  00410D17: add         edi,18h
  00410D1A: sub         ecx,0Ch
  00410D1D: jmp         00410B62
  00410D22: mov         dword ptr [edi],eax
  00410D24: mov         dword ptr [edi+4],eax
  00410D27: mov         dword ptr [edi+8],eax
  00410D2A: mov         dword ptr [edi+0Ch],eax
  00410D2D: mov         dword ptr [edi+10h],eax
  00410D30: mov         dword ptr [edi+14h],eax
  00410D33: mov         word ptr [edi+18h],ax
  00410D37: add         esi,1Bh
  00410D3A: add         edi,1Ah
  00410D3D: sub         ecx,0Dh
  00410D40: jmp         00410B62
  00410D45: mov         dword ptr [edi],eax
  00410D47: mov         dword ptr [edi+4],eax
  00410D4A: mov         dword ptr [edi+8],eax
  00410D4D: mov         dword ptr [edi+0Ch],eax
  00410D50: mov         dword ptr [edi+10h],eax
  00410D53: mov         dword ptr [edi+14h],eax
  00410D56: mov         dword ptr [edi+18h],eax
  00410D59: add         esi,1Dh
  00410D5C: add         edi,1Ch
  00410D5F: sub         ecx,0Eh
  00410D62: jmp         00410B62
  00410D67: mov         dword ptr [edi],eax
  00410D69: mov         dword ptr [edi+4],eax
  00410D6C: mov         dword ptr [edi+8],eax
  00410D6F: mov         dword ptr [edi+0Ch],eax
  00410D72: mov         dword ptr [edi+10h],eax
  00410D75: mov         dword ptr [edi+14h],eax
  00410D78: mov         dword ptr [edi+18h],eax
  00410D7B: mov         word ptr [edi+1Ch],ax
  00410D7F: add         esi,1Fh
  00410D82: add         edi,1Eh
  00410D85: sub         ecx,0Fh
  00410D88: jmp         00410B62
  00410D8D: mov         dword ptr [edi],eax
  00410D8F: mov         dword ptr [edi+4],eax
  00410D92: mov         dword ptr [edi+8],eax
  00410D95: mov         dword ptr [edi+0Ch],eax
  00410D98: mov         dword ptr [edi+10h],eax
  00410D9B: mov         dword ptr [edi+14h],eax
  00410D9E: mov         dword ptr [edi+18h],eax
  00410DA1: mov         dword ptr [edi+1Ch],eax
  00410DA4: add         esi,21h
  00410DA7: add         edi,20h
  00410DAA: sub         ecx,10h
  00410DAD: jmp         00410B62
  00410DB2: xor         ebx,ebx
  00410DB4: mov         bl,byte ptr [esi+1]
  00410DB7: add         edi,ebx
  00410DB9: add         edi,ebx
  00410DBB: add         esi,2
  00410DBE: sub         ecx,ebx
  00410DC0: jmp         00410B62
  00410DC5: add         edi,dword ptr ds:[00D3B53Ch]
  00410DCB: dec         edx
  00410DCC: jg          00410B5B
  00410DD2: pop         edi
  00410DD3: pop         esi
  00410DD4: pop         ebx
  00410DD5: pop         ebp
  00410DD6: ret
  00410DD7: nop
  00410DD8: nop
  00410DD9: nop
  00410DDA: nop
  00410DDB: nop
  00410DDC: nop
  00410DDD: nop
  00410DDE: nop
  00410DDF: nop
  00410DE0: push        ebp

  Summary

      16E000 .text
