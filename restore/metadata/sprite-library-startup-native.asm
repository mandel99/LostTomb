
Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  00414420: sub         esp,78h
  00414423: push        esi
  00414424: push        0
  00414426: push        0BCA2B8h
  0041442B: push        57924Ch
  00414430: call        004CEB60
  00414435: mov         esi,dword ptr ds:[0056F078h]
  0041443B: add         esp,0Ch
  0041443E: mov         dword ptr ds:[00E38E94h],10h
  00414448: mov         dword ptr ds:[005E06D0h],1
  00414452: push        0
  00414454: push        579244h
  00414459: call        esi
  0041445B: push        0
  0041445D: push        57923Ch
  00414462: call        esi
  00414464: mov         eax,dword ptr ds:[00E38DACh]
  00414469: pop         esi
  0041446A: imul        eax,dword ptr ds:[00E38DB0h]
  00414471: shl         eax,1
  00414473: cmp         eax,124F80h
  00414478: jg          0041447F
  0041447A: mov         eax,124F80h
  0041447F: push        eax
  00414480: call        00561B68
  00414485: mov         dword ptr ds:[00E92E1Ch],eax
  0041448A: mov         eax,57922Ch
  0041448F: add         esp,4
  00414492: mov         dword ptr [esp+50h],eax
  00414496: mov         dword ptr [esp+54h],eax
  0041449A: mov         dword ptr [esp+58h],eax
  0041449E: lea         eax,[esp]
  004144A2: mov         ecx,0F55BB0h
  004144A7: push        eax
  004144A8: mov         dword ptr [esp+4],579218h
  004144B0: mov         dword ptr [esp+8],579210h
  004144B8: mov         dword ptr [esp+0Ch],579208h
  004144C0: mov         dword ptr [esp+10h],5791F8h
  004144C8: mov         dword ptr [esp+14h],5791E8h
  004144D0: mov         dword ptr [esp+18h],5791DCh
  004144D8: mov         dword ptr [esp+1Ch],5791D0h
  004144E0: mov         dword ptr [esp+20h],5791C4h
  004144E8: mov         dword ptr [esp+24h],5791B8h
  004144F0: mov         dword ptr [esp+28h],5791A8h
  004144F8: mov         dword ptr [esp+2Ch],57919Ch
  00414500: mov         dword ptr [esp+30h],579194h
  00414508: mov         dword ptr [esp+34h],579188h
  00414510: mov         dword ptr [esp+38h],579178h
  00414518: mov         dword ptr [esp+3Ch],57916Ch
  00414520: mov         dword ptr [esp+40h],579160h
  00414528: mov         dword ptr [esp+44h],579154h
  00414530: mov         dword ptr [esp+48h],579144h
  00414538: mov         dword ptr [esp+4Ch],579134h
  00414540: mov         dword ptr [esp+50h],579124h
  00414548: mov         dword ptr [esp+60h],576284h
  00414550: mov         dword ptr [esp+64h],579110h
  00414558: mov         dword ptr [esp+68h],579100h
  00414560: mov         dword ptr [esp+6Ch],5790F0h
  00414568: mov         dword ptr [esp+70h],5790E0h
  00414570: mov         dword ptr [esp+74h],5790D0h
  00414578: mov         dword ptr [esp+78h],5790C0h
  00414580: call        0053DD10
  00414585: push        0
  00414587: call        00520B20
  0041458C: push        7
  0041458E: push        0Bh
  00414590: call        004CC110
  00414595: mov         ecx,dword ptr ds:[00E20768h]
  0041459B: push        ecx
  0041459C: call        004CF7B0
  004145A1: push        1
  004145A3: call        004D0280
  004145A8: mov         edx,dword ptr ds:[00E20768h]
  004145AE: push        edx
  004145AF: call        004CF7B0
  004145B4: mov         eax,dword ptr ds:[00E92E1Ch]
  004145B9: add         esp,18h
  004145BC: test        eax,eax
  004145BE: jne         004145C9
  004145C0: mov         eax,1
  004145C5: add         esp,78h
  004145C8: ret
  004145C9: lea         eax,[esp]
  004145CD: mov         ecx,0F55BB0h
  004145D2: push        eax
  004145D3: call        0053DD10
  004145D8: push        1
  004145DA: call        00520B20
  004145DF: push        8
  004145E1: push        0Bh
  004145E3: call        004CC110
  004145E8: mov         ecx,dword ptr ds:[00E20768h]
  004145EE: push        ecx
  004145EF: call        004CF7B0
  004145F4: push        1
  004145F6: call        004D0280
  004145FB: mov         edx,dword ptr ds:[00E20768h]
  00414601: push        edx
  00414602: call        004CF7B0
  00414607: add         esp,18h
  0041460A: lea         eax,[esp]
  0041460E: mov         ecx,0F55BB0h
  00414613: push        eax
  00414614: call        0053DD10
  00414619: push        2
  0041461B: call        00520B20
  00414620: push        9
  00414622: push        0Bh
  00414624: call        004CC110
  00414629: mov         ecx,dword ptr ds:[00E20768h]
  0041462F: push        ecx
  00414630: call        004CF7B0
  00414635: push        1
  00414637: call        004D0280
  0041463C: mov         edx,dword ptr ds:[00E20768h]
  00414642: push        edx
  00414643: call        004CF7B0
  00414648: add         esp,18h
  0041464B: lea         eax,[esp]
  0041464F: mov         ecx,0F55BB0h
  00414654: push        eax
  00414655: call        0053DD10
  0041465A: lea         ecx,[esp]
  0041465E: push        ecx
  0041465F: mov         ecx,0F55BB0h
  00414664: call        0053DD10
  00414669: lea         edx,[esp]
  0041466D: mov         ecx,0F55BB0h
  00414672: push        edx
  00414673: call        0053DD10
  00414678: lea         eax,[esp]
  0041467C: mov         ecx,0F55BB0h
  00414681: push        eax
  00414682: call        0053DD10
  00414687: lea         ecx,[esp]
  0041468B: push        ecx
  0041468C: mov         ecx,0F55BB0h
  00414691: call        0053DD10
  00414696: push        3
  00414698: call        00520B20
  0041469D: push        0Ah
  0041469F: push        0Bh
  004146A1: call        004CC110
  004146A6: mov         edx,dword ptr ds:[00E20768h]
  004146AC: push        edx
  004146AD: call        004CF7B0
  004146B2: push        1
  004146B4: call        004D0280
  004146B9: mov         eax,dword ptr ds:[00E20768h]
  004146BE: push        eax
  004146BF: call        004CF7B0
  004146C4: add         esp,18h
  004146C7: lea         ecx,[esp]
  004146CB: push        ecx
  004146CC: mov         ecx,0F55BB0h
  004146D1: call        0053DD40
  004146D6: push        3
  004146D8: call        00520B20
  004146DD: push        0Bh
  004146DF: push        0Bh
  004146E1: call        004CC110
  004146E6: mov         edx,dword ptr ds:[00E20768h]
  004146EC: push        edx
  004146ED: call        004CF7B0
  004146F2: push        1
  004146F4: call        004D0280
  004146F9: mov         eax,dword ptr ds:[00E20768h]
  004146FE: push        eax
  004146FF: call        004CF7B0
  00414704: call        00520AF0
  00414709: push        1
  0041470B: call        004D0280
  00414710: push        0
  00414712: push        0
  00414714: push        5790A4h
  00414719: call        004CE6E0
  0041471E: add         esp,28h
  00414721: xor         eax,eax
  00414723: add         esp,78h
  00414726: ret
  00414727: nop
  00414728: nop
  00414729: nop
  0041472A: nop
  0041472B: nop
  0041472C: nop
  0041472D: nop
  0041472E: nop
  0041472F: nop
  00414730: mov         al,byte ptr [esp+4]
  00414734: mov         byte ptr ds:[00784CB0h],al
  00414739: call        00486E70
  0041473E: call        004DEA50
  00414743: xor         eax,eax
  00414745: ret
  00414746: nop
  00414747: nop
  00414748: nop
  00414749: nop
  0041474A: nop
  0041474B: nop
  0041474C: nop
  0041474D: nop
  0041474E: nop
  0041474F: nop
  00414750: 68

  Summary

      16E000 .text
