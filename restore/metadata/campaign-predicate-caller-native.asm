
Dump of file build\smoke-game\Pharaoh.exe

File Type: EXECUTABLE IMAGE

  004CB6A0: mov         eax,dword ptr fs:[00000000h]
  004CB6A6: push        0FFFFFFFFh
  004CB6A8: push        56B69Bh
  004CB6AD: push        eax
  004CB6AE: mov         dword ptr fs:[00000000h],esp
  004CB6B5: sub         esp,120h
  004CB6BB: mov         ecx,6FDD08h
  004CB6C0: call        0041BAB0
  004CB6C5: mov         byte ptr ds:[00D3B189h],al
  004CB6CA: mov         al,byte ptr ds:[00700998h]
  004CB6CF: test        al,al
  004CB6D1: mov         byte ptr ds:[00D3B188h],1
  004CB6D8: mov         byte ptr ds:[00D3B18Bh],0
  004CB6DF: mov         byte ptr ds:[00D3B18Ah],0
  004CB6E6: je          004CB70E
  004CB6E8: push        5763BCh
  004CB6ED: push        700998h
  004CB6F2: call        00561A0A
  004CB6F7: add         esp,8
  004CB6FA: test        eax,eax
  004CB6FC: je          004CB70E
  004CB6FE: push        eax
  004CB6FF: E8
  004CB700: nop

  Summary

      16E000 .text
