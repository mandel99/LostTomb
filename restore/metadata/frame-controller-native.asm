
Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  004D0280: cmp         dword ptr ds:[005E06B0h],1
  004D0287: je          004D0392
  004D028D: mov         eax,dword ptr ds:[00F54054h]
  004D0292: test        eax,eax
  004D0294: je          004D02A3
  004D0296: mov         eax,dword ptr ds:[00E92E04h]
  004D029B: test        eax,eax
  004D029D: jne         004D0392
  004D02A3: cmp         byte ptr ds:[00E38DDAh],5
  004D02AA: je          004D02C5
  004D02AC: mov         eax,dword ptr ds:[0078D7A0h]
  004D02B1: test        eax,eax
  004D02B3: jle         004D02C5
  004D02B5: cdq
  004D02B6: mov         ecx,88h
  004D02BB: idiv        eax,ecx
  004D02BD: test        edx,edx
  004D02BF: je          004D035E
  004D02C5: call        004D0040
  004D02CA: mov         edx,dword ptr ds:[00E92E08h]
  004D02D0: mov         ecx,dword ptr [esp+4]
  004D02D4: add         edx,eax
  004D02D6: cmp         ecx,1
  004D02D9: mov         dword ptr ds:[00E92E08h],edx
  004D02DF: mov         eax,32h
  004D02E4: jne         004D02EB
  004D02E6: mov         dword ptr ds:[00E92E08h],eax
  004D02EB: mov         cl,byte ptr ds:[00E38DC8h]
  004D02F1: test        cl,cl
  004D02F3: jne         004D0301
  004D02F5: cmp         dword ptr ds:[00E92E08h],eax
  004D02FB: jl          004D0392
  004D0301: mov         al,byte ptr ds:[00E38DDAh]
  004D0306: mov         dword ptr ds:[00E92E08h],0
  004D0310: cmp         al,5
  004D0312: mov         byte ptr ds:[00E38DC8h],0
  004D0319: je          004D0359
  004D031B: mov         ecx,dword ptr ds:[0078D7A0h]
  004D0321: test        ecx,ecx
  004D0323: jle         004D0359
  004D0325: xor         edx,edx
  004D0327: mov         dl,al
  004D0329: mov         eax,edx
  004D032B: dec         eax
  004D032C: cmp         eax,48h
  004D032F: ja          004D0359
  004D0331: xor         ecx,ecx
  004D0333: mov         cl,byte ptr [eax+004D039Ch]
  004D0339: jmp         dword ptr [ecx*4+004D0394h]
  004D0340: call        0051E990
  004D0345: push        1
  004D0347: push        0
  004D0349: push        0
  004D034B: push        1
  004D034D: push        1
  004D034F: mov         ecx,6DE518h
  004D0354: call        004D05F0
  004D0359: jmp         004D03F0
  004D035E: push        esi
  004D035F: push        0
  004D0361: push        1
  004D0363: push        0
  004D0365: push        0Fh
  004D0367: push        1
  004D0369: mov         ecx,6DE518h
  004D036E: call        004D05F0
  004D0373: call        004D03F0
  004D0378: mov         esi,8
  004D037D: push        1
  004D037F: mov         ecx,6DE518h
  004D0384: call        004D0650
  004D0389: call        004D03F0
  004D038E: dec         esi
  004D038F: jne         004D037D
  004D0391: pop         esi
  004D0392: ret
  004D0393: nop
  004D0394: inc         eax
  004D0395: add         ecx,dword ptr [ebp]
  004D0398: pop         ecx
  004D0399: add         ecx,dword ptr [ebp]
  004D039C: add         byte ptr [ecx],al
  004D039E: add         byte ptr [ecx],al
  004D03A0: add         dword ptr [ecx],eax
  004D03A2: add         dword ptr [ecx],eax
  004D03A4: add         dword ptr [ecx],eax
  004D03A6: add         dword ptr [ecx],eax
  004D03A8: add         dword ptr [ecx],eax
  004D03AA: add         dword ptr [ecx],eax
  004D03AC: add         dword ptr [ecx],eax
  004D03AE: add         dword ptr [ecx],eax
  004D03B0: add         dword ptr [ecx],eax
  004D03B2: add         dword ptr [ecx],eax
  004D03B4: add         dword ptr [ecx],eax
  004D03B6: add         dword ptr [ecx],eax
  004D03B8: add         dword ptr [eax],eax
  004D03BA: add         byte ptr [eax],al
  004D03BC: add         dword ptr [ecx],eax
  004D03BE: add         dword ptr [ecx],eax
  004D03C0: add         dword ptr [ecx],eax
  004D03C2: add         dword ptr [ecx],eax
  004D03C4: add         dword ptr [ecx],eax
  004D03C6: add         dword ptr [ecx],eax
  004D03C8: add         dword ptr [ecx],eax
  004D03CA: add         dword ptr [ecx],eax
  004D03CC: add         dword ptr [ecx],eax
  004D03CE: add         dword ptr [ecx],eax
  004D03D0: add         dword ptr [ecx],eax
  004D03D2: add         dword ptr [ecx],eax
  004D03D4: add         dword ptr [ecx],eax
  004D03D6: add         dword ptr [ecx],eax
  004D03D8: add         dword ptr [ecx],eax
  004D03DA: add         dword ptr [ecx],eax
  004D03DC: add         dword ptr [ecx],eax
  004D03DE: add         dword ptr [ecx],eax
  004D03E0: add         dword ptr [ecx],eax
  004D03E2: add         dword ptr [ecx],eax
  004D03E4: 00
  004D03E5: nop
  004D03E6: nop

  Summary

      16E000 .text
