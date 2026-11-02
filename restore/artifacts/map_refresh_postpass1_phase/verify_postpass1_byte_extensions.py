"""Four supplemental fallback BYTE upper-bit probes; preserve the3088-case core oracle."""
from pathlib import Path
import hashlib,json,sys
HERE=Path(__file__).resolve().parent
CORE=HERE/'verify_postpass1.py';sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
assert sha(CORE)=='f3660eaaa095fe7104275319312a059c819fefa57a862b9b37af2e220bad6f6a','core oracle drift'
base=[0,16,16,8,8,9,9,1,0x800,0x800,99,1,0,0x27f,1000,0,8,8]
cases=[]
for value in [0x80,0x81,0xf8,0xff]:
    p=base.copy();p[12]=value;cases.append(p)
wrapper=Path(__file__).resolve();wrapper_hash=sha(wrapper);text=CORE.read_text()
patches=[('complete=len(cases)','cases=BYTE_EXTENSION_CASES;complete=len(cases)'),
 ("((ROOT/'metadata' if args.production else HERE)/('map-refresh-postpass1-partial.json' if args.limit else 'map-refresh-postpass1-emulation.json')).write_text(json.dumps(report,indent=2)+'\\n')", """assert report['checked']==4 and report['comparisons']==16
report.update({'supplemental_byte_probes':True,'core_corpus_changed':False,'core_oracle_sha256':'f3660eaaa095fe7104275319312a059c819fefa57a862b9b37af2e220bad6f6a','extension_purpose':'ACF85C upper-bit/low-bit fallback variation:80,81,F8,FF','extension_wrapper_sha256':BYTE_WRAPPER_HASH})
report['limitations']=[v.replace('All other213-body instructions execute in full corpus.','The separate3088-case core proves all other213-body instructions.') for v in report['limitations']]
report['limitations'].insert(0,'Supplemental4 cases only; this report makes no complete instruction/branch coverage claim. The separate immutable3088-case core supplies that evidence.')
assert sha(BYTE_WRAPPER)==BYTE_WRAPPER_HASH
report['source_hashes'][rel(BYTE_WRAPPER)]=BYTE_WRAPPER_HASH
((ROOT/'metadata' if args.production else HERE)/'map-refresh-postpass1-byte-extensions.json').write_text(json.dumps(report,indent=2)+'\\n')""")]
for old,new in patches:assert text.count(old)==1,old;text=text.replace(old,new)
original_argv=sys.argv.copy();sys.argv=[str(CORE),*sys.argv[1:],'--limit','4']
try:exec(compile(text,str(CORE),'exec'),{'__name__':'__main__','__file__':str(CORE),'BYTE_EXTENSION_CASES':cases,'BYTE_WRAPPER':wrapper,'BYTE_WRAPPER_HASH':wrapper_hash})
finally:sys.argv=original_argv
