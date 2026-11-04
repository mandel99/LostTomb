Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  0055DBC0: push        ebp
  0055DBC1: mov         ebp,esp
  0055DBC3: push        ecx
  0055DBC4: mov         dword ptr [ebp-4],ecx
  0055DBC7: mov         eax,dword ptr ds:[012663B0h]
  0055DBCC: add         eax,0Ch
  0055DBCF: mov         dword ptr ds:[012663B0h],eax
  0055DBD4: mov         ecx,dword ptr [ebp-4]
  0055DBD7: mov         dword ptr [ecx+8],0
  0055DBDE: mov         edx,dword ptr [ebp-4]
  0055DBE1: mov         dword ptr [edx+4],0
  0055DBE8: mov         eax,dword ptr [ebp-4]
  0055DBEB: mov         dword ptr [eax],0
  0055DBF1: mov         eax,dword ptr [ebp-4]
  0055DBF4: mov         esp,ebp
  0055DBF6: pop         ebp
  0055DBF7: ret
  0055DBF8: push        ebp
  0055DBF9: mov         ebp,esp
  0055DBFB: push        ecx
  0055DBFC: mov         dword ptr [ebp-4],ecx
  0055DBFF: mov         eax,dword ptr ds:[012663B0h]
  0055DC04: sub         eax,0Ch
  0055DC07: mov         dword ptr ds:[012663B0h],eax
  0055DC0C: mov         ecx,dword ptr [ebp-4]
  0055DC0F: call        0055DC18
  0055DC14: mov         esp,ebp
  0055DC16: pop         ebp
  0055DC17: ret
  0055DC18: push        ebp
  0055DC19: mov         ebp,esp
  0055DC1B: sub         esp,10h
  0055DC1E: mov         dword ptr [ebp-0Ch],ecx
  0055DC21: mov         eax,dword ptr [ebp-0Ch]
  0055DC24: cmp         dword ptr [eax+8],0
  0055DC28: je          0055DC5F
  0055DC2A: mov         ecx,dword ptr [ebp-0Ch]
  0055DC2D: mov         edx,dword ptr [ecx+8]
  0055DC30: mov         dword ptr [ebp-8],edx
  0055DC33: mov         eax,dword ptr [ebp-8]
  0055DC36: mov         dword ptr [ebp-4],eax
  0055DC39: cmp         dword ptr [ebp-4],0
  0055DC3D: je          0055DC4E
  0055DC3F: push        3
  0055DC41: mov         ecx,dword ptr [ebp-4]
  0055DC44: call        00560210
  0055DC49: mov         dword ptr [ebp-10h],eax
  0055DC4C: jmp         0055DC55
  0055DC4E: mov         dword ptr [ebp-10h],0
  0055DC55: mov         ecx,dword ptr [ebp-0Ch]
  0055DC58: mov         dword ptr [ecx+8],0
  0055DC5F: mov         esp,ebp
  0055DC61: pop         ebp
  0055DC62: ret
  0055DC63: push        ebp
  0055DC64: mov         ebp,esp
  0055DC66: push        0FFFFFFFFh
  0055DC68: push        56E14Bh
  0055DC6D: mov         eax,dword ptr fs:[00000000h]
  0055DC73: push        eax
  0055DC74: mov         dword ptr fs:[00000000h],esp
  0055DC7B: sub         esp,14h
  0055DC7E: mov         dword ptr [ebp-1Ch],ecx
  0055DC81: cmp         dword ptr [ebp+8],1
  0055DC85: jge         0055DC8E
  0055DC87: xor         eax,eax
  0055DC89: jmp         0055DD2F
  0055DC8E: mov         ecx,dword ptr [ebp-1Ch]
  0055DC91: call        0055DC18
  0055DC96: mov         eax,dword ptr [ebp+8]
  0055DC99: mov         dword ptr [ebp-10h],eax
  0055DC9C: mov         ecx,dword ptr [ebp-10h]
  0055DC9F: lea         edx,[ecx*8+00000004h]
  0055DCA6: push        edx
  0055DCA7: call        00561A1D
  0055DCAC: add         esp,4
  0055DCAF: mov         dword ptr [ebp-18h],eax
  0055DCB2: mov         dword ptr [ebp-4],0
  0055DCB9: cmp         dword ptr [ebp-18h],0
  0055DCBD: je          0055DCEE
  0055DCBF: push        5602B0h
  0055DCC4: push        560280h
  0055DCC9: mov         eax,dword ptr [ebp-18h]
  0055DCCC: mov         ecx,dword ptr [ebp-10h]
  0055DCCF: mov         dword ptr [eax],ecx
  0055DCD1: mov         edx,dword ptr [ebp-10h]
  0055DCD4: push        edx
  0055DCD5: push        8
  0055DCD7: mov         eax,dword ptr [ebp-18h]
  0055DCDA: add         eax,4
  0055DCDD: push        eax
  0055DCDE: call        00561441
  0055DCE3: mov         ecx,dword ptr [ebp-18h]
  0055DCE6: add         ecx,4
  0055DCE9: mov         dword ptr [ebp-20h],ecx
  0055DCEC: jmp         0055DCF5
  0055DCEE: mov         dword ptr [ebp-20h],0
  0055DCF5: mov         edx,dword ptr [ebp-20h]
  0055DCF8: mov         dword ptr [ebp-14h],edx
  0055DCFB: mov         dword ptr [ebp-4],0FFFFFFFFh
  0055DD02: mov         eax,dword ptr [ebp-1Ch]
  0055DD05: mov         ecx,dword ptr [ebp-14h]
  0055DD08: mov         dword ptr [eax+8],ecx
  0055DD0B: mov         edx,dword ptr [ebp-1Ch]
  0055DD0E: cmp         dword ptr [edx+8],0
  0055DD12: jne         0055DD18
  0055DD14: xor         eax,eax
  0055DD16: jmp         0055DD2F
  0055DD18: mov         eax,dword ptr [ebp-1Ch]
  0055DD1B: mov         ecx,dword ptr [ebp+8]
  0055DD1E: mov         dword ptr [eax],ecx
  0055DD20: mov         edx,dword ptr [ebp-1Ch]
  0055DD23: mov         dword ptr [edx+4],0
  0055DD2A: mov         eax,1
  0055DD2F: mov         ecx,dword ptr [ebp-0Ch]
  0055DD32: mov         dword ptr fs:[00000000h],ecx
  0055DD39: mov         esp,ebp
  0055DD3B: pop         ebp
  0055DD3C: ret         4
  0055DD3F: push        ebp
  0055DD40: mov         ebp,esp
  0055DD42: push        0FFFFFFFFh
  0055DD44: push        56E15Eh
  0055DD49: mov         eax,dword ptr fs:[00000000h]
  0055DD4F: push        eax
  0055DD50: mov         dword ptr fs:[00000000h],esp
  0055DD57: sub         esp,24h
  0055DD5A: mov         dword ptr [ebp-30h],ecx
  0055DD5D: lea         ecx,[ebp-14h]
  0055DD60: call        00560280
  0055DD65: mov         dword ptr [ebp-4],0
  0055DD6C: mov         eax,dword ptr [ebp-30h]
  0055DD6F: mov         ecx,dword ptr [eax+4]
  0055DD72: mov         dword ptr [ebp-18h],ecx
  0055DD75: mov         edx,dword ptr [ebp-30h]
  0055DD78: mov         eax,dword ptr [ebp-30h]
  0055DD7B: mov         ecx,dword ptr [edx+4]
  0055DD7E: cmp         ecx,dword ptr [eax]
  0055DD80: jl          0055DDA0
  0055DD82: mov         dword ptr [ebp-20h],0FFFFFFFFh
  0055DD89: mov         dword ptr [ebp-4],0FFFFFFFFh
  0055DD90: lea         ecx,[ebp-14h]
  0055DD93: call        005602B0
  0055DD98: mov         eax,dword ptr [ebp-20h]
  0055DD9B: jmp         0055DE92
  0055DDA0: mov         edx,dword ptr [ebp-30h]
  0055DDA3: cmp         dword ptr [edx+8],0
  0055DDA7: jne         0055DDC7
  0055DDA9: mov         dword ptr [ebp-24h],0FFFFFFFFh
  0055DDB0: mov         dword ptr [ebp-4],0FFFFFFFFh
  0055DDB7: lea         ecx,[ebp-14h]
  0055DDBA: call        005602B0
  0055DDBF: mov         eax,dword ptr [ebp-24h]
  0055DDC2: jmp         0055DE92
  0055DDC7: mov         dword ptr [ebp-1Ch],0
  0055DDCE: jmp         0055DDD9
  0055DDD0: mov         eax,dword ptr [ebp-1Ch]
  0055DDD3: add         eax,1
  0055DDD6: mov         dword ptr [ebp-1Ch],eax
  0055DDD9: mov         ecx,dword ptr [ebp-30h]
  0055DDDC: mov         edx,dword ptr [ebp-1Ch]
  0055DDDF: cmp         edx,dword ptr [ecx+4]
  0055DDE2: jge         0055DE2C
  0055DDE4: mov         eax,dword ptr [ebp-30h]
  0055DDE7: mov         ecx,dword ptr [eax+8]
  0055DDEA: mov         edx,dword ptr [ebp-1Ch]
  0055DDED: cmp         dword ptr [ecx+edx*8],0
  0055DDF1: je          0055DE2A
  0055DDF3: mov         eax,dword ptr [ebp+8]
  0055DDF6: push        eax
  0055DDF7: mov         ecx,dword ptr [ebp-30h]
  0055DDFA: mov         edx,dword ptr [ecx+8]
  0055DDFD: mov         eax,dword ptr [ebp-1Ch]
  0055DE00: mov         ecx,dword ptr [edx+eax*8]
  0055DE03: push        ecx
  0055DE04: call        005631B0
  0055DE09: add         esp,8
  0055DE0C: test        eax,eax
  0055DE0E: jne         0055DE2A
  0055DE10: mov         edx,dword ptr [ebp-1Ch]
  0055DE13: mov         dword ptr [ebp-28h],edx
  0055DE16: mov         dword ptr [ebp-4],0FFFFFFFFh
  0055DE1D: lea         ecx,[ebp-14h]
  0055DE20: call        005602B0
  0055DE25: mov         eax,dword ptr [ebp-28h]
  0055DE28: jmp         0055DE92
  0055DE2A: jmp         0055DDD0
  0055DE2C: cmp         dword ptr [ebp+0Ch],0
  0055DE30: je          0055DE4C
  0055DE32: mov         eax,dword ptr [ebp+8]
  0055DE35: push        eax
  0055DE36: call        0056B177
  0055DE3B: add         esp,4
  0055DE3E: mov         ecx,dword ptr [ebp-30h]
  0055DE41: mov         edx,dword ptr [ecx+8]
  0055DE44: mov         ecx,dword ptr [ebp-18h]
  0055DE47: mov         dword ptr [edx+ecx*8],eax
  0055DE4A: jmp         0055DE5B
  0055DE4C: mov         edx,dword ptr [ebp-30h]
  0055DE4F: mov         eax,dword ptr [edx+8]
  0055DE52: mov         ecx,dword ptr [ebp-18h]
  0055DE55: mov         edx,dword ptr [ebp+8]
  0055DE58: mov         dword ptr [eax+ecx*8],edx
  0055DE5B: mov         eax,dword ptr [ebp-30h]
  0055DE5E: mov         ecx,dword ptr [eax+8]
  0055DE61: mov         edx,dword ptr [ebp-18h]
  0055DE64: mov         eax,dword ptr [ebp+0Ch]
  0055DE67: mov         dword ptr [ecx+edx*8+4],eax
  0055DE6B: mov         ecx,dword ptr [ebp-30h]
  0055DE6E: mov         edx,dword ptr [ecx+4]
  0055DE71: add         edx,1
  0055DE74: mov         eax,dword ptr [ebp-30h]
  0055DE77: mov         dword ptr [eax+4],edx
  0055DE7A: mov         ecx,dword ptr [ebp-18h]
  0055DE7D: mov         dword ptr [ebp-2Ch],ecx
  0055DE80: mov         dword ptr [ebp-4],0FFFFFFFFh
  0055DE87: lea         ecx,[ebp-14h]
  0055DE8A: call        005602B0
  0055DE8F: mov         eax,dword ptr [ebp-2Ch]
  0055DE92: mov         ecx,dword ptr [ebp-0Ch]
  0055DE95: mov         dword ptr fs:[00000000h],ecx
  0055DE9C: mov         esp,ebp
  0055DE9E: pop         ebp
  0055DE9F: ret         8
  0055DEA2: push        ebp
  0055DEA3: mov         ebp,esp
  0055DEA5: sub         esp,28h
  0055DEA8: mov         dword ptr [ebp-28h],ecx
  0055DEAB: cmp         dword ptr [ebp+8],1
  0055DEAF: jge         0055DEB8
  0055DEB1: xor         eax,eax
  0055DEB3: jmp         0055E149
  0055DEB8: mov         eax,dword ptr [ebp-28h]
  0055DEBB: cmp         dword ptr [eax+0000019Ch],0
  0055DEC2: je          0055DF5E
  0055DEC8: mov         ecx,dword ptr [ebp-28h]
  0055DECB: call        0055EC7C
  0055DED0: mov         dword ptr [ebp-4],0
  0055DED7: jmp         0055DEE2
  0055DED9: mov         ecx,dword ptr [ebp-4]
  0055DEDC: add         ecx,1
  0055DEDF: mov         dword ptr [ebp-4],ecx
  0055DEE2: mov         edx,dword ptr [ebp-28h]
  0055DEE5: mov         eax,dword ptr [ebp-4]
  0055DEE8: cmp         eax,dword ptr [edx+00000198h]
  0055DEEE: jge         0055DF5E
  0055DEF0: mov         ecx,dword ptr [ebp-4]
  0055DEF3: push        ecx
  0055DEF4: mov         ecx,dword ptr [ebp-28h]
  0055DEF7: call        0055E5D2
  0055DEFC: mov         edx,dword ptr [ebp-28h]
  0055DEFF: mov         eax,dword ptr [edx+0000019Ch]
  0055DF05: mov         ecx,dword ptr [ebp-4]
  0055DF08: mov         edx,dword ptr [eax+ecx*4]
  0055DF0B: push        edx
  0055DF0C: call        dword ptr ds:[0056F2F0h]
  0055DF12: mov         eax,dword ptr [ebp-4]
  0055DF15: imul        eax,eax,0Ch
  0055DF18: mov         ecx,dword ptr [ebp-28h]
  0055DF1B: mov         edx,dword ptr [ecx+000001ACh]
  0055DF21: cmp         dword ptr [edx+eax+4],0
  0055DF26: je          0055DF59
  0055DF28: mov         eax,dword ptr [ebp-4]
  0055DF2B: imul        eax,eax,0Ch
  0055DF2E: mov         ecx,dword ptr [ebp-28h]
  0055DF31: mov         edx,dword ptr [ecx+000001ACh]
  0055DF37: mov         eax,dword ptr [edx+eax+4]
  0055DF3B: push        eax
  0055DF3C: call        dword ptr ds:[0056F308h]
  0055DF42: mov         ecx,dword ptr [ebp-4]
  0055DF45: imul        ecx,ecx,0Ch
  0055DF48: mov         edx,dword ptr [ebp-28h]
  0055DF4B: mov         eax,dword ptr [edx+000001ACh]
  0055DF51: mov         dword ptr [eax+ecx+4],0
  0055DF59: jmp         0055DED9
  0055DF5E: mov         ecx,dword ptr [ebp-28h]
  0055DF61: cmp         dword ptr [ecx+0000019Ch],0
  0055DF68: je          0055DF8F
  0055DF6A: mov         edx,dword ptr [ebp-28h]
  0055DF6D: mov         eax,dword ptr [edx+0000019Ch]
  0055DF73: mov         dword ptr [ebp-8],eax
  0055DF76: mov         ecx,dword ptr [ebp-8]
  0055DF79: push        ecx
  0055DF7A: call        00561A2B
  0055DF7F: add         esp,4
  0055DF82: mov         edx,dword ptr [ebp-28h]
  0055DF85: mov         dword ptr [edx+0000019Ch],0
  0055DF8F: mov         eax,dword ptr [ebp-28h]
  0055DF92: cmp         dword ptr [eax+000001A0h],0
  0055DF99: je          0055DFC0
  0055DF9B: mov         ecx,dword ptr [ebp-28h]
  0055DF9E: mov         edx,dword ptr [ecx+000001A0h]
  0055DFA4: mov         dword ptr [ebp-0Ch],edx
  0055DFA7: mov         eax,dword ptr [ebp-0Ch]
  0055DFAA: push        eax
  0055DFAB: call        00561A2B
  0055DFB0: add         esp,4
  0055DFB3: mov         ecx,dword ptr [ebp-28h]
  0055DFB6: mov         dword ptr [ecx+000001A0h],0
  0055DFC0: mov         edx,dword ptr [ebp-28h]
  0055DFC3: cmp         dword ptr [edx+000001A8h],0
  0055DFCA: je          0055DFF1
  0055DFCC: mov         eax,dword ptr [ebp-28h]
  0055DFCF: mov         ecx,dword ptr [eax+000001A8h]
  0055DFD5: mov         dword ptr [ebp-10h],ecx
  0055DFD8: mov         edx,dword ptr [ebp-10h]
  0055DFDB: push        edx
  0055DFDC: call        00561A2B
  0055DFE1: add         esp,4
  0055DFE4: mov         eax,dword ptr [ebp-28h]
  0055DFE7: mov         dword ptr [eax+000001A8h],0
  0055DFF1: mov         ecx,dword ptr [ebp-28h]
  0055DFF4: cmp         dword ptr [ecx+000001ACh],0
  0055DFFB: je          0055E022
  0055DFFD: mov         edx,dword ptr [ebp-28h]
  0055E000: mov         eax,dword ptr [edx+000001ACh]
  0055E006: mov         dword ptr [ebp-14h],eax
  0055E009: mov         ecx,dword ptr [ebp-14h]
  0055E00C: push        ecx
  0055E00D: call        00561A2B
  0055E012: add         esp,4
  0055E015: mov         edx,dword ptr [ebp-28h]
  0055E018: mov         dword ptr [edx+000001ACh],0
  0055E022: mov         eax,dword ptr [ebp+8]
  0055E025: shl         eax,2
  0055E028: push        eax
  0055E029: call        00561A1D
  0055E02E: add         esp,4
  0055E031: mov         dword ptr [ebp-18h],eax
  0055E034: mov         ecx,dword ptr [ebp-28h]
  0055E037: mov         edx,dword ptr [ebp-18h]
  0055E03A: mov         dword ptr [ecx+0000019Ch],edx
  0055E040: mov         eax,dword ptr [ebp-28h]
  0055E043: cmp         dword ptr [eax+0000019Ch],0
  0055E04A: jne         0055E053
  0055E04C: xor         eax,eax
  0055E04E: jmp         0055E149
  0055E053: mov         ecx,dword ptr [ebp+8]
  0055E056: shl         ecx,2
  0055E059: push        ecx
  0055E05A: call        00561A1D
  0055E05F: add         esp,4
  0055E062: mov         dword ptr [ebp-1Ch],eax
  0055E065: mov         edx,dword ptr [ebp-28h]
  0055E068: mov         eax,dword ptr [ebp-1Ch]
  0055E06B: mov         dword ptr [edx+000001A0h],eax
  0055E071: mov         ecx,dword ptr [ebp-28h]
  0055E074: cmp         dword ptr [ecx+000001A0h],0
  0055E07B: jne         0055E084
  0055E07D: xor         eax,eax
  0055E07F: jmp         0055E149
  0055E084: mov         edx,dword ptr [ebp+8]
  0055E087: imul        edx,edx,28h
  0055E08A: push        edx
  0055E08B: call        00561A1D
  0055E090: add         esp,4
  0055E093: mov         dword ptr [ebp-20h],eax
  0055E096: mov         eax,dword ptr [ebp-28h]
  0055E099: mov         ecx,dword ptr [ebp-20h]
  0055E09C: mov         dword ptr [eax+000001A8h],ecx
  0055E0A2: mov         edx,dword ptr [ebp-28h]
  0055E0A5: cmp         dword ptr [edx+000001A8h],0
  0055E0AC: jne         0055E0B5
  0055E0AE: xor         eax,eax
  0055E0B0: jmp         0055E149
  0055E0B5: mov         eax,dword ptr [ebp+8]
  0055E0B8: imul        eax,eax,0Ch
  0055E0BB: push        eax
  0055E0BC: call        00561A1D
  0055E0C1: add         esp,4
  0055E0C4: mov         dword ptr [ebp-24h],eax
  0055E0C7: mov         ecx,dword ptr [ebp-28h]
  0055E0CA: mov         edx,dword ptr [ebp-24h]
  0055E0CD: mov         dword ptr [ecx+000001ACh],edx
  0055E0D3: mov         eax,dword ptr [ebp-28h]
  0055E0D6: cmp         dword ptr [eax+000001ACh],0
  0055E0DD: jne         0055E0E3
  0055E0DF: xor         eax,eax
  0055E0E1: jmp         0055E149
  0055E0E3: mov         ecx,dword ptr [ebp-28h]
  0055E0E6: mov         edx,dword ptr [ebp+8]
  0055E0E9: mov         dword ptr [ecx+00000198h],edx
  0055E0EF: mov         dword ptr [ebp-4],0
  0055E0F6: jmp         0055E101
  0055E0F8: mov         eax,dword ptr [ebp-4]
  0055E0FB: add         eax,1
  0055E0FE: mov         dword ptr [ebp-4],eax
  0055E101: mov         ecx,dword ptr [ebp-28h]
  0055E104: mov         edx,dword ptr [ebp-4]
  0055E107: cmp         edx,dword ptr [ecx+00000198h]
  0055E10D: jge         0055E144
  0055E10F: mov         eax,dword ptr [ebp-28h]
  0055E112: mov         ecx,dword ptr [eax+00000178h]
  0055E118: push        ecx
  0055E119: call        dword ptr ds:[0056F2F4h]
  0055E11F: mov         edx,dword ptr [ebp-28h]
  0055E122: mov         ecx,dword ptr [edx+0000019Ch]
  0055E128: mov         edx,dword ptr [ebp-4]
  0055E12B: mov         dword ptr [ecx+edx*4],eax
  0055E12E: mov         eax,dword ptr [ebp-28h]
  0055E131: mov         ecx,dword ptr [eax+0000019Ch]
  0055E137: mov         edx,dword ptr [ebp-4]
  0055E13A: cmp         dword ptr [ecx+edx*4],0
  0055E13E: jne         0055E142
  0055E140: jmp         0055E144
  0055E142: jmp         0055E0F8
  0055E144: mov         eax,1
  0055E149: mov         esp,ebp
  0055E14B: pop         ebp
  0055E14C: ret         4
  0055E14F: push        ebp
  0055E150: mov         ebp,esp
  0055E152: sub         esp,20h
  0055E155: mov         dword ptr [ebp-20h],ecx
  0055E158: cmp         dword ptr [ebp+8],1
  0055E15C: jge         0055E165
  0055E15E: xor         eax,eax
  0055E160: jmp         0055E385
  0055E165: mov         eax,dword ptr [ebp-20h]
  0055E168: cmp         dword ptr [eax+000001F0h],0
  0055E16F: je          0055E1FC
  0055E175: mov         ecx,dword ptr [ebp-20h]
  0055E178: call        0055EC7C
  0055E17D: mov         dword ptr [ebp-4],0
  0055E184: jmp         0055E18F
  0055E186: mov         ecx,dword ptr [ebp-4]
  0055E189: add         ecx,1
  0055E18C: mov         dword ptr [ebp-4],ecx
  0055E18F: mov         edx,dword ptr [ebp-20h]
  0055E192: mov         eax,dword ptr [ebp-4]
  0055E195: cmp         eax,dword ptr [edx+000001E4h]
  0055E19B: jge         0055E1FC
  0055E19D: mov         ecx,dword ptr [ebp-20h]
  0055E1A0: mov         edx,dword ptr [ecx+000001F0h]
  0055E1A6: mov         eax,dword ptr [ebp-4]
  0055E1A9: mov         ecx,dword ptr [edx+eax*4]
  0055E1AC: push        ecx
  0055E1AD: call        dword ptr ds:[0056F2E8h]
  0055E1B3: mov         edx,dword ptr [ebp-4]
  0055E1B6: imul        edx,edx,0Ch
  0055E1B9: mov         eax,dword ptr [ebp-20h]
  0055E1BC: mov         ecx,dword ptr [eax+000001F8h]
  0055E1C2: cmp         dword ptr [ecx+edx+4],0
  0055E1C7: je          0055E1FA
  0055E1C9: mov         edx,dword ptr [ebp-4]
  0055E1CC: imul        edx,edx,0Ch
  0055E1CF: mov         eax,dword ptr [ebp-20h]
  0055E1D2: mov         ecx,dword ptr [eax+000001F8h]
  0055E1D8: mov         edx,dword ptr [ecx+edx+4]
  0055E1DC: push        edx
  0055E1DD: call        dword ptr ds:[0056F308h]
  0055E1E3: mov         eax,dword ptr [ebp-4]
  0055E1E6: imul        eax,eax,0Ch
  0055E1E9: mov         ecx,dword ptr [ebp-20h]
  0055E1EC: mov         edx,dword ptr [ecx+000001F8h]
  0055E1F2: mov         dword ptr [edx+eax+4],0
  0055E1FA: jmp         0055E186
  0055E1FC: mov         eax,dword ptr [ebp-20h]
  0055E1FF: cmp         dword ptr [eax+000001F0h],0
  0055E206: je          0055E22D
  0055E208: mov         ecx,dword ptr [ebp-20h]
  0055E20B: mov         edx,dword ptr [ecx+000001F0h]
  0055E211: mov         dword ptr [ebp-8],edx
  0055E214: mov         eax,dword ptr [ebp-8]
  0055E217: push        eax
  0055E218: call        00561A2B
  0055E21D: add         esp,4
  0055E220: mov         ecx,dword ptr [ebp-20h]
  0055E223: mov         dword ptr [ecx+000001F0h],0
  0055E22D: mov         edx,dword ptr [ebp-20h]
  0055E230: cmp         dword ptr [edx+000001F4h],0
  0055E237: je          0055E25E
  0055E239: mov         eax,dword ptr [ebp-20h]
  0055E23C: mov         ecx,dword ptr [eax+000001F4h]
  0055E242: mov         dword ptr [ebp-0Ch],ecx
  0055E245: mov         edx,dword ptr [ebp-0Ch]
  0055E248: push        edx
  0055E249: call        00561A2B
  0055E24E: add         esp,4
  0055E251: mov         eax,dword ptr [ebp-20h]
  0055E254: mov         dword ptr [eax+000001F4h],0
  0055E25E: mov         ecx,dword ptr [ebp-20h]
  0055E261: cmp         dword ptr [ecx+000001F8h],0
  0055E268: je          0055E28F
  0055E26A: mov         edx,dword ptr [ebp-20h]
  0055E26D: mov         eax,dword ptr [edx+000001F8h]
  0055E273: mov         dword ptr [ebp-10h],eax
  0055E276: mov         ecx,dword ptr [ebp-10h]
  0055E279: push        ecx
  0055E27A: call        00561A2B
  0055E27F: add         esp,4
  0055E282: mov         edx,dword ptr [ebp-20h]
  0055E285: mov         dword ptr [edx+000001F8h],0
  0055E28F: mov         eax,dword ptr [ebp+8]
  0055E292: shl         eax,2
  0055E295: push        eax
  0055E296: call        00561A1D
  0055E29B: add         esp,4
  0055E29E: mov         dword ptr [ebp-14h],eax
  0055E2A1: mov         ecx,dword ptr [ebp-20h]
  0055E2A4: mov         edx,dword ptr [ebp-14h]
  0055E2A7: mov         dword ptr [ecx+000001F0h],edx
  0055E2AD: mov         eax,dword ptr [ebp-20h]
  0055E2B0: cmp         dword ptr [eax+000001F0h],0
  0055E2B7: jne         0055E2C0
  0055E2B9: xor         eax,eax
  0055E2BB: jmp         0055E385
  0055E2C0: mov         ecx,dword ptr [ebp+8]
  0055E2C3: imul        ecx,ecx,28h
  0055E2C6: push        ecx
  0055E2C7: call        00561A1D
  0055E2CC: add         esp,4
  0055E2CF: mov         dword ptr [ebp-18h],eax
  0055E2D2: mov         edx,dword ptr [ebp-20h]
  0055E2D5: mov         eax,dword ptr [ebp-18h]
  0055E2D8: mov         dword ptr [edx+000001F4h],eax
  0055E2DE: mov         ecx,dword ptr [ebp-20h]
  0055E2E1: cmp         dword ptr [ecx+000001F4h],0
  0055E2E8: jne         0055E2F1
  0055E2EA: xor         eax,eax
  0055E2EC: jmp         0055E385
  0055E2F1: mov         edx,dword ptr [ebp+8]
  0055E2F4: imul        edx,edx,0Ch
  0055E2F7: push        edx
  0055E2F8: call        00561A1D
  0055E2FD: add         esp,4
  0055E300: mov         dword ptr [ebp-1Ch],eax
  0055E303: mov         eax,dword ptr [ebp-20h]
  0055E306: mov         ecx,dword ptr [ebp-1Ch]
  0055E309: mov         dword ptr [eax+000001F8h],ecx
  0055E30F: mov         edx,dword ptr [ebp-20h]
  0055E312: cmp         dword ptr [edx+000001F8h],0
  0055E319: jne         0055E31F
  0055E31B: xor         eax,eax
  0055E31D: jmp         0055E385
  0055E31F: mov         eax,dword ptr [ebp-20h]
  0055E322: mov         ecx,dword ptr [ebp+8]
  0055E325: mov         dword ptr [eax+000001E4h],ecx
  0055E32B: mov         dword ptr [ebp-4],0
  0055E332: jmp         0055E33D
  0055E334: mov         edx,dword ptr [ebp-4]
  0055E337: add         edx,1
  0055E33A: mov         dword ptr [ebp-4],edx
  0055E33D: mov         eax,dword ptr [ebp-20h]
  0055E340: mov         ecx,dword ptr [ebp-4]
  0055E343: cmp         ecx,dword ptr [eax+000001E4h]
  0055E349: jge         0055E380
  0055E34B: mov         edx,dword ptr [ebp-20h]
  0055E34E: mov         eax,dword ptr [edx+000001E8h]
  0055E354: push        eax
  0055E355: call        dword ptr ds:[0056F2ECh]
  0055E35B: mov         ecx,dword ptr [ebp-20h]
  0055E35E: mov         edx,dword ptr [ecx+000001F0h]
  0055E364: mov         ecx,dword ptr [ebp-4]
  0055E367: mov         dword ptr [edx+ecx*4],eax
  0055E36A: mov         edx,dword ptr [ebp-20h]
  0055E36D: mov         eax,dword ptr [edx+000001F0h]
  0055E373: mov         ecx,dword ptr [ebp-4]
  0055E376: cmp         dword ptr [eax+ecx*4],0
  0055E37A: jne         0055E37E
  0055E37C: jmp         0055E380
  0055E37E: jmp         0055E334
  0055E380: mov         eax,1
  0055E385: mov         esp,ebp
  0055E387: pop         ebp
  0055E388: ret         4
  0055E38B: push        ebp
  0055E38C: mov         ebp,esp
  0055E38E: sub         esp,8
  0055E391: mov         dword ptr [ebp-8],ecx
  0055E394: mov         ecx,dword ptr [ebp-8]
  0055E397: call        0055DBC0
  0055E39C: mov         eax,dword ptr [ebp-8]
  0055E39F: mov         dword ptr [eax+00000200h],0
  0055E3A9: mov         ecx,dword ptr [ebp-8]
  0055E3AC: mov         dword ptr [ecx+0000019Ch],0
  0055E3B6: mov         edx,dword ptr [ebp-8]
  0055E3B9: mov         dword ptr [edx+000001A0h],0
  0055E3C3: mov         eax,dword ptr [ebp-8]
  0055E3C6: mov         dword ptr [eax+000001A8h],0
  0055E3D0: mov         ecx,dword ptr [ebp-8]
  0055E3D3: mov         dword ptr [ecx+000001ACh],0
  0055E3DD: mov         edx,dword ptr [ebp-8]
  0055E3E0: mov         dword ptr [edx+000001F0h],0
  0055E3EA: mov         eax,dword ptr [ebp-8]
  0055E3ED: mov         dword ptr [eax+000001F4h],0
  0055E3F7: mov         ecx,dword ptr [ebp-8]
  0055E3FA: mov         dword ptr [ecx+000001F8h],0
  0055E404: mov         edx,dword ptr [ebp-8]
  0055E407: mov         dword ptr [edx+0000018Ch],5622h
  0055E411: mov         eax,dword ptr [ebp-8]
  0055E414: mov         dword ptr [eax+00000190h],10h
  0055E41E: mov         ecx,dword ptr [ebp-8]
  0055E421: mov         dword ptr [ecx+00000194h],2
  0055E42B: push        28h
  0055E42D: push        0FFFFFFFFh
  0055E42F: mov         edx,dword ptr [ebp-8]
  0055E432: add         edx,1B0h
  0055E438: push        edx
  0055E439: call        00563240
  0055E43E: add         esp,0Ch
  0055E441: mov         eax,dword ptr [ebp-8]
  0055E444: mov         dword ptr [eax+0000014Ch],64h
  0055E44E: mov         ecx,dword ptr [ebp-8]
  0055E451: mov         dword ptr [ecx+00000150h],64h
  0055E45B: mov         edx,dword ptr [ebp-8]
  0055E45E: mov         dword ptr [edx+00000154h],64h
  0055E468: mov         eax,dword ptr [ebp-8]
  0055E46B: mov         dword ptr [eax+00000158h],64h
  0055E475: mov         ecx,dword ptr [ebp-8]
  0055E478: mov         dword ptr [ecx+000001ECh],0
  0055E482: mov         edx,dword ptr [ebp-8]
  0055E485: mov         dword ptr [edx+000001FCh],0
  0055E48F: mov         dword ptr [ebp-4],0
  0055E496: jmp         0055E4A1
  0055E498: mov         eax,dword ptr [ebp-4]
  0055E49B: add         eax,1
  0055E49E: mov         dword ptr [ebp-4],eax
  0055E4A1: cmp         dword ptr [ebp-4],3
  0055E4A5: jge         0055E4BA
  0055E4A7: mov         ecx,dword ptr [ebp-4]
  0055E4AA: mov         edx,dword ptr [ebp-8]
  0055E4AD: mov         dword ptr [edx+ecx*4+000001D8h],0
  0055E4B8: jmp         0055E498
  0055E4BA: mov         dword ptr [ebp-4],0
  0055E4C1: jmp         0055E4CC
  0055E4C3: mov         eax,dword ptr [ebp-4]
  0055E4C6: add         eax,1
  0055E4C9: mov         dword ptr [ebp-4],eax
  0055E4CC: cmp         dword ptr [ebp-4],1Ah
  0055E4D0: jge         0055E51F
  0055E4D2: mov         ecx,dword ptr [ebp-4]
  0055E4D5: imul        ecx,ecx,0Ch
  0055E4D8: mov         edx,dword ptr [ebp-4]
  0055E4DB: imul        edx,edx,0Ch
  0055E4DE: mov         eax,dword ptr [ebp-8]
  0055E4E1: mov         ecx,dword ptr [ecx+00604628h]
  0055E4E7: mov         dword ptr [eax+edx+0Ch],ecx
  0055E4EB: mov         edx,dword ptr [ebp-4]
  0055E4EE: imul        edx,edx,0Ch
  0055E4F1: mov         eax,dword ptr [ebp-4]
  0055E4F4: imul        eax,eax,0Ch
  0055E4F7: mov         ecx,dword ptr [ebp-8]
  0055E4FA: mov         edx,dword ptr [edx+0060462Ch]
  0055E500: mov         dword ptr [ecx+eax+10h],edx
  0055E504: mov         eax,dword ptr [ebp-4]
  0055E507: imul        eax,eax,0Ch
  0055E50A: mov         ecx,dword ptr [ebp-4]
  0055E50D: imul        ecx,ecx,0Ch
  0055E510: mov         edx,dword ptr [ebp-8]
  0055E513: mov         eax,dword ptr [eax+00604630h]
  0055E519: mov         dword ptr [edx+ecx+14h],eax
  0055E51D: jmp         0055E4C3
  0055E51F: mov         ecx,dword ptr [ebp-8]
  0055E522: mov         dword ptr [ecx+00000148h],0FFFFFFFFh
  0055E52C: mov         edx,dword ptr [ebp-8]
  0055E52F: mov         dword ptr [edx+00000144h],1
  0055E539: mov         eax,dword ptr [ebp-8]
  0055E53C: mov         dword ptr [eax+00000168h],0
  0055E546: mov         ecx,dword ptr [ebp-8]
  0055E549: mov         dword ptr [ecx+0000016Ch],3F000000h
  0055E553: mov         edx,dword ptr [ebp-8]
  0055E556: mov         dword ptr [edx+00000170h],3D75C28Fh
  0055E560: mov         eax,dword ptr [ebp-8]
  0055E563: mov         dword ptr [eax+00000174h],3F000000h
  0055E56D: mov         ecx,dword ptr [ebp-8]
  0055E570: mov         dword ptr [ecx+00000164h],5
  0055E57A: mov         edx,dword ptr [ebp-8]
  0055E57D: mov         dword ptr ds:[012663ACh],edx
  0055E583: mov         eax,dword ptr [ebp-8]
  0055E586: mov         esp,ebp
  0055E588: pop         ebp
  0055E589: ret
  0055E58A: push        ebp
  0055E58B: mov         ebp,esp
  0055E58D: push        0FFFFFFFFh
  0055E58F: push        56E171h
  0055E594: mov         eax,dword ptr fs:[00000000h]
  0055E59A: push        eax
  0055E59B: mov         dword ptr fs:[00000000h],esp
  0055E5A2: push        ecx
  0055E5A3: mov         dword ptr [ebp-10h],ecx
  0055E5A6: mov         dword ptr [ebp-4],0
  0055E5AD: mov         ecx,dword ptr [ebp-10h]
  0055E5B0: call        0055E92B
  0055E5B5: mov         dword ptr [ebp-4],0FFFFFFFFh
  0055E5BC: mov         ecx,dword ptr [ebp-10h]
  0055E5BF: call        0055DBF8
  0055E5C4: mov         ecx,dword ptr [ebp-0Ch]
  0055E5C7: mov         dword ptr fs:[00000000h],ecx
  0055E5CE: mov         esp,ebp
  0055E5D0: pop         ebp
  0055E5D1: ret
  0055E5D2: push        ebp
  0055E5D3: mov         ebp,esp
  0055E5D5: push        ecx
  0055E5D6: mov         dword ptr [ebp-4],ecx
  0055E5D9: mov         eax,dword ptr [ebp+8]
  0055E5DC: imul        eax,eax,28h
  0055E5DF: mov         ecx,dword ptr [ebp-4]
  0055E5E2: mov         edx,dword ptr [ecx+000001A8h]
  0055E5E8: mov         dword ptr [edx+eax],0FFFFFFFFh
  0055E5EF: mov         eax,dword ptr [ebp+8]
  0055E5F2: imul        eax,eax,28h
  0055E5F5: mov         ecx,dword ptr [ebp-4]
  0055E5F8: mov         edx,dword ptr [ecx+000001A8h]
  0055E5FE: mov         dword ptr [edx+eax+4],0FFFFFFFFh
  0055E606: mov         eax,dword ptr [ebp+8]
  0055E609: imul        eax,eax,28h
  0055E60C: mov         ecx,dword ptr [ebp-4]
  0055E60F: mov         edx,dword ptr [ecx+000001A8h]
  0055E615: mov         dword ptr [edx+eax+8],64h
  0055E61D: mov         eax,dword ptr [ebp+8]
  0055E620: imul        eax,eax,28h
  0055E623: mov         ecx,dword ptr [ebp-4]
  0055E626: mov         edx,dword ptr [ecx+000001A8h]
  0055E62C: mov         dword ptr [edx+eax+0Ch],40h
  0055E634: mov         eax,dword ptr [ebp+8]
  0055E637: imul        eax,eax,28h
  0055E63A: mov         ecx,dword ptr [ebp-4]
  0055E63D: mov         edx,dword ptr [ecx+000001A8h]
  0055E643: mov         dword ptr [edx+eax+10h],1
  0055E64B: mov         eax,dword ptr [ebp+8]
  0055E64E: imul        eax,eax,28h
  0055E651: mov         ecx,dword ptr [ebp-4]
  0055E654: mov         edx,dword ptr [ecx+000001A8h]
  0055E65A: mov         dword ptr [edx+eax+14h],0
  0055E662: mov         eax,dword ptr [ebp+8]
  0055E665: imul        eax,eax,28h
  0055E668: mov         ecx,dword ptr [ebp-4]
  0055E66B: mov         edx,dword ptr [ecx+000001A8h]
  0055E671: mov         dword ptr [edx+eax+18h],0FFFFFFFFh
  0055E679: mov         eax,dword ptr [ebp+8]
  0055E67C: imul        eax,eax,28h
  0055E67F: mov         ecx,dword ptr [ebp-4]
  0055E682: mov         edx,dword ptr [ecx+000001A8h]
  0055E688: mov         dword ptr [edx+eax+1Ch],0
  0055E690: mov         eax,dword ptr [ebp+8]
  0055E693: imul        eax,eax,28h
  0055E696: mov         ecx,dword ptr [ebp-4]
  0055E699: mov         edx,dword ptr [ecx+000001A8h]
  0055E69F: mov         dword ptr [edx+eax+20h],0
  0055E6A7: mov         eax,dword ptr [ebp+8]
  0055E6AA: imul        eax,eax,28h
  0055E6AD: mov         ecx,dword ptr [ebp-4]
  0055E6B0: mov         edx,dword ptr [ecx+000001A8h]
  0055E6B6: mov         dword ptr [edx+eax+24h],0
  0055E6BE: mov         esp,ebp
  0055E6C0: pop         ebp
  0055E6C1: ret         4
  0055E6C4: push        ebp
  0055E6C5: mov         ebp,esp
  0055E6C7: sub         esp,0A0h
  0055E6CD: mov         dword ptr [ebp+FFFFFF60h],ecx
  0055E6D3: mov         dword ptr [ebp+FFFFFF68h],94h
  0055E6DD: lea         eax,[ebp+FFFFFF68h]
  0055E6E3: push        eax
  0055E6E4: call        dword ptr ds:[0056F124h]
  0055E6EA: test        eax,eax
  0055E6EC: jne         0055E6F5
  0055E6EE: xor         eax,eax
  0055E6F0: jmp         0055E927
  0055E6F5: cmp         dword ptr [ebp+FFFFFF78h],2
  0055E6FC: jne         0055E71E
  0055E6FE: cmp         dword ptr [ebp+FFFFFF6Ch],5
  0055E705: jae         0055E71E
  0055E707: mov         ecx,dword ptr [ebp+FFFFFF60h]
  0055E70D: mov         dword ptr [ecx+00000200h],1
  0055E717: xor         eax,eax
  0055E719: jmp         0055E927
  0055E71E: call        dword ptr ds:[0056F2DCh]
  0055E724: push        40000h
  0055E729: push        28h
  0055E72B: call        dword ptr ds:[0056F2E0h]
  0055E731: push        0
  0055E733: push        0Fh
  0055E735: call        dword ptr ds:[0056F2E0h]
  0055E73B: mov         edx,dword ptr [ebp+FFFFFF60h]
  0055E741: mov         word ptr [edx+0000017Ch],1
  0055E74A: mov         eax,dword ptr [ebp+FFFFFF60h]
  0055E750: mov         ecx,dword ptr [ebp+FFFFFF60h]
  0055E756: mov         dx,word ptr [ecx+00000194h]
  0055E75D: mov         word ptr [eax+0000017Eh],dx
  0055E764: mov         eax,dword ptr [ebp+FFFFFF60h]
  0055E76A: mov         ecx,dword ptr [ebp+FFFFFF60h]
  0055E770: mov         edx,dword ptr [ecx+0000018Ch]
  0055E776: mov         dword ptr [eax+00000180h],edx
  0055E77C: mov         eax,dword ptr [ebp+FFFFFF60h]
  0055E782: mov         eax,dword ptr [eax+00000190h]
  0055E788: cdq
  0055E789: and         edx,7
  0055E78C: add         eax,edx
  0055E78E: sar         eax,3
  0055E791: mov         ecx,dword ptr [ebp+FFFFFF60h]
  0055E797: mov         edx,dword ptr [ecx+0000018Ch]
  0055E79D: imul        edx,eax
  0055E7A0: mov         eax,dword ptr [ebp+FFFFFF60h]
  0055E7A6: imul        edx,dword ptr [eax+00000194h]
  0055E7AD: mov         ecx,dword ptr [ebp+FFFFFF60h]
  0055E7B3: mov         dword ptr [ecx+00000184h],edx
  0055E7B9: mov         edx,dword ptr [ebp+FFFFFF60h]
  0055E7BF: mov         eax,dword ptr [edx+00000190h]
  0055E7C5: cdq
  0055E7C6: and         edx,7
  0055E7C9: add         eax,edx
  0055E7CB: sar         eax,3
  0055E7CE: mov         ecx,dword ptr [ebp+FFFFFF60h]
  0055E7D4: imul        eax,dword ptr [ecx+00000194h]
  0055E7DB: mov         edx,dword ptr [ebp+FFFFFF60h]
  0055E7E1: mov         word ptr [edx+00000188h],ax
  0055E7E8: mov         eax,dword ptr [ebp+FFFFFF60h]
  0055E7EE: mov         ecx,dword ptr [ebp+FFFFFF60h]
  0055E7F4: mov         dx,word ptr [ecx+00000190h]
  0055E7FB: mov         word ptr [eax+0000018Ah],dx
  0055E802: mov         eax,dword ptr [ebp+FFFFFF60h]
  0055E808: add         eax,17Ch
  0055E80D: push        eax
  0055E80E: push        0FFFFFFFFh
  0055E810: push        0
  0055E812: mov         ecx,dword ptr [ebp+FFFFFF60h]
  0055E818: add         ecx,178h
  0055E81E: push        ecx
  0055E81F: call        dword ptr ds:[0056F2E4h]
  0055E825: mov         dword ptr [ebp-4],eax
  0055E828: cmp         dword ptr [ebp-4],0
  0055E82C: je          0055E840
  0055E82E: push        604760h
  0055E833: call        dword ptr ds:[0056F0F8h]
  0055E839: xor         eax,eax
  0055E83B: jmp         0055E927
  0055E840: push        5
  0055E842: mov         ecx,dword ptr [ebp+FFFFFF60h]
  0055E848: call        0055DEA2
  0055E84D: test        eax,eax
  0055E84F: jne         0055E858
  0055E851: xor         eax,eax
  0055E853: jmp         0055E927
  0055E858: mov         dword ptr [ebp+FFFFFF64h],0
  0055E862: jmp         0055E873
  0055E864: mov         edx,dword ptr [ebp+FFFFFF64h]
  0055E86A: add         edx,1
  0055E86D: mov         dword ptr [ebp+FFFFFF64h],edx
  0055E873: mov         eax,dword ptr [ebp+FFFFFF60h]
  0055E879: mov         ecx,dword ptr [ebp+FFFFFF64h]
  0055E87F: cmp         ecx,dword ptr [eax+00000198h]
  0055E885: jge         0055E89B
  0055E887: mov         edx,dword ptr [ebp+FFFFFF64h]
  0055E88D: push        edx
  0055E88E: mov         ecx,dword ptr [ebp+FFFFFF60h]
  0055E894: call        0055E5D2
  0055E899: jmp         0055E864
  0055E89B: mov         dword ptr [ebp+FFFFFF64h],0
  0055E8A5: jmp         0055E8B6
  0055E8A7: mov         eax,dword ptr [ebp+FFFFFF64h]
  0055E8AD: add         eax,1
  0055E8B0: mov         dword ptr [ebp+FFFFFF64h],eax
  0055E8B6: mov         ecx,dword ptr [ebp+FFFFFF60h]
  0055E8BC: mov         edx,dword ptr [ebp+FFFFFF64h]
  0055E8C2: cmp         edx,dword ptr [ecx+00000198h]
  0055E8C8: jge         0055E922
  0055E8CA: mov         eax,dword ptr [ebp+FFFFFF64h]
  0055E8D0: imul        eax,eax,0Ch
  0055E8D3: mov         ecx,dword ptr [ebp+FFFFFF60h]
  0055E8D9: mov         edx,dword ptr [ecx+000001ACh]
  0055E8DF: mov         dword ptr [edx+eax],0FFFFFFFFh
  0055E8E6: mov         eax,dword ptr [ebp+FFFFFF64h]
  0055E8EC: imul        eax,eax,0Ch
  0055E8EF: mov         ecx,dword ptr [ebp+FFFFFF60h]
  0055E8F5: mov         edx,dword ptr [ecx+000001ACh]
  0055E8FB: mov         dword ptr [edx+eax+4],0
  0055E903: mov         eax,dword ptr [ebp+FFFFFF64h]
  0055E909: imul        eax,eax,0Ch
  0055E90C: mov         ecx,dword ptr [ebp+FFFFFF60h]
  0055E912: mov         edx,dword ptr [ecx+000001ACh]
  0055E918: mov         dword ptr [edx+eax+8],0
  0055E920: jmp         0055E8A7
  0055E922: mov         eax,1
  0055E927: mov         esp,ebp
  0055E929: pop         ebp
  0055E92A: ret
  0055E92B: push        ebp

  Summary

      16E000 .text
