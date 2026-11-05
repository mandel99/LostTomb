"""One shared setup adapted from the existing next3 production oracle."""
import struct
from case_api import DATA_START,DATA_END,S,STACK_START,STACK_END
def put(u,a,v):u.mem_write(a&0xffffffff,struct.pack('<I',v&0xffffffff))
def word(u,a,v):u.mem_write(a&0xffffffff,struct.pack('<H',v&65535))
def byte(u,a,v):u.mem_write(a&0xffffffff,bytes([v&255]))
def get(u,a):return struct.unpack('<I',u.mem_read(a&0xffffffff,4))[0]
def setup(u,seed,cfg,profile):
    u.mem_write(DATA_START,seed);u.mem_write(STACK_START,b'\xcc'*(STACK_END-STACK_START));u.mem_write(S,b'\xa5'*0x50000)
    for at,size in [(0x7999d8,4000*264),(0xbcd5f8,2000*388),(0xbcb9c8,50*144),(0xb350dc,51984*2)]:u.mem_write(at,bytes(size))
    if cfg.get('setup','actor')=='actor':
        actor=cfg.get('actor',1);offset=(actor*388)&0xffffffff;kind=cfg.get('kind',profile.get('kind') or 0)
        for at,v in [(0xd056f8,actor),(0xeaac28,1),(0xbca2e0,cfg.get('rotation',0)),(0xd3b1c4,cfg.get('history',0)),(0xd3b374,cfg.get('previous',0x13572468)),(0xd3b384,cfg.get('first',0x12345678)),(0xd3b380,cfg.get('second',0x3456789a)),(0xbca2d8,1000),(0xbca2d0,228),(0xbca2d4,228)]:put(u,at,v)
        for n,at in enumerate(profile.get('animation_bases',[])):put(u,at,10000+n*1000)
        for at,v in [(0xbcd602,kind),(0xbcd606,1),(0xbcd630,cfg.get('state',8)),(0xbcd5f9,cfg.get('animation',0)),(0xbcd609,cfg.get('direction',0)),(0xbcd60a,cfg.get('previous_direction',2)),(0xbcd65a,cfg.get('ordinal',3)),(0xbcd758,cfg.get('cadence',0))]:byte(u,at+offset,v)
        for at,v in [(0xbcd740,cfg.get('counter',0)),(0xbcd73c,cfg.get('cycle',12)),(0xbcd618,1000+11*228+10)]:put(u,at+offset,v)
        put(u,0x7090ec+kind*56,cfg.get('speed',12))
        for at,v in [(0xbcd60c,10),(0xbcd60e,11),(0xbcd61c,cfg.get('target_x',30)),(0xbcd61e,cfg.get('target_y',31)),(0xbcd62e,cfg.get('delay',5)),(0xbcd656,cfg.get('linked',2)),(0xbcd658,cfg.get('group',1))]:word(u,at+offset,v)
        for linked in [2,3]:
            for at,v in [(0xbcd630,8),(0xbcd606,1)]:byte(u,at+linked*388,v)
            for at,v in [(0xbcd60c,cfg.get('linked_x',30)),(0xbcd60e,cfg.get('linked_y',31))]:word(u,at+linked*388,v)
        for at,v in [(0xbcba02,40),(0xbcba04,41),(0xbcb9fe,50),(0xbcba00,51)]:word(u,at+144*cfg.get('group',1),v)
    for key,writer in [('stores',put),('words',word),('bytes',byte)]:
        for at,v in cfg.get(key,[]):writer(u,at,v)
    for at,raw in cfg.get('regions',[]):u.mem_write(at,bytes.fromhex(raw)if isinstance(raw,str)else bytes(raw))
