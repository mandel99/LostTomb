"""Bounded QuickTime core recipes; no DLL, PE, host or replay is run here."""
import struct

MODULE=0x12663b8
LAZY,DISPATCH,EXPORT_B,EXPORT_A=0x604804,0x604808,0x60480c,0x604810
SLOTS=(LAZY,DISPATCH,EXPORT_B,EXPORT_A)
QT_NAME=0x604860
NAMES=(0x60483c,0x60482c,0x604814)
DLL=b'QuickTime.qts'
EVENT=0x32002000
PORT,PROC_A,PROC_B=0x31000900,0x31000920,0x31000940
H1,H2,H3,H4,H5=0x71000000,0x82000000,0x83000000,0x84000000,0x85000000
PATH_ROLE=['stack-output','qt-loader-path']
FRAME=(0x80000000,0x11223344,0xfedcba98,0x01234567)
GATES={0x560330:0x1d000a,0x560560:0x2000c,0x560580:0x20023,
       0x5605a0:0x200dd,0x5605c0:0x2018b,0x5605e0:0x10000}
TARGETS=(0x5603f0,0x560360,*GATES,0x5602f0)


def word(value):return struct.pack('<I',value&0xffffffff)
def path_token(path):return ['stack-cstring','qt-loader-path',(path+b'\0').hex()]


class Case:
    def __init__(self,entry,label,args=(),cleanup=0):
        self.mem={MODULE:bytearray(word(0)),EVENT:bytearray(b'\xa5'*32)}
        self.slots={LAZY:'loader',DISPATCH:'missing',EXPORT_B:'missing',EXPORT_A:'missing'}
        self.initial_slots=None;self.written=set();self.eps={}
        self.c=dict(id=f'c7-quicktime-core/{entry:08x}/'+label,entry=entry,
            receiver=0x89abcdef,arguments=list(args),named_arguments=list(args),canonical_arguments=list(args),
            initial_eax=0xa5b6c7d8,initial_edx=0x33445566,eflags=0xad7,
            native_cleanup=cleanup,named_cleanup=cleanup,canonical_cleanup=cleanup,
            result_mask=None if entry==0x560360 else 0xffffffff,
            canonical_registers_and_flags=False,endpoints=[],
            expected_endpoint_counts={},expected_endpoint_arguments={},
            authored_endpoint_order=[],authored_endpoint_observations=[],
            expected_endpoint_registers={},expected_opaque_argument_words=None,
            no_PE_binding=True)

    def put(self,at,value):
        assert at==MODULE
        self.mem[MODULE][:]=word(value)

    def get(self):return int.from_bytes(self.mem[MODULE],'little')

    def start(self):
        self.initial_slots=dict(self.slots)
        self.c['stores']=[(at,bytes(data))for at,data in self.mem.items()]
        self.c['stores'] += [(at,word(value))for at,value in self.slots.items()if isinstance(value,int)]
        self.c['watched']=[(at,len(data))for at,data in self.mem.items()]
        self.c['write_ranges']=list(self.c['watched'])+[(LAZY,16)]
        for name,arity,cleanup in [('GetModuleHandleA',1,4),('GetModuleFileNameA',3,12),
            ('LoadLibraryA',1,4),('GetSystemDirectoryA',2,8),('GetProcAddress',2,8),('FreeLibrary',1,4)]:
            spec=dict(token=name,import_name=name,arity=arity,native_cleanup=cleanup,
                compiled_cleanup=cleanup,result_kind='full32',value=0,responses=[],
                clobber_ecx=0xdead4321,clobber_edx=0xbeef3456)
            if name=='GetModuleFileNameA':spec['pointer_args']={1:'qt-loader-path'}
            if name=='GetSystemDirectoryA':spec['pointer_args']={0:'qt-loader-path'}
            if name=='LoadLibraryA':spec['private_input_args']={0:dict(role='qt-loader-path',cstring_max=260)}
            self.add_endpoint(spec)

    def add_endpoint(self,spec):
        self.eps[spec['token']]=spec
        self.c['expected_endpoint_counts'][spec['token']]=0
        self.c['expected_endpoint_arguments'][spec['token']]=[]

    def event(self,token,args,value=0,mutations=(),**fields):
        # Relocated native/C code PCs are never included in observations.
        # Only already-written common export/zero values may be observed.
        observe=[(MODULE,4)]+[(a,4)for a in sorted(self.written)]
        observed=[[MODULE,4,self.get()]]+[[a,4,self.slots[a]]for a in sorted(self.written)]
        assert all(isinstance(v,int)for _,_,v in observed)
        self.c['authored_endpoint_order'].append(token)
        self.c['authored_endpoint_observations'].append(observed)
        self.c['expected_endpoint_counts'][token]+=1
        self.c['expected_endpoint_arguments'][token].append([a&0xffffffff if isinstance(a,int)else a for a in args])
        response=dict(value=value,observe=observe,**fields)
        if mutations:response['mutations']=list(mutations)
        self.eps[token]['responses'].append(response)
        for at,width,new in mutations:
            assert at==MODULE and width==4
            self.put(at,new)

    def slot(self,at,value):
        assert at in SLOTS and isinstance(value,int)
        self.slots[at]=value;self.written.add(at)

    def dispatcher(self,arity,cleanup=0,component=False):
        spec=dict(token='qt-dispatcher',controlled_address=PORT,arity=arity,
            native_cleanup=cleanup,compiled_cleanup=cleanup,result_kind='full32',value=0,
            registers=['eax'],responses=[])
        if component:spec['private_input_args']={1:dict(role='qt-component-parameters',width=8)}
        self.add_endpoint(spec)

    def dispatch(self,args,selector,result=0xf1234567):
        self.event('qt-dispatcher',args,result)
        self.c['expected_endpoint_registers'].setdefault('qt-dispatcher',[]).append({'eax':selector})
        self.c['expected_selector']=selector
        return result

    def end(self,expected=None):
        fps=[]
        for at,initial in self.initial_slots.items():
            if not isinstance(initial,str):continue
            fp=dict(address=at,native=0x560360 if initial=='loader'else 0x560350,
                compiled_symbol='FUN_00560360'if initial=='loader'else'restore_quicktime_missing_dispatcher')
            final=self.slots[at]
            if final!=initial:
                assert isinstance(final,int)
                fp['expected_final_value']=final
            fps.append(fp)
        self.c['function_pointers']=fps
        self.c['endpoints']=list(self.eps.values())
        self.c['authored_final_memory']=[(at,bytes(data).hex())for at,data in self.mem.items()]
        # Literal values here are zero or shared controlled export tokens,
        # never a native/compiled image's differing physical code address.
        self.c['authored_final_memory'] += [(at,word(value).hex())for at,value in self.slots.items()if isinstance(value,int)]
        self.c['expected_final_slot_values']={a:v for a,v in self.slots.items()if isinstance(v,int)}
        self.c['expected_unchanged_default_slots']=[a for a,v in self.slots.items()if isinstance(v,str)]
        if expected is not None:
            assert self.c['result_mask']is not None
            self.c['expected_masked_result']=expected&0xffffffff
        return self.c


def filename(c,module,path):
    payload=[]if path is None else[(1,0,(path+b'\0').hex())]
    c.event('GetModuleFileNameA',[module,PATH_ROLE,260],0 if path is None else len(path),writes_arg_bytes=payload)


def load(c,path,value):c.event('LoadLibraryA',[path_token(path)],value)


def system(c,path,result):
    payload=[]if path is None else[(0,0,(path+b'\0').hex())]
    c.event('GetSystemDirectoryA',[PATH_ROLE,260],0 if path is None else len(path),writes_arg_bytes=payload)
    if path is not None:load(c,path+b'\\'+DLL,result)
    return 0 if path is None else result


def sibling(c,path,result):
    filename(c,0,path)
    if path is not None:
        position=max(path.rfind(b'\\'),path.rfind(b'/'))
        if position>=0:
            load(c,path[:position+1]+DLL,result)
            return result
    return 0


def acquire_success(c):
    # The actual5603F0 executes; these are only its declared Win32 imports.
    c.event('GetModuleHandleA',[QT_NAME],0)
    result=sibling(c,b'C:\\Game\\Pharaoh.exe',H1)
    assert result==H1
    return result


def acquire_failure(c):
    c.event('GetModuleHandleA',[QT_NAME],0)
    filename(c,0,None)
    return system(c,None,0)


def resolve(c,existing,results,acquired=False,reload=False,free_value=1,loader_failure=False):
    if not existing:
        loaded=acquire_failure(c)if loader_failure else acquire_success(c)
        c.put(MODULE,loaded)
        if not loaded:return
    for index,value in enumerate(results):
        mutations=[(MODULE,4,(H2,H3,H4)[index])]if reload else[]
        c.event('GetProcAddress',[c.get(),NAMES[index]],value,mutations=mutations)
        c.slot((EXPORT_A,EXPORT_B,DISPATCH)[index],value)
        if not value:break
    if len(results)==3 and all(results):c.slot(LAZY,0)
    if acquired:
        mutation=[(MODULE,4,H5)]if reload else[]
        c.event('FreeLibrary',[c.get()],free_value,mutations=mutation)
        c.put(MODULE,0)


def loader_cases():
    out=[]
    for label in ('loaded-module-success','loaded-module-load-failure-no-fallback',
        'module-filename-fails-then-app','application-backslash-success',
        'application-forward-slash-success','application-load-fails-system-success',
        'application-without-slash-system-success','system-directory-failure','system-load-failure'):
        c=Case(0x5603f0,label);c.start()
        if label.startswith('loaded-module'):
            c.event('GetModuleHandleA',[QT_NAME],H1)
            filename(c,H1,b'C:\\QT\\QuickTime.qts')
            result=0 if 'failure'in label else H2
            load(c,b'C:\\QT\\QuickTime.qts',result)
        elif label=='module-filename-fails-then-app':
            c.event('GetModuleHandleA',[QT_NAME],H1);filename(c,H1,None)
            result=sibling(c,b'C:\\Game\\Pharaoh.exe',H2)
        else:
            c.event('GetModuleHandleA',[QT_NAME],0)
            if label=='system-directory-failure':
                filename(c,0,None);result=system(c,None,0)
            elif label=='application-forward-slash-success':
                result=sibling(c,b'/opt/game/Pharaoh.exe',H2)
            elif label=='application-backslash-success':
                result=sibling(c,b'C:\\Game\\Pharaoh.exe',H2)
            elif label=='application-without-slash-system-success':
                sibling(c,b'Pharaoh.exe',0);result=system(c,b'C:\\Windows\\System32',H3)
            else:
                sibling(c,b'C:\\Game\\Pharaoh.exe',0)
                result=system(c,b'C:\\Windows\\System32',0 if label=='system-load-failure'else H3)
        out.append(c.end(result))
    return out


def resolver_cases():
    out=[]
    specs=[('existing-all-exports',True,[PROC_A,PROC_B,PORT],False,False),
        ('existing-first-GetProc-fails',True,[0],False,False),
        ('existing-second-GetProc-fails',True,[PROC_A,0],False,False),
        ('existing-third-GetProc-fails',True,[PROC_A,PROC_B,0],False,False),
        ('acquired-live-module-reloads-and-free',False,[PROC_A,PROC_B,PORT],True,False),
        ('acquired-failed-lookup-free-false-clears',False,[0],False,False),
        ('loader-failure-no-lookup-or-free',False,[],False,True),
        ('existing-live-module-reloads-no-free',True,[PROC_A,PROC_B,PORT],True,False)]
    for label,existing,values,reload,failed in specs:
        c=Case(0x560360,label);c.put(MODULE,H1 if existing else 0);c.start()
        resolve(c,existing,values,acquired=not existing and not failed,reload=reload,
            free_value=0 if 'free-false'in label else 0x100,loader_failure=failed)
        out.append(c.end()) # Logical void: never author initializer EAX.
    return out


def gate_cases():
    out=[]
    for entry,selector in GATES.items():
        for cleanup in(0,8):
            c=Case(entry,f'opaque-frame-controlled-RET{cleanup}',FRAME,cleanup)
            c.slots[LAZY]=0;c.slots[DISPATCH]=PORT;c.start()
            c.dispatcher(len(FRAME),cleanup)
            result=c.dispatch(list(FRAME),selector)
            c.c['expected_opaque_argument_words']=list(FRAME)
            c.c['raw_frame_contract']='Four caller-supplied probe DWORDs; this is not a recovered SDK arity. Controlled callback cleanup is declared by the case.'
            out.append(c.end(result))
        c=Case(entry,'default-missing-dispatcher',FRAME)
        c.slots[LAZY]=0;c.start()
        c.c['expected_opaque_argument_words']=list(FRAME)
        c.c['expected_selector']=selector
        out.append(c.end((selector&0xffff0000)|0xf7d1))
        c=Case(entry,'actual-lazy-loader-and-export-resolution',FRAME)
        c.start();c.dispatcher(len(FRAME))
        resolve(c,False,[PROC_A,PROC_B,PORT],acquired=True)
        result=c.dispatch(list(FRAME),selector)
        c.c['expected_opaque_argument_words']=list(FRAME)
        out.append(c.end(result))
    c=Case(0x5605a0,'actual-lazy-load-fails-to-default',FRAME);c.start()
    resolve(c,False,[],loader_failure=True)
    c.c['expected_selector']=GATES[0x5605a0];c.c['expected_opaque_argument_words']=list(FRAME)
    out.append(c.end((GATES[0x5605a0]&0xffff0000)|0xf7d1))
    return out


def component_cases():
    out=[]
    for label,controller,event,lazy,missing in [
        ('contiguous8B-controller-event',0x81234567,EVENT,False,False),
        ('contiguous8B-null-fields',0,0,False,False),
        ('contiguous8B-after-actual-loader',0xf2345678,EVENT,True,False),
        ('actual-child-default-missing',0x81234567,EVENT,False,True)]:
        c=Case(0x5602f0,label,[controller,event])
        if not lazy:c.slots[LAZY]=0
        if not lazy and not missing:c.slots[DISPATCH]=PORT
        c.start()
        if lazy:resolve(c,False,[PROC_A,PROC_B,PORT],acquired=True)
        if missing:result=0x1f7d1
        else:
            c.dispatcher(2,component=True)
            block=struct.pack('<BBhI',0,4,7,event).hex()
            result=c.dispatch([controller,['stack-input','qt-component-parameters',block]],0x10000)
            c.c['expected_component_block']=block
        out.append(c.end(result))
    return out


def cases(registry=None):
    out=loader_cases()+resolver_cases()+gate_cases()+component_cases()
    if registry is None:return out
    present={int(m['address'],16):m for m in registry['members']}
    selected=[c for c in out if c['entry']in present]
    for c in selected:
        providers=present[c['entry']].get('providers')
        if providers:c['expected_invocations']=1+sum(bool(v.get('named'))+bool(v.get('canonical'))for v in providers.values())
    return selected


definitions=cases
