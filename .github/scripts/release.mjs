import { createHash } from 'node:crypto';
import { readFile, mkdir, writeFile, appendFile } from 'node:fs/promises';
import path from 'node:path';
import { pathToFileURL } from 'node:url';

const sha256 = data => createHash('sha256').update(data).digest('hex');

export function staleReleases(releases) {
  return releases.filter(r => !r.draft).sort((a, b) =>
    Date.parse(b.published_at ?? b.created_at) - Date.parse(a.published_at ?? a.created_at)
    || b.id - a.id).slice(3);
}

async function listReleases(request, base) {
  const releases = [];
  for (let page = 1; ; ++page) {
    const batch = await request('GET', `${base}?per_page=100&page=${page}`);
    releases.push(...batch);
    if (batch.length < 100) return releases;
  }
}

export async function publishAndPrune({ request, upload, repository, tag, commit, artifacts, log = console.log }) {
  const base = `/repos/${repository}/releases`;
  const previous = await listReleases(request, base);
  let release = previous.find(r => r.tag_name === tag);
  if (release && !release.draft) {
    // A rerun after publication can finish cleanup without replacing binaries.
    if (release.target_commitish !== commit) throw new Error('Published tag already belongs to a different source commit.');
    log(`Release ${tag} is already published; preserving its assets.`);
  } else {
    const metadata = {
      tag_name: tag, target_commitish: commit,
      name: `LostTomb ${tag} - Work in progress (Windows x86)`,
      draft: true, prerelease: true, make_latest: 'false',
      body: [
        '## Work in progress / experimental prerelease',
        '',
        'Automatically built from the tagged C sources with GitHub Actions.',
        'Download `PharaohRestoreC.exe` and place it beside `Pharaoh.inf`,',
        '`Language.inf` and `Data` in your own Pharaoh + Cleopatra 2.1 installation.',
        'Keep the installation\'s supporting files and media DLLs available.',
        'Game data, original executables and third-party DLLs are not included.',
        '',
        'Read `README.txt` for run instructions and `build-info.json` for source/build identification.',
        'The Windows x86 Release build and link/data diagnostic passed. Live gameplay is not confirmed.',
        'Crashes, visual glitches and incomplete gameplay remain possible.',
        '',
        'Reconstruction tools: Rea, Ghidra, Capstone, Unicorn, Python, MSVC, CMake, MSBuild, PowerShell and OpenAI Codex.',
        '',
        'Only the three most recently published releases are retained; Git tags and source history remain available.'
      ].join('\n')
    };
    release = release
      ? await request('PATCH', `${base}/${release.id}`, metadata)
      : await request('POST', base, metadata);
    for (const artifact of artifacts) {
      const existing = release.assets?.find(a => a.name === artifact.name);
      if (existing) await request('DELETE', `${base}/assets/${existing.id}`);
      const asset = await upload(release, artifact);
      if (asset.size !== artifact.data.length || asset.digest !== `sha256:${sha256(artifact.data)}`) {
        throw new Error(`Uploaded asset failed size/digest verification: ${artifact.name}`);
      }
    }
    release = await request('PATCH', `${base}/${release.id}`, { draft: false, make_latest: 'false' });
    if (release.draft) throw new Error('Release publication did not finish.');
  }
  // Never prune anything until all uploads and publication have succeeded.
  const published = await listReleases(request, base);
  for (const stale of staleReleases(published)) {
    await request('DELETE', `${base}/${stale.id}`);
    log(`Removed older release ${stale.tag_name}; its Git tag is preserved.`);
  }
  return release;
}

async function artifactsForBuild(env) {
  const exe = await readFile('restore/build/c-only/Release/PharaohRestoreC.exe');
  const info = {
    project: 'LostTomb', tag: env.GITHUB_REF_NAME, source_commit: env.GITHUB_SHA,
    status: 'work in progress / experimental prerelease', platform: 'Windows x86 (Win32)',
    executable: 'PharaohRestoreC.exe', bytes: exe.length, sha256: sha256(exe),
    workflow_run: `${env.GITHUB_SERVER_URL}/${env.GITHUB_REPOSITORY}/actions/runs/${env.GITHUB_RUN_ID}`,
    validation: 'Windows x86 Release build and no-argument link/data diagnostic passed. Live gameplay not confirmed.',
    original_executable_required_for_build: false, original_executable_required_for_runtime: false,
    runtime_dependencies: 'User-supplied Pharaoh + Cleopatra 2.1 game data and installation media DLLs'
  };
  const readme = [
    'LostTomb - Work in progress / experimental Windows x86 build', '',
    'Place PharaohRestoreC.exe in your own Pharaoh + Cleopatra 2.1 installation,',
    'beside Pharaoh.inf, Language.inf and Data. Keep its supporting files and media DLLs.',
    'The launcher applies 16-bit color compatibility when needed.',
    'The original EXE is not required; the original game data is required.',
    'Game data, media and third-party DLLs are not distributed.', '',
    'Crashes, graphical glitches and incomplete gameplay remain possible.',
    'Use a separate copy of your saves while testing this unfinished reconstruction.', '',
    `Sources and instructions: ${env.GITHUB_SERVER_URL}/${env.GITHUB_REPOSITORY}`, ''
  ].join('\n');
  const artifacts = [
    { name: 'PharaohRestoreC.exe', data: exe, type: 'application/octet-stream' },
    { name: 'README.txt', data: Buffer.from(readme), type: 'text/plain' },
    { name: 'build-info.json', data: Buffer.from(JSON.stringify(info, null, 2) + '\n'), type: 'application/json' }
  ];
  artifacts.push({ name: 'SHA256SUMS.txt', type: 'text/plain', data: Buffer.from(
    artifacts.map(a => `${sha256(a.data)}  ${a.name}\n`).join('')) });
  await mkdir('release-output', { recursive: true });
  for (const a of artifacts) await writeFile(path.join('release-output', a.name), a.data);
  return artifacts;
}

async function main() {
  const env = process.env;
  if (env.GITHUB_EVENT_NAME !== 'push' || env.GITHUB_REF_TYPE !== 'tag' || !env.GITHUB_REF_NAME) {
    throw new Error('Releases require a pushed Git tag.');
  }
  if (!env.GH_TOKEN || !/^[\w.-]+\/[\w.-]+$/.test(env.GITHUB_REPOSITORY ?? '') ||
      !/^[0-9a-f]{40}$/.test(env.GITHUB_SHA ?? '') || env.GITHUB_SERVER_URL !== 'https://github.com') {
    throw new Error('Missing or invalid GitHub Actions release context.');
  }
  const artifacts = await artifactsForBuild(env);
  async function send(method, url, body, type = 'application/json') {
    const response = await fetch(url, {
      method, headers: { Authorization: `Bearer ${env.GH_TOKEN}`, Accept: 'application/vnd.github+json',
        'Content-Type': type, 'X-GitHub-Api-Version': '2026-03-10' },
      body: body === undefined ? undefined : body, signal: AbortSignal.timeout(120000)
    });
    if (!response.ok) throw new Error(`GitHub ${method} ${response.status}: ${await response.text()}`);
    return response.status === 204 ? undefined : response.json();
  }
  const request = (method, route, body) => send(method, `https://api.github.com${route}`,
    body === undefined ? undefined : JSON.stringify(body));
  const upload = (release, artifact) => {
    const url = new URL(release.upload_url.replace(/\{.*$/, ''));
    if (url.origin !== 'https://uploads.github.com' ||
        url.pathname !== `/repos/${env.GITHUB_REPOSITORY}/releases/${release.id}/assets`) {
      throw new Error('Unexpected release upload endpoint.');
    }
    url.searchParams.set('name', artifact.name);
    return send('POST', url, artifact.data, artifact.type);
  };
  const release = await publishAndPrune({ request, upload, repository: env.GITHUB_REPOSITORY,
    tag: env.GITHUB_REF_NAME, commit: env.GITHUB_SHA, artifacts });
  console.log(release.html_url);
  if (env.GITHUB_STEP_SUMMARY) await appendFile(env.GITHUB_STEP_SUMMARY,
    `Published [${env.GITHUB_REF_NAME}](${release.html_url}) with a direct Windows x86 EXE. Retaining the newest three releases.\n`);
}

if (process.argv[1] && import.meta.url === pathToFileURL(path.resolve(process.argv[1])).href) {
  main().catch(error => { console.error(error.message); process.exitCode = 1; });
}
