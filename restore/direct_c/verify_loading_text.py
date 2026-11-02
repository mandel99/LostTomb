"""Whole4CF7B0 loading text/frame placement; OS and raster endpoints controlled."""
from verify_options_startup import *

APIS=[('GetDC@4',0x56f1d4,1),('GetTextExtentPointA@16',0x56f008,4),
      ('SetBkColor@8',0x56f018,2),('TextOutA@20',0x56f014,5),('ReleaseDC@8',0x56f1d8,2)]

def exercise(width,height,dw,dh,text_size,count=12,dc=True,mutate=False):
    results=[]
    for c,u,state in machines:
        reset(u,state);u.mem_write(S,b'Loading data\0')
        for at,v in [(0xe38db0,width),(0xe38dac,height),(0xe38db8,dw),(0xe38db4,dh)]:put(u,at,v)
        calls=[]
        hook(c,state,0x4cc000,1,lambda cpu,args:count)
        def rectangle(cpu,args):
            calls.append(('rectangle',args))
            if mutate:
                put(cpu,0xe38db0,2048);put(cpu,0xe38dac,1536)
                put(cpu,0xe38db8,4000);put(cpu,0xe38db4,3000)
            return 0
        hook(c,state,0x4cd9d0,5,rectangle)
        def action(cpu,args,name):
            if name=='GetTextExtentPointA@16':
                assert args[:3]==(0x1234,S,count)
                put(cpu,args[3],text_size[0]);put(cpu,args[3]+4,text_size[1])
                calls.append((name,args[:3]))
            else:calls.append((name,args))
            return (0x1234 if dc else 0) if name=='GetDC@4' else 1
        for i,(name,slot,n) in enumerate(APIS):
            at=STOP+0x100+i*16
            put(u,sym('_imp__'+name) if c else slot,at)
            state['hooks'][at]=(n,n*4,lambda cpu,args,k=name:action(cpu,args,k))
        run(c,u,0x4cf7b0,[S])
        results.append(tuple(calls))
    assert results[0]==results[1],('whole loading text geometry',width,height,dw,dh,text_size,count,dc,mutate,results)

if __name__=='__main__':
    n=0
    for (w,h),(dw,dh),size in itertools.product([(640,480),(800,600),(1024,768)],[(1920,1080),(800,600)],[(80,16),(400,32)]):
        exercise(w,h,dw,dh,size);n+=1
    for w,h in [(640,480),(800,600),(1024,768)]:
        exercise(w,h,w,h,(80,16),mutate=True);n+=1
    exercise(800,600,1920,1080,(80,16),count=0);n+=1
    exercise(800,600,1920,1080,(80,16),dc=False);n+=1
    print('PASS whole loading text/frame, full game/desktop dimensions and retained positions:',n,flush=True)
    print('Verified target SHA256:',target_sha)
