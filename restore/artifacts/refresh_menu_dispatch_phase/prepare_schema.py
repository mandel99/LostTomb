"""Describe deterministic RAM inputs for the whole refresh comparator."""
from pathlib import Path
import json
HERE=Path(__file__).resolve().parent
review=json.loads((HERE/'native-review.json').read_text());contract=json.loads((HERE/'fixture-contracts.json').read_text())
fields={int(f['address'],16):f['direct_widths'][0] for f in review['fields'] if f['direct_widths']}
fields.update({0x704c08:4,0x704dac:4,0x704db0:4,0x704dbc:4})
fields=[{'address':a,'width':n} for a,n in sorted(fields.items())]
regions=[(0x605930,0x130),(0x6fdc40,0x90),(0x702790,0x30),(0x704bf0,0x210),(0x7056b0,0x30),(0x775900,0x40),(0x77f738,0x340),(0x7999d8,257*264+32),(0xd3a6b0,0x30),(0xd3b170,0x30),(0xe38d90,0x310),(0xea4086,8*0x49d8+32),(0xf53dc0,0x1b0),(0xf55b70,0x30)]
regions.append((0x4ac24086,0x100))
options=['seed','mutation_token','mutation_phase','mutation_field','mutation_value','mutation_ordinal','query_mask','training_tag','actor_tag','record_byte','synthetic_tag','actor_high_tag','repeats']
defaults={0xe38da4:144,0xe38da8:192,0xe38dac:480,0xe38db0:640,0xf53f08:24,0xf53f0c:32,0xf53f00:4,0xf53f1c:30,0xf53f20:30,0xe38e68:1,0x775928:1,0x704c08:8,0xe38dda:1}
values=[defaults.get(f['address'],0) for f in fields]
opt_defaults=[17,0xffffffff,0,0,0,0,0xffffffff,2,0x48,0,1,0x46,1]
returns=[int(r['native_entry'],16) for r in contract['controlled_endpoints']]
return_defaults={0x41bb30:2,0x41b8f0:1,0x426450:1,0x4d6740:1,0x4cde80:2,0x5013a0:1,0x5027b0:1}
values+=opt_defaults+[return_defaults.get(a,0) for a in returns]
lines=['/* Deterministic test input schema, not product declarations. */','#ifndef REFRESH_FIXTURE_SCHEMA_H','#define REFRESH_FIXTURE_SCHEMA_H',f'#define RF_GLOBALS {len(fields)}',f'#define RF_RETURNS {len(returns)}',f'#define RF_FIELDS {len(values)}',f'#define RF_EVENT_FIELDS {8+len(fields)}']
for j,f in enumerate(fields):lines.append(f'#define RF_G_{f["address"]:08X} {j}')
for j,name in enumerate(options):lines.append(f'#define RF_{name.upper()} {len(fields)+j}')
for j,a in enumerate(returns):lines.append(f'#define RF_R_{a:08X} {len(fields)+len(options)+j}')
lines+=['static const uint32_t rf_globals[RF_GLOBALS][2]={'+','.join('{0x%xu,%u}'%(f['address'],f['width']) for f in fields)+'};','static const uint32_t rf_regions[][2]={'+','.join('{0x%xu,%u}'%(a,n) for a,n in regions)+'};','static const uint32_t rf_returns[RF_RETURNS]={'+','.join('0x%xu'%a for a in returns)+'};','static const uint32_t rf_defaults[RF_FIELDS]={'+','.join(str(v)+'u' for v in values)+'};','#endif']
(HERE/'tests/fixture_schema.h').write_text('\n'.join(lines)+'\n')
(HERE/'fixture-schema.json').write_text(json.dumps({'fields':fields,'options':{name:len(fields)+j for j,name in enumerate(options)},'returns':{f'{a:08x}':len(fields)+len(options)+j for j,a in enumerate(returns)},'defaults':values,'regions':regions,'parameter_count':len(values),'event_fields':8+len(fields)},indent=2)+'\n')
print(json.dumps({'parameters':len(values),'global_fields':len(fields),'return_boundaries':len(returns),'watched_bytes':sum(n for a,n in regions)}))
