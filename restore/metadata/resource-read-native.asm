
Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  004CE9E0: push        esi
  004CE9E1: mov         esi,dword ptr ds:[00E20768h]
  004CE9E7: push        edi
  004CE9E8: push        89h
  004CE9ED: push        5
  004CE9EF: call        004CC110
  004CE9F4: mov         edi,dword ptr ds:[00E20768h]
  004CE9FA: push        8Ah
  004CE9FF: push        5
  004CEA01: call        004CC110
  004CEA06: mov         eax,dword ptr ds:[00E20768h]
  004CEA0B: add         esp,10h
  004CEA0E: mov         dword ptr ds:[00E20768h],esi
  004CEA14: push        40001h
  004CEA19: push        edi
  004CEA1A: push        eax
  004CEA1B: push        0
  004CEA1D: call        dword ptr ds:[0056F1DCh]
  004CEA23: pop         edi
  004CEA24: pop         esi
  004CEA25: ret
  004CEA26: nop
  004CEA27: nop
  004CEA28: nop
  004CEA29: nop
  004CEA2A: nop
  004CEA2B: nop
  004CEA2C: nop
  004CEA2D: nop
  004CEA2E: nop
  004CEA2F: nop
  004CEA30: push        ebx
  004CEA31: push        esi
  004CEA32: push        edi
  004CEA33: mov         edi,dword ptr [esp+10h]
  004CEA37: push        8000h
  004CEA3C: push        edi
  004CEA3D: mov         ebx,1
  004CEA42: call        00568EAB
  004CEA47: mov         esi,eax
  004CEA49: add         esp,8
  004CEA4C: test        esi,esi
  004CEA4E: jl          004CEA6D
  004CEA50: push        esi
  004CEA51: call        0056B1B6
  004CEA56: mov         edx,dword ptr [esp+18h]
  004CEA5A: mov         edi,eax
  004CEA5C: push        esi
  004CEA5D: sub         edi,edx
  004CEA5F: call        005647EB
  004CEA64: add         esp,8
  004CEA67: mov         eax,edi
  004CEA69: pop         edi
  004CEA6A: pop         esi
  004CEA6B: pop         ebx
  004CEA6C: ret
  004CEA6D: mov         al,byte ptr ds:[00E38E62h]
  004CEA72: or          ecx,0FFFFFFFFh
  004CEA75: mov         byte ptr ds:[005E05ACh],al
  004CEA7A: xor         eax,eax
  004CEA7C: mov         byte ptr ds:[005E05AFh],0
  004CEA83: push        8000h
  004CEA88: repne scas  byte ptr es:[edi]
  004CEA8A: not         ecx
  004CEA8C: sub         edi,ecx
  004CEA8E: push        5E05ACh
  004CEA93: mov         esi,edi
  004CEA95: mov         edx,ecx
  004CEA97: mov         edi,5E05ACh
  004CEA9C: or          ecx,0FFFFFFFFh
  004CEA9F: repne scas  byte ptr es:[edi]
  004CEAA1: mov         ecx,edx
  004CEAA3: dec         edi
  004CEAA4: shr         ecx,2
  004CEAA7: rep movs    dword ptr es:[edi],dword ptr [esi]
  004CEAA9: mov         ecx,edx
  004CEAAB: and         ecx,3
  004CEAAE: rep movs    byte ptr es:[edi],byte ptr [esi]
  004CEAB0: call        00568EAB
  004CEAB5: mov         esi,eax
  004CEAB7: add         esp,8
  004CEABA: test        esi,esi
  004CEABC: jge         004CEAE8
  004CEABE: cmp         ebx,1
  004CEAC1: jne         004CEAE4
  004CEAC3: call        004CE9E0
  004CEAC8: push        8000h
  004CEACD: push        5E05ACh
  004CEAD2: mov         ebx,eax
  004CEAD4: call        00568EAB
  004CEAD9: mov         esi,eax
  004CEADB: add         esp,8
  004CEADE: test        esi,esi
  004CEAE0: jl          004CEABE
  004CEAE2: jmp         004CEAE8
  004CEAE4: test        esi,esi
  004CEAE6: jl          004CEB05
  004CEAE8: push        esi
  004CEAE9: call        0056B1B6
  004CEAEE: mov         edx,dword ptr [esp+18h]
  004CEAF2: mov         edi,eax
  004CEAF4: push        esi
  004CEAF5: sub         edi,edx
  004CEAF7: call        005647EB
  004CEAFC: add         esp,8
  004CEAFF: mov         eax,edi
  004CEB01: pop         edi
  004CEB02: pop         esi
  004CEB03: pop         ebx
  004CEB04: ret
  004CEB05: pop         edi
  004CEB06: pop         esi
  004CEB07: xor         eax,eax
  004CEB09: pop         ebx
  004CEB0A: ret
  004CEB0B: nop
  004CEB0C: nop
  004CEB0D: nop
  004CEB0E: nop
  004CEB0F: nop
  004CEB10: mov         eax,dword ptr [esp+4]
  004CEB14: push        esi
  004CEB15: push        5763BCh
  004CEB1A: push        eax
  004CEB1B: call        00561A0A
  004CEB20: mov         esi,eax
  004CEB22: add         esp,8
  004CEB25: test        esi,esi
  004CEB27: jne         004CEB2B
  004CEB29: pop         esi
  004CEB2A: ret
  004CEB2B: push        esi
  004CEB2C: call        0056B22E
  004CEB31: push        eax
  004CEB32: call        0056B1B6
  004CEB37: mov         ecx,dword ptr [esp+14h]
  004CEB3B: push        esi
  004CEB3C: push        eax
  004CEB3D: push        1
  004CEB3F: push        ecx
  004CEB40: call        00561CA1
  004CEB45: push        esi
  004CEB46: call        00561994
  004CEB4B: add         esp,1Ch
  004CEB4E: mov         eax,1
  004CEB53: pop         esi
  004CEB54: ret
  004CEB55: nop
  004CEB56: nop
  004CEB57: nop
  004CEB58: nop
  004CEB59: nop
  004CEB5A: nop
  004CEB5B: nop
  004CEB5C: nop
  004CEB5D: nop
  004CEB5E: nop
  004CEB5F: nop
  004CEB60: push        ebx
  004CEB61: push        esi
  004CEB62: push        edi
  004CEB63: mov         edi,dword ptr [esp+10h]
  004CEB67: push        5763BCh
  004CEB6C: push        edi
  004CEB6D: mov         ebx,1
  004CEB72: call        00561A0A
  004CEB77: mov         esi,eax
  004CEB79: add         esp,8
  004CEB7C: test        esi,esi
  004CEB7E: jne         004CEBFB
  004CEB80: mov         al,byte ptr ds:[00E38E62h]
  004CEB85: or          ecx,0FFFFFFFFh
  004CEB88: mov         byte ptr ds:[005E05ACh],al
  004CEB8D: xor         eax,eax
  004CEB8F: mov         byte ptr ds:[005E05AFh],0
  004CEB96: push        5763BCh
  004CEB9B: repne scas  byte ptr es:[edi]
  004CEB9D: not         ecx
  004CEB9F: sub         edi,ecx
  004CEBA1: push        5E05ACh
  004CEBA6: mov         esi,edi
  004CEBA8: mov         edx,ecx
  004CEBAA: mov         edi,5E05ACh
  004CEBAF: or          ecx,0FFFFFFFFh
  004CEBB2: repne scas  byte ptr es:[edi]
  004CEBB4: mov         ecx,edx
  004CEBB6: dec         edi
  004CEBB7: shr         ecx,2
  004CEBBA: rep movs    dword ptr es:[edi],dword ptr [esi]
  004CEBBC: mov         ecx,edx
  004CEBBE: and         ecx,3
  004CEBC1: rep movs    byte ptr es:[edi],byte ptr [esi]
  004CEBC3: call        00561A0A
  004CEBC8: mov         esi,eax
  004CEBCA: add         esp,8
  004CEBCD: test        esi,esi
  004CEBCF: jne         004CEBFB
  004CEBD1: cmp         ebx,1
  004CEBD4: jne         004CEBF7
  004CEBD6: call        004CE9E0
  004CEBDB: push        5763BCh
  004CEBE0: push        5E05ACh
  004CEBE5: mov         ebx,eax
  004CEBE7: call        00561A0A
  004CEBEC: mov         esi,eax
  004CEBEE: add         esp,8
  004CEBF1: test        esi,esi
  004CEBF3: je          004CEBD1
  004CEBF5: jmp         004CEBFB
  004CEBF7: test        esi,esi
  004CEBF9: je          004CEC38
  004CEBFB: push        esi
  004CEBFC: call        0056B22E
  004CEC01: push        eax
  004CEC02: call        0056B1B6
  004CEC07: mov         ebx,dword ptr [esp+20h]
  004CEC0B: push        0
  004CEC0D: push        ebx
  004CEC0E: push        esi
  004CEC0F: mov         edi,eax
  004CEC11: call        00561EA4
  004CEC16: mov         eax,dword ptr [esp+28h]
  004CEC1A: sub         edi,ebx
  004CEC1C: push        esi
  004CEC1D: push        edi
  004CEC1E: push        1
  004CEC20: push        eax
  004CEC21: call        00561CA1
  004CEC26: push        esi
  004CEC27: call        00561994
  004CEC2C: add         esp,28h
  004CEC2F: mov         eax,1
  004CEC34: pop         edi
  004CEC35: pop         esi
  004CEC36: pop         ebx
  004CEC37: ret
  004CEC38: pop         edi
  004CEC39: pop         esi
  004CEC3A: xor         eax,eax
  004CEC3C: pop         ebx
  004CEC3D: ret
  004CEC3E: nop
  004CEC3F: nop
  004CEC40: mov         eax,dword ptr [esp+4]
  004CEC44: push        esi
  004CEC45: push        edi
  004CEC46: push        8000h
  004CEC4B: push        eax
  004CEC4C: call        00568EAB
  004CEC51: mov         esi,eax
  004CEC53: add         esp,8
  004CEC56: cmp         esi,0FFFFFFFFh
  004CEC59: jne         004CEC60
  004CEC5B: pop         edi
  004CEC5C: xor         eax,eax
  004CEC5E: pop         esi
  004CEC5F: ret
  004CEC60: push        esi
  004CEC61: call        0056B1B6
  004CEC66: mov         edi,eax
  004CEC68: mov         eax,dword ptr ds:[00E36C70h]
  004CEC6D: add         esp,4
  004CEC70: sub         edi,1F5Ch
  004CEC76: test        eax,eax
  004CEC78: je          004CEC8D
  004CEC7A: push        eax
  004CEC7B: call        00561A2B
  004CEC80: add         esp,4
  004CEC83: mov         dword ptr ds:[00E36C70h],0
  004CEC8D: push        edi
  004CEC8E: call        00561A1D
  004CEC93: push        1Ch
  004CEC95: push        0E36B78h
  004CEC9A: push        esi
  004CEC9B: mov         dword ptr ds:[00E36C70h],eax
  004CECA0: call        005660ED
  004CECA5: push        1F40h
  004CECAA: push        0E34C38h
  004CECAF: push        esi
  004CECB0: call        005660ED
  004CECB5: mov         ecx,dword ptr ds:[00E36C70h]
  004CECBB: push        edi
  004CECBC: push        ecx
  004CECBD: push        esi
  004CECBE: call        005660ED
  004CECC3: push        esi
  004CECC4: call        005647EB
  004CECC9: mov         edx,dword ptr [esp+3Ch]
  004CECCD: push        8000h
  004CECD2: push        edx
  004CECD3: call        00568EAB
  004CECD8: mov         esi,eax
  004CECDA: add         esp,34h
  004CECDD: cmp         esi,0FFFFFFFFh
  004CECE0: jne         004CECE7
  004CECE2: pop         edi
  004CECE3: xor         eax,eax
  004CECE5: pop         esi
  004CECE6: ret
  004CECE7: push        esi
  004CECE8: call        0056B1B6
  004CECED: mov         edi,eax
  004CECEF: mov         eax,dword ptr ds:[00E36C74h]
  004CECF4: add         esp,4
  004CECF7: sub         edi,13898h
  004CECFD: test        eax,eax
  004CECFF: je          004CED14
  004CED01: push        eax
  004CED02: call        00561A2B
  004CED07: add         esp,4
  004CED0A: mov         dword ptr ds:[00E36C74h],0
  004CED14: push        edi
  004CED15: call        00561A1D
  004CED1A: push        13898h
  004CED1F: push        0E209A0h
  004CED24: push        esi
  004CED25: mov         dword ptr ds:[00E36C74h],eax
  004CED2A: call        005660ED
  004CED2F: mov         eax,dword ptr ds:[00E36C74h]
  004CED34: push        edi
  004CED35: push        eax
  004CED36: push        esi
  004CED37: call        005660ED
  004CED3C: push        esi
  004CED3D: call        005647EB
  004CED42: add         esp,20h
  004CED45: mov         eax,1
  004CED4A: pop         edi
  004CED4B: pop         esi
  004CED4C: ret
  004CED4D: nop
  004CED4E: nop
  004CED4F: nop
  004CED50: mov         eax,dword ptr [esp+4]
  004CED54: push        esi
  004CED55: push        8000h
  004CED5A: push        eax
  004CED5B: call        00568EAB
  004CED60: mov         esi,eax
  004CED62: add         esp,8
  004CED65: cmp         esi,0FFFFFFFFh
  004CED68: jne         004CED6E
  004CED6A: xor         eax,eax
  004CED6C: pop         esi
  004CED6D: ret
  004CED6E: push        edi
  004CED6F: push        esi
  004CED70: call        0056B1B6
  004CED75: push        esi
  004CED76: mov         edi,eax
  004CED78: call        005647EB
  004CED7D: add         esp,8
  004CED80: mov         eax,edi
  004CED82: pop         edi
  004CED83: pop         esi
  004CED84: ret
  004CED85: nop
  004CED86: nop
  004CED87: nop
  004CED88: nop
  004CED89: nop
  004CED8A: nop
  004CED8B: nop
  004CED8C: nop
  004CED8D: nop
  004CED8E: nop
  004CED8F: nop
  004CED90: 8B

  Summary

      16E000 .text
