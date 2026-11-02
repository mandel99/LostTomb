"""Small PE loader for owned native-oracle tests."""
from pathlib import Path
import struct

class PE:
    def __init__(self,path):
        raw=Path(path).read_bytes()
        u16=lambda p:struct.unpack_from('<H',raw,p)[0]
        u32=lambda p:struct.unpack_from('<I',raw,p)[0]
        pe=u32(60);opt=pe+24;table=opt+u16(pe+20)
        self.base=u32(opt+28);self.exports=u32(opt+96)
        self.data=bytearray(u32(opt+56));self.data[:u32(opt+60)]=raw[:u32(opt+60)]
        for j in range(u16(pe+6)):
            p=table+j*40;va,n,offset=u32(p+12),u32(p+16),u32(p+20)
            self.data[va:va+n]=raw[offset:offset+n]
    def entry(self,name):
        u32=lambda p:struct.unpack_from('<I',self.data,p)[0]
        u16=lambda p:struct.unpack_from('<H',self.data,p)[0]
        e=self.exports;functions,names,ordinals=u32(e+28),u32(e+32),u32(e+36)
        for j in range(u32(e+24)):
            p=u32(names+j*4);end=self.data.index(0,p)
            if self.data[p:end]==name.encode():return self.base+u32(functions+4*u16(ordinals+2*j))
        raise AssertionError(name)

