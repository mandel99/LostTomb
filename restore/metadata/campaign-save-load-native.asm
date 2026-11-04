Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  0041B680: mov         eax,4A9Ch
  0041B685: call        00561F30
  0041B68A: push        ebx
  0041B68B: push        ebp
  0041B68C: mov         ebx,ecx
  0041B68E: push        esi
  0041B68F: push        edi
  0041B690: lea         ecx,[esp+10h]
  0041B694: call        0041AC50
  0041B699: mov         eax,dword ptr [esp+00004AB0h]
  0041B6A0: xor         ebp,ebp
  0041B6A2: push        5763BCh
  0041B6A7: push        eax
  0041B6A8: mov         word ptr [ebx+00004A86h],bp
  0041B6AF: mov         word ptr [ebx+00004A84h],bp
  0041B6B6: mov         word ptr [ebx+00004A8Ah],bp
  0041B6BD: mov         word ptr [ebx+00004A88h],bp
  0041B6C4: mov         word ptr [ebx+00004A8Eh],bp
  0041B6CB: mov         word ptr [ebx+00004A8Ch],bp
  0041B6D2: mov         word ptr [ebx+00004A92h],bp
  0041B6D9: mov         word ptr [ebx+00004A90h],bp
  0041B6E0: mov         word ptr [ebx+00004A96h],bp
  0041B6E7: mov         word ptr [ebx+00004A94h],bp
  0041B6EE: mov         word ptr [ebx+00004A9Ah],bp
  0041B6F5: mov         word ptr [ebx+00004A98h],bp
  0041B6FC: call        00561A0A
  0041B701: mov         esi,eax
  0041B703: add         esp,8
  0041B706: cmp         esi,ebp
  0041B708: je          0041B889
  0041B70E: push        esi
  0041B70F: push        1
  0041B711: lea         ecx,[esp+18h]
  0041B715: push        4A9Ch
  0041B71A: push        ecx
  0041B71B: call        00561CA1
  0041B720: mov         edi,eax
  0041B722: add         esp,10h
  0041B725: cmp         edi,ebp
  0041B727: jne         0041B74D
  0041B729: push        ebp
  0041B72A: push        ebp
  0041B72B: push        esi
  0041B72C: call        00561EA4
  0041B731: push        esi
  0041B732: push        1
  0041B734: lea         edx,[esp+24h]
  0041B738: push        4A6Ch
  0041B73D: push        edx
  0041B73E: call        00561CA1
  0041B743: add         esp,1Ch
  0041B746: mov         edi,eax
  0041B748: jmp         0041B801
  0041B74D: mov         ax,word ptr [esp+00004A94h]
  0041B755: mov         cx,word ptr [esp+00004A96h]
  0041B75D: mov         dx,word ptr [esp+00004A98h]
  0041B765: mov         word ptr [ebx+00004A84h],ax
  0041B76C: mov         ax,word ptr [esp+00004A9Ah]
  0041B774: mov         word ptr [ebx+00004A86h],cx
  0041B77B: mov         cx,word ptr [esp+00004A9Ch]
  0041B783: mov         word ptr [ebx+00004A88h],dx
  0041B78A: mov         dx,word ptr [esp+00004A9Eh]
  0041B792: mov         word ptr [ebx+00004A8Ah],ax
  0041B799: mov         ax,word ptr [esp+00004AA0h]
  0041B7A1: mov         word ptr [ebx+00004A8Ch],cx
  0041B7A8: mov         cx,word ptr [esp+00004AA2h]
  0041B7B0: mov         word ptr [ebx+00004A8Eh],dx
  0041B7B7: mov         dx,word ptr [esp+00004AA4h]
  0041B7BF: mov         word ptr [ebx+00004A90h],ax
  0041B7C6: mov         ax,word ptr [esp+00004AA6h]
  0041B7CE: mov         word ptr [ebx+00004A92h],cx
  0041B7D5: mov         cx,word ptr [esp+00004AA8h]
  0041B7DD: mov         word ptr [ebx+00004A94h],dx
  0041B7E4: mov         dx,word ptr [esp+00004AAAh]
  0041B7EC: mov         word ptr [ebx+00004A96h],ax
  0041B7F3: mov         word ptr [ebx+00004A98h],cx
  0041B7FA: mov         word ptr [ebx+00004A9Ah],dx
  0041B801: push        esi
  0041B802: call        00561994
  0041B807: add         esp,4
  0041B80A: cmp         edi,1
  0041B80D: jne         0041B889
  0041B80F: lea         edi,[esp+00002CA0h]
  0041B816: or          ecx,0FFFFFFFFh
  0041B819: xor         eax,eax
  0041B81B: lea         edx,[ebx+00002C90h]
  0041B821: repne scas  byte ptr es:[edi]
  0041B823: not         ecx
  0041B825: sub         edi,ecx
  0041B827: mov         eax,ecx
  0041B829: mov         esi,edi
  0041B82B: mov         edi,edx
  0041B82D: shr         ecx,2
  0041B830: rep movs    dword ptr es:[edi],dword ptr [esi]
  0041B832: mov         ecx,eax
  0041B834: and         ecx,3
  0041B837: rep movs    byte ptr es:[edi],byte ptr [esi]
  0041B839: mov         ecx,76Ch
  0041B83E: lea         esi,[esp+00002CE4h]
  0041B845: lea         edi,[ebx+00002CD4h]
  0041B84B: rep movs    dword ptr es:[edi],dword ptr [esi]
  0041B84D: mov         eax,dword ptr [ebx+00002C8Ch]
  0041B853: xor         edi,edi
  0041B855: cmp         eax,ebp
  0041B857: jle         0041B87A
  0041B859: lea         esi,[ebx+3Ch]
  0041B85C: lea         eax,[ebx+00002D1Ch]
  0041B862: cmp         dword ptr [eax],ebp
  0041B864: setne       cl
  0041B867: mov         byte ptr [esi],cl
  0041B869: mov         ecx,dword ptr [ebx+00002C8Ch]
  0041B86F: inc         edi
  0041B870: add         eax,4Ch
  0041B873: add         esi,40h
  0041B876: cmp         edi,ecx
  0041B878: jl          0041B862
  0041B87A: pop         edi
  0041B87B: pop         esi
  0041B87C: pop         ebp
  0041B87D: mov         al,1
  0041B87F: pop         ebx
  0041B880: add         esp,4A9Ch
  0041B886: ret         4
  0041B889: pop         edi
  0041B88A: pop         esi
  0041B88B: pop         ebp
  0041B88C: xor         al,al
  0041B88E: pop         ebx
  0041B88F: add         esp,4A9Ch
  0041B895: ret         4
  0041B898: nop
  0041B899: nop
  0041B89A: nop
  0041B89B: nop
  0041B89C: nop
  0041B89D: nop
  0041B89E: nop
  0041B89F: nop
  0041B8A0: 8B

  Summary

      16E000 .text
