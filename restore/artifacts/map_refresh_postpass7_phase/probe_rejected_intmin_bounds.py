"""Native-only diagnostic of an invalid map-size input; no C equivalence case."""
from pathlib import Path
import hashlib,json,sys
HERE=Path(__file__).resolve().parent;main=HERE/'verify_postpass7.py';source=main.read_text();saved=sys.argv;sys.argv=[str(main),'--limit','1'];ns={'__file__':str(main),'__name__':'__native_invalid_domain_probe__'}
try:exec(compile(source.split('provided=[args.exe',1)[0],str(main),'exec'),ns)
finally:sys.argv=saved
p=ns['default'].copy();p[1]=p[2]=0x80000000;p[3]=p[4]=0xffffffff;p[5]=p[6]=1
try:ns['native'](p)
except AssertionError as exc:
    vm=ns['vm'];sp=vm.reg_read(ns['UC_X86_REG_ESP']);read=ns['read'];pc=vm.reg_read(ns['UC_X86_REG_EIP'])
    result={'format':1,'native_sha256':ns['PIN'],'original_game_executed':False,'main_oracle_sha256':hashlib.sha256(main.read_bytes()).hexdigest(),'classification':'Rejected invalid size metadata, native-only diagnostic; no algorithmfix or equivalenceclaim','C_cases':0,'native_only_invalid_domain_probes':1,'parameters':p,'observed_rejection':repr(exc),'native_PC':f'{pc:08x}','native_clipped_xmax':read(sp+0x48),'native_clipped_ymax':read(sp+0x4c),'current_native_column':vm.reg_read(ns['UC_X86_REG_EDI']),'native_instructions_observed':len(ns['hits']),'explanation':'INT32_MIN width/height minus1 wraps to INT32_MAX; original rectangle scanner then reads past the prepared bounded map. Bounded map domain0..228 and safe negative extensions are separate. This is a watch-contract rejection, not a claimed nativeCPUfault or Ccounterexample.'}
    assert pc==0x487897 and result['native_clipped_xmax']==0x7fffffff and result['native_clipped_ymax']==0x7fffffff,result
    (HERE/'rejected-intmin-bound-probe.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
else:raise AssertionError('Invalid native scan unexpectedly stayed within prepared domain')
