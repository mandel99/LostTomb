"""Publish immutable evidence for existing C comparison members without source credit."""
import argparse,json,re
from pathlib import Path
from registry import ROOT,LEDGER,digest,save


def reference(path):
    path=Path(path)
    return dict(path=path.relative_to(ROOT).as_posix(),sha256=digest(path))


def read(path):return json.loads(Path(path).read_bytes())


def publish(result,packet,phase,scope):
    assert re.fullmatch(r'[a-z0-9][a-z0-9-]*',phase)
    result=Path(result).resolve();packet=Path(packet).resolve()
    assert packet.is_relative_to(ROOT) and packet.is_dir()
    report=read(result);directory=result.parent;binding=read(directory/'binding.json')
    assert report['cases']>0 and report['failed_cases']==0 and report['inputs_stable']
    assert all(c['passed']for c in report['cases_detail'])
    assert report['binding_sha256']==digest(directory/'binding.json')
    assert report['case_inputs_sha256']==digest(directory/'case-inputs.json')
    assert digest(LEDGER)==binding['registry']['source_ledger_sha256'],'ledger changed after replay'
    for name,sha in binding['source_hashes'].items():assert digest(ROOT/name)==sha,name
    registry=binding['registry'];ledger=read(LEDGER)
    assert registry['source_entries']==ledger['selected_entries']==len(ledger['members'])
    for image in registry['compiled_images'].values():
        assert digest(ROOT/image['path'])==image['sha256']
        assert digest(ROOT/image['map_path'])==image['map_sha256']
    selected={int(m['address'],16)for m in ledger['members']}
    entries={int(a,16)for a in report['compared_entries']}
    members={int(m['address'],16):m for m in registry['members']}
    assert entries and entries.isdisjoint(selected)
    for address in entries:
        m=members[address]
        assert m['supplemental_comparison_only'] and m['new_source_credit']==m['new_entry_credit']==0
        assert digest(ROOT/m['source'])==m['source_sha256']
    manifest=read(packet/'manifest.json')
    assert digest(packet/'manifest.json')==registry['supplemental_comparison']['manifest_sha256']
    for item in manifest['files']:assert digest(packet/item['path'])==item['sha256']
    protocol=read(packet/'protocol.json')
    dest=ROOT/f'metadata/cohort7-{phase}-comparison.json';assert not dest.exists(),'receipt exists'
    snapshot=ROOT/f'build/actor-animal-following/{phase}-publication/executed-master.json'
    assert not snapshot.exists();snapshot.parent.mkdir(parents=True,exist_ok=True)
    snapshot.write_bytes(LEDGER.read_bytes())
    packet_hashes={p.relative_to(ROOT).as_posix():digest(p)for p in packet.iterdir()if p.is_file()}
    rows=[]
    for address in sorted(entries):
        m=members[address];cases=[c for c in report['cases_detail']if int(c['entry'],16)==address]
        rows.append(dict(address=hex(address),name=m['name'],source=m['source'],source_sha256=m['source_sha256'],body_sha256=m['body_sha256'],case_ids=[c['id']for c in cases],cases=len(cases),invocations=sum(len(c['routes'])for c in cases)))
    receipt=dict(schema='cohort7-supplemental-comparison-v1',native_sha256=registry['native']['sha256'],selected_source_entries=registry['source_entries'],comparison_registry_entries=registry['comparison_entries'],supplemental_entries_compared=len(entries),new_selected_entries=0,new_algorithms=0,entries_compared=len(entries),cases=report['cases'],invocations=report['invocations'],failed_cases=0,inputs_stable=True,result=reference(result),binding=reference(directory/'binding.json'),case_inputs=reference(directory/'case-inputs.json'),source_input_ledger=reference(snapshot),packet_manifest=reference(packet/'manifest.json'),contracts=reference(packet/'protocol.json'),publication_tool=reference(__file__),source_hashes=packet_hashes,compiled_images=registry['compiled_images'],members=rows,scope=scope,pending_domains=protocol.get('pending_domains',['Unvisited mapped inputs, callbacks, allocation/fault and alias domains remain unproved.']),joint_gate_complete=False,cohort_validation_complete=False,whole_game_equivalence=False,host_execution=False)
    save(dest,receipt)
    runner_path=ROOT/'metadata/cohort7-joint-runner-contracts.json';runner=read(runner_path)
    runner.setdefault('supplemental_comparisons',[]).append(reference(dest));save(runner_path,runner)
    progress_path=ROOT/'metadata/cohort7-source-progress.json';progress=read(progress_path)
    progress['joint_runner_contracts']=reference(runner_path)
    progress.setdefault('supplemental_comparisons',[]).append(reference(dest));save(progress_path,progress)
    plan_path=ROOT/'metadata/cohort7-joint-verification-plan.json';plan=read(plan_path)
    plan['runner_contracts']=reference(runner_path)
    plan.setdefault('supplemental_comparisons',[]).append(reference(dest))
    present=set()
    for row in plan['compiled_only_contract_entries']:
        address=int(row['address'],16)
        if address in entries:
            if row.get('focused_comparison'):row.setdefault('prior_focused_comparisons',[]).append(row['focused_comparison'])
            row.update(status='focused_current_image_independent_entry_cases_passed_remaining_domains_pending',focused_case_ids=next(r['case_ids']for r in rows if r['address']==hex(address)),focused_comparison=reference(dest),whole_entry_equivalence=False,current_C_provider_source=members[address]['source'],current_C_provider_definition=members[address]['name'])
            present.add(address)
    assert present==entries,'supplemental entry is absent from explicit compiled-only plan'
    done=sum('focused_comparison'in r for r in plan['compiled_only_contract_entries'])
    remaining=len(plan['compiled_only_contract_entries'])-done
    plan['compiled_only_entries_with_focused_current_comparison']=done
    plan['compiled_only_entries_without_independent_current_comparison']=remaining
    save(plan_path,plan)
    queue_path=ROOT/'metadata/bulk-reconstruction-next-queue.json';queue=read(queue_path)
    queue['joint_verification_plan']=reference(plan_path);queue['latest_focused_joint_comparison']=reference(dest)
    queue['next_stage']=f'Source100 frozen. Continue{remaining} compiled-only entries without independent focused comparison, all unvisited domains and supporting funclets; no new source cohort.'
    save(queue_path,queue)
    ledger['current_finite_comparison']=reference(dest);ledger['joint_runner_contracts']=reference(runner_path)
    ledger['cohort7_joint_verification_plan']=reference(plan_path)
    ledger['source_hashes'].update(packet_hashes)
    for path in [Path(__file__),dest,runner_path,progress_path,plan_path,queue_path]:ledger['source_hashes'][path.relative_to(ROOT).as_posix()]=digest(path)
    save(LEDGER,ledger)
    print(json.dumps(dict(entries=len(entries),cases=report['cases'],invocations=report['invocations'],new_selected_entries=0,compiled_only_remaining_without_focused_comparison=remaining,receipt=str(dest))))


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--result',type=Path,required=True);p.add_argument('--packet',type=Path,required=True)
    p.add_argument('--phase',required=True);p.add_argument('--scope',required=True)
    args=p.parse_args();publish(args.result,args.packet,args.phase,args.scope)
