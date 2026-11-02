import test from 'node:test';
import assert from 'node:assert/strict';
import { createHash } from 'node:crypto';
import { spawnSync } from 'node:child_process';
import { publishAndPrune, staleReleases } from './release.mjs';

const old = id => ({ id, tag_name: `v${id}`, draft: false,
  published_at: new Date(Date.UTC(2026, 0, id)).toISOString() });
const artifact = { name: 'PharaohRestoreC.exe', data: Buffer.from('test-binary') };
const digest = 'sha256:' + createHash('sha256').update(artifact.data).digest('hex');

test('refuses branch pushes before packaging or making any API calls', () => {
  const result = spawnSync(process.execPath, ['.github/scripts/release.mjs'], {
    env: { ...process.env, GITHUB_EVENT_NAME: 'push', GITHUB_REF_TYPE: 'branch',
      GITHUB_REF_NAME: 'main', GH_TOKEN: '' }, encoding: 'utf8'
  });
  assert.equal(result.status, 1);
  assert.match(result.stderr, /Releases require a pushed Git tag/);
});

test('retains three newest publications across stable and WIP releases, leaving drafts and tags alone', () => {
  const releases = [old(2), { ...old(6), draft: true }, old(1),
    { ...old(4), prerelease: true }, old(5), old(3)];
  assert.deepEqual(staleReleases(releases).map(r => r.id), [2, 1]);
  assert.deepEqual(staleReleases([old(1), old(2), old(3)]), []);
});

function fixture({ uploadFails = false, badDigest = false, existingPublished = false } = {}) {
  const events = [];
  const release = { ...old(6), draft: !existingPublished, target_commitish: 'abc', assets: [] };
  const previous = [old(1), old(3), { ...old(9), draft: true }, old(2), old(5), old(4)];
  let published = existingPublished, listings = 0;
  const request = async (method, route, body) => {
    events.push([method, route, body]);
    if (method === 'GET') {
      ++listings;
      return published ? [...previous, { ...release, draft: false }] : previous;
    }
    if (method === 'POST') { assert.equal(body.draft, true);return release; }
    if (method === 'PATCH') { published = !body.draft;return { ...release, draft: body.draft }; }
    if (method === 'DELETE') { assert.match(route, /\/releases\/\d+$/);return; }
    assert.fail(`Unexpected ${method} ${route}`);
  };
  const upload = async () => {
    events.push(['UPLOAD']);
    if (uploadFails) throw new Error('upload failed');
    return { size: artifact.data.length, digest: badDigest ? 'sha256:wrong' : digest };
  };
  return { events, listings: () => listings, options: { request, upload, repository: 'owner/repo',
    tag: 'v6', commit: 'abc', artifacts: [artifact], log: () => {} } };
}

test('publishes verified artifacts before pruning only the older published releases', async () => {
  const f = fixture();
  await publishAndPrune(f.options);
  const mutations = f.events.filter(e => e[0] !== 'GET');
  assert.deepEqual(mutations.map(e => e[0]), ['POST', 'UPLOAD', 'PATCH', 'DELETE', 'DELETE', 'DELETE']);
  assert.deepEqual(mutations.slice(3).map(e => e[1]),
    ['/repos/owner/repo/releases/3', '/repos/owner/repo/releases/2', '/repos/owner/repo/releases/1']);
  assert.equal(mutations[2][2].draft, false);
});

for (const flag of ['uploadFails', 'badDigest']) {
  test(`does not publish or prune previous releases when ${flag}`, async () => {
    const f = fixture({ [flag]: true });
    await assert.rejects(publishAndPrune(f.options));
    assert.equal(f.listings(), 1);
    assert.ok(!f.events.some(e => ['PATCH', 'DELETE'].includes(e[0])));
  });
}

test('rerunning an already published commit preserves its assets and finishes retention', async () => {
  const f = fixture({ existingPublished: true });
  await publishAndPrune(f.options);
  assert.ok(!f.events.some(e => ['POST', 'PATCH', 'UPLOAD'].includes(e[0])));
  assert.equal(f.events.filter(e => e[0] === 'DELETE').length, 3);
});

test('reads all API pages before pruning, including more than 100 releases', async () => {
  const release = { ...old(200), target_commitish: 'abc' };
  const many = [release, ...Array.from({ length: 104 }, (_, i) => old(i + 1))];
  const deleted = [];
  const request = async (method, route) => {
    if (method === 'DELETE') { deleted.push(Number(route.split('/').at(-1)));return; }
    const page = Number(new URL('https://api.github.com' + route).searchParams.get('page'));
    return many.slice((page - 1) * 100, page * 100);
  };
  await publishAndPrune({ request, upload: () => assert.fail('No replacement upload'), repository: 'owner/repo',
    tag: release.tag_name, commit: 'abc', artifacts: [artifact], log: () => {} });
  assert.equal(deleted.length, 102);
  assert.ok(!deleted.some(id => [200, 104, 103].includes(id)));
});
