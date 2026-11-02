"""Native/C name-picker callbacks, actual button tables and full panel drawing.

Real language lookup executes against read-only game text. Text-editor reset
and draw endpoints are controlled; no host game or profile write is used.
"""
from verify_options_startup import *

def setup(u,state,selected=1):
    reset(u,state);u.mem_write(SP-0x5000,b'\xa5'*0x5000)
    data=(ROOT/'build/debug-startup-game/Pharaoh_Text.eng').read_bytes()
    u.mem_write(0xe34c38,data[28:8028]);u.mem_write(S+0x1000,data[8028:])
    put(u,0xe36c70,S+0x1000);put(u,0xd3b198,selected)
    byte(u,0xe38dda,81);byte(u,0xe38dca,0)

def edit_hooks(c,u,state,mutation=False):
    calls=[]
    for token,n in [(0x4cc500,1),(0x4cc3c0,0),(0x4cc3e0,0)]:
        def edit(cpu,args,t=token):
            calls.append((t,args,bytes(cpu.mem_read(0xe38e74,32))))
            if mutation:put(cpu,0xd3b198,256+len(calls))
            return 0
        hook(c,state,token,n,edit)
    return calls

def callback(token,selected=1,mode=54,dirty=0,mutation=False):
    results=[]
    for c,u,state in machines:
        setup(u,state,selected);byte(u,0xe38dda,mode);byte(u,0xe38dca,dirty)
        calls=edit_hooks(c,u,state,mutation)
        if c:run(0,u,sym('restore_menu_callback'),[token])
        else:run(0,u,token)
        results.append((tuple(calls),snapshot(u)))
    equal(results,('name callback',hex(token),selected,mode,dirty,mutation))

def click(table,index,side='left',origin=(192,144)):
    results=[]
    for c,u,state in machines:
        setup(u,state);calls=edit_hooks(c,u,state)
        byte(u,0xe38dda,54 if table==0x5c5af8 else 81)
        p=table+index*28;x,y=origin
        left,top=struct.unpack('<2h',u.mem_read(p,4))
        put(u,0xe38e34,x+left+1);put(u,0xe38e30,y+top+1)
        for a in [0xe38e00,0xe38e02,0xe38e03,0xe38e04,0xe38df4,0xe38df6,0xe38df7,0xe38df8]:byte(u,a,0)
        if side=='left':byte(u,0xe38e04,1)
        if side=='right':byte(u,0xe38df8,1)
        result=run(c,u,0x4261d0,[x,y,p,1])
        results.append(((result,tuple(calls)),snapshot(u)))
    equal(results,('actual button click',hex(table),index,side,origin))

def draw(resolution=3,hover=0,selected=-1,mutation=False):
    results=[]
    for c,u,state in machines:
        setup(u,state)
        for at,value in [(0xe38e68,resolution),(0x7056c8,hover),(0x6059fc,selected),
                         (0xe38da8,257),(0xe38da4,-513),(0xf56f8c,0x12340100),
                         (0xf56fa4,0x23450200),(0xf56fac,0x34560300)]:put(u,at,value)
        calls=[]
        for token,n in [(0x536820,4),(0x4d6200,9),(0x4cd1a0,5),(0x4d6380,9)]:
            def observe(cpu,args,t=token):
                calls.append((t,args))
                if mutation:
                    for at in [0xe38da8,0xf56f8c,0xf56fa4,0xf56fac]:put(cpu,at,get(cpu,at)+257)
                    put(cpu,0xe38da4,get(cpu,0xe38da4)-513)
                    put(cpu,0x7056c8,(get(cpu,0x7056c8)+1)%44)
                    put(cpu,0x6059fc,(get(cpu,0x6059fc)+1)%42)
                return 0
            hook(c,state,token,n,observe)
        run(c,u,0x520d00)
        assert sum(t==0x4d6200 for t,args in calls)==42
        results.append((tuple(calls),snapshot(u)))
    equal(results,('name panel',resolution,hover,selected,mutation))

if __name__=='__main__':
    callback(0x40c890)
    if '--one' in sys.argv:print('PASS name panel opener through production callback registry');exit()
    n=1
    for token,mode,dirty in itertools.product([0x40c890,0x40c810],[0,54,81,255],[0,1,255]):
        callback(token,mode=mode,dirty=dirty);n+=1
    for selected,mutation in itertools.product([*range(1,21),*range(22,42)],[False,True]):
        callback(0x40c830,selected=selected,mode=81,mutation=mutation);n+=1
    print('PASS all three callbacks, real catalog names and post-editor selection reload:',n,flush=True)
    clicks=0
    for index,side in itertools.product(range(43),['left','right','none']):
        click(0x5c4358,index,side);clicks+=1
    for origin,side in itertools.product([(192,144),(0,0),(257,-513)],['left','right','none']):
        click(0x5c5af8,0,side,origin);clicks+=1
    print('PASS opening, all43 name/heading/close buttons and right/no-click behavior:',clicks,flush=True)
    panels=0
    for resolution,hover,selected,mutation in itertools.product([1,2,3,257],[0,22,43],[1,21,257],[False,True]):
        draw(resolution,hover,selected,mutation);panels+=1
    print('PASS complete42-row panel, fonts, heading/hover priority and callback reloads:',panels,flush=True)
    print('PASS total cases:',n+clicks+panels,'target SHA256',target_sha,flush=True)
