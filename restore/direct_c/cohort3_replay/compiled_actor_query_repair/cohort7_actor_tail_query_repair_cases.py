"""Derived 4F53D0 fixtures with independently authored 4EF450 outputs.

The helper executes normally. Its output count/vector are never stubbed.
Original actor-tail fixtures are preserved in their original packet.
"""
from compiled_flood_regions.cohort7_compiled_acceptance_cases import Case as BaseCase

ENTRY=0x4f53d0
BUILDING=2
ROW=0x7999d8+BUILDING*264
OUTPUT=0xeaac44
OUTPUT_WORDS=144
OUTPUT_GUARD=OUTPUT-4
COUNT=0xeaaf58
SENTINEL=0xc7c7c7c7


class Case(BaseCase):
    def __init__(self,label):
        super().__init__(ENTRY,label,0x89abcdef,[BUILDING],named=[BUILDING],result=255)
        self.c['id']='c7-actor-tail-query-repair/004f53d0/'+label
        self.c.update(compare_x87_values=True,compare_x87_control=True,
            initial_fpcw=0x27f,initial_x87=['0000000000000080ff3f','000000000000008000c0'])


def make_case(label,component,classify=0,blocked=0,early=False,reset=False):
    c=Case(label)
    c.block(ROW,16*264,0)
    for i in range(16):
        c.put(ROW+i*264+0x55,1,1)
        c.put(ROW+i*264+0x56,component,1)
        c.put(ROW+i*264+0x57,0,1)
    c.put(ROW+0x55,2 if early else 1,1)
    c.put(ROW+0x54,0,1)   # Actual category0, original row-count16.
    c.put(ROW+18,0,2)     # Signed building-link0 => first ID2.
    c.put(ROW+12,1000)
    if reset:
        c.put(ROW+14*264+0x56,2,1)
        c.put(ROW+15*264+0x56,2,1)
    c.block(OUTPUT_GUARD,4+OUTPUT_WORDS*4+4,0xc7)
    c.put(OUTPUT_GUARD,0xa1b2c3d4)
    c.put(OUTPUT+OUTPUT_WORDS*4,0xe5f60718)
    for at,value in [(COUNT,0x91234567),(0xeaae90,0),(0x5e4ef4,0),(0xbca2d8,1229)]:
        c.block(at,4);c.put(at,value)
    c.start([(COUNT,4),(0xeaae90,4)]+[(OUTPUT+i*4,4)for i in range(16)]+
        [(OUTPUT_GUARD,4),(OUTPUT+OUTPUT_WORDS*4,4)])
    c.endpoint('classify-category',0x4ecea0,1,'al8')
    c.endpoint('validate-pyramid-cells',0x4ed360,4)
    if early:
        ids=[];logical_count=0x91234567;answer=0
    elif component:
        # Explicit finite vectors derived from86 native instructions, not
        # from execution of native/C or its generated frame model.
        ids=([16,17]+list(range(4,16)))if reset else list(range(2,18))
        logical_count=2 if reset else 16
        for index,value in enumerate(ids):c.put(OUTPUT+index*4,value)
        c.put(COUNT,logical_count)
        answer=0   # Parent rejects nonzero count before classifier.
    else:
        ids=[];logical_count=0;c.put(COUNT,0)
        c.event('classify-category',[0],classify)
        if classify:
            c.event('validate-pyramid-cells',[0xffffffff,0xffffffff,3,0],
                mutations=[(0xeaae90,4,blocked)])
        answer=int(not blocked)
    c.c['helper_output_oracle']=dict(count=logical_count,
        changed_prefix=ids,remaining_word=SENTINEL,
        output_base=OUTPUT,watched_words=OUTPUT_WORDS,
        preserves_stale_tail=reset,helper_executed=not early)
    return c.end(answer)


def cases(registry=None):
    out=[
        make_case('original-component1-appends16-before-parent-reject',1),
        make_case('best-row-reset-keeps-stale-output-tail',1,reset=True),
        make_case('component0-skips16-classifier-false',0),
        make_case('component0-skips16-signed-cell-clear',0,classify=1),
        make_case('component0-skips16-DWORD-cell-blocked',0,classify=1,blocked=0x100),
        make_case('BYTE-progress-early-return-preserves-helper-outputs',1,early=True)]
    if registry is None:return out
    matches=[m for m in registry['members']if int(m['address'],16)==ENTRY]
    if not matches:return[]
    providers=matches[0].get('providers')
    if providers:
        n=1+sum(bool(v.get('named'))+bool(v.get('canonical'))for v in providers.values())
        for c in out:c['expected_invocations']=n
    return out


definitions=cases
