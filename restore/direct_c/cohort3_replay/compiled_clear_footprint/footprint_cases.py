"""Authored small-footprint cases; existing C algorithm is not replaced."""
import struct
ENTRY=0x47c480
ORIGIN=12000
DIM_X,DIM_Y,ERROR=0xbc973c,0xbc9738,0xbc9734
FIELDS=[(0xa9010c,1,1),(0xb285cc,1,1),(0xb4e6fc,2,2),(0xb67d1c,1,1),
        (0xa9cc1c,2,2),(0xae8e7c,1,1),(0xaf598c,4,4),(0xadc36c,1,1),
        (0xacf85c,1,1),(0xb7482c,4,4)]
BOUNDARIES=[('road',0x47a220,3),('region',0x4767d0,6),('edge',0x4746d0,3),
            ('road-region',0x477500,4),('building-region',0x4777c0,4),
            ('terrain-region',0x475cc0,5),('water-region',0x4776f0,4),('image-region',0x477890,4)]


def packed(v,n):return int(v&((1<<(n*8))-1)).to_bytes(n,'little')


class Fixture:
    def __init__(self,label,building=7,x=5,y=6,kind=183):
        self.mem={};self.final={};self.episodes=[];self.responses={n:[]for n,_,_ in BOUNDARIES}
        self.args=[building&0xffffffff,x&0xffffffff,y&0xffffffff]
        self.label=label;self.building=building;self.kind=kind;self.x=x;self.y=y
        self.put(ERROR,0xabcdef00,4);self.put(DIM_X,4,4);self.put(DIM_Y,5,4)
        self.put(0xbca2d0,64,4);self.put(0xbca2d4,64,4)
        self.put(0xbca2d8,ORIGIN-y*228-x,4);self.put(0xf56b54,0x81234560,4)
        record=(0x7999e8+building*264)&0xffffffff
        self.put(record,kind,2);self.put((record-4)&0xffffffff,ORIGIN,4)
        self.cells=[ORIGIN+r*228+c for r in range(3)for c in range(3)]
        for i,cell in enumerate(self.cells):
            values=[0xa5,0xe0,building&0xffff,0xc0,0xb6b7,0x88,0x0707ffff,0x99,0x80+i,0x55550000+i]
            for (base,stride,n),v in zip(FIELDS,values):self.put(base+cell*stride,v,n)

    def put(self,at,v,n):self.mem[at]=packed(v,n);self.final[at]=packed(v,n)
    def expect(self,at,v,n):assert at in self.final;self.final[at]=packed(v,n)
    def dimensions(self,x,y):self.expect(DIM_X,x,4);self.expect(DIM_Y,y,4)
    def call(self,token,args,mutate=()):
        observation=[[DIM_X,4,int.from_bytes(self.final[DIM_X],'little')],[DIM_Y,4,int.from_bytes(self.final[DIM_Y],'little')]]
        self.episodes.append((token,args,observation))
        self.responses[token].append(dict(observe=[(DIM_X,4),(DIM_Y,4)],mutations=list(mutate)))
        for at,n,v in mutate:self.expect(at,v,n)
    def redraw(self,changing=False):
        for index,(token,_,_) in enumerate(BOUNDARIES[1:]):
            cols=int.from_bytes(self.final[DIM_X],'little');rows=int.from_bytes(self.final[DIM_Y],'little')
            args=[self.x,self.y,self.x+cols,self.y+rows]
            if token=='region':args += [0,0]
            elif token=='edge':args=[self.x,self.y,cols]
            elif token=='terrain-region':args += [0]
            mutations=[(DIM_X,4,index+1),(DIM_Y,4,index+2)]if changing else []
            self.call(token,[v&0xffffffff for v in args],mutations)
    def clear(self,indices,owner=True,mask=0x03052e84,roads=()):
        # Cleared indices and road branches are independently authored inputs,
        # never inferred from the implementation's membership predicate.
        for i in indices:
            cell=self.cells[i]
            if owner:self.expect(0xa9010c+cell,self.kind&255,1)
            for base,stride,n,value in [(0xb285cc,1,1,0xe0),(0xb4e6fc,2,2,0),(0xb67d1c,1,1,0xc0),
                    (0xa9cc1c,2,2,0),(0xae8e7c,1,1,0),(0xadc36c,1,1,0)]:self.expect(base+cell*stride,value,n)
            if i in roads:self.expect(0xaf598c+cell*4,4,4)
            else:
                flags=int.from_bytes(self.mem[0xaf598c+cell*4],'little')
                self.expect(0xaf598c+cell*4,flags&mask,4)
                self.expect(0xb7482c+cell*4,0x81234560+((0x80+i)&7),4)
    def end(self):
        endpoints=[dict(token=name,native=at,compiled_symbols=[f'FUN_{at:08x}'],arity=n,
            native_cleanup=0,compiled_cleanup=0,result_kind='void',clobber_ecx=0xdeadbeef,
            clobber_edx=0xbeefdead,responses=self.responses[name])for name,at,n in BOUNDARIES]
        return dict(id='c7-clear-footprint/'+self.label,entry=ENTRY,receiver=0x88776655,
            arguments=self.args,named_arguments=self.args,canonical_arguments=self.args,
            native_cleanup=0,named_cleanup=0,canonical_cleanup=0,eflags=0xad7,result_mask=None,
            canonical_registers_and_flags=False,stores=list(self.mem.items()),watched=[(a,len(v))for a,v in self.mem.items()],
            write_ranges=[(a,len(v))for a,v in self.mem.items()],endpoints=endpoints,
            expected_endpoint_counts={n:len(self.responses[n])for n,_,_ in BOUNDARIES},
            expected_endpoint_arguments={n:[a for token,a,_ in self.episodes if token==n]for n,_,_ in BOUNDARIES},
            authored_endpoint_order=[token for token,_,_ in self.episodes],
            authored_endpoint_observations=[o for _,_,o in self.episodes],
            authored_final_memory=[(a,v.hex())for a,v in self.final.items()],
            initial_fpcw=0x27f,initial_x87=['0000000000000080ff3f','000000000000008000c0'],
            compare_x87_values=True,compare_x87_control=True)


def definitions(registry=None):
    out=[]
    for label,x,y in [('negative-x',-1,6),('negative-y',5,-1),('x-at-width',64,6),('y-at-height',5,64)]:
        c=Fixture(label,x=x,y=y);c.expect(ERROR,1,4);out.append(c.end())
    c=Fixture('full-DWORD-blocked-two');c.put(0xaf598c+ORIGIN*4,2,4);c.dimensions(2,2);out.append(c.end())
    c=Fixture('normal-two-by-two');c.dimensions(2,2);c.clear([0,1,3,4]);c.redraw();out.append(c.end())
    c=Fixture('signed-owner-word-mismatch');c.put(0xb4e6fc+ORIGIN*2,0xffff,2);c.dimensions(2,2);c.clear([1,3,4]);c.redraw();out.append(c.end())
    c=Fixture('zero-building-keeps-terrain-owner',building=0);c.dimensions(2,2);c.clear([0,1,3,4],owner=False);c.redraw();out.append(c.end())
    c=Fixture('processing-three-by-three',kind=100);c.dimensions(3,3);c.clear(list(range(9)));c.redraw();out.append(c.end())
    c=Fixture('kind99-keeps-terrain-owner',kind=99);c.clear([0],owner=False);c.dimensions(1,1);c.redraw();out.append(c.end())
    c=Fixture('redraw-reloads-dimensions');c.dimensions(2,2);c.clear([0,1,3,4]);c.redraw(changing=True);out.append(c.end())
    c=Fixture('road-child-shrinks-loop');c.put(0xaf598c+ORIGIN*4,4,4);c.dimensions(2,2);c.clear([0],roads=[0]);c.call('road',[5,6,1],[(DIM_X,4,1),(DIM_Y,4,1)]);c.redraw();out.append(c.end())
    c=Fixture('mask138-retains-origin-flags',kind=138);c.put(0xb285cc+ORIGIN,0xe1,1);c.dimensions(2,2);c.clear([0,1,3,4],mask=0x03052fc4);c.redraw();out.append(c.end())
    for label,size,cleared in [('monument-zero',0,[]),('monument-negative',0xffffffff,[]),('monument-two',2,[0,1,3,4])]:
        c=Fixture(label,kind=213);c.put(0x7999e8+7*264+2,1,2);c.put(0x601de0+52,size,4)
        c.dimensions(size,size);c.clear(cleared);c.redraw();out.append(c.end())
    c=Fixture('positive-id-does-not-match-negative-WORD',building=32768);c.dimensions(2,2);c.redraw();out.append(c.end())
    c=Fixture('negative-id-matches-negative-WORD',building=-1);c.dimensions(2,2);c.clear([0,1,3,4]);c.redraw();out.append(c.end())
    c=Fixture('low-byte-two-is-not-DWORD-two');c.put(0xaf598c+ORIGIN*4,0x01000002,4);c.dimensions(2,2);c.clear([0,1,3,4]);c.redraw();out.append(c.end())
    if registry is not None:
        members={int(m['address'],16):m for m in registry['members']};out=[c for c in out if c['entry']in members]
        for c in out:c['expected_invocations']=1+sum(bool(v['named'])+bool(v['canonical'])for v in members[c['entry']]['providers'].values())
    return out
