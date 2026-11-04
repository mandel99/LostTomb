
Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  00414BD0: call        0051E930
  00414BD5: push        1
  00414BD7: push        1265170h
  00414BDC: call        0053F790
  00414BE1: mov         eax,dword ptr ds:[00E92E1Ch]
  00414BE6: add         esp,8
  00414BE9: test        eax,eax
  00414BEB: je          00414BF6
  00414BED: push        eax
  00414BEE: call        00561BDC
  00414BF3: add         esp,4
  00414BF6: mov         eax,dword ptr ds:[00E38DACh]
  00414BFB: imul        eax,dword ptr ds:[00E38DB0h]
  00414C02: shl         eax,1
  00414C04: cmp         eax,124F80h
  00414C09: jg          00414C10
  00414C0B: mov         eax,124F80h
  00414C10: push        eax
  00414C11: call        00561B68
  00414C16: mov         dword ptr ds:[00E92E1Ch],eax
  00414C1B: mov         eax,dword ptr ds:[00F53F78h]
  00414C20: add         esp,4
  00414C23: cmp         eax,1
  00414C26: jne         00414C2F
  00414C28: call        004DDEC0
  00414C2D: jmp         00414C34
  00414C2F: call        004DDF20
  00414C34: call        004DDF90
  00414C39: jmp         004DE080
  00414C3E: nop

  Summary

      16E000 .text
