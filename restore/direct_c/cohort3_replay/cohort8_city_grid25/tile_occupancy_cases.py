"""Additional native-derived486810 occupied-tile cases; no execution at import."""
from pathlib import Path
import sys
sys.dont_write_bytecode=True
OUT=Path(__file__).resolve().parent
sys.path.insert(0,str(OUT.parent/'cohort8-city-grid-joint25'))
from city_cases import Case,A,O,W,H,XOUT,YOUT

def occupied_tiles(variant):
    origin=0x1234;negative=bool(variant)
    row=-1 if negative else 1;left=-3 if negative else 2
    first=origin+row*228+left
    c=Case(0x486810,'occupied-WORD-ID-and-BYTE-kind/'+
           ('isolated-BYTE-stride-negative-delta'if negative else'WORD-stride-high-ID'),
           (17,19,0,2,left,row,left+2,row+1))
    c.seed(O,origin);c.seed(W,64);c.seed(H,3)
    for base in(0xbc9804,0xbc9f24,0xbc9b94):c.input(base-4,920)
    c.seed(0xbc9804+row*4,first);c.seed(0xbc9f24+row*4,left);c.seed(0xbc9b94+row*4,left+2)
    c.seed(A(0)+2,0x4b,1)
    for j,actor in enumerate((257,258)):
        tile=first+j
        c.seed(0xaf598c+tile*4,2);c.seed(0xa43eac+tile,-1,1)
        c.seed(0xb350dc+tile*2,actor,2)
        # Neutralize the old wrong WORD address in the second case to expose
        # the independent DWORD-vs-BYTE actor bank error. These seeds are not
        # read by the correct native path.
        c.seed(0xb350dc+tile*8,actor if negative else 0,2)
        kind=(0x19 if j==0 else 0x4b)if negative else(0x4b if j==0 else 0x19)
        c.seed(A(actor)+2,kind,1)
        if negative:c.seed(0xbcd602+actor*1552,0x4b if j==0 else 0x19,4)
    c.cell(0x978d98,1,1);c.cell(XOUT,0x11223344);c.cell(YOUT,0x55667788)
    c.block(0x96e6b0,16,0xa5);c.begin()
    chosen=first if negative else first+1
    c.put(0x96e6b4,chosen)
    c.call('select_tile',17,19,0x96e6b4,1,result=1)
    c.put(XOUT,left if negative else left+1);c.put(YOUT,row)
    return c.finish(1)

def cases():return[occupied_tiles(0),occupied_tiles(1)]
