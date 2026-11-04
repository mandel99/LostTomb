# 2D sample playback and cache lifecycle, 2026-11-04

Six canonical entries delegate to named C providers. Five decompiler bodies
are replaced; the existing reset is rewritten to preserve native pointer
reloads rather than copying into one cached destination. No assembler or game
algorithm stub is added.

| Address | Provider | Owned native instructions |
| --- | --- | ---: |
| 0055E5D2 | restore_sound_reset_sample | 57 |
| 0055ECF3 | restore_sound_refresh_cache_references | 63 |
| 0055EDD0 | restore_sound_load_sample_cache | 154 |
| 0055F02D | restore_sound_find_available_channel | 28 |
| 0055F07F | restore_sound_find_cached_sample | 34 |
| 0055F0E8 | restore_sound_play_sample | 174 |

`RestoreSoundRequest` names the first five channel DWORDs: owner, file, volume,
pan and loop count. Each channel record is40 bytes; offset24 holds its cache
index. Each12-byte cache row contains file ID, backend data pointer and active
reference count. The preserved object offsets are count198, handles19C,
records1A8, cache1AC, DWORD reverb enable168, three raw reverb arguments16C/170/174
and DWORD disabled200.

The native behavior includes several consequential details:

- Refresh first clears every cache reference count, then counts playing
  channels whose full backend status equals4. It rereads channel cache indices
  and cache pointers separately when calculating and storing an increment.
- Cache search requires both a matching file ID and nonzero data pointer.
  Free-channel search accepts any full status value other than4. These two
  search methods have no disabled gate.
- Cold loading rejects null/empty filenames, then chooses the first cache
  entry with zero references. Existing data is freed before resetting the
  selected channel. It copies the five request fields before backend file
  loading; failure leaves those stores and the published null data pointer in
  place. Disabled cold loading returns1. Other early failures return-1.
- Playback checks signed file range using the wrapped `file_count-1` value,
  suppresses an already playing owner/file pair, selects a channel, refreshes
  cache references and searches/loads its file. Cached playback updates the
  channel's cache index, but does not copy the five request fields again.
- Native AL stores followed by MOVSX narrow both selected channel and cached
  index to signed bytes. Index128 becomes-128,255 becomes-1 and256 becomes0.
  A cold-load result is compared with full-1 before narrowing, so a successful
  slot255 is still used and publishes signed cache index-1. No new clamp or
  sentinel test is introduced after that conversion.
- Backend calls consume fresh handle/request fields in original argument-read
  order. Reverb arguments are read third, second, first, then the handle.
  Reset reloads the channel-record pointer for each of ten DWORD stores, so a
  pointer alias can redirect later stores or reproduce the original fault.

Both current Release binaries pass545 scenarios /1635 invocations each, all510
owned instructions and all56 conditional outcomes, with79 real composition
scenarios. The oracle runs actual lookup, refresh, eviction, reset, cold
loading and playback; only ten Miles endpoints are controlled. Cases include
low-byte wrap/sign boundaries, partial stores on failed reads, backend pointer
changes, request aliases, counted read mutations, DWORD/BYTE status differences,
wrapped file-count subtraction and matching explicit memory faults.

The existing source-only playback fixture also passes160 assertions in each of
Debug and Release. The initialization fixture is rebound to the actual new
reset provider and passes355 cases in both configurations.

All static/private bytes, ordered writes, child/backend arguments and meaningful
full EAX results are compared. Normal-return stack and preserved registers are
checked. Void incidental EAX and instruction/register state at faults are
excluded. Physical audio/file parsing, backend ownership internals, concurrency,
and native OS exception dispatch are not established by these finite tests.

Evidence and hashes: `sound-playback-native.json`,
`sound-playback-rea-evidence.json`, `sound-playback-diagnostic.json`,
`sound-playback-game.json` and `sound-playback-host.json`.
