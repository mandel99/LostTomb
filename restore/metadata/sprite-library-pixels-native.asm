
Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  0053F660: sub         esp,104h
  0053F666: mov         eax,dword ptr ds:[006018F8h]
  0053F66B: mov         cx,word ptr ds:[006018FCh]
  0053F672: push        ebx
  0053F673: push        esi
  0053F674: push        edi
  0053F675: mov         dword ptr [esp+0Ch],eax
  0053F679: mov         word ptr [esp+10h],cx
  0053F67E: mov         ecx,3Fh
  0053F683: xor         eax,eax
  0053F685: lea         edi,[esp+12h]
  0053F689: rep stos    dword ptr es:[edi]
  0053F68B: stos        word ptr es:[edi]
  0053F68D: mov         edi,dword ptr [esp+00000114h]
  0053F694: or          ecx,0FFFFFFFFh
  0053F697: xor         eax,eax
  0053F699: lea         edx,[esp+0Ch]
  0053F69D: repne scas  byte ptr es:[edi]
  0053F69F: not         ecx
  0053F6A1: sub         edi,ecx
  0053F6A3: push        eax
  0053F6A4: mov         esi,edi
  0053F6A6: mov         ebx,ecx
  0053F6A8: mov         edi,edx
  0053F6AA: or          ecx,0FFFFFFFFh
  0053F6AD: repne scas  byte ptr es:[edi]
  0053F6AF: mov         ecx,ebx
  0053F6B1: dec         edi
  0053F6B2: shr         ecx,2
  0053F6B5: rep movs    dword ptr es:[edi],dword ptr [esi]
  0053F6B7: mov         ecx,ebx
  0053F6B9: lea         edx,[esp+10h]
  0053F6BD: and         ecx,3
  0053F6C0: rep movs    byte ptr es:[edi],byte ptr [esi]
  0053F6C2: mov         edi,601900h
  0053F6C7: or          ecx,0FFFFFFFFh
  0053F6CA: repne scas  byte ptr es:[edi]
  0053F6CC: not         ecx
  0053F6CE: sub         edi,ecx
  0053F6D0: mov         esi,edi
  0053F6D2: mov         ebx,ecx
  0053F6D4: mov         edi,edx
  0053F6D6: or          ecx,0FFFFFFFFh
  0053F6D9: repne scas  byte ptr es:[edi]
  0053F6DB: mov         ecx,ebx
  0053F6DD: dec         edi
  0053F6DE: shr         ecx,2
  0053F6E1: rep movs    dword ptr es:[edi],dword ptr [esi]
  0053F6E3: mov         ecx,ebx
  0053F6E5: and         ecx,3
  0053F6E8: rep movs    byte ptr es:[edi],byte ptr [esi]
  0053F6EA: mov         edi,dword ptr [esp+0000011Ch]
  0053F6F1: push        edi
  0053F6F2: call        0053F790
  0053F6F7: mov         esi,dword ptr [esp+00000128h]
  0053F6FE: mov         ebx,dword ptr [esp+00000134h]
  0053F705: add         esp,8
  0053F708: test        esi,esi
  0053F70A: jne         0053F71C
  0053F70C: lea         eax,[esp+0Ch]
  0053F710: push        ebx
  0053F711: push        eax
  0053F712: call        004CEA30
  0053F717: add         esp,8
  0053F71A: mov         esi,eax
  0053F71C: test        esi,esi
  0053F71E: jle         0053F72B
  0053F720: push        esi
  0053F721: call        00561A1D
  0053F726: add         esp,4
  0053F729: mov         dword ptr [edi],eax
  0053F72B: mov         edi,dword ptr [edi]
  0053F72D: test        esi,esi
  0053F72F: jle         0053F783
  0053F731: mov         ecx,dword ptr [esp+00000128h]
  0053F738: push        ebx
  0053F739: lea         eax,[esp+10h]
  0053F73D: lea         edx,[edi+ecx]
  0053F740: push        edx
  0053F741: push        eax
  0053F742: call        004CEB60
  0053F747: add         esp,0Ch
  0053F74A: cmp         eax,1
  0053F74D: jne         0053F783
  0053F74F: mov         al,byte ptr [esp+0000011Ch]
  0053F756: test        al,al
  0053F758: je          0053F783
  0053F75A: mov         al,byte ptr [esp+00000124h]
  0053F761: push        esi
  0053F762: test        al,al
  0053F764: push        edi
  0053F765: je          0053F77B
  0053F767: call        004CDEB0
  0053F76C: add         esp,8
  0053F76F: mov         eax,esi
  0053F771: pop         edi
  0053F772: pop         esi
  0053F773: pop         ebx
  0053F774: add         esp,104h
  0053F77A: ret
  0053F77B: call        004CDF20
  0053F780: add         esp,8
  0053F783: mov         eax,esi
  0053F785: pop         edi
  0053F786: pop         esi
  0053F787: pop         ebx
  0053F788: add         esp,104h
  0053F78E: ret
  0053F78F: nop
  0053F790: push        esi

  Summary

      16E000 .text
