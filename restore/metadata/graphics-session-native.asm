
Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  004D0040: call        dword ptr ds:[0056F22Ch]
  004D0046: mov         edx,dword ptr ds:[00E36C88h]
  004D004C: mov         ecx,eax
  004D004E: sub         eax,edx
  004D0050: mov         dword ptr ds:[00E36C88h],ecx
  004D0056: ret
  004D0057: nop
  004D0058: nop
  004D0059: nop
  004D005A: nop
  004D005B: nop
  004D005C: nop
  004D005D: nop
  004D005E: nop
  004D005F: nop
  004D0060: sub         esp,0C8h
  004D0066: call        00426B40
  004D006B: cmp         eax,1
  004D006E: push        0
  004D0070: push        0
  004D0072: jne         004D01C9
  004D0078: push        5E0870h
  004D007D: call        004CE6E0
  004D0082: add         esp,0Ch
  004D0085: call        00426FF0
  004D008A: cmp         eax,1
  004D008D: sete        al
  004D0090: test        al,al
  004D0092: jne         004D0142
  004D0098: push        0
  004D009A: push        0
  004D009C: push        5E0844h
  004D00A1: call        004CE6E0
  004D00A6: push        0
  004D00A8: push        0
  004D00AA: push        5E0824h
  004D00AF: call        004CE6E0
  004D00B4: push        1
  004D00B6: call        00426EE0
  004D00BB: mov         eax,dword ptr ds:[00E92E4Ch]
  004D00C0: mov         ecx,dword ptr ds:[005E06D8h]
  004D00C6: push        eax
  004D00C7: mov         dword ptr ds:[00E38E68h],eax
  004D00CC: mov         dword ptr ds:[00E38E5Ch],ecx
  004D00D2: call        004CFD60
  004D00D7: mov         edx,dword ptr ds:[00E92DFCh]
  004D00DD: mov         eax,dword ptr ds:[00E38DACh]
  004D00E2: mov         ecx,dword ptr ds:[00E38DB0h]
  004D00E8: push        edx
  004D00E9: push        eax
  004D00EA: push        ecx
  004D00EB: lea         edx,[esp+00000090h]
  004D00F2: push        5E0804h
  004D00F7: push        edx
  004D00F8: call        00561A36
  004D00FD: push        0
  004D00FF: lea         eax,[esp+0000009Ch]
  004D0106: push        0
  004D0108: push        eax
  004D0109: call        004CE6E0
  004D010E: add         esp,40h
  004D0111: call        00426B40
  004D0116: cmp         eax,1
  004D0119: sete        al
  004D011C: test        al,al
  004D011E: je          004D012F
  004D0120: call        00426FF0
  004D0125: cmp         eax,1
  004D0128: sete        al
  004D012B: test        al,al
  004D012D: jne         004D0142
  004D012F: push        1
  004D0131: call        00426EE0
  004D0136: add         esp,4
  004D0139: xor         eax,eax
  004D013B: add         esp,0C8h
  004D0141: ret
  004D0142: mov         ecx,dword ptr ds:[00E39174h]
  004D0148: mov         edx,dword ptr ds:[00E39178h]
  004D014E: push        esi
  004D014F: push        ecx
  004D0150: push        edx
  004D0151: mov         ecx,6DE518h
  004D0156: call        004D0480
  004D015B: mov         eax,dword ptr ds:[00E39178h]
  004D0160: lea         ecx,[esp+4]
  004D0164: push        eax
  004D0165: push        5E07F0h
  004D016A: push        ecx
  004D016B: call        00561A36
  004D0170: mov         esi,dword ptr ds:[0056F0F8h]
  004D0176: add         esp,0Ch
  004D0179: lea         edx,[esp+4]
  004D017D: push        edx
  004D017E: call        esi
  004D0180: mov         eax,dword ptr ds:[00E39174h]
  004D0185: lea         ecx,[esp+4]
  004D0189: push        eax
  004D018A: push        5E07DCh
  004D018F: push        ecx
  004D0190: call        00561A36
  004D0195: add         esp,0Ch
  004D0198: lea         edx,[esp+4]
  004D019C: push        edx
  004D019D: call        esi
  004D019F: mov         eax,dword ptr ds:[00E38E68h]
  004D01A4: mov         ecx,dword ptr ds:[00E38E5Ch]
  004D01AA: mov         dword ptr ds:[00E92E4Ch],eax
  004D01AF: mov         dword ptr ds:[005E06D8h],ecx
  004D01B5: mov         byte ptr ds:[005E06D4h],0
  004D01BC: mov         eax,1
  004D01C1: pop         esi
  004D01C2: add         esp,0C8h
  004D01C8: ret
  004D01C9: push        5E07BCh
  004D01CE: call        004CE6E0
  004D01D3: add         esp,0Ch
  004D01D6: xor         eax,eax
  004D01D8: add         esp,0C8h
  004D01DE: ret
  004D01DF: nop
  004D01E0: mov         eax,dword ptr ds:[00E92E00h]
  004D01E5: test        eax,eax
  004D01E7: je          004D01F9
  004D01E9: mov         ecx,dword ptr [eax]
  004D01EB: push        eax
  004D01EC: call        dword ptr [ecx+8]
  004D01EF: mov         dword ptr ds:[00E92E00h],0
  004D01F9: mov         eax,dword ptr ds:[00E390E0h]
  004D01FE: test        eax,eax
  004D0200: je          004D0219
  004D0202: mov         edx,dword ptr [esp+4]
  004D0206: push        edx
  004D0207: call        00426EE0
  004D020C: add         esp,4
  004D020F: mov         dword ptr ds:[00E390E0h],0
  004D0219: ret
  004D021A: nop
  004D021B: nop
  004D021C: nop
  004D021D: nop
  004D021E: nop
  004D021F: nop
  004D0220: push        0
  004D0222: mov         dword ptr ds:[005E06B0h],1
  004D022C: call        004D01E0
  004D0231: add         esp,4
  004D0234: call        004D0060
  004D0239: test        eax,eax
  004D023B: mov         dword ptr ds:[00E390E0h],eax
  004D0240: jne         004D0254
  004D0242: push        eax
  004D0243: push        eax
  004D0244: push        5E088Ch
  004D0249: call        004CE6E0
  004D024E: add         esp,0Ch
  004D0251: xor         eax,eax
  004D0253: ret
  004D0254: call        00426F60
  004D0259: mov         dword ptr ds:[005E06B0h],0
  004D0263: mov         dword ptr ds:[00E3912Ch],1
  004D026D: mov         byte ptr ds:[00E38DCAh],1
  004D0274: mov         eax,1
  004D0279: ret
  004D027A: nop
  004D027B: nop
  004D027C: nop
  004D027D: nop
  004D027E: nop
  004D027F: nop
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
  004D03E4: add         byte ptr [eax+90909090h],dl
  004D03EA: nop
  004D03EB: nop
  004D03EC: nop
  004D03ED: nop
  004D03EE: nop
  004D03EF: nop
  004D03F0: sub         esp,20h
  004D03F3: push        esi
  004D03F4: lea         eax,[esp+14h]
  004D03F8: push        10h
  004D03FA: push        eax
  004D03FB: push        0E39130h
  004D0400: call        004CBEA0
  004D0405: mov         eax,dword ptr ds:[00E38DACh]
  004D040A: mov         ecx,dword ptr ds:[00E38DB0h]
  004D0410: mov         dword ptr [esp+1Ch],eax
  004D0414: mov         eax,dword ptr ds:[00E38E5Ch]
  004D0419: xor         edx,edx
  004D041B: add         esp,0Ch
  004D041E: cmp         eax,edx
  004D0420: mov         eax,dword ptr ds:[00E39178h]
  004D0425: mov         dword ptr [esp+4],edx
  004D0429: mov         dword ptr [esp+8],edx
  004D042D: mov         dword ptr [esp+0Ch],ecx
  004D0431: jne         004D0459
  004D0433: cmp         eax,edx
  004D0435: je          004D0476
  004D0437: mov         ecx,dword ptr ds:[00E39174h]
  004D043D: cmp         ecx,edx
  004D043F: je          004D0476
  004D0441: mov         esi,dword ptr [eax]
  004D0443: push        edx
  004D0444: push        edx
  004D0445: lea         edx,[esp+0Ch]
  004D0449: push        edx
  004D044A: push        ecx
  004D044B: lea         ecx,[esp+24h]
  004D044F: push        ecx
  004D0450: push        eax
  004D0451: call        dword ptr [esi+14h]
  004D0454: pop         esi
  004D0455: add         esp,20h
  004D0458: ret
  004D0459: cmp         eax,edx
  004D045B: je          004D0476
  004D045D: mov         ecx,dword ptr ds:[00E39174h]
  004D0463: cmp         ecx,edx
  004D0465: je          004D0476
  004D0467: mov         esi,dword ptr [eax]
  004D0469: push        edx
  004D046A: push        0E39130h
  004D046F: push        ecx
  004D0470: push        edx
  004D0471: push        edx
  004D0472: push        eax
  004D0473: call        dword ptr [esi+1Ch]
  004D0476: pop         esi
  004D0477: add         esp,20h
  004D047A: ret
  004D047B: nop
  004D047C: nop
  004D047D: nop
  004D047E: nop
  004D047F: nop
  004D0480: sub         esp,0D0h
  004D0486: push        ebx
  004D0487: push        ebp
  004D0488: mov         ebx,dword ptr [esp+000000DCh]
  004D048F: mov         ebp,dword ptr [esp+000000E0h]
  004D0496: push        esi
  004D0497: mov         esi,ecx
  004D0499: push        edi
  004D049A: mov         ecx,1Bh
  004D049F: xor         eax,eax
  004D04A1: lea         edi,[esp+10h]
  004D04A5: rep stos    dword ptr es:[edi]
  004D04A7: push        0
  004D04A9: lea         ecx,[esp+14h]
  004D04AD: push        1
  004D04AF: mov         dword ptr [esi+1Ch],ebx
  004D04B2: mov         dword ptr [esi+20h],ebp
  004D04B5: mov         eax,dword ptr [ebx]
  004D04B7: push        ecx
  004D04B8: push        0
  004D04BA: push        ebx
  004D04BB: mov         dword ptr [esp+24h],6Ch
  004D04C3: call        dword ptr [eax+64h]
  004D04C6: test        eax,eax
  004D04C8: je          004D04D1
  004D04CA: push        5E08F0h
  004D04CF: jmp         004D04FE
  004D04D1: mov         edx,dword ptr [esp+34h]
  004D04D5: push        0
  004D04D7: mov         dword ptr [esi+24h],edx
  004D04DA: mov         eax,dword ptr [ebx]
  004D04DC: push        ebx
  004D04DD: call        dword ptr [eax+00000080h]
  004D04E3: mov         ecx,dword ptr [esi+24h]
  004D04E6: lea         edx,[esp+7Ch]
  004D04EA: push        ecx
  004D04EB: push        5E08D8h
  004D04F0: push        edx
  004D04F1: call        00561A36
  004D04F6: add         esp,0Ch
  004D04F9: lea         eax,[esp+7Ch]
  004D04FD: push        eax
  004D04FE: mov         ebx,dword ptr ds:[0056F0F8h]
  004D0504: call        ebx
  004D0506: xor         eax,eax
  004D0508: mov         ecx,1Bh
  004D050D: lea         edi,[esp+10h]
  004D0511: push        eax
  004D0512: lea         edx,[esp+14h]
  004D0516: push        1
  004D0518: rep stos    dword ptr es:[edi]
  004D051A: mov         ecx,dword ptr [ebp]
  004D051D: push        edx
  004D051E: push        eax
  004D051F: push        ebp
  004D0520: mov         dword ptr [esp+24h],6Ch
  004D0528: call        dword ptr [ecx+64h]
  004D052B: test        eax,eax
  004D052D: je          004D0536
  004D052F: push        5E08BCh
  004D0534: jmp         004D0564
  004D0536: mov         eax,dword ptr [esp+34h]
  004D053A: push        0
  004D053C: mov         dword ptr [esi+28h],eax
  004D053F: mov         ecx,dword ptr [ebp]
  004D0542: push        ebp
  004D0543: call        dword ptr [ecx+00000080h]
  004D0549: mov         edx,dword ptr [esi+28h]
  004D054C: lea         eax,[esp+7Ch]
  004D0550: push        edx
  004D0551: push        5E08A4h
  004D0556: push        eax
  004D0557: call        00561A36
  004D055C: add         esp,0Ch
  004D055F: lea         ecx,[esp+7Ch]
  004D0563: push        ecx
  004D0564: call        ebx
  004D0566: mov         eax,dword ptr [esp+18h]
  004D056A: mov         edx,dword ptr [esp+1Ch]
  004D056E: mov         dword ptr [esi+0Ch],eax
  004D0571: mov         eax,dword ptr [esp+68h]
  004D0575: cmp         eax,0F800h
  004D057A: mov         dword ptr [esi+8],edx
  004D057D: jne         004D05A9
  004D057F: cmp         dword ptr [esp+6Ch],7E0h
  004D0587: jne         004D05DA
  004D0589: cmp         dword ptr [esp+70h],1Fh
  004D058E: jne         004D05DA
  004D0590: mov         dword ptr [esi+10h],1
  004D0597: pop         edi
  004D0598: pop         esi
  004D0599: pop         ebp
  004D059A: mov         eax,1
  004D059F: pop         ebx
  004D05A0: add         esp,0D0h
  004D05A6: ret         8
  004D05A9: cmp         eax,7C00h
  004D05AE: jne         004D05DA
  004D05B0: cmp         dword ptr [esp+6Ch],3E0h
  004D05B8: jne         004D05DA
  004D05BA: cmp         dword ptr [esp+70h],1Fh
  004D05BF: jne         004D05DA
  004D05C1: mov         dword ptr [esi+10h],0
  004D05C8: pop         edi
  004D05C9: pop         esi
  004D05CA: pop         ebp
  004D05CB: mov         eax,1
  004D05D0: pop         ebx
  004D05D1: add         esp,0D0h
  004D05D7: ret         8
  004D05DA: mov         dword ptr [esi+10h],0FFFFFFFFh
  004D05E1: pop         edi
  004D05E2: pop         esi
  004D05E3: pop         ebp
  004D05E4: xor         eax,eax
  004D05E6: pop         ebx
  004D05E7: add         esp,0D0h
  004D05ED: ret         8
  004D05F0: mov         eax,dword ptr [esp+8]
  004D05F4: push        esi
  004D05F5: push        0Fh
  004D05F7: push        0
  004D05F9: mov         esi,ecx
  004D05FB: push        eax
  004D05FC: call        004D0630
  004D0601: mov         ecx,dword ptr [esp+1Ch]
  004D0605: mov         dl,byte ptr [esp+20h]
  004D0609: add         esp,0Ch
  004D060C: mov         dword ptr [esi+4],ecx
  004D060F: mov         ecx,dword ptr [esp+8]
  004D0613: mov         dword ptr [esi],eax
  004D0615: mov         eax,dword ptr [esp+18h]
  004D0619: push        ecx
  004D061A: mov         ecx,esi
  004D061C: mov         byte ptr [esi+18h],dl
  004D061F: mov         dword ptr [esi+14h],eax
  004D0622: call        004D0650
  004D0627: pop         esi
  004D0628: ret         14h
  004D062B: nop
  004D062C: nop
  004D062D: nop
  004D062E: nop
  004D062F: nop
  004D0630: mov         ecx,dword ptr [esp+4]
  004D0634: mov         eax,dword ptr [esp+8]
  004D0638: cmp         ecx,eax
  004D063A: jl          004D0646
  004D063C: mov         eax,dword ptr [esp+0Ch]
  004D0640: cmp         ecx,eax
  004D0642: jg          004D0646
  004D0644: mov         eax,ecx
  004D0646: ret
  004D0647: nop
  004D0648: nop
  004D0649: nop
  004D064A: nop
  004D064B: nop
  004D064C: nop
  004D064D: nop
  004D064E: nop
  004D064F: nop
  004D0650: sub         esp,14h
  004D0653: push        ebx
  004D0654: push        ebp
  004D0655: push        esi
  004D0656: push        edi
  004D0657: mov         edi,ecx
  004D0659: mov         dword ptr [esp+1Ch],edi
  004D065D: mov         eax,dword ptr [edi]
  004D065F: test        eax,eax
  004D0661: jle         004D0A27
  004D0667: cmp         dword ptr [edi+10h],0FFFFFFFFh
  004D066B: je          004D0A27
  004D0671: mov         ebx,dword ptr [edi+4]
  004D0674: dec         eax
  004D0675: mov         dword ptr [edi],eax
  004D0677: mov         al,byte ptr [edi+18h]
  004D067A: test        al,al
  004D067C: je          004D075B
  004D0682: mov         eax,dword ptr ds:[00D3B1C4h]
  004D0687: mov         ecx,dword ptr ds:[00D3B374h]
  004D068D: mov         dword ptr [eax*4+00D3B1C8h],ecx
  004D0694: inc         eax
  004D0695: cmp         eax,64h
  004D0698: mov         dword ptr ds:[00D3B1C4h],eax
  004D069D: jl          004D06A9
  004D069F: mov         dword ptr ds:[00D3B1C4h],0
  004D06A9: mov         eax,dword ptr ds:[00D3B384h]
  004D06AE: mov         edx,dword ptr ds:[00D3B380h]
  004D06B4: mov         ebp,1Fh
  004D06B9: mov         esi,eax
  004D06BB: mov         ecx,eax
  004D06BD: shr         esi,4
  004D06C0: and         esi,1
  004D06C3: and         ecx,1
  004D06C6: mov         edi,edx
  004D06C8: xor         esi,ecx
  004D06CA: shr         edi,4
  004D06CD: mov         ecx,edx
  004D06CF: and         edi,1
  004D06D2: and         ecx,1
  004D06D5: xor         edi,ecx
  004D06D7: shr         eax,1
  004D06D9: shr         edx,1
  004D06DB: test        esi,esi
  004D06DD: je          004D06E4
  004D06DF: or          eax,40000000h
  004D06E4: test        edi,edi
  004D06E6: je          004D06EE
  004D06E8: or          edx,40000000h
  004D06EE: dec         ebp
  004D06EF: jne         004D06B9
  004D06F1: mov         ecx,eax
  004D06F3: mov         esi,eax
  004D06F5: and         ecx,7Fh
  004D06F8: and         esi,7FFFh
  004D06FE: mov         dword ptr ds:[00D3B374h],ecx
  004D0704: mov         ecx,eax
  004D0706: and         ecx,7
  004D0709: mov         dword ptr ds:[00D3B370h],esi
  004D070F: mov         dword ptr ds:[00D3B378h],ecx
  004D0715: mov         ecx,edx
  004D0717: and         ecx,7FFFh
  004D071D: mov         dword ptr ds:[00D3B380h],edx
  004D0723: mov         dword ptr ds:[00D3B364h],ecx
  004D0729: mov         ecx,edx
  004D072B: and         ecx,7Fh
  004D072E: mov         dword ptr ds:[00D3B384h],eax
  004D0733: mov         dword ptr ds:[00D3B368h],ecx
  004D0739: mov         ecx,edx
  004D073B: and         ecx,7
  004D073E: and         esi,8000000Fh
  004D0744: mov         dword ptr ds:[00D3B36Ch],ecx
  004D074A: jns         004D0751
  004D074C: dec         esi
  004D074D: or          esi,0FFFFFFF0h
  004D0750: inc         esi

  Summary

      16E000 .text
