"""Native/C mission HUD states and real catalog/number composition.
Draw and layout endpoints are controlled; the host game is never started.
"""
from verify_options_startup import *


def prepare(u,state,limit=1,survival=0,tomb_warning=0,won=0,gate=0,mode=2,
            start=-319,year=-331,month=0,scene=3,pause=0,event=0,debug=0,allow=0):
    reset(u,state);u.mem_write(SP-0x5000,b'\xa5'*0x5000)
    for a,v in [(0xeaac28,scene),(0x784c48,limit),(0x784c50,survival),
                (0x78d7b8,start),(0x78d7bc,year),(0x78d7c0,month),
                (0xea5d40+scene*0x49d8,won),(0xea6018+scene*0x49d8,mode),
                (0xf56f84,0x12340100),(0xf56f8c,0x23450200),
                (0xe38da8,257),(0xe38da4,-513),(0xe92dfc,2),
                (0xe20778,0x12340080),(0x605a00,event),(0x605a04,0x123456),
                (0x605a08,3),(0xf53f70,debug),(0x605a0c,allow),
                (0xea1880+scene*0x49d8,513),(0xea5f18+scene*0x49d8,1025)]:
        put(u,a,v)
    byte(u,0x6de514,tomb_warning);byte(u,0xe38e60,pause);byte(u,0xe38dda,31);word(u,0x784b60,gate)


def case(real=False,mutation='',relocated=False,expect=None,**settings):
    results=[]
    for c,u,state in machines:
        prepare(u,state,**settings);calls=[];seen=[]
        def change(cpu):
            if mutation=='draw':
                put(cpu,0xf56f84,get(cpu,0xf56f84)+257)
                put(cpu,0xe38da8,get(cpu,0xe38da8)+513);put(cpu,0xe38da4,get(cpu,0xe38da4)-257)
            elif mutation=='event':put(cpu,0x605a00,4)
        def provider(token,n,fn):hook(c,state,token,n,fn)
        def leaf(cpu,args,token):
            calls.append((token,args));change(cpu);return 0
        for token,n in [(0x4cda40,5),(0x536820,4),(0x4d6250,9)]:
            provider(token,n,lambda cpu,args,t=token:leaf(cpu,args,t))
        provider(0x430cc0,0,lambda cpu,args:449 if mutation=='width' else 1024)
        provider(0x51f780,0,lambda cpu,args:calls.append(('empty',)) or 0)
        def draw(cpu,args):
            p=args[0];text=bytes(cpu.mem_read(p,256)).split(b'\0',1)[0]
            entry=(text,*args[1:4],args[4]&65535);seen.append(entry);calls.append(('draw',entry))
            put(cpu,0xe20778,get(cpu,0xe20778)+len(text)*6);change(cpu);return 0
        if real:
            p=(ROOT/'build/smoke-game/Pharaoh_Text.eng').read_bytes()
            u.mem_write(0xe34c38,p[28:8028]);u.mem_write(S+0x1000,p[8028:]);put(u,0xe36c70,S+0x1000)
            provider(0x4cc770,5,draw)
            provider(0x4d5d90,0,lambda cpu,args:put(cpu,0xe94d9c,1) or 0)
            provider(0x4d5d70,1,lambda cpu,args:3)
            put(u,0xe94d9c,1)
            put(u,S+0x48008,777);put(u,S+0x4800c,-999)
            provider(0x4d5fe0,4,lambda cpu,args:S+0x48000 if relocated else 0)
            provider(0x4d6090,9,lambda cpu,args:calls.append(('layout',args)) or 0)
        else:
            def label(cpu,args):
                calls.append(('label',args));put(cpu,0xe20778,0x123400c0);change(cpu);return 0
            def number(cpu,args):
                calls.append(('number',args));put(cpu,0xe20778,get(cpu,0xe20778)+19);change(cpu);return 0
            provider(0x4d62f0,8,label);provider(0x4d6860,9,number)
            provider(0x4cc770,5,lambda cpu,args:leaf(cpu,args,0x4cc770))
        run(c,u,0x51f790,budget=5000000)
        if expect is not None:
            assert any(t[0]==expect and t[4]==0xf800 for t in seen),(expect,seen)
        results.append(((calls,seen),snapshot(u)))
    assert results[0][0]==results[1][0],(settings,real,mutation,
        next(((i,a,b) for i,(a,b) in enumerate(zip(results[0][0][0],results[1][0][0])) if a!=b),None))
    equal(results,(settings,real,mutation))
    return results[1][0]


if __name__=='__main__':
    sym('restore_city_draw_mission_status')
    n=0
    for limit,survival,tomb_warning,won,gate in itertools.product([0,1,256],[0,1,256],[0,1],[0,1],[0,256]):
        case(limit=limit,survival=survival,tomb_warning=tomb_warning,won=won,gate=gate);n+=1
    print('PASS mission HUD gates: %d native/C DWORD flags, WORD gate and mode priorities'%n,flush=True)
    n=0
    for mode,start,year,month in itertools.product([0,1,2,256],[-332,-331,-330,-319],[-331,-1,256],[0,1,11,256]):
        case(mode=mode,start=start,year=year,month=month);n+=1
    print('PASS calendar: %d native/C signed years, scenario extension and month arithmetic'%n,flush=True)
    n=0
    for event,debug,allow,pause in itertools.product([0,1,2,3,4,257],[0,1,256],[0,1,257],[0,1]):
        case(event=event,debug=debug,allow=allow,pause=pause);n+=1
    for mutation in ['draw','event','width']:
        for limit,survival in [(1,0),(0,1),(0,0)]:
            case(limit=limit,survival=survival,event=1,debug=1,allow=1,pause=1,mutation=mutation);n+=1
    print('PASS other HUD branches: %d native/C notices, debug/event text, pause and callback reloads'%n,flush=True)
    n=0
    for limit,survival,month,relocated in itertools.product([0,1],[0,1],[0,1,11],[False,True]):
        case(real=True,limit=limit,survival=survival,month=month,relocated=relocated);n+=1
    for won,tomb_warning,pause in itertools.product([0,1],[0,1],[0,1]):
        case(real=True,won=won,tomb_warning=tomb_warning,pause=pause);n+=1
    for settings,expect in [
        ({'start':-319,'month':0},b'@144 '),
        ({'start':-319,'month':1},b'@143 '),
        ({'start':-319,'month':11},b'@133 '),
        ({'limit':0,'survival':1,'start':-319},b'@144 '),
    ]:
        trace,seen=case(real=True,expect=expect,**settings);n+=1
        if expect==b'@144 ':print('native/C mission text:',[(t[0].decode('ascii'),t[3],hex(t[4])) for t in seen if t[4]==0xf800],flush=True)
    print('PASS real catalog/number compositions: %d native/C cases'%n,flush=True)
    print('target SHA256',target_sha)
