"""Whole native/C temple-complex preview, including all32 recovered paint arms.
Projection, library loading, keyboard and sprite endpoints are controlled.
Actual data tables, geometry, terrain scan and library-kind query execute.
"""
from verify_options_startup import *


def compare(values,label):
    assert values[0][0]==values[1][0],(label,'calls',
        next(((i,a,b) for i,(a,b) in enumerate(zip(values[0][0],values[1][0])) if a!=b),None),
        tuple(len(v[0]) for v in values))
    equal(values,label)


def preview(camera=0,rotation=0,tool=65,existing=0,key=0,problem=0,tiles='original',mutation='',override=None):
    values=[]
    for c,u,state in machines:
        reset(u,state);u.mem_write(SP-0x5000,b'\xa5'*0x5000)
        for a,v in [(0xeaac28,3),(0xbca304,80),(0xbca308,70),(0xbca2d8,1000),
                    (0xbca2e0,camera),(0xbc9714,rotation),(0x70e44c,36),(0x70e448,15),
                    (0x70e458,30),(0x70e45c,60),(0xf56be4,123456),(0xf56d7c,10000),
                    (0xf56d84,20000),(0xf56d8c,30000),(0x70e120,0x1234a5a5)]:put(u,a,v)
        word(u,0xea5ce8+3*0x49d8,existing)
        u.mem_write(0xaf598c,bytes(228*228*4))
        if problem:
            table=0x5e00cc+(rotation+int(camera/2)*4)*8
            x=(get(u,table)+80)&0xffffffff;y=(get(u,table+4)+70)&0xffffffff
            if problem==1:put(u,0xaf598c+(1000+y*228+x)*4,4)
            elif problem==2:
                for cell in range(15000,25000):put(u,0xaf598c+cell*4,4)
            elif problem==3:
                for cell in range(15000,25000):put(u,0xaf598c+cell*4,0x11002880)
        for i in range(1000):put(u,0xf55bb4+i*8,100000+i*1000)
        # These named building/footprint IDs are entries in that same tag array.
        # Keep building sprites distinct so output-slot mutations target only
        # the immediately active tile projection, never an expired C frame.
        for a,v in [(0xf56d7c,10000),(0xf56d84,20000),(0xf56d8c,30000),(0xf56be4,123456)]:put(u,a,v)
        if tiles!='original':
            for i in range(91):
                p=0x5df9b0+i*20
                put(u,p,0 if tiles=='empty' or tiles=='sparse' and i%3 else i+1)
                for j in range(4):put(u,p+4+j*4,10+i*7+j)
        calls=[];projections=0;draws=0;last_outputs=None
        def project(cpu,args):
            nonlocal projections,last_outputs
            projections+=1;calls.append(('project',args[:2]));last_outputs=args[2:]
            put(cpu,args[2],600+projections*13);put(cpu,args[3],400+projections*17)
            if mutation=='projection' and projections==1:
                put(cpu,0xbca2e0,6);put(cpu,0xbc9714,3);put(cpu,0x70e44c,77)
            if mutation=='terrain' and projections>1:
                put(cpu,0xaf598c+(1000+args[1]*228+args[0])*4,4 if projections%2 else 0)
            return 0
        def key_state(cpu,args):
            calls.append(('key',args))
            if mutation=='key':put(cpu,0xbca2e0,2);put(cpu,0xbc9714,1)
            return key
        hook(c,state,0x4f0ba0,4,project)
        key_slot=sym('_imp__GetAsyncKeyState@4') if c else 0x56f1e4
        saved_key=get(u,key_slot);put(u,key_slot,S+0x49000)
        state['hooks'][S+0x49000]=(1,4,key_state)
        def library(cpu,args):
            logical=args if c else (cpu.reg_read(UC_X86_REG_ECX),*args)
            calls.append(('library',logical))
            if mutation=='library':put(cpu,0xbca2e0,4);put(cpu,0xbc9714,2)
            if override is not None:put(cpu,0xbca2e0,override[0]);put(cpu,0xbc9714,override[1])
            return 1
        state['hooks'][address(c,0x53dd90)]=(3 if c else 2,0 if c else 8,library)
        def draw(cpu,args,token):
            nonlocal draws
            draws+=1;calls.append((token,args,get(cpu,0x70e120)))
            if mutation in ('draw','outputs'):
                put(cpu,0xf56d7c,get(cpu,0xf56d7c)+1);put(cpu,0xf56d84,get(cpu,0xf56d84)+3)
                put(cpu,0xf56d8c,get(cpu,0xf56d8c)+5);put(cpu,0x70e44c,get(cpu,0x70e44c)+7)
                put(cpu,0x70e448,get(cpu,0x70e448)+1);put(cpu,0x70e458,get(cpu,0x70e458)+1)
                for i in range(1000):put(cpu,0xf55bb4+i*8,get(cpu,0xf55bb4+i*8)+1)
                if mutation=='outputs' and token in (0x53e590,0x53f250) and args[0]>=100000 and last_outputs:
                    put(cpu,last_outputs[0],get(cpu,last_outputs[0])+11);put(cpu,last_outputs[1],get(cpu,last_outputs[1])+13)
            if mutation=='camera' and draws==1:put(cpu,0xbca2e0,6);put(cpu,0xbc9714,3)
            return 0
        for token,n in [(0x53e590,3),(0x53f250,3),(0x53f4f0,4)]:
            hook(c,state,token,n,lambda cpu,args,t=token:draw(cpu,args,t),cleanup=0 if c else n*4)
        run(c,u,0x4c0f30,[tool],budget=5000000)
        put(u,key_slot,saved_key)
        values.append((calls,snapshot(u)))
    compare(values,(camera,rotation,tool,existing,key,problem,tiles,mutation,override))


def kind(tool):
    values=[]
    for c,u,state in machines:
        reset(u,state);value=run(c,u,0x4c0ed0,[tool]);values.append((value,snapshot(u)))
    compare(values,('kind',tool))


if __name__=='__main__':
    sym('restore_temple_complex_preview')
    n=0
    for tool in list(range(58,72))+[-1,0x10000041]:kind(tool);n+=1
    print('PASS temple library kind: %d native/C full-DWORD table cases'%n,flush=True)
    n=0
    for camera,rotation,tiles in itertools.product([0,2,4,6],[0,1,2,3],['original','dense','empty','sparse']):
        preview(camera,rotation,tiles=tiles);n+=1
    for camera,rotation in itertools.product([0,2,4,6],[0,1,2,3]):
        preview(camera,rotation,key=0x12348001,tiles='dense');n+=1
    print('PASS temple valid preview: %d native/C rotations, key cycle and complete tile order cases'%n,flush=True)
    n=0
    for camera,rotation,existing,problem in itertools.product([0,2,4,6],[0,1,2,3],[0,1,256],[1,2,3]):
        preview(camera,rotation,existing=existing,problem=problem);n+=1
    print('PASS temple masks: %d native/C91-cell scans, allowed terrain and existing-complex cases'%n,flush=True)
    n=0
    for tool,camera in itertools.product(range(60,70),[0,2,4,6]):preview(camera,tool=tool);n+=1
    for mutation,camera in itertools.product(['projection','key','library','draw','outputs','camera'],[0,2,4,6]):
        preview(camera,tiles='dense',mutation=mutation);n+=1
    for camera in [1,3,5,-1]:preview(camera);n+=1
    for camera in [0,2,4,6]:preview(camera,existing=1,problem=1,mutation='terrain');n+=1
    print('PASS temple libraries/callbacks: %d native/C selector and cached/reloaded values'%n,flush=True)
    n=0
    for camera,rotation in itertools.product([0,2,4,6],[0,1,2,3]):
        preview(tiles='dense',override=(camera,rotation));n+=1
    print('PASS all32 paint arms: %d full native/C compositions with controlled post-load orientation'%n,flush=True)
    print('target SHA256',target_sha)
