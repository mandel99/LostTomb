# Eleven callbacks: C-first source checkpoint

This block follows the requested translation-first workflow. Eleven more
callbacks now have ordinary named C bodies in `runtime/actor_command.c`, with
their canonical entries and full-result raw dispatch connected to those bodies.
Their obsolete per-function register models are removed. The generic service
frame helper remains for other callbacks; it was not removed with Missionary.

| Entry | Kind/name | C name suffix | Native instructions |
| --- | --- | --- | ---: |
| 494F70 | 38 Docker | docker | 567 |
| 497010 | 41 Barbarian | barbarian | 278 |
| 497450 | 42 Sentry | sentry | 383 |
| 49A150 | 58 Native Trader | native_trader | 276 |
| 49A5A0 | 59 Arrow | arrow | 208 |
| 49A880 | 60 Javelin | javelin | 238 |
| 49AC00 | 61 Bolt | bolt | 201 |
| 49AEB0 | 62 Ballista | ballista | 247 |
| 49B280 | 63 Creature | creature | 7 |
| 49B2A0 | 64 Missionary | missionary | 63 |
| 49B610 | 67 Shipwreck | shipwreck | 72 |

Every named entry is `uint32_t restore_actor_update_SUFFIX(void)`. The2,540
native instructions are authenticated static scopes, not execution coverage.
Exact body/registry observations were reread with REA and checked against the
pinned original. Unobserved provider ownership is not invented.

Both main Release images link and the entire C module passes Win32 C11
`/W4 /WX /GS- /Ob0 /O2` compilation. **Detailed whole-block main/native
comparison and broad regression validation remain pending.** Earlier candidate
protocol/interface witnesses are not rebound to the current main images.

The C preserves the meaningful distinctions instead of using one generic
callback model. Examples include Docker's two timer increments and resource
overlay flip, Native Trader's final low-byte replacement, the projectile
division-before-multiplication damage rule, full child Boolean tests, signed
age/timers, cached aiming actors versus fresh launch IDs, and Shipwreck's
separate WORD multiply/+7 stores. Creature returns actorID; Missionary returns
orientation or the death sprite base, not the stored final sprite.

Barbarian's early terrain path uses the existing void4BD250 provider and
returns the cached actorID explicitly. This is derived from its authenticated
21-byte native body, which defines EAX from that argument and never changes it;
no invented result bridge or assumed physical C-void result is used.

Four callbacks add typed cue consumers: Arrow49A7A7, Javelin49AA69/49AB21 and
Bolt49AE06. Their local native suffix support exists; the combined current C
source inventory will be refreshed at the later verification milestone. The
full old RAW cue route and the earlier Ostrich/Antelope compatibility paths
remain unchanged. Physical volatile-register outputs are not a new guarantee.

This builds on the six-animal source checkpoint `5b5a9ce`. Those six and these
eleven give17 newly transcribed callbacks /6,734 static native instructions
after the last accepted native-comparison checkpoint `1683857`. Both earlier
records keep their original hashes and remain historical. No host game was
launched, CMake is unchanged, and original CD fallback remains commented and
disabled. The complete game reconstruction goal remains unfinished.

Current source/build hashes and explicit pending states are in
`actor-service-translation-batch.json`; curated native bytes are in
`actor-service-translation-rea-records.json`.
