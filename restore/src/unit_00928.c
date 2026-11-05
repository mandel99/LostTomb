/* Complete original-owner C repair of native00462180..00466FF1.
 * Reuses unit_00928.c's generated control structure; zero new-algorithm credit.
 * Native-byte SHA256:4edec20a296fa76fc2336997146e3136a58f30d820b462d12bcc05e6f8217d20.
 * Scalar sizes preserve the decompiler's original pointer units. Cast loads
 * and stores preserve the independent native BYTE/WORD/DWORD widths.
 */
#include "city_active_buildings.h"
#include "city_updates.h"
#include "city_spawn_helpers.h"
#include "city_worker_supply.h"
#include "city_worker_updates.h"
#include "production_updates.h"
#include "actor_basic_profile.h"
#include "actor_create.h"
#include "actor_transport.h"
#include "actor_predicate.h"
#include "actor_work.h"
#include "menu_resource_neighbors.h"
#include "menu_building_kind.h"
#include "menu_prepare_integer.h"
#include "map_commit_links.h"
#include "placement_support.h"
#include "menu_list_scroll.h"
#include "road_place.h"
#include <windows.h>
#include <mmsystem.h>

extern void FUN_00417ae0(int32_t index);
extern int32_t FUN_004807d0(int32_t x,int32_t y,int32_t kind);
#define CITY_ADD32(a,b) ((int32_t)((uint32_t)(a)+(uint32_t)(b)))
#define CITY_MUL32(a,b) ((int32_t)((uint32_t)(a)*(uint32_t)(b)))

#define DAT_00579600 (*(volatile int32_t *)(uintptr_t)0x00579600u)
#define DAT_00605a0c (*(volatile int32_t *)(uintptr_t)0x00605a0cu)
#define DAT_007027c3 (*(volatile uint8_t *)(uintptr_t)0x007027c3u)
#define DAT_007027c4 (*(volatile uint8_t *)(uintptr_t)0x007027c4u)
#define DAT_00705eb8 (*(volatile uint8_t *)(uintptr_t)0x00705eb8u)
#define DAT_007063c4 (*(volatile int32_t *)(uintptr_t)0x007063c4u)
#define DAT_0070661c (*(volatile int32_t *)(uintptr_t)0x0070661cu)
#define DAT_007066bc (*(volatile int32_t *)(uintptr_t)0x007066bcu)
#define DAT_0070670c (*(volatile int32_t *)(uintptr_t)0x0070670cu)
#define DAT_0070693c (*(volatile int32_t *)(uintptr_t)0x0070693cu)
#define DAT_00706964 (*(volatile int32_t *)(uintptr_t)0x00706964u)
#define DAT_00708214 (*(volatile int32_t *)(uintptr_t)0x00708214u)
#define DAT_00709648 (*(volatile int32_t *)(uintptr_t)0x00709648u)
#define DAT_0078d584 (*(volatile int32_t *)(uintptr_t)0x0078d584u)
#define DAT_0078d790 (*(volatile int32_t *)(uintptr_t)0x0078d790u)
#define DAT_0078d7c4 (*(volatile int32_t *)(uintptr_t)0x0078d7c4u)
#define DAT_0078d828 (*(volatile int32_t *)(uintptr_t)0x0078d828u)
#define DAT_007900c2 (*(volatile int32_t *)(uintptr_t)0x007900c2u)
#define DAT_007999d8 (*(volatile uint8_t *)(uintptr_t)0x007999d8u)
#define DAT_007999d9 (*(volatile uint8_t *)(uintptr_t)0x007999d9u)
#define DAT_007999db (*(volatile uint8_t *)(uintptr_t)0x007999dbu)
#define DAT_007999dc (*(volatile int32_t *)(uintptr_t)0x007999dcu)
#define DAT_007999e0 (*(volatile uint8_t *)(uintptr_t)0x007999e0u)
#define DAT_007999e4 (*(volatile uint8_t *)(uintptr_t)0x007999e4u)
#define DAT_007999e8 (*(volatile uint16_t *)(uintptr_t)0x007999e8u)
#define DAT_007999ea (*(volatile uint16_t *)(uintptr_t)0x007999eau)
#define DAT_007999f0 (*(volatile uint16_t *)(uintptr_t)0x007999f0u)
#define DAT_007999fe (*(volatile uint16_t *)(uintptr_t)0x007999feu)
#define DAT_00799a00 (*(volatile uint16_t *)(uintptr_t)0x00799a00u)
#define DAT_00799a02 (*(volatile uint16_t *)(uintptr_t)0x00799a02u)
#define DAT_00799a04 (*(volatile uint16_t *)(uintptr_t)0x00799a04u)
#define DAT_00799a08 (*(volatile uint8_t *)(uintptr_t)0x00799a08u)
#define DAT_00799a0a (*(volatile uint8_t *)(uintptr_t)0x00799a0au)
#define DAT_00799a0d (*(volatile uint8_t *)(uintptr_t)0x00799a0du)
#define DAT_00799a10 (*(volatile uint8_t *)(uintptr_t)0x00799a10u)
#define DAT_00799a12 (*(volatile uint8_t *)(uintptr_t)0x00799a12u)
#define DAT_00799a14 (*(volatile uint16_t *)(uintptr_t)0x00799a14u)
#define DAT_00799a18 (*(volatile uint16_t *)(uintptr_t)0x00799a18u)
#define DAT_00799a1b (*(volatile uint8_t *)(uintptr_t)0x00799a1bu)
#define DAT_00799a2c (*(volatile uint16_t *)(uintptr_t)0x00799a2cu)
#define DAT_00799a2e (*(volatile uint16_t *)(uintptr_t)0x00799a2eu)
#define DAT_00799a3c (*(volatile uint16_t *)(uintptr_t)0x00799a3cu)
#define DAT_00799a3e (*(volatile uint16_t *)(uintptr_t)0x00799a3eu)
#define DAT_00799a40 (*(volatile uint16_t *)(uintptr_t)0x00799a40u)
#define DAT_00799a42 (*(volatile uint16_t *)(uintptr_t)0x00799a42u)
#define DAT_00799a44 (*(volatile uint16_t *)(uintptr_t)0x00799a44u)
#define DAT_00799a76 (*(volatile uint8_t *)(uintptr_t)0x00799a76u)
#define DAT_00799a78 (*(volatile uint8_t *)(uintptr_t)0x00799a78u)
#define DAT_00799a7a (*(volatile int32_t *)(uintptr_t)0x00799a7au)
#define DAT_00799a7e (*(volatile uint16_t *)(uintptr_t)0x00799a7eu)
#define DAT_00799a80 (*(volatile uint16_t *)(uintptr_t)0x00799a80u)
#define DAT_00799a82 (*(volatile uint16_t *)(uintptr_t)0x00799a82u)
#define DAT_00799a84 (*(volatile int32_t *)(uintptr_t)0x00799a84u)
#define DAT_00799a8a (*(volatile uint8_t *)(uintptr_t)0x00799a8au)
#define DAT_00799a8c (*(volatile uint8_t *)(uintptr_t)0x00799a8cu)
#define DAT_00799a8e (*(volatile uint8_t *)(uintptr_t)0x00799a8eu)
#define DAT_00799a98 (*(volatile uint16_t *)(uintptr_t)0x00799a98u)
#define DAT_00799a9a (*(volatile uint16_t *)(uintptr_t)0x00799a9au)
#define DAT_00799a9c (*(volatile int32_t *)(uintptr_t)0x00799a9cu)
#define DAT_00799aa4 (*(volatile int32_t *)(uintptr_t)0x00799aa4u)
#define DAT_00799af0 (*(volatile uint16_t *)(uintptr_t)0x00799af0u)
#define DAT_009784dc (*(volatile int32_t *)(uintptr_t)0x009784dcu)
#define DAT_00978d98 (*(volatile uint8_t *)(uintptr_t)0x00978d98u)
#define DAT_00978d9a (*(volatile uint8_t *)(uintptr_t)0x00978d9au)
#define DAT_00ac2d4c (*(volatile int8_t *)(uintptr_t)0x00ac2d4cu)
#define DAT_00acf85c (*(volatile uint8_t *)(uintptr_t)0x00acf85cu)
#define DAT_00b7482c (*(volatile int32_t *)(uintptr_t)0x00b7482cu)
#define DAT_00bca348 (*(volatile uint8_t *)(uintptr_t)0x00bca348u)
#define DAT_00bcb9ac (*(volatile int32_t *)(uintptr_t)0x00bcb9acu)
#define DAT_00bcd602 (*(volatile uint8_t *)(uintptr_t)0x00bcd602u)
#define DAT_00bcd603 (*(volatile uint8_t *)(uintptr_t)0x00bcd603u)
#define DAT_00bcd606 (*(volatile uint8_t *)(uintptr_t)0x00bcd606u)
#define DAT_00bcd60c (*(volatile uint16_t *)(uintptr_t)0x00bcd60cu)
#define DAT_00bcd60e (*(volatile uint16_t *)(uintptr_t)0x00bcd60eu)
#define DAT_00bcd61c (*(volatile uint8_t *)(uintptr_t)0x00bcd61cu)
#define DAT_00bcd61e (*(volatile uint8_t *)(uintptr_t)0x00bcd61eu)
#define DAT_00bcd62e (*(volatile uint8_t *)(uintptr_t)0x00bcd62eu)
#define DAT_00bcd630 (*(volatile uint8_t *)(uintptr_t)0x00bcd630u)
#define DAT_00bcd652 (*(volatile uint16_t *)(uintptr_t)0x00bcd652u)
#define DAT_00bcd656 (*(volatile uint16_t *)(uintptr_t)0x00bcd656u)
#define DAT_00bcd668 (*(volatile uint8_t *)(uintptr_t)0x00bcd668u)
#define DAT_00bcd66c (*(volatile uint8_t *)(uintptr_t)0x00bcd66cu)
#define DAT_00bcd672 (*(volatile uint8_t *)(uintptr_t)0x00bcd672u)
#define DAT_00bcd690 (*(volatile uint8_t *)(uintptr_t)0x00bcd690u)
#define DAT_00bcd6a4 (*(volatile uint8_t *)(uintptr_t)0x00bcd6a4u)
#define DAT_00bcd714 (*(volatile uint8_t *)(uintptr_t)0x00bcd714u)
#define DAT_00bcd71c (*(volatile uint8_t *)(uintptr_t)0x00bcd71cu)
#define DAT_00bcd720 (*(volatile uint8_t *)(uintptr_t)0x00bcd720u)
#define DAT_00bcd724 (*(volatile uint8_t *)(uintptr_t)0x00bcd724u)
#define DAT_00bcd786 (*(volatile uint8_t *)(uintptr_t)0x00bcd786u)
#define DAT_00d056a0 (*(volatile int32_t *)(uintptr_t)0x00d056a0u)
#define DAT_00d056a4 (*(volatile int32_t *)(uintptr_t)0x00d056a4u)
#define DAT_00d056a8 (*(volatile int32_t *)(uintptr_t)0x00d056a8u)
#define DAT_00d056ac (*(volatile int32_t *)(uintptr_t)0x00d056acu)
#define DAT_00d056b0 (*(volatile int32_t *)(uintptr_t)0x00d056b0u)
#define DAT_00d3b1c4 (*(volatile int32_t *)(uintptr_t)0x00d3b1c4u)
#define DAT_00d3b1c8 (*(volatile int32_t *)(uintptr_t)0x00d3b1c8u)
#define DAT_00d3b374 (*(volatile int32_t *)(uintptr_t)0x00d3b374u)
#define DAT_00d3b380 (*(volatile uint32_t *)(uintptr_t)0x00d3b380u)
#define DAT_00d3b384 (*(volatile uint32_t *)(uintptr_t)0x00d3b384u)
#define DAT_00d3b4e0 (*(volatile uint16_t *)(uintptr_t)0x00d3b4e0u)
#define DAT_00d3b4e4 (*(volatile uint16_t *)(uintptr_t)0x00d3b4e4u)
#define DAT_00e38ea8 (*(volatile int8_t *)(uintptr_t)0x00e38ea8u)
#define DAT_00e38ebc (*(volatile int32_t *)(uintptr_t)0x00e38ebcu)
#define DAT_00ea1894 (*(volatile uint8_t *)(uintptr_t)0x00ea1894u)
#define DAT_00ea4000 (*(volatile uint8_t *)(uintptr_t)0x00ea4000u)
#define DAT_00ea4138 (*(volatile int32_t *)(uintptr_t)0x00ea4138u)
#define DAT_00ea5ce8 (*(volatile uint16_t *)(uintptr_t)0x00ea5ce8u)
#define DAT_00ea5e0c (*(volatile int32_t *)(uintptr_t)0x00ea5e0cu)
#define DAT_00ea5f9c (*(volatile int32_t *)(uintptr_t)0x00ea5f9cu)
#define DAT_00ea5fe4 (*(volatile uint8_t *)(uintptr_t)0x00ea5fe4u)
#define DAT_00ea5ff0 (*(volatile uint8_t *)(uintptr_t)0x00ea5ff0u)
#define DAT_00ea6204 (*(volatile uint8_t *)(uintptr_t)0x00ea6204u)
#define DAT_00eaac28 (*(volatile int32_t *)(uintptr_t)0x00eaac28u)
#define DAT_00f53dd8 (*(volatile int32_t *)(uintptr_t)0x00f53dd8u)
#define DAT_00f53ddc (*(volatile int32_t *)(uintptr_t)0x00f53ddcu)
#define DAT_00f569d4 (*(volatile uint32_t *)(uintptr_t)0x00f569d4u)
#define _DAT_00d3b364 (*(volatile uint32_t *)(uintptr_t)0x00d3b364u)
#define _DAT_00d3b368 (*(volatile uint32_t *)(uintptr_t)0x00d3b368u)
#define _DAT_00d3b36c (*(volatile uint32_t *)(uintptr_t)0x00d3b36cu)
#define _DAT_00d3b370 (*(volatile uint32_t *)(uintptr_t)0x00d3b370u)
#define _DAT_00d3b378 (*(volatile uint32_t *)(uintptr_t)0x00d3b378u)
#define _DAT_00d3b4e0 (*(volatile uint32_t *)(uintptr_t)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(volatile uint32_t *)(uintptr_t)0x00d3b4e4u)

void restore_city_update_active_buildings(void)

{
  uint64_t uVar1;
  int8_t cVar2;
  uint8_t uVar3;
  uint16_t uVar4;
  int16_t sVar5;
  uint16_t uVar6;
  volatile uint8_t *pcVar7;
  int32_t iVar8;
  int32_t iVar9;
  uint32_t uVar10;
  int32_t iVar11;
  volatile uint32_t *puVar12;
  uint32_t uVar13;
  volatile int16_t *psVar14;
  int32_t iVar15;
  int32_t iVar16;
  volatile int32_t *piVar17;
  uint32_t *puVar18;
  int32_t iVar19;
  uint8_t bVar20;
  int16_t sVar21;
  int32_t iVar22;
  volatile int16_t *psVar23;
  uint32_t uVar24;
  DWORD (WINAPI *pcVar25)(void);
  uint32_t uVar26;
  uint32_t uVar27;
  int32_t iVar28;
  uint8_t bVar29;
  uint32_t uVar30;
  uint32_t uVar31;
  uint8_t local_49;
  uint8_t local_48;
  int8_t local_47;
  uint8_t local_46;
  int8_t local_45;
  int32_t local_44;
  int32_t local_40;
  int32_t local_3c;
  int32_t local_38;
  int32_t local_34;
  int32_t local_30;
  int32_t local_2c;
  int32_t local_28;
  uint32_t local_24;
  int32_t local_20;
  uint32_t local_1c;
  int32_t local_18;
  int32_t local_14;
  int32_t local_10 [4];
  int32_t periodic_delay;

  local_28 = 100;
  local_2c = 100;
  local_47 = (int8_t)('\0');
  local_3c = 0;
  local_34 = 0;
  local_49 = (uint8_t)('\0');
  local_48 = (uint8_t)('\0');
  local_24 = 0;
  /* Native462192 snapshots the delay before the two reset stores. */
  periodic_delay = DAT_00bcb9ac;
  DAT_0078d584 = 0;
  DAT_00bca348 = (uint8_t)('\0');
  local_1c = 0;
  if (0 < periodic_delay) {
    DAT_00bcb9ac = CITY_ADD32(periodic_delay,-1);
  }
  timeGetTime();
  iVar11 = DAT_0078d790;
  if (0 < iVar11) {
    psVar14 = (volatile int16_t *)(uintptr_t)0x00799af0u;
    iVar22 = iVar11;
    do {
      sVar21 = (int16_t)(*psVar14);
      if ((((sVar21 == 0xb3) || (sVar21 == 0xb2)) || (sVar21 == 0xb1)) || (sVar21 == 0xe7)) {
        *(volatile uint8_t *)(psVar14 + 0x4b) = (uint8_t)(0);
      }
      psVar14 = psVar14 + 0x84;
      iVar22 = CITY_ADD32(iVar22,-1);
    } while (iVar22 != 0);
  }
  pcVar7 = (volatile uint8_t *)(uintptr_t)0x00bcd786u;
  do {
    if (pcVar7[4] != '\0') {
      cVar2 = (int8_t)(*pcVar7);
      if (cVar2 == 'Q') {
        iVar22 = (int32_t)*(volatile int16_t *)(pcVar7 + 0x50);
        if ((0 < iVar22) && (iVar22 <= iVar11)) {
          bVar20 = (uint8_t)(*(volatile int8_t *)(&DAT_00799a7e + CITY_MUL32(iVar22,0x84)) + 1);
          *(volatile uint8_t *)(&DAT_00799a7e + CITY_MUL32(iVar22,0x84)) = (uint8_t)(bVar20);
          if ((&DAT_007999e8)[(uint32_t)CITY_MUL32(iVar22,0x84)] == 0xb3) goto joined_r0x00462334;
        }
      }
      else if (cVar2 == 'P') {
        iVar22 = (int32_t)*(volatile int16_t *)(pcVar7 + 0x50);
        if ((0 < iVar22) && (iVar22 <= iVar11)) {
          bVar20 = (uint8_t)(*(volatile int8_t *)(&DAT_00799a7e + CITY_MUL32(iVar22,0x84)) + 1);
          *(volatile uint8_t *)(&DAT_00799a7e + CITY_MUL32(iVar22,0x84)) = (uint8_t)(bVar20);
          if ((&DAT_007999e8)[(uint32_t)CITY_MUL32(iVar22,0x84)] == 0xb2) goto joined_r0x00462334;
        }
      }
      else if (cVar2 == 'O') {
        iVar22 = (int32_t)*(volatile int16_t *)(pcVar7 + 0x50);
        if ((0 < iVar22) && (iVar22 <= iVar11)) {
          bVar20 = (uint8_t)(*(volatile int8_t *)(&DAT_00799a7e + CITY_MUL32(iVar22,0x84)) + 1);
          *(volatile uint8_t *)(&DAT_00799a7e + CITY_MUL32(iVar22,0x84)) = (uint8_t)(bVar20);
          if ((&DAT_007999e8)[(uint32_t)CITY_MUL32(iVar22,0x84)] == 0xb1) {
joined_r0x00462334:
            if (4 < bVar20) {
              pcVar7[0x2e] = (uint8_t)('\x04');
              pcVar7[0x116] = (uint8_t)('\0');
              pcVar7[0x117] = (uint8_t)('\0');
              pcVar7[0x5b] = (uint8_t)('\x01');
            }
          }
        }
      }
      else if (((cVar2 == 'l') && (iVar22 = (int32_t)*(volatile int16_t *)(pcVar7 + 0x50), 0 < iVar22)) &&
              (iVar22 <= iVar11)) {
        bVar20 = (uint8_t)(*(volatile int8_t *)(&DAT_00799a7e + CITY_MUL32(iVar22,0x84)) + 1);
        *(volatile uint8_t *)(&DAT_00799a7e + CITY_MUL32(iVar22,0x84)) = (uint8_t)(bVar20);
        if ((&DAT_007999e8)[(uint32_t)CITY_MUL32(iVar22,0x84)] == 0xe7) goto joined_r0x00462334;
      }
    }
    pcVar7 = pcVar7 + 0x184;
  } while ((int32_t)(uintptr_t)pcVar7 < 0xc8ad42);
  pcVar25 = timeGetTime;
  (*pcVar25)();
  (*pcVar25)();
  local_44 = 1;
  if (0 < DAT_0078d790) {
    do {
      iVar11 = local_44;
      iVar22 = CITY_MUL32(local_44,0x108);
      local_44 = iVar11;
      if ((&DAT_007999d8)[(uint32_t)iVar22] != '\x01') goto switchD_004623ca_caseD_26;
      if ((&DAT_007999e8)[(uint32_t)CITY_MUL32(local_44,0x84)] != 0x49) {
        *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1) = (uint8_t)(0);
      }
      uVar4 = (uint16_t)((uint16_t)local_44);
      switch((&DAT_007999e8)[(uint32_t)CITY_MUL32(local_44,0x84)]) {
      case 0x18:
      case 0x19:
      case 0x1a:
      case 0x1b:
      case 0x1c:
      case 0x1d:
        iVar8 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                             (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
        if (((iVar8 != 0) &&
            (cVar2 = (int8_t)((&DAT_00799a0a)[(uint32_t)iVar22]), (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(cVar2 + 1U),
            uVar31 = _DAT_00d3b4e4, uVar13 = _DAT_00d3b4e0, 0x28 < (uint8_t)(cVar2 + 1U))) &&
           (DAT_0078d584 == 0)) {
          (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(0);
          DAT_0078d584 = 1;
          iVar8 = FUN_004b4800((&DAT_007999d9)[(uint32_t)iVar22],0x28,uVar31,uVar13,4);
          iVar22 = CITY_MUL32(iVar8,0x184);
          (&DAT_00bcd630)[(uint32_t)iVar22] = (uint8_t)(1);
          goto LAB_0046463b;
        }
        break;
      case 0x1e:
        if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(local_44,0x84)] < 1) {
          *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1) = (uint8_t)(2);
        }
        iVar8 = FUN_00475b50(local_44);
        if ((iVar8 == iVar11) &&
           (iVar8 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                                 (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]),
           iVar8 != 0)) {
          if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 0x33) {
            restore_city_ensure_labor_seeker(iVar11);
          }
          if ((*(volatile int16_t *)(&DAT_00799a76 + iVar22) == 0) || (*(volatile int16_t *)(&DAT_00799a78 + iVar22) == 0)
             ) {
            iVar8 = FUN_004cde60((int32_t)(int16_t)(&DAT_00799a18)[(uint32_t)CITY_MUL32(iVar11,0x84)],
                                 *(volatile uint32_t *)
                                  (&DAT_00705eb8 + (int16_t)(&DAT_007999e8)[(uint32_t)CITY_MUL32(iVar11,0x84)] * 0x28));
            if (iVar8 < 100) {
              if (iVar8 < 0x4b) {
                if (iVar8 < 0x32) {
                  if (iVar8 < 0x19) {
                    if (iVar8 < 1) break;
                    bVar20 = (uint8_t)(0x2c);
                  }
                  else {
                    bVar20 = (uint8_t)(0x1d);
                  }
                }
                else {
                  bVar20 = (uint8_t)(0xf);
                }
              }
              else {
                bVar20 = (uint8_t)(7);
              }
            }
            else {
              bVar20 = (uint8_t)(3);
            }
            cVar2 = (int8_t)((&DAT_00799a0a)[(uint32_t)iVar22]);
            (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(cVar2 + 1U);
            uVar31 = _DAT_00d3b4e4;
            uVar13 = _DAT_00d3b4e0;
            if (bVar20 < (uint8_t)(cVar2 + 1U)) {
              if ((*(volatile int8_t *)((uint32_t)(uintptr_t)&DAT_00799a7e + iVar22 + 1) == '\0') ||
                 (*(volatile int16_t *)(&DAT_00799a76 + iVar22) != 0)) {
                if ((*(volatile int8_t *)(&DAT_00799a80 + CITY_MUL32(iVar11,0x84)) == '\0') ||
                   (*(volatile int16_t *)(&DAT_00799a78 + iVar22) != 0)) break;
                (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(0);
                iVar8 = FUN_004b4800((&DAT_007999d9)[(uint32_t)iVar22],0x10,uVar31,uVar13,0);
                *(volatile int16_t *)(&DAT_00799a78 + iVar22) = (int16_t)((int16_t)iVar8);
                iVar22 = CITY_MUL32(iVar8,0x184);
                (&DAT_00bcd630)[(uint32_t)iVar22] = (uint8_t)(0xc);
              }
              else {
                (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(0);
                iVar8 = FUN_004b4800((&DAT_007999d9)[(uint32_t)iVar22],0xf,uVar31,uVar13,0);
                *(volatile int16_t *)(&DAT_00799a76 + iVar22) = (int16_t)((int16_t)iVar8);
                iVar22 = CITY_MUL32(iVar8,0x184);
                (&DAT_00bcd630)[(uint32_t)iVar22] = (uint8_t)(0xc);
              }
              goto LAB_0046463b;
            }
          }
        }
        break;
      case 0x1f:
        if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(local_44,0x84)] < 1) {
          *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1) = (uint8_t)(2);
        }
        iVar8 = FUN_00475b50(local_44);
        if (iVar8 == iVar11) {
          sVar21 = (int16_t)((&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)]);
          if (0 < sVar21) {
            iVar8 = sVar21 * 0x184;
            if ((((&DAT_00bcd606)[(uint32_t)iVar8] != '\0') &&
                (((&DAT_00bcd602)[(uint32_t)iVar8] == '\x0f' || ((&DAT_00bcd602)[(uint32_t)iVar8] == '\0')))) &&
               ((int16_t)(&DAT_00bcd652)[sVar21 * 0xc2] == iVar11)) break;
            (&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)(0);
          }
          iVar8 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                               (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
          if (iVar8 != 0) {
            if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 0x33) {
              restore_city_ensure_labor_seeker(iVar11);
            }
            iVar8 = FUN_004cde60((int32_t)(int16_t)(&DAT_00799a18)[(uint32_t)CITY_MUL32(iVar11,0x84)],
                                 *(volatile uint32_t *)
                                  (&DAT_00705eb8 + (int16_t)(&DAT_007999e8)[(uint32_t)CITY_MUL32(iVar11,0x84)] * 0x28));
            if (iVar8 < 100) {
              if (iVar8 < 0x4b) {
                if (iVar8 < 0x32) {
                  if (iVar8 < 0x19) {
                    if (iVar8 < 1) break;
                    bVar20 = (uint8_t)(0x2c);
                  }
                  else {
                    bVar20 = (uint8_t)(0x1d);
                  }
                }
                else {
                  bVar20 = (uint8_t)(0xf);
                }
              }
              else {
                bVar20 = (uint8_t)(7);
              }
            }
            else {
              bVar20 = (uint8_t)(3);
            }
            cVar2 = (int8_t)((&DAT_00799a0a)[(uint32_t)iVar22]);
            (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(cVar2 + 1U);
            uVar13 = _DAT_00d3b4e4;
            uVar31 = _DAT_00d3b4e0;
            if ((bVar20 < (uint8_t)(cVar2 + 1U)) &&
               (*(volatile int8_t *)((uint32_t)(uintptr_t)&DAT_00799a7e + iVar22 + 1) != '\0')) {
              (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(0);
              uVar3 = (uint8_t)((&DAT_007999d9)[(uint32_t)iVar22]);
              uVar30 = 0xf;
              goto LAB_00462522;
            }
          }
        }
        break;
      case 0x20:
        if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(local_44,0x84)] < 1) {
          *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1) = (uint8_t)(2);
        }
        sVar21 = (int16_t)((&DAT_00799a02)[(uint32_t)CITY_MUL32(local_44,0x84)]);
        if (0 < sVar21) {
          iVar8 = sVar21 * 0x184;
          if (((&DAT_00bcd606)[(uint32_t)iVar8] != '\0') &&
             ((((&DAT_00bcd602)[(uint32_t)iVar8] == '\x12' || ((&DAT_00bcd602)[(uint32_t)iVar8] == '\0')) &&
              ((int16_t)(&DAT_00bcd652)[sVar21 * 0xc2] == local_44)))) break;
          (&DAT_00799a02)[(uint32_t)CITY_MUL32(local_44,0x84)] = (uint16_t)(0);
        }
        iVar8 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                             (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
        if (iVar8 != 0) {
          if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 0x33) {
            restore_city_ensure_labor_seeker(iVar11);
          }
          iVar8 = FUN_004cde60((int32_t)(int16_t)(&DAT_00799a18)[(uint32_t)CITY_MUL32(iVar11,0x84)],
                               *(volatile uint32_t *)
                                (&DAT_00705eb8 + (int16_t)(&DAT_007999e8)[(uint32_t)CITY_MUL32(iVar11,0x84)] * 0x28));
          if (iVar8 < 100) {
            if (iVar8 < 0x4b) {
              if (iVar8 < 0x32) {
                if (iVar8 < 0x19) {
                  if (iVar8 < 1) break;
                  bVar20 = (uint8_t)(0x50);
                }
                else {
                  bVar20 = (uint8_t)(0x32);
                }
              }
              else {
                bVar20 = (uint8_t)(0x1e);
              }
            }
            else {
              bVar20 = (uint8_t)(0xf);
            }
          }
          else {
            bVar20 = (uint8_t)(7);
          }
          cVar2 = (int8_t)((&DAT_00799a0a)[(uint32_t)iVar22]);
          (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(cVar2 + 1U);
          uVar13 = _DAT_00d3b4e4;
          if ((bVar20 < (uint8_t)(cVar2 + 1U)) &&
             (DAT_007063c4 <= (int16_t)(&DAT_00799a14)[(uint32_t)CITY_MUL32(iVar11,0x84)])) {
            (&DAT_00799a14)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)((&DAT_00799a14)[(uint32_t)CITY_MUL32(iVar11,0x84)] - (int16_t)DAT_007063c4);
            uVar31 = _DAT_00d3b4e0;
            (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(0);
            uVar3 = (uint8_t)((&DAT_007999d9)[(uint32_t)iVar22]);
            uVar30 = 0x12;
LAB_00462522:
            iVar8 = FUN_004b4800(uVar3,uVar30,uVar13,uVar31,0);
            (&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)((int16_t)iVar8);
            iVar22 = CITY_MUL32(iVar8,0x184);
            (&DAT_00bcd630)[(uint32_t)iVar22] = (uint8_t)(0xc);
LAB_0046463b:
            *(volatile uint16_t *)((uint32_t)(uintptr_t)&DAT_00bcd652 + iVar22) = (uint16_t)(uVar4);
            FUN_004b1df0(iVar8);
            local_44 = iVar11;
          }
        }
        break;
      case 0x21:
        if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(local_44,0x84)] < 1) {
          *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1) = (uint8_t)(2);
        }
        iVar8 = FUN_00475b50(local_44);
        if ((iVar8 == iVar11) &&
           (iVar8 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                                 (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]),
           iVar8 != 0)) {
          if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 0x33) {
            restore_city_ensure_labor_seeker(iVar11);
          }
          if (((*(volatile int16_t *)(&DAT_00799a76 + iVar22) == 0) ||
              (*(volatile int16_t *)(&DAT_00799a78 + iVar22) == 0)) ||
             (*(volatile int16_t *)(&DAT_00799a7a + CITY_MUL32(iVar11,0x42)) == 0)) {
            iVar8 = FUN_004cde60((int32_t)(int16_t)(&DAT_00799a18)[(uint32_t)CITY_MUL32(iVar11,0x84)],
                                 *(volatile uint32_t *)
                                  (&DAT_00705eb8 + (int16_t)(&DAT_007999e8)[(uint32_t)CITY_MUL32(iVar11,0x84)] * 0x28));
            if (iVar8 < 100) {
              if (iVar8 < 0x4b) {
                if (iVar8 < 0x32) {
                  if (iVar8 < 0x19) {
                    if (iVar8 < 1) break;
                    bVar20 = (uint8_t)(0x46);
                  }
                  else {
                    bVar20 = (uint8_t)(0x28);
                  }
                }
                else {
                  bVar20 = (uint8_t)(0x14);
                }
              }
              else {
                bVar20 = (uint8_t)(0xc);
              }
            }
            else {
              bVar20 = (uint8_t)(6);
            }
            cVar2 = (int8_t)((&DAT_00799a0a)[(uint32_t)iVar22]);
            (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(cVar2 + 1U);
            uVar31 = _DAT_00d3b4e4;
            uVar13 = _DAT_00d3b4e0;
            if (bVar20 < (uint8_t)(cVar2 + 1U)) {
              if ((*(volatile int8_t *)((uint32_t)(uintptr_t)&DAT_00799a7e + iVar22 + 1) != '\0') &&
                 (*(volatile int16_t *)(&DAT_00799a76 + iVar22) == 0)) {
                (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(0);
                iVar8 = FUN_004b4800((&DAT_007999d9)[(uint32_t)iVar22],0xf,uVar31,uVar13,0);
                *(volatile int16_t *)(&DAT_00799a76 + iVar22) = (int16_t)((int16_t)iVar8);
                (&DAT_00bcd630)[(uint32_t)CITY_MUL32(iVar8,0x184)] = (uint8_t)(0xc);
                (&DAT_00bcd652)[(uint32_t)CITY_MUL32(iVar8,0xc2)] = (uint16_t)(uVar4);
                FUN_004b1df0(iVar8);
              }
              uVar31 = _DAT_00d3b4e4;
              uVar13 = _DAT_00d3b4e0;
              if ((*(volatile int8_t *)(&DAT_00799a80 + CITY_MUL32(iVar11,0x84)) != '\0') &&
                 (*(volatile int16_t *)(&DAT_00799a78 + iVar22) == 0)) {
                (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(0);
                iVar8 = FUN_004b4800((&DAT_007999d9)[(uint32_t)iVar22],0x10,uVar31,uVar13,0);
                *(volatile int16_t *)(&DAT_00799a78 + iVar22) = (int16_t)((int16_t)iVar8);
                (&DAT_00bcd630)[(uint32_t)CITY_MUL32(iVar8,0x184)] = (uint8_t)(0xc);
                (&DAT_00bcd652)[(uint32_t)CITY_MUL32(iVar8,0xc2)] = (uint16_t)(uVar4);
                FUN_004b1df0(iVar8);
              }
              uVar31 = _DAT_00d3b4e4;
              uVar13 = _DAT_00d3b4e0;
              if ((*(volatile int8_t *)((uint32_t)(uintptr_t)&DAT_00799a80 + iVar22 + 1) != '\0') &&
                 (*(volatile int16_t *)(&DAT_00799a7a + CITY_MUL32(iVar11,0x42)) == 0)) {
                (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(0);
                iVar8 = FUN_004b4800((&DAT_007999d9)[(uint32_t)iVar22],0x11,uVar31,uVar13,0);
                *(volatile int16_t *)(&DAT_00799a7a + CITY_MUL32(iVar11,0x42)) = (int16_t)((int16_t)iVar8);
                iVar22 = CITY_MUL32(iVar8,0x184);
                (&DAT_00bcd630)[(uint32_t)iVar22] = (uint8_t)(0xc);
                goto LAB_0046463b;
              }
            }
          }
        }
        break;
      case 0x22:
        if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(local_44,0x84)] < 1) {
          *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1) = (uint8_t)(2);
        }
        iVar8 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                             (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
        if (iVar8 != 0) {
          if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 0x33) {
            restore_city_ensure_labor_seeker(iVar11);
          }
          iVar8 = FUN_004cde60((int32_t)(int16_t)(&DAT_00799a18)[(uint32_t)CITY_MUL32(iVar11,0x84)],
                               *(volatile uint32_t *)
                                (&DAT_00705eb8 + (int16_t)(&DAT_007999e8)[(uint32_t)CITY_MUL32(iVar11,0x84)] * 0x28));
          if (iVar8 < 100) {
            if (iVar8 < 0x4b) {
              if (iVar8 < 0x32) {
                if (iVar8 < 0x19) {
                  if (iVar8 < 1) break;
                  bVar20 = (uint8_t)(0x2c);
                }
                else {
                  bVar20 = (uint8_t)(0x1d);
                }
              }
              else {
                bVar20 = (uint8_t)(0xf);
              }
            }
            else {
              bVar20 = (uint8_t)(7);
            }
          }
          else {
            bVar20 = (uint8_t)(3);
          }
          cVar2 = (int8_t)((&DAT_00799a0a)[(uint32_t)iVar22]);
          (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(cVar2 + 1U);
          uVar31 = _DAT_00d3b4e4;
          uVar13 = _DAT_00d3b4e0;
          if (bVar20 < (uint8_t)(cVar2 + 1U)) {
            (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(0);
            uVar3 = (uint8_t)((&DAT_007999d9)[(uint32_t)iVar22]);
            uVar30 = 0x10;
LAB_00463bfc:
            iVar22 = FUN_004b4800(uVar3,uVar30,uVar31,uVar13,0);
            (&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)((int16_t)iVar22);
            (&DAT_00bcd630)[(uint32_t)CITY_MUL32(iVar22,0x184)] = (uint8_t)(8);
            (&DAT_00bcd652)[(uint32_t)CITY_MUL32(iVar22,0xc2)] = (uint16_t)(uVar4);
            local_44 = iVar11;
          }
        }
        break;
      case 0x23:
        if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(local_44,0x84)] < 1) {
          *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1) = (uint8_t)(2);
        }
        iVar8 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                             (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
        if (iVar8 != 0) {
          if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 0x33) {
            restore_city_ensure_labor_seeker(iVar11);
          }
          iVar8 = FUN_004cde60((int32_t)(int16_t)(&DAT_00799a18)[(uint32_t)CITY_MUL32(iVar11,0x84)],
                               *(volatile uint32_t *)
                                (&DAT_00705eb8 + (int16_t)(&DAT_007999e8)[(uint32_t)CITY_MUL32(iVar11,0x84)] * 0x28));
          if (iVar8 < 100) {
            if (iVar8 < 0x4b) {
              if (iVar8 < 0x32) {
                if (iVar8 < 0x19) {
                  if (iVar8 < 1) break;
                  bVar20 = (uint8_t)(0x3c);
                }
                else {
                  bVar20 = (uint8_t)(0x23);
                }
              }
              else {
                bVar20 = (uint8_t)(0x14);
              }
            }
            else {
              bVar20 = (uint8_t)(10);
            }
          }
          else {
            bVar20 = (uint8_t)(5);
          }
          cVar2 = (int8_t)((&DAT_00799a0a)[(uint32_t)iVar22]);
          (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(cVar2 + 1U);
          uVar31 = _DAT_00d3b4e4;
          uVar13 = _DAT_00d3b4e0;
          if (bVar20 < (uint8_t)(cVar2 + 1U)) {
            (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(0);
            uVar3 = (uint8_t)((&DAT_007999d9)[(uint32_t)iVar22]);
            uVar30 = 0x11;
            goto LAB_00463bfc;
          }
        }
        break;
      case 0x24:
        if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(local_44,0x84)] < 1) {
          *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1) = (uint8_t)(2);
        }
        iVar8 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                             (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
        if (iVar8 != 0) {
          if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 0x33) {
            restore_city_ensure_labor_seeker(iVar11);
          }
          iVar8 = FUN_004cde60((int32_t)(int16_t)(&DAT_00799a18)[(uint32_t)CITY_MUL32(iVar11,0x84)],
                               *(volatile uint32_t *)
                                (&DAT_00705eb8 + (int16_t)(&DAT_007999e8)[(uint32_t)CITY_MUL32(iVar11,0x84)] * 0x28));
          if (iVar8 < 100) {
            if (iVar8 < 0x4b) {
              if (iVar8 < 0x32) {
                if (iVar8 < 0x19) {
                  if (iVar8 < 1) break;
                  bVar20 = (uint8_t)(0x2c);
                }
                else {
                  bVar20 = (uint8_t)(0x1d);
                }
              }
              else {
                bVar20 = (uint8_t)(0xf);
              }
            }
            else {
              bVar20 = (uint8_t)(7);
            }
          }
          else {
            bVar20 = (uint8_t)(3);
          }
          cVar2 = (int8_t)((&DAT_00799a0a)[(uint32_t)iVar22]);
          (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(cVar2 + 1U);
          uVar31 = _DAT_00d3b4e4;
          uVar13 = _DAT_00d3b4e0;
          if (bVar20 < (uint8_t)(cVar2 + 1U)) {
            (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(0);
            uVar3 = (uint8_t)((&DAT_007999d9)[(uint32_t)iVar22]);
            uVar30 = 0xf;
            goto LAB_00463bfc;
          }
        }
        break;
      case 0x25:
        if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(local_44,0x84)] < 1) {
          *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1) = (uint8_t)(2);
        }
        iVar8 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                             (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
        if (iVar8 != 0) {
          if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 0x33) {
            restore_city_ensure_labor_seeker(iVar11);
          }
          iVar8 = FUN_004cde60((int32_t)(int16_t)(&DAT_00799a18)[(uint32_t)CITY_MUL32(iVar11,0x84)],
                               *(volatile uint32_t *)
                                (&DAT_00705eb8 + (int16_t)(&DAT_007999e8)[(uint32_t)CITY_MUL32(iVar11,0x84)] * 0x28));
          if (iVar8 < 100) {
            if (iVar8 < 0x4b) {
              if (iVar8 < 0x32) {
                if (iVar8 < 0x19) {
                  if (iVar8 < 1) break;
                  bVar20 = (uint8_t)(0x5a);
                }
                else {
                  bVar20 = (uint8_t)(0x3c);
                }
              }
              else {
                bVar20 = (uint8_t)(0x1e);
              }
            }
            else {
              bVar20 = (uint8_t)(0xf);
            }
          }
          else {
            bVar20 = (uint8_t)(7);
          }
          cVar2 = (int8_t)((&DAT_00799a0a)[(uint32_t)iVar22]);
          (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(cVar2 + 1U);
          uVar31 = _DAT_00d3b4e4;
          uVar13 = _DAT_00d3b4e0;
          if (bVar20 < (uint8_t)(cVar2 + 1U)) {
            (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(0);
            uVar3 = (uint8_t)((&DAT_007999d9)[(uint32_t)iVar22]);
            uVar30 = 0x12;
            goto LAB_00463bfc;
          }
        }
        break;
      case 0x2e:
        if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(local_44,0x84)] < 1) {
          *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1) = (uint8_t)(2);
        }
        sVar21 = (int16_t)((&DAT_00799a02)[(uint32_t)CITY_MUL32(local_44,0x84)]);
        if (0 < sVar21) {
          iVar8 = sVar21 * 0x184;
          if ((((&DAT_00bcd606)[(uint32_t)iVar8] != '\0') &&
              (((&DAT_00bcd602)[(uint32_t)iVar8] == '!' || ((&DAT_00bcd602)[(uint32_t)iVar8] == '\0')))) &&
             ((int16_t)(&DAT_00bcd652)[sVar21 * 0xc2] == local_44)) break;
          (&DAT_00799a02)[(uint32_t)CITY_MUL32(local_44,0x84)] = (uint16_t)(0);
        }
        iVar8 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                             (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
        if (iVar8 != 0) {
          if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 0x33) {
            restore_city_ensure_labor_seeker(iVar11);
          }
          iVar8 = FUN_004cde60((int32_t)(int16_t)(&DAT_00799a18)[(uint32_t)CITY_MUL32(iVar11,0x84)],
                               *(volatile uint32_t *)
                                (&DAT_00705eb8 + (int16_t)(&DAT_007999e8)[(uint32_t)CITY_MUL32(iVar11,0x84)] * 0x28));
          if (iVar8 < 100) {
            if (iVar8 < 0x4b) {
              if (iVar8 < 0x32) {
                if (iVar8 < 0x19) {
                  if (iVar8 < 1) break;
                  bVar20 = (uint8_t)(0x2c);
                }
                else {
                  bVar20 = (uint8_t)(0x1d);
                }
              }
              else {
                bVar20 = (uint8_t)(0xf);
              }
            }
            else {
              bVar20 = (uint8_t)(7);
            }
          }
          else {
            bVar20 = (uint8_t)(3);
          }
          cVar2 = (int8_t)((&DAT_00799a0a)[(uint32_t)iVar22]);
          (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(cVar2 + 1U);
          uVar31 = _DAT_00d3b4e4;
          uVar13 = _DAT_00d3b4e0;
          if (bVar20 < (uint8_t)(cVar2 + 1U)) {
            (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(0);
            uVar3 = (uint8_t)((&DAT_007999d9)[(uint32_t)iVar22]);
            uVar30 = 0x21;
LAB_00466271:
            iVar8 = FUN_004b4800(uVar3,uVar30,uVar31,uVar13,0);
            (&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)((int16_t)iVar8);
            iVar22 = CITY_MUL32(iVar8,0x184);
            (&DAT_00bcd630)[(uint32_t)iVar22] = (uint8_t)(1);
            goto LAB_0046463b;
          }
        }
        break;
      case 0x2f:
        if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(local_44,0x84)] < 1) {
          *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1) = (uint8_t)(2);
        }
        sVar21 = (int16_t)((&DAT_00799a02)[(uint32_t)CITY_MUL32(local_44,0x84)]);
        if (0 < sVar21) {
          iVar8 = sVar21 * 0x184;
          if ((((&DAT_00bcd606)[(uint32_t)iVar8] != '\0') &&
              (((&DAT_00bcd602)[(uint32_t)iVar8] == '\"' || ((&DAT_00bcd602)[(uint32_t)iVar8] == '\0')))) &&
             ((int16_t)(&DAT_00bcd652)[sVar21 * 0xc2] == local_44)) break;
          (&DAT_00799a02)[(uint32_t)CITY_MUL32(local_44,0x84)] = (uint16_t)(0);
        }
        iVar8 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                             (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
        if (iVar8 != 0) {
          if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 0x33) {
            restore_city_ensure_labor_seeker(iVar11);
          }
          iVar8 = FUN_004cde60((int32_t)(int16_t)(&DAT_00799a18)[(uint32_t)CITY_MUL32(iVar11,0x84)],
                               *(volatile uint32_t *)
                                (&DAT_00705eb8 + (int16_t)(&DAT_007999e8)[(uint32_t)CITY_MUL32(iVar11,0x84)] * 0x28));
          if (iVar8 < 100) {
            if (iVar8 < 0x4b) {
              if (iVar8 < 0x32) {
                if (iVar8 < 0x19) {
                  if (iVar8 < 1) break;
                  bVar20 = (uint8_t)(0x2c);
                }
                else {
                  bVar20 = (uint8_t)(0x1d);
                }
              }
              else {
                bVar20 = (uint8_t)(0xf);
              }
            }
            else {
              bVar20 = (uint8_t)(7);
            }
          }
          else {
            bVar20 = (uint8_t)(3);
          }
          cVar2 = (int8_t)((&DAT_00799a0a)[(uint32_t)iVar22]);
          (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(cVar2 + 1U);
          iVar8 = DAT_0070661c;
          if (bVar20 < (uint8_t)(cVar2 + 1U)) {
            if ((((&DAT_00ea5ce8)[(uint32_t)CITY_MUL32(DAT_00eaac28,0x24ec)] != 0) &&
                (iVar9 = FUN_004c5120((&DAT_00ea5f9c)[(uint32_t)CITY_MUL32(DAT_00eaac28,0x1276)]), iVar9 == 3)) &&
               (cVar2 = (int8_t)(FUN_004c52d0((&DAT_00ea5f9c)[(uint32_t)CITY_MUL32(DAT_00eaac28,0x1276)],1)), cVar2 != '\0')) {
              iVar8 = FUN_004cde40(iVar8,DAT_00706964);
            }
            uVar13 = _DAT_00d3b4e0;
            if (iVar8 <= (int16_t)(&DAT_00799a14)[(uint32_t)CITY_MUL32(iVar11,0x84)]) {
              (&DAT_00799a14)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)((&DAT_00799a14)[(uint32_t)CITY_MUL32(iVar11,0x84)] - (int16_t)iVar8);
              uVar31 = _DAT_00d3b4e4;
              (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(0);
              uVar3 = (uint8_t)((&DAT_007999d9)[(uint32_t)iVar22]);
              uVar30 = 0x22;
              goto LAB_00466271;
            }
          }
        }
        break;
      case 0x31:
        if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(local_44,0x84)] < 1) {
          *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1) = (uint8_t)(2);
        }
        sVar21 = (int16_t)((&DAT_00799a02)[(uint32_t)CITY_MUL32(local_44,0x84)]);
        if (0 < sVar21) {
          iVar8 = sVar21 * 0x184;
          if ((((&DAT_00bcd606)[(uint32_t)iVar8] != '\0') &&
              (((&DAT_00bcd602)[(uint32_t)iVar8] == '\x1f' || ((&DAT_00bcd602)[(uint32_t)iVar8] == '\0')))) &&
             ((int16_t)(&DAT_00bcd652)[sVar21 * 0xc2] == local_44)) break;
          (&DAT_00799a02)[(uint32_t)CITY_MUL32(local_44,0x84)] = (uint16_t)(0);
        }
        iVar8 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                             (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
        if (iVar8 != 0) {
          if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 0x33) {
            restore_city_ensure_labor_seeker(iVar11);
          }
          iVar8 = FUN_004cde60((int32_t)(int16_t)(&DAT_00799a18)[(uint32_t)CITY_MUL32(iVar11,0x84)],
                               *(volatile uint32_t *)
                                (&DAT_00705eb8 + (int16_t)(&DAT_007999e8)[(uint32_t)CITY_MUL32(iVar11,0x84)] * 0x28));
          if (iVar8 < 100) {
            if (iVar8 < 0x4b) {
              if (iVar8 < 0x32) {
                if (iVar8 < 0x19) {
                  if (iVar8 < 1) break;
                  bVar20 = (uint8_t)(0x2c);
                }
                else {
                  bVar20 = (uint8_t)(0x1d);
                }
              }
              else {
                bVar20 = (uint8_t)(0xf);
              }
            }
            else {
              bVar20 = (uint8_t)(7);
            }
          }
          else {
            bVar20 = (uint8_t)(3);
          }
          cVar2 = (int8_t)((&DAT_00799a0a)[(uint32_t)iVar22]);
          (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(cVar2 + 1U);
          uVar31 = _DAT_00d3b4e4;
          uVar13 = _DAT_00d3b4e0;
          if (bVar20 < (uint8_t)(cVar2 + 1U)) {
            (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(0);
            uVar30 = 0x1f;
LAB_00466268:
            uVar3 = (uint8_t)((&DAT_007999d9)[(uint32_t)iVar22]);
            goto LAB_00466271;
          }
        }
        break;
      case 0x33:
        if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(local_44,0x84)] < 1) {
          *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1) = (uint8_t)(2);
        }
        sVar21 = (int16_t)((&DAT_00799a02)[(uint32_t)CITY_MUL32(local_44,0x84)]);
        if (0 < sVar21) {
          iVar8 = sVar21 * 0x184;
          if ((((&DAT_00bcd606)[(uint32_t)iVar8] != '\0') &&
              (((&DAT_00bcd602)[(uint32_t)iVar8] == '\x1d' || ((&DAT_00bcd602)[(uint32_t)iVar8] == '\0')))) &&
             ((int16_t)(&DAT_00bcd652)[sVar21 * 0xc2] == local_44)) break;
          (&DAT_00799a02)[(uint32_t)CITY_MUL32(local_44,0x84)] = (uint16_t)(0);
        }
        iVar8 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                             (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
        if (iVar8 != 0) {
          if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 0x33) {
            restore_city_ensure_labor_seeker(iVar11);
          }
          iVar8 = FUN_004cde60((int32_t)(int16_t)(&DAT_00799a18)[(uint32_t)CITY_MUL32(iVar11,0x84)],
                               *(volatile uint32_t *)
                                (&DAT_00705eb8 + (int16_t)(&DAT_007999e8)[(uint32_t)CITY_MUL32(iVar11,0x84)] * 0x28));
          if (iVar8 < 100) {
            if (iVar8 < 0x4b) {
              if (iVar8 < 0x32) {
                if (iVar8 < 0x19) {
                  if (iVar8 < 1) break;
                  bVar20 = (uint8_t)(100);
                }
                else {
                  bVar20 = (uint8_t)(0x32);
                }
              }
              else {
                bVar20 = (uint8_t)(0x19);
              }
            }
            else {
              bVar20 = (uint8_t)(10);
            }
          }
          else {
            bVar20 = (uint8_t)(5);
          }
          cVar2 = (int8_t)((&DAT_00799a0a)[(uint32_t)iVar22]);
          (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(cVar2 + 1U);
          iVar8 = DAT_007066bc;
          if (bVar20 < (uint8_t)(cVar2 + 1U)) {
            if ((((&DAT_00ea5ce8)[(uint32_t)CITY_MUL32(DAT_00eaac28,0x24ec)] != 0) &&
                (iVar9 = FUN_004c5120((&DAT_00ea5f9c)[(uint32_t)CITY_MUL32(DAT_00eaac28,0x1276)]), iVar9 == 2)) &&
               (cVar2 = (int8_t)(FUN_004c52d0((&DAT_00ea5f9c)[(uint32_t)CITY_MUL32(DAT_00eaac28,0x1276)],2)), cVar2 != '\0')) {
              iVar8 = FUN_004cde40(iVar8,DAT_0070693c);
            }
            if (iVar8 <= (int16_t)(&DAT_00799a14)[(uint32_t)CITY_MUL32(iVar11,0x84)]) {
              (&DAT_00799a14)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)((&DAT_00799a14)[(uint32_t)CITY_MUL32(iVar11,0x84)] - (int16_t)iVar8);
              bVar29 = (uint8_t)(DAT_00e38ebc == 0);
              (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(0);
              if ((((bVar29) && (DAT_00e38ea8 == DAT_00579600)) && (DAT_007027c4 == '\0')) &&
                 (DAT_007027c3 != '\0')) {
                DAT_0078d828 = 0;
                FUN_00417ae0(0xc);
              }
              uVar3 = (uint8_t)((&DAT_007999d9)[(uint32_t)iVar22]);
              uVar30 = 0x1d;
              uVar31 = _DAT_00d3b4e4;
              uVar13 = _DAT_00d3b4e0;
              goto LAB_00466271;
            }
          }
        }
        break;
      case 0x35:
        if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(local_44,0x84)] < 1) {
          *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1) = (uint8_t)(2);
        }
        sVar21 = (int16_t)((&DAT_00799a02)[(uint32_t)CITY_MUL32(local_44,0x84)]);
        if (0 < sVar21) {
          iVar8 = sVar21 * 0x184;
          if ((((&DAT_00bcd606)[(uint32_t)iVar8] != '\0') &&
              (((&DAT_00bcd602)[(uint32_t)iVar8] == '\x1e' || ((&DAT_00bcd602)[(uint32_t)iVar8] == '\0')))) &&
             ((int16_t)(&DAT_00bcd652)[sVar21 * 0xc2] == local_44)) break;
          (&DAT_00799a02)[(uint32_t)CITY_MUL32(local_44,0x84)] = (uint16_t)(0);
        }
        iVar8 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                             (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
        if (iVar8 != 0) {
          if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 0x33) {
            restore_city_ensure_labor_seeker(iVar11);
          }
          iVar8 = FUN_004cde60((int32_t)(int16_t)(&DAT_00799a18)[(uint32_t)CITY_MUL32(iVar11,0x84)],
                               *(volatile uint32_t *)
                                (&DAT_00705eb8 + (int16_t)(&DAT_007999e8)[(uint32_t)CITY_MUL32(iVar11,0x84)] * 0x28));
          if (iVar8 < 100) {
            if (iVar8 < 0x4b) {
              if (iVar8 < 0x32) {
                if (iVar8 < 0x19) {
                  if (iVar8 < 1) break;
                  bVar20 = (uint8_t)(100);
                }
                else {
                  bVar20 = (uint8_t)(0x32);
                }
              }
              else {
                bVar20 = (uint8_t)(0x19);
              }
            }
            else {
              bVar20 = (uint8_t)(10);
            }
          }
          else {
            bVar20 = (uint8_t)(5);
          }
          cVar2 = (int8_t)((&DAT_00799a0a)[(uint32_t)iVar22]);
          (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(cVar2 + 1U);
          iVar8 = DAT_0070670c;
          if (bVar20 < (uint8_t)(cVar2 + 1U)) {
            if ((((&DAT_00ea5ce8)[(uint32_t)CITY_MUL32(DAT_00eaac28,0x24ec)] != 0) &&
                (iVar9 = FUN_004c5120((&DAT_00ea5f9c)[(uint32_t)CITY_MUL32(DAT_00eaac28,0x1276)]), iVar9 == 2)) &&
               (cVar2 = (int8_t)(FUN_004c52d0((&DAT_00ea5f9c)[(uint32_t)CITY_MUL32(DAT_00eaac28,0x1276)],2)), cVar2 != '\0')) {
              iVar8 = FUN_004cde40(iVar8,DAT_0070693c);
            }
            uVar13 = _DAT_00d3b4e0;
            if (iVar8 <= (int16_t)(&DAT_00799a14)[(uint32_t)CITY_MUL32(iVar11,0x84)]) {
              (&DAT_00799a14)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)((&DAT_00799a14)[(uint32_t)CITY_MUL32(iVar11,0x84)] - (int16_t)iVar8);
              uVar31 = _DAT_00d3b4e4;
              (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(0);
              uVar3 = (uint8_t)((&DAT_007999d9)[(uint32_t)iVar22]);
              uVar30 = 0x1e;
              goto LAB_00466271;
            }
          }
        }
        break;
      case 0x37:
        if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(local_44,0x84)] < 1) {
          *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1) = (uint8_t)(2);
        }
        sVar21 = (int16_t)((&DAT_00799a02)[(uint32_t)CITY_MUL32(local_44,0x84)]);
        if (0 < sVar21) {
          iVar8 = sVar21 * 0x184;
          if ((((&DAT_00bcd606)[(uint32_t)iVar8] != '\0') &&
              (((&DAT_00bcd602)[(uint32_t)iVar8] == 'X' || ((&DAT_00bcd602)[(uint32_t)iVar8] == '\0')))) &&
             ((int16_t)(&DAT_00bcd652)[sVar21 * 0xc2] == local_44)) break;
          (&DAT_00799a02)[(uint32_t)CITY_MUL32(local_44,0x84)] = (uint16_t)(0);
        }
        iVar8 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                             (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
        if (iVar8 != 0) {
          if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 0x65) {
            restore_city_ensure_labor_seeker(iVar11);
          }
          iVar8 = FUN_004cde60((int32_t)(int16_t)(&DAT_00799a18)[(uint32_t)CITY_MUL32(iVar11,0x84)],
                               *(volatile uint32_t *)
                                (&DAT_00705eb8 + (int16_t)(&DAT_007999e8)[(uint32_t)CITY_MUL32(iVar11,0x84)] * 0x28));
          if (iVar8 < 100) {
            if (iVar8 < 0x4b) {
              if (iVar8 < 0x32) {
                if (iVar8 < 0x19) {
                  if (iVar8 < 1) break;
                  bVar20 = (uint8_t)(0xf);
                }
                else {
                  bVar20 = (uint8_t)(7);
                }
              }
              else {
                bVar20 = (uint8_t)(3);
              }
            }
            else {
              bVar20 = (uint8_t)(1);
            }
          }
          else {
            bVar20 = (uint8_t)(0);
          }
          cVar2 = (int8_t)((&DAT_00799a0a)[(uint32_t)iVar22]);
          (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(cVar2 + 1U);
          uVar31 = _DAT_00d3b4e4;
          uVar13 = _DAT_00d3b4e0;
          if (bVar20 < (uint8_t)(cVar2 + 1U)) {
            (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(0);
            uVar3 = (uint8_t)((&DAT_007999d9)[(uint32_t)iVar22]);
            uVar30 = 0x58;
            goto LAB_00463bfc;
          }
        }
        break;
      case 0x39:
        FUN_004b76b0(local_44);
        local_44 = iVar11;
        break;
      case 0x3b:
        if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(local_44,0x84)] < 1) {
          *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1) = (uint8_t)(2);
        }
        iVar8 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                             (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
        if (iVar8 != 0) {
          if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 0x33) {
            restore_city_ensure_labor_seeker(iVar11);
          }
          if (0 < (int16_t)(&DAT_00799a18)[(uint32_t)CITY_MUL32(iVar11,0x84)]) {
            if ((*(volatile int16_t *)(&DAT_00799a08 + iVar22) == 0) && ((&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)] != 0))
            {
              iVar8 = FUN_004b4800((&DAT_007999d9)[(uint32_t)iVar22],0x3e,
                                   (int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                                   (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),0);
              *(volatile int16_t *)(&DAT_00799a08 + iVar22) = (int16_t)((int16_t)iVar8);
              (&DAT_00bcd652)[(uint32_t)CITY_MUL32(iVar8,0xc2)] = (uint16_t)(uVar4);
              (&DAT_00bcd630)[(uint32_t)CITY_MUL32(iVar8,0x184)] = (uint8_t)(8);
            }
            sVar21 = (int16_t)((&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)]);
            if (0 < sVar21) {
              iVar22 = sVar21 * 0x184;
              if (((&DAT_00bcd606)[(uint32_t)iVar22] == '\0') ||
                 ((((&DAT_00bcd602)[(uint32_t)iVar22] != '*' && ((&DAT_00bcd602)[(uint32_t)iVar22] != '\0')) ||
                  ((int16_t)(&DAT_00bcd652)[sVar21 * 0xc2] != iVar11)))) {
                (&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)(0);
              }
            }
            if ((int16_t)(&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 1) {
              DAT_00bcb9ac = 2;
              local_44 = iVar11;
            }
          }
        }
        break;
      case 0x3c:
      case 0x3d:
      case 0x3e:
      case 0x3f:
      case 0x40:
      case 0x41:
      case 0x42:
      case 0x43:
      case 0x44:
      case 0x45:
      case 0x97:
      case 0x9c:
        if (*(volatile int16_t *)(&DAT_00799a10 + iVar22) == 0) {
          if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(local_44,0x84)] < 1) {
            *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1) = (uint8_t)(2);
          }
          sVar21 = (int16_t)((&DAT_00799a02)[(uint32_t)CITY_MUL32(local_44,0x84)]);
          if (0 < sVar21) {
            iVar8 = sVar21 * 0x184;
            if (((&DAT_00bcd606)[(uint32_t)iVar8] != '\0') &&
               ((((&DAT_00bcd602)[(uint32_t)iVar8] == '\x1b' || ((&DAT_00bcd602)[(uint32_t)iVar8] == '\0')) &&
                ((int16_t)(&DAT_00bcd652)[sVar21 * 0xc2] == local_44)))) break;
            (&DAT_00799a02)[(uint32_t)CITY_MUL32(local_44,0x84)] = (uint16_t)(0);
          }
          if (((int16_t)(&DAT_007999e8)[(uint32_t)CITY_MUL32(local_44,0x84)] < 0x41) ||
             (0x45 < (int16_t)(&DAT_007999e8)[(uint32_t)CITY_MUL32(local_44,0x84)])) {
            iVar8 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                                 (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
          }
          else {
            iVar8 = FUN_00480460((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                                 (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),
                                 *(volatile uint8_t *)(&DAT_00799a82 + CITY_MUL32(local_44,0x84)));
          }
          if (iVar8 != 0) {
            if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 0x33) {
              restore_city_ensure_labor_seeker(iVar11);
            }
            iVar8 = FUN_004cde60((int32_t)(int16_t)(&DAT_00799a18)[(uint32_t)CITY_MUL32(iVar11,0x84)],
                                 *(volatile uint32_t *)
                                  (&DAT_00705eb8 + (int16_t)(&DAT_007999e8)[(uint32_t)CITY_MUL32(iVar11,0x84)] * 0x28));
            if (*(volatile int32_t *)(&DAT_00705eb8 + (int16_t)(&DAT_007999e8)[(uint32_t)CITY_MUL32(iVar11,0x84)] * 0x28) < 1) {
              bVar20 = (uint8_t)(7);
            }
            else if (iVar8 < 100) {
              if (iVar8 < 0x4b) {
                if (iVar8 < 0x32) {
                  if (iVar8 < 0x19) {
                    if (iVar8 < 1) break;
                    bVar20 = (uint8_t)(0x14);
                  }
                  else {
                    bVar20 = (uint8_t)(0xf);
                  }
                }
                else {
                  bVar20 = (uint8_t)(10);
                }
              }
              else {
                bVar20 = (uint8_t)(7);
              }
            }
            else {
              bVar20 = (uint8_t)(3);
            }
            cVar2 = (int8_t)((&DAT_00799a0a)[(uint32_t)iVar22]);
            (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(cVar2 + 1U);
            uVar31 = _DAT_00d3b4e4;
            uVar13 = _DAT_00d3b4e0;
            if (bVar20 < (uint8_t)(cVar2 + 1U)) {
              (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(0);
              iVar22 = FUN_004b4800((&DAT_007999d9)[(uint32_t)iVar22],0x1b,uVar31,uVar13,0);
              (&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)((int16_t)iVar22);
              iVar11 = CITY_MUL32(iVar22,0x184);
              (&DAT_00bcd630)[(uint32_t)iVar11] = (uint8_t)(1);
              (&DAT_00bcd652)[(uint32_t)CITY_MUL32(iVar22,0xc2)] = (uint16_t)((int16_t)local_44);
              uVar13 = FUN_004c5120((int32_t)(int16_t)local_44);
              switch(uVar13) {
              case 0:
                *(volatile int16_t *)(&DAT_00bcd668 + iVar11) = (int16_t)((int16_t)DAT_00d056b0);
                DAT_00d056b0 = CITY_ADD32(DAT_00d056b0,1);
                *(volatile uint32_t *)(&DAT_00bcd690 + iVar11) = 0x10a;
                if (0xf < DAT_00d056b0) {
                  DAT_00d056b0 = 0;
                  FUN_004b1df0(iVar22);
                  goto switchD_004623ca_caseD_26;
                }
                break;
              case 1:
                *(volatile int16_t *)(&DAT_00bcd668 + iVar11) = (int16_t)((int16_t)DAT_00d056ac);
                DAT_00d056ac = CITY_ADD32(DAT_00d056ac,1);
                *(volatile uint32_t *)(&DAT_00bcd690 + iVar11) = 0x10b;
                if (0xf < DAT_00d056ac) {
                  DAT_00d056ac = 0;
                  FUN_004b1df0(iVar22);
                  goto switchD_004623ca_caseD_26;
                }
                break;
              case 2:
                *(volatile int16_t *)(&DAT_00bcd668 + iVar11) = (int16_t)((int16_t)DAT_00d056a8);
                DAT_00d056a8 = CITY_ADD32(DAT_00d056a8,1);
                *(volatile uint32_t *)(&DAT_00bcd690 + iVar11) = 0x10c;
                if (0xf < DAT_00d056a8) {
                  DAT_00d056a8 = 0;
                  FUN_004b1df0(iVar22);
                  goto switchD_004623ca_caseD_26;
                }
                break;
              case 3:
                *(volatile int16_t *)(&DAT_00bcd668 + iVar11) = (int16_t)((int16_t)DAT_00d056a4);
                DAT_00d056a4 = CITY_ADD32(DAT_00d056a4,1);
                *(volatile uint32_t *)(&DAT_00bcd690 + iVar11) = 0x10d;
                if (0xf < DAT_00d056a4) {
                  DAT_00d056a4 = 0;
                  FUN_004b1df0(iVar22);
                  goto switchD_004623ca_caseD_26;
                }
                break;
              case 4:
                *(volatile int16_t *)(&DAT_00bcd668 + iVar11) = (int16_t)((int16_t)DAT_00d056a0);
                DAT_00d056a0 = CITY_ADD32(DAT_00d056a0,1);
                *(volatile uint32_t *)(&DAT_00bcd690 + iVar11) = 0x10e;
                if (0xf < DAT_00d056a0) {
                  DAT_00d056a0 = 0;
                }
              }
              FUN_004b1df0(iVar22);
            }
          }
        }
        break;
      case 0x46:
      case 0xbe:
        local_10[0] = 0xffffffff;
        local_1c = local_1c + 1;
        local_10[1] = 0xffffffff;
        local_10[2] = 0xffffffff;
        local_10[3] = 0xffffffff;
        local_45 = (int8_t)(restore_city_refresh_fertility_building_image(local_44));
        if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 1) {
          *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1) = (uint8_t)(2);
        }
        iVar8 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                             (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
        if (iVar8 != 0) {
          if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 0x33) {
            restore_city_ensure_labor_seeker(iVar11);
          }
          iVar8 = FUN_004cde60((int32_t)(int16_t)(&DAT_00799a18)[(uint32_t)CITY_MUL32(iVar11,0x84)],
                               *(volatile uint32_t *)
                                (&DAT_00705eb8 + (int16_t)(&DAT_007999e8)[(uint32_t)CITY_MUL32(iVar11,0x84)] * 0x28));
          if (iVar8 < 100) {
            if (iVar8 < 0x4b) {
              if (iVar8 < 0x32) {
                if (iVar8 < 0x19) {
                  if (iVar8 < 1) break;
                  bVar20 = (uint8_t)(10);
                }
                else {
                  bVar20 = (uint8_t)(5);
                }
              }
              else {
                bVar20 = (uint8_t)(4);
              }
            }
            else {
              bVar20 = (uint8_t)(3);
            }
          }
          else {
            bVar20 = (uint8_t)(2);
          }
          iVar9 = 0;
          iVar8 = iVar11;
          if (0 < DAT_00f53dd8) {
            piVar17 = (volatile int32_t *)(uintptr_t)0x00f53ddcu;
            iVar15 = DAT_00f53dd8;
            do {
              if (*piVar17 != 0) {
                switch(*piVar17) {
                case 1:
                  iVar16 = 0;
                  break;
                case 2:
                  iVar16 = 1;
                  break;
                case 3:
                  iVar16 = 2;
                  break;
                case 4:
                  iVar16 = 3;
                  break;
                case 5:
                  iVar16 = 4;
                  break;
                case 6:
                  iVar16 = 5;
                  break;
                case 7:
                  iVar16 = 6;
                  break;
                case 8:
                  iVar16 = 7;
                  break;
                default:
                  iVar16 = -1;
                  break;
                case 0xd:
                  iVar16 = 9;
                  break;
                case 0xf:
                  iVar16 = 8;
                  break;
                case 0x11:
                  iVar16 = 10;
                  break;
                case 0x13:
                  iVar16 = 0xb;
                }
                iVar9 = (int32_t)((uint32_t)iVar9 + (uint32_t)(int16_t)(&DAT_00799a2e)[(uint32_t)iVar16 + ((uint32_t)CITY_MUL32(iVar8,0x20) + (uint32_t)local_44) * 4]);
                iVar8 = local_44;
              }
              piVar17 = piVar17 + 1;
              iVar15 = CITY_ADD32(iVar15,-1);
            } while (iVar15 != 0);
          }
          local_38 = (int32_t)((uint32_t)(int16_t)(&DAT_00799a40)[(uint32_t)CITY_MUL32(iVar11,0x84)] + iVar9 +
                     (int32_t)(int16_t)(&DAT_00799a3e)[(uint32_t)CITY_MUL32(iVar11,0x84)] +
                     (int32_t)(int16_t)(&DAT_00799a42)[(uint32_t)CITY_MUL32(iVar11,0x84)] +
                     (int32_t)(int16_t)(&DAT_00799a44)[(uint32_t)CITY_MUL32(iVar11,0x84)]);
          sVar21 = (int16_t)((&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)]);
          if (sVar21 < 1) {
LAB_00464bfd:
            if (local_38 == 0) goto LAB_00464c6e;
            cVar2 = (int8_t)((&DAT_00799a0a)[(uint32_t)iVar22]);
            (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(cVar2 + 1U);
            uVar31 = _DAT_00d3b4e4;
            uVar13 = _DAT_00d3b4e0;
            if ((uint8_t)(cVar2 + 1U) <= bVar20) break;
            (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(0);
            iVar9 = FUN_004b4800((&DAT_007999d9)[(uint32_t)iVar22],0x1a,uVar31,uVar13,0);
            (&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)((int16_t)iVar9);
            (&DAT_00bcd630)[(uint32_t)CITY_MUL32(iVar9,0x184)] = (uint8_t)(1);
            (&DAT_00bcd652)[(uint32_t)CITY_MUL32(iVar9,0xc2)] = (uint16_t)((int16_t)iVar8);
            FUN_004b1df0(iVar9);
          }
          else {
            iVar9 = sVar21 * 0x184;
            if (((&DAT_00bcd606)[(uint32_t)iVar9] == '\0') ||
               ((((&DAT_00bcd602)[(uint32_t)iVar9] != '\x1a' && ((&DAT_00bcd602)[(uint32_t)iVar9] != '\0')) ||
                ((int16_t)(&DAT_00bcd652)[sVar21 * 0xc2] != iVar8)))) {
              (&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)(0);
              goto LAB_00464bfd;
            }
LAB_00464c6e:
            restore_city_refresh_nearby_house_needs(iVar8,10);
          }
          local_46 = (uint8_t)(0);
          if ((&DAT_00799a04)[(uint32_t)CITY_MUL32(iVar11,0x84)] == 0) {
            uVar10 = DAT_0078d7c4 & 0x80000007;
            if ((int32_t)uVar10 < 0) {
              uVar10 = (uVar10 - 1 | 0xfffffff8) + 1;
            }
            uVar24 = local_1c & 0x80000007;
            if ((int32_t)uVar24 < 0) {
              uVar24 = (uVar24 - 1 | 0xfffffff8) + 1;
            }
            if (uVar10 == uVar24) {
              FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                           (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
              iVar8 = local_44;
              iVar9 = FUN_00456fc0(local_44,0,local_10,&local_46);
              if (0 < iVar9) {
                iVar15 = FUN_004b4800((&DAT_007999d9)[(uint32_t)iVar22],0x27,_DAT_00d3b4e4,_DAT_00d3b4e0,0);
                (&DAT_00799a04)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)((int16_t)iVar15);
                iVar16 = CITY_MUL32(iVar15,0x184);
                puVar18 = (uint32_t *)local_10;
                iVar11 = 4;
                (&DAT_00bcd630)[(uint32_t)iVar16] = (uint8_t)(8);
                (&DAT_00bcd652)[(uint32_t)CITY_MUL32(iVar15,0xc2)] = (uint16_t)((int16_t)iVar8);
                (&DAT_00bcd656)[(uint32_t)CITY_MUL32(iVar15,0xc2)] = (uint16_t)((int16_t)iVar9);
                puVar12 = (volatile uint32_t *)(&DAT_00bcd6a4 + iVar16);
                do {
                  uVar13 = *puVar18;
                  puVar18 = puVar18 + 1;
                  puVar12[-4] = uVar13;
                  *puVar12 = 0;
                  puVar12[4] = 0;
                  puVar12 = puVar12 + 1;
                  iVar11 = CITY_ADD32(iVar11,-1);
                } while (iVar11 != 0);
                iVar9 = CITY_MUL32(iVar9,0x108);
                iVar11 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar9 + 2),
                                      (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar9),(&DAT_007999db)[(uint32_t)iVar9])
                ;
                if (iVar11 == 0) {
                  iVar11 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar9 + 2),
                                        (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar9),3);
                  if (iVar11 == 0) {
                    (&DAT_00bcd630)[(uint32_t)iVar16] = (uint8_t)(9);
                    *(volatile uint16_t *)(&DAT_00bcd61c + iVar16) = (uint16_t)((&DAT_00bcd60c)[(uint32_t)CITY_MUL32(iVar15,0xc2)]);
                    *(volatile uint16_t *)(&DAT_00bcd61e + iVar16) = (uint16_t)((&DAT_00bcd60e)[(uint32_t)CITY_MUL32(iVar15,0xc2)]);
                  }
                  else {
                    uVar4 = (uint16_t)(DAT_00d3b4e0);
                    *(volatile uint16_t *)(&DAT_00bcd61c + iVar16) = (uint16_t)(DAT_00d3b4e4);
                    *(volatile uint16_t *)(&DAT_00bcd61e + iVar16) = (uint16_t)(uVar4);
                  }
                }
                else {
                  uVar4 = (uint16_t)(DAT_00d3b4e0);
                  *(volatile uint16_t *)(&DAT_00bcd61c + iVar16) = (uint16_t)(DAT_00d3b4e4);
                  *(volatile uint16_t *)(&DAT_00bcd61e + iVar16) = (uint16_t)(uVar4);
                }
              }
            }
          }
          else {
            iVar8 = (int16_t)(&DAT_00799a04)[(uint32_t)CITY_MUL32(iVar11,0x84)] * 0x184;
            if (((&DAT_00bcd606)[(uint32_t)iVar8] != '\x01') ||
               (((&DAT_00bcd602)[(uint32_t)iVar8] != '\'' && ((&DAT_00bcd602)[(uint32_t)iVar8] != '\x05')))) {
              (&DAT_00799a04)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)(0);
            }
          }
          if (*(volatile int16_t *)(&DAT_00799a8e + iVar22) == 0) {
            uVar10 = DAT_0078d7c4 & 0x80000007;
            if ((int32_t)uVar10 < 0) {
              uVar10 = (uVar10 - 1 | 0xfffffff8) + 1;
            }
            uVar24 = local_1c & 0x80000007;
            if ((int32_t)uVar24 < 0) {
              uVar24 = (uVar24 - 1 | 0xfffffff8) + 1;
            }
            if (uVar10 == uVar24) {
              FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                           (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
              iVar11 = local_44;
              iVar8 = FUN_00456fc0(local_44,1,local_10,&local_46);
              if (0 < iVar8) {
                iVar9 = FUN_004b4800((&DAT_007999d9)[(uint32_t)iVar22],0x27,_DAT_00d3b4e4,_DAT_00d3b4e0,0);
                *(volatile int16_t *)(&DAT_00799a8e + iVar22) = (int16_t)((int16_t)iVar9);
                iVar16 = CITY_MUL32(iVar9,0x184);
                puVar18 = (uint32_t *)local_10;
                iVar15 = 4;
                (&DAT_00bcd630)[(uint32_t)iVar16] = (uint8_t)(8);
                (&DAT_00bcd652)[(uint32_t)CITY_MUL32(iVar9,0xc2)] = (uint16_t)((int16_t)iVar11);
                (&DAT_00bcd656)[(uint32_t)CITY_MUL32(iVar9,0xc2)] = (uint16_t)((int16_t)iVar8);
                puVar12 = (volatile uint32_t *)(&DAT_00bcd6a4 + iVar16);
                do {
                  uVar13 = *puVar18;
                  puVar18 = puVar18 + 1;
                  puVar12[-4] = uVar13;
                  *puVar12 = 0;
                  puVar12[4] = 0;
                  puVar12 = puVar12 + 1;
                  iVar15 = CITY_ADD32(iVar15,-1);
                } while (iVar15 != 0);
                iVar8 = CITY_MUL32(iVar8,0x108);
                iVar11 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar8 + 2),
                                      (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar8),(&DAT_007999db)[(uint32_t)iVar8])
                ;
                if (iVar11 == 0) {
                  iVar11 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar8 + 2),
                                        (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar8),3);
                  if (iVar11 == 0) {
                    (&DAT_00bcd630)[(uint32_t)iVar16] = (uint8_t)(9);
                    *(volatile uint16_t *)(&DAT_00bcd61c + iVar16) = (uint16_t)((&DAT_00bcd60c)[(uint32_t)CITY_MUL32(iVar9,0xc2)]);
                    *(volatile uint16_t *)(&DAT_00bcd61e + iVar16) = (uint16_t)((&DAT_00bcd60e)[(uint32_t)CITY_MUL32(iVar9,0xc2)]);
                  }
                  else {
                    uVar4 = (uint16_t)(DAT_00d3b4e0);
                    *(volatile uint16_t *)(&DAT_00bcd61c + iVar16) = (uint16_t)(DAT_00d3b4e4);
                    *(volatile uint16_t *)(&DAT_00bcd61e + iVar16) = (uint16_t)(uVar4);
                  }
                }
                else {
                  uVar4 = (uint16_t)(DAT_00d3b4e0);
                  *(volatile uint16_t *)(&DAT_00bcd61c + iVar16) = (uint16_t)(DAT_00d3b4e4);
                  *(volatile uint16_t *)(&DAT_00bcd61e + iVar16) = (uint16_t)(uVar4);
                }
              }
            }
          }
          else {
            iVar11 = *(volatile int16_t *)(&DAT_00799a8e + iVar22) * 0x184;
            if (((&DAT_00bcd606)[(uint32_t)iVar11] != '\x01') ||
               (((&DAT_00bcd602)[(uint32_t)iVar11] != '\'' && ((&DAT_00bcd602)[(uint32_t)iVar11] != '\x05')))) {
              *(volatile uint16_t *)(&DAT_00799a8e + iVar22) = (uint16_t)(0);
            }
          }
          if (local_45 != '\0') {
            if ((*(volatile int16_t *)(&DAT_00799a8c + iVar22) == 0) && (local_38 != 0)) {
              iVar11 = FUN_004b4800((&DAT_007999d9)[(uint32_t)iVar22],0x1a,_DAT_00d3b4e4,_DAT_00d3b4e0,0);
              *(volatile int16_t *)(&DAT_00799a8c + iVar22) = (int16_t)((int16_t)iVar11);
              (&DAT_00bcd630)[(uint32_t)CITY_MUL32(iVar11,0x184)] = (uint8_t)(1);
              (&DAT_00bcd652)[(uint32_t)CITY_MUL32(iVar11,0xc2)] = (uint16_t)((uint16_t)local_44);
              FUN_004b1df0(iVar11);
            }
            else {
              iVar11 = *(volatile int16_t *)(&DAT_00799a8c + iVar22) * 0x184;
              if (((&DAT_00bcd606)[(uint32_t)iVar11] != '\x01') ||
                 (((&DAT_00bcd602)[(uint32_t)iVar11] != '\x1a' && ((&DAT_00bcd602)[(uint32_t)iVar11] != '\x05')))) {
                *(volatile uint16_t *)(&DAT_00799a8c + iVar22) = (uint16_t)(0);
              }
            }
          }
        }
        break;
      case 0x47:
      case 0xbf:
        local_3c = 0;
        local_28 = 100;
        local_2c = 100;
        sVar21 = (int16_t)((&DAT_00799a02)[(uint32_t)CITY_MUL32(local_44,0x84)]);
        local_34 = 0;
        local_30 = 0;
        local_20 = 0;
        if (sVar21 < 1) {
          iVar8 = 0;
        }
        else {
          iVar8 = sVar21 * 0x184;
          if ((&DAT_00bcd606)[(uint32_t)iVar8] == '\0') {
            iVar8 = 0;
            (&DAT_00799a02)[(uint32_t)CITY_MUL32(local_44,0x84)] = (uint16_t)(0);
          }
          else if (((&DAT_00bcd602)[(uint32_t)iVar8] == '\t') || ((&DAT_00bcd602)[(uint32_t)iVar8] == '\0')) {
            if ((int16_t)(&DAT_00bcd652)[sVar21 * 0xc2] == local_44) {
              iVar8 = 1;
            }
            else {
              iVar8 = 0;
              (&DAT_00799a02)[(uint32_t)CITY_MUL32(local_44,0x84)] = (uint16_t)(0);
            }
          }
          else {
            iVar8 = 0;
            (&DAT_00799a02)[(uint32_t)CITY_MUL32(local_44,0x84)] = (uint16_t)(0);
          }
        }
        sVar21 = (int16_t)(*(volatile int16_t *)(&DAT_00799a8e + iVar22));
        if (sVar21 < 1) {
          iVar9 = 0;
        }
        else {
          iVar9 = sVar21 * 0x184;
          if ((&DAT_00bcd606)[(uint32_t)iVar9] == '\0') {
            *(volatile uint16_t *)(&DAT_00799a8e + iVar22) = (uint16_t)(0);
            iVar9 = 0;
          }
          else if (((&DAT_00bcd602)[(uint32_t)iVar9] == '\t') || ((&DAT_00bcd602)[(uint32_t)iVar9] == '\0')) {
            if ((int16_t)(&DAT_00bcd652)[sVar21 * 0xc2] == local_44) {
              iVar9 = 1;
            }
            else {
              *(volatile uint16_t *)(&DAT_00799a8e + iVar22) = (uint16_t)(0);
              iVar9 = 0;
            }
          }
          else {
            *(volatile uint16_t *)(&DAT_00799a8e + iVar22) = (uint16_t)(0);
            iVar9 = 0;
          }
        }
        if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(local_44,0x84)] < 1) {
          *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1) = (uint8_t)(2);
        }
        local_40 = iVar8;
        local_38 = iVar9;
        iVar15 = FUN_004806a0((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                              (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22));
        if (iVar15 != 0) {
          if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 0x65) {
            restore_city_ensure_labor_seeker(local_44);
          }
          iVar15 = local_44;
          uVar31 = _DAT_00d3b4e4;
          uVar13 = _DAT_00d3b4e0;
          if ((iVar8 == 0) || (_DAT_00d3b4e0 = uVar13, _DAT_00d3b4e4 = uVar31, iVar9 == 0)) {
            FUN_0045b880(local_44,&local_3c,&local_34,&local_28,&local_2c,&local_30,&local_20);
            iVar8 = FUN_0045ce90(iVar15,local_3c);
            _DAT_00d3b4e0 = uVar13;
            _DAT_00d3b4e4 = uVar31;
            if (-1 < iVar8) {
              if (iVar8 == 0) {
                iVar8 = FUN_0045bb30(local_3c,local_34);
                if ((iVar8 != 0) &&
                   (FUN_004611d0(local_28,local_2c,iVar8,&local_48,&local_49,&local_24),
                   local_24 != 0)) {
                  if (local_48 == '\0') {
                    if ((local_40 == 0) && (local_30 != 0)) {
                      iVar22 = FUN_004b4800((&DAT_007999d9)[(uint32_t)iVar22],9,
                                            (int32_t)(int16_t)(&DAT_007999fe)[(uint32_t)CITY_MUL32(iVar11,0x84)],
                                            (int32_t)(int16_t)(&DAT_00799a00)[(uint32_t)CITY_MUL32(iVar11,0x84)],4);
                      iVar8 = CITY_MUL32(iVar22,0x184);
                      (&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)((int16_t)iVar22);
                      (&DAT_00bcd630)[(uint32_t)iVar8] = (uint8_t)(8);
                      (&DAT_00bcd603)[(uint32_t)iVar8] = (uint8_t)(0);
                      (&DAT_00bcd672)[(uint32_t)iVar8] = (uint8_t)((uint8_t)local_3c);
                      *(volatile uint16_t *)(&DAT_00bcd66c + iVar8) = (uint16_t)(0);
                      goto LAB_00463794;
                    }
                  }
                  else {
                    if (local_40 == 0) goto LAB_00463332;
LAB_0046370a:
                    if (local_38 == 0) {
                      iVar8 = local_20;
                      if (local_49 != '\0') {
                        iVar8 = local_30;
                      }
                      if (iVar8 != 0) {
                        iVar11 = FUN_004b4800((&DAT_007999d9)[(uint32_t)iVar22],9,
                                              (int32_t)(int16_t)(&DAT_007999fe)[(uint32_t)CITY_MUL32(iVar11,0x84)],
                                              (int32_t)(int16_t)(&DAT_00799a00)[(uint32_t)CITY_MUL32(iVar11,0x84)],4);
                        iVar8 = CITY_MUL32(iVar11,0x184);
                        (&DAT_00bcd630)[(uint32_t)iVar8] = (uint8_t)(8);
                        (&DAT_00bcd603)[(uint32_t)iVar8] = (uint8_t)(0);
                        uVar3 = (uint8_t)((uint8_t)local_3c);
                        if (local_49 == '\0') {
                          uVar3 = (uint8_t)((uint8_t)local_34);
                        }
                        (&DAT_00bcd672)[(uint32_t)iVar8] = (uint8_t)(uVar3);
                        *(volatile uint16_t *)(&DAT_00bcd66c + iVar8) = (uint16_t)(0);
                        *(volatile int16_t *)(&DAT_00799a8e + iVar22) = (int16_t)((int16_t)iVar11);
                        goto LAB_00463794;
                      }
                    }
                  }
                }
              }
              else if (local_40 == 0) {
                iVar9 = FUN_004b4800((&DAT_007999d9)[(uint32_t)iVar22],9,
                                     (int32_t)(int16_t)(&DAT_007999fe)[(uint32_t)CITY_MUL32(iVar11,0x84)],
                                     (int32_t)(int16_t)(&DAT_00799a00)[(uint32_t)CITY_MUL32(iVar11,0x84)],4);
                iVar15 = CITY_MUL32(iVar9,0x184);
                cVar2 = (int8_t)(*(volatile int8_t *)((uint32_t)(uintptr_t)&DAT_007900c2 +
                                 (uint32_t)*(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a98 + iVar22 + 1) * 0xc4 + iVar8));
                (&DAT_00bcd630)[(uint32_t)iVar15] = (uint8_t)(8);
                (&DAT_00bcd603)[(uint32_t)iVar15] = (uint8_t)((int8_t)iVar8);
                if (cVar2 == '\x03') {
                  switch(iVar8) {
                  case 0x18:
                  case 0x19:
                  case 0x1a:
                  case 0x1e:
                  case 0x23:
                    goto switchD_004635c4_caseD_18;
                  default:
                    if (((iVar8 == 0xc) || (iVar8 == 0x1c)) || (sVar21 = (int16_t)(400), iVar8 == 10))
                    goto switchD_004635c4_caseD_18;
                  }
                }
                else {
switchD_004635c4_caseD_18:
                  sVar21 = (int16_t)(100);
                }
                sVar5 = (int16_t)((&DAT_00799a2e)[(uint32_t)iVar8 + (uint32_t)CITY_MUL32(local_44,0x84)]);
                if (sVar21 < (int16_t)(&DAT_00799a2e)[(uint32_t)iVar8 + (uint32_t)CITY_MUL32(local_44,0x84)]) {
                  sVar5 = (int16_t)(sVar21);
                }
                *(volatile int16_t *)(&DAT_00bcd66c + iVar15) = (int16_t)(sVar5);
                (&DAT_00bcd652)[(uint32_t)CITY_MUL32(iVar9,0xc2)] = (uint16_t)((int16_t)local_44);
                iVar22 = DAT_00eaac28;
                sVar21 = (int16_t)(*(volatile int16_t *)(&DAT_00bcd66c + iVar15));
                uVar3 = (uint8_t)((&DAT_00bcd603)[(uint32_t)iVar15]);
                (&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)((int16_t)iVar9);
                FUN_0045e360(iVar22,local_44,uVar3,(int32_t)sVar21);
              }
            }
          }
        }
        break;
      case 0x48:
      case 0xc1:
        local_3c = 0;
        local_28 = 100;
        local_2c = 100;
        sVar21 = (int16_t)((&DAT_00799a02)[(uint32_t)CITY_MUL32(local_44,0x84)]);
        local_34 = 0;
        if (sVar21 < 1) {
          iVar8 = 0;
        }
        else {
          iVar8 = sVar21 * 0x184;
          if ((&DAT_00bcd606)[(uint32_t)iVar8] == '\0') {
            iVar8 = 0;
            (&DAT_00799a02)[(uint32_t)CITY_MUL32(local_44,0x84)] = (uint16_t)(0);
          }
          else if (((&DAT_00bcd602)[(uint32_t)iVar8] == '\t') || ((&DAT_00bcd602)[(uint32_t)iVar8] == '\0')) {
            if ((int16_t)(&DAT_00bcd652)[sVar21 * 0xc2] == local_44) {
              iVar8 = 1;
            }
            else {
              iVar8 = 0;
              (&DAT_00799a02)[(uint32_t)CITY_MUL32(local_44,0x84)] = (uint16_t)(0);
            }
          }
          else {
            iVar8 = 0;
            (&DAT_00799a02)[(uint32_t)CITY_MUL32(local_44,0x84)] = (uint16_t)(0);
          }
        }
        sVar21 = (int16_t)(*(volatile int16_t *)(&DAT_00799a8e + iVar22));
        if (sVar21 < 1) {
          iVar9 = 0;
        }
        else {
          iVar9 = sVar21 * 0x184;
          if ((&DAT_00bcd606)[(uint32_t)iVar9] == '\0') {
            *(volatile uint16_t *)(&DAT_00799a8e + iVar22) = (uint16_t)(0);
            iVar9 = 0;
          }
          else if (((&DAT_00bcd602)[(uint32_t)iVar9] == '\t') || ((&DAT_00bcd602)[(uint32_t)iVar9] == '\0')) {
            if ((int16_t)(&DAT_00bcd652)[sVar21 * 0xc2] == local_44) {
              iVar9 = 1;
            }
            else {
              *(volatile uint16_t *)(&DAT_00799a8e + iVar22) = (uint16_t)(0);
              iVar9 = 0;
            }
          }
          else {
            *(volatile uint16_t *)(&DAT_00799a8e + iVar22) = (uint16_t)(0);
            iVar9 = 0;
          }
        }
        if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(local_44,0x84)] < 1) {
          *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1) = (uint8_t)(2);
        }
        iVar16 = 8;
        iVar15 = local_44;
        do {
          iVar15 = (int32_t)*(volatile int16_t *)(&DAT_00799a12 + CITY_MUL32(iVar15,0x108));
          if (iVar15 != 0) {
            *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + CITY_MUL32(iVar15,0x108) + 1) =
                 (uint8_t)(*(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1));
          }
          iVar16 = CITY_ADD32(iVar16,-1);
        } while (iVar16 != 0);
        local_40 = iVar8;
        local_38 = iVar9;
        iVar15 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                              (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
        if ((iVar15 != 0) ||
           (iVar15 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                                  (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),3), iVar15 != 0)) {
          if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 0x65) {
            restore_city_ensure_labor_seeker(iVar11);
          }
          iVar15 = local_44;
          uVar31 = _DAT_00d3b4e4;
          uVar13 = _DAT_00d3b4e0;
          if ((iVar8 == 0) || (_DAT_00d3b4e0 = uVar13, _DAT_00d3b4e4 = uVar31, iVar9 == 0)) {
            FUN_0045b880(local_44,&local_3c,&local_34,&local_28,&local_2c,&local_30,&local_20);
            iVar8 = FUN_0045bf00(iVar15,local_3c);
            _DAT_00d3b4e0 = uVar13;
            _DAT_00d3b4e4 = uVar31;
            if (-1 < iVar8) {
              if (iVar8 == 0) {
                iVar8 = FUN_0045bb30(local_3c,local_34);
                if ((iVar8 != 0) &&
                   (FUN_004611d0(local_28,local_2c,iVar8,&local_48,&local_49,&local_24),
                   local_24 != 0)) {
                  if (local_48 == '\0') {
                    if (local_40 != 0) break;
                  }
                  else if (local_40 != 0) goto LAB_0046370a;
LAB_00463332:
                  if (local_30 != 0) {
                    iVar22 = FUN_004b4800((&DAT_007999d9)[(uint32_t)iVar22],9,
                                          (int32_t)(int16_t)(&DAT_007999fe)[(uint32_t)CITY_MUL32(iVar11,0x84)],
                                          (int32_t)(int16_t)(&DAT_00799a00)[(uint32_t)CITY_MUL32(iVar11,0x84)],4);
                    iVar8 = CITY_MUL32(iVar22,0x184);
                    (&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)((int16_t)iVar22);
                    (&DAT_00bcd630)[(uint32_t)iVar8] = (uint8_t)(8);
                    (&DAT_00bcd603)[(uint32_t)iVar8] = (uint8_t)(0);
                    (&DAT_00bcd672)[(uint32_t)iVar8] = (uint8_t)((uint8_t)local_3c);
                    *(volatile uint16_t *)(&DAT_00bcd66c + iVar8) = (uint16_t)(0);
LAB_00463794:
                    *(volatile uint16_t *)((uint32_t)(uintptr_t)&DAT_00bcd652 + iVar8) = (uint16_t)((uint16_t)local_44);
                  }
                }
              }
              else if (local_40 == 0) {
                iVar9 = FUN_004b4800((&DAT_007999d9)[(uint32_t)iVar22],9,
                                     (int32_t)(int16_t)(&DAT_007999fe)[(uint32_t)CITY_MUL32(iVar11,0x84)],
                                     (int32_t)(int16_t)(&DAT_00799a00)[(uint32_t)CITY_MUL32(iVar11,0x84)],4);
                iVar15 = CITY_MUL32(iVar9,0x184);
                cVar2 = (int8_t)(*(volatile int8_t *)((uint32_t)(uintptr_t)&DAT_007900c2 +
                                 (uint32_t)*(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a98 + iVar22 + 1) * 0xc4 + iVar8));
                (&DAT_00bcd630)[(uint32_t)iVar15] = (uint8_t)(8);
                (&DAT_00bcd603)[(uint32_t)iVar15] = (uint8_t)((int8_t)iVar8);
                if (cVar2 == '\x03') {
                  switch(iVar8) {
                  case 0x18:
                  case 0x19:
                  case 0x1a:
                  case 0x1e:
                  case 0x23:
                    goto switchD_00463241_caseD_18;
                  default:
                    if (((iVar8 == 0xc) || (iVar8 == 0x1c)) || (local_38 = 400, iVar8 == 10))
                    goto switchD_00463241_caseD_18;
                  }
                }
                else {
switchD_00463241_caseD_18:
                  local_38 = 100;
                }
                iVar22 = FUN_0045d510(local_44,iVar8);
                if (local_38 < iVar22) {
                  uVar4 = (uint16_t)((uint16_t)local_38);
                }
                else {
                  uVar4 = (uint16_t)(FUN_0045d510(local_44,iVar8));
                }
                *(volatile uint16_t *)(&DAT_00bcd66c + iVar15) = (uint16_t)(uVar4);
                (&DAT_00bcd652)[(uint32_t)CITY_MUL32(iVar9,0xc2)] = (uint16_t)((int16_t)local_44);
                iVar22 = DAT_00eaac28;
                sVar21 = (int16_t)(*(volatile int16_t *)(&DAT_00bcd66c + iVar15));
                uVar3 = (uint8_t)((&DAT_00bcd603)[(uint32_t)iVar15]);
                (&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)((int16_t)iVar9);
                FUN_0045b570(iVar22,(int32_t)(int16_t)local_44,uVar3,(int32_t)sVar21);
              }
            }
          }
        }
        break;
      case 0x4a:
        if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(local_44,0x84)] < 1) {
          *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1) = (uint8_t)(2);
        }
        iVar8 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                             (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
        if (iVar8 != 0) {
          if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 0x33) {
            restore_city_ensure_labor_seeker(iVar11);
          }
          uVar6 = (uint16_t)(*(volatile uint16_t *)(&DAT_00799a8c + iVar22) & 0xf000);
          if ((*(volatile uint16_t *)(&DAT_00799a8c + iVar22) & 0xf000) == 0) {
            cVar2 = (int8_t)(FUN_00460f70(iVar11,&local_18));
            if (cVar2 == '\0') {
              cVar2 = (int8_t)(FUN_00460fb0(&local_18));
              if ((cVar2 != '\0') &&
                 (((local_18 != 0x4e && (local_18 != 0x4d)) ||
                  (99 < (int16_t)(&DAT_00799a14)[(uint32_t)CITY_MUL32(iVar11,0x84)])))) {
                *(volatile uint16_t *)(&DAT_00799a8c + iVar22) = (uint16_t)((uint16_t)local_18 | 0x1000);
                local_44 = iVar11;
              }
            }
            else {
              *(volatile uint16_t *)(&DAT_00799a8c + iVar22) = (uint16_t)((uint16_t)local_18 | 0x2000);
              local_44 = iVar11;
            }
          }
          else {
            if (uVar6 == 0x1000) {
              local_45 = (int8_t)('\x01');
            }
            else {
              if (uVar6 != 0x2000) {
                *(volatile uint16_t *)(&DAT_00799a8c + iVar22) = (uint16_t)(0);
                local_44 = iVar11;
                break;
              }
              local_45 = (int8_t)('\0');
            }
            iVar8 = FUN_004cde60((int32_t)(int16_t)(&DAT_00799a18)[(uint32_t)CITY_MUL32(iVar11,0x84)],
                                 *(volatile uint32_t *)
                                  (&DAT_00705eb8 + (int16_t)(&DAT_007999e8)[(uint32_t)CITY_MUL32(iVar11,0x84)] * 0x28));
            if (iVar8 < 100) {
              if (iVar8 < 0x4b) {
                if (iVar8 < 0x32) {
                  if (iVar8 < 0x19) {
                    uVar10 = (iVar8 < 1) - 1 & 2;
                  }
                  else {
                    uVar10 = 4;
                  }
                }
                else {
                  uVar10 = 6;
                }
              }
              else {
                uVar10 = 8;
              }
            }
            else {
              uVar10 = 10;
            }
            if (local_45 == '\0') {
              iVar8 = (*(volatile uint16_t *)(&DAT_00799a8c + iVar22) & 0xfff) * 0x184;
              if ((&DAT_00bcd606)[(uint32_t)iVar8] == '\x02') {
                *(volatile uint16_t *)(&DAT_00799a8c + iVar22) = (uint16_t)(0);
              }
              else {
                sVar21 = (int16_t)((&DAT_00799a14)[(uint32_t)CITY_MUL32(iVar11,0x84)]);
                if ((0 < sVar21) || (DAT_00605a0c != 0)) {
                  iVar9 = *(volatile int32_t *)(&DAT_00bcd720 + iVar8);
                  bVar29 = (uint8_t)(DAT_00605a0c == 0);
                  *(volatile uint32_t *)(&DAT_00bcd720 + iVar8) = iVar9 + uVar10;
                  if (bVar29) {
                    (&DAT_00799a14)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)(sVar21 + -1);
                  }
                  if (*(volatile int32_t *)(&DAT_00bcd724 + iVar8) <= (int32_t)(iVar9 + uVar10)) {
                    *(volatile int32_t *)(&DAT_00bcd720 + iVar8) = *(volatile int32_t *)(&DAT_00bcd724 + iVar8);
                    cVar2 = (int8_t)((&DAT_00bcd602)[(uint32_t)iVar8]);
                    *(volatile uint16_t *)(&DAT_00799a8c + iVar22) = (uint16_t)(0);
                    if (cVar2 == 'M') {
                      (&DAT_00bcd630)[(uint32_t)iVar8] = (uint8_t)(8);
                    }
                    else {
                      (&DAT_00bcd630)[(uint32_t)iVar8] = (uint8_t)(8);
                    }
                  }
                }
              }
            }
            else {
              uVar6 = (uint16_t)(*(volatile uint16_t *)(&DAT_00799a8c + iVar22) & 0xfff);
              local_38 = restore_city_find_vacant_service_building(uVar6);
              if (local_38 == 0) {
                *(volatile uint16_t *)(&DAT_00799a8c + iVar22) = (uint16_t)(0);
                (&DAT_00799a2c)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)(0);
              }
              else {
                if ((((int16_t)(&DAT_00799a14)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 100) && (DAT_00605a0c == 0)) &&
                   (uVar6 != 0x19)) {
                  uVar10 = 0;
                  cVar2 = (int8_t)(FUN_00460fb0(&local_14));
                  if ((cVar2 != '\0') && (local_14 == 0x19)) {
                    *(volatile uint16_t *)(&DAT_00799a8c + iVar22) = (uint16_t)(0);
                    (&DAT_00799a2c)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)(0);
                  }
                }
                (&DAT_00799a2c)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)((&DAT_00799a2c)[(uint32_t)CITY_MUL32(iVar11,0x84)] + (int16_t)uVar10);
                if (0x9f < (int16_t)(&DAT_00799a2c)[(uint32_t)CITY_MUL32(iVar11,0x84)]) {
                  (&DAT_00799a2c)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)(0);
                  iVar8 = FUN_004806c0((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                                       (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),
                                       (&DAT_007999db)[(uint32_t)iVar22]);
                  if (iVar8 != 0) {
                    iVar8 = FUN_004b4800((&DAT_007999d9)[(uint32_t)iVar22],uVar6,_DAT_00d3b4e4,_DAT_00d3b4e0,0
                                        );
                    uVar13 = DAT_00709648;
                    if (uVar6 == 0x19) {
                      iVar11 = CITY_MUL32(iVar8,0x184);
                      (&DAT_00bcd630)[(uint32_t)iVar11] = (uint8_t)(8);
                      *(volatile uint32_t *)(&DAT_00bcd71c + iVar11) = 0;
                      *(volatile uint32_t *)(&DAT_00bcd720 + iVar11) = uVar13;
                      *(volatile uint32_t *)(&DAT_00bcd724 + iVar11) = uVar13;
                    }
                    else if (uVar6 == 0x4d) {
                      (&DAT_00799a14)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)((&DAT_00799a14)[(uint32_t)CITY_MUL32(iVar11,0x84)] + -100);
                      (&DAT_00bcd630)[(uint32_t)CITY_MUL32(iVar8,0x184)] = (uint8_t)(8);
                      FUN_004620c0(iVar8);
                    }
                    else {
                      (&DAT_00799a14)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)((&DAT_00799a14)[(uint32_t)CITY_MUL32(iVar11,0x84)] + -100);
                      (&DAT_00bcd630)[(uint32_t)CITY_MUL32(iVar8,0x184)] = (uint8_t)(8);
                      restore_actor_initialize_type_parameters(iVar8);
                    }
                    (&DAT_00bcd652)[(uint32_t)CITY_MUL32(iVar8,0xc2)] = (uint16_t)((int16_t)local_38);
                    *(volatile int16_t *)(&DAT_00799a8e + local_38 * 0x108) = (int16_t)((int16_t)iVar8);
                    *(volatile uint16_t *)(&DAT_00799a8c + iVar22) = (uint16_t)(0);
                  }
                }
              }
            }
          }
        }
        break;
      case 0x4b:
      case 0xc0:
        if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(local_44,0x84)] < 1) {
          *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1) = (uint8_t)(2);
        }
        iVar8 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                             (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
        if (iVar8 != 0) {
          if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 0x33) {
            restore_city_ensure_labor_seeker(iVar11);
          }
          iVar11 = FUN_004cde60((int32_t)(int16_t)(&DAT_00799a18)[(uint32_t)CITY_MUL32(iVar11,0x84)],
                                *(volatile uint32_t *)
                                 (&DAT_00705eb8 + (int16_t)(&DAT_007999e8)[(uint32_t)CITY_MUL32(iVar11,0x84)] * 0x28));
          if (iVar11 < 1) {
            local_38 = 0;
          }
          else if (iVar11 < 0x32) {
            local_38 = 1;
          }
          else {
            local_38 = (0x4a < iVar11) + 2;
          }
          psVar14 = (volatile int16_t *)((uint32_t)(uintptr_t)&DAT_00799a84 + iVar22 + 2);
          iVar8 = 0;
          iVar11 = 3;
          psVar23 = psVar14;
          do {
            iVar9 = (int32_t)*psVar23;
            if (iVar9 != 0) {
              if (((&DAT_00bcd602)[(uint32_t)CITY_MUL32(iVar9,0x184)] == '&') &&
                 ((int16_t)(&DAT_00bcd652)[(uint32_t)CITY_MUL32(iVar9,0xc2)] == local_44)) {
                iVar8 = CITY_ADD32(iVar8,1);
              }
              else {
                *psVar23 = (int16_t)(0);
              }
            }
            psVar23 = psVar23 + 1;
            iVar11 = CITY_ADD32(iVar11,-1);
          } while (iVar11 != 0);
          if (local_38 < iVar8) {
            iVar11 = 2;
            psVar14 = (volatile int16_t *)(&DAT_00799a8a + iVar22);
            do {
              if (*psVar14 != 0) {
                (&DAT_00bcd606)[*psVar14 * 0x184] = (uint8_t)(2);
                break;
              }
              iVar11 = CITY_ADD32(iVar11,-1);
              psVar14 = psVar14 + -1;
            } while (-1 < iVar11);
          }
          else if (iVar8 < local_38) {
            iVar11 = FUN_004b4800((&DAT_007999d9)[(uint32_t)iVar22],0x26,_DAT_00d3b4e4,_DAT_00d3b4e0,4);
            (&DAT_00bcd630)[(uint32_t)CITY_MUL32(iVar11,0x184)] = (uint8_t)(8);
            (&DAT_00bcd652)[(uint32_t)CITY_MUL32(iVar11,0xc2)] = (uint16_t)((int16_t)local_44);
            iVar22 = 0;
            do {
              if (*psVar14 == 0) {
                *(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_00799a84 + (iVar22 + CITY_MUL32(local_44,0x84)) * 2 + 2) = (int16_t)((int16_t)iVar11);
                break;
              }
              iVar22 = CITY_ADD32(iVar22,1);
              psVar14 = psVar14 + 1;
            } while (iVar22 < 3);
          }
        }
        break;
      case 0x4c:
        if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(local_44,0x84)] < 1) {
          *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1) = (uint8_t)(2);
        }
        if ((*(volatile int16_t *)(&DAT_00799a8c + iVar22) != 0) &&
           ((iVar8 = *(volatile int16_t *)(&DAT_00799a8c + iVar22) * 0x184, (&DAT_00bcd606)[(uint32_t)iVar8] != '\x01' ||
            ((&DAT_00bcd602)[(uint32_t)iVar8] != '\x19')))) {
          *(volatile uint16_t *)(&DAT_00799a8c + iVar22) = (uint16_t)(0);
        }
        iVar8 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                             (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
        if (iVar8 != 0) {
          if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 0x33) {
            restore_city_ensure_labor_seeker(iVar11);
          }
          sVar21 = (int16_t)((&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)]);
          if (0 < sVar21) {
            iVar8 = sVar21 * 0x184;
            if (((&DAT_00bcd606)[(uint32_t)iVar8] != '\0') &&
               ((((&DAT_00bcd602)[(uint32_t)iVar8] == '\x04' || ((&DAT_00bcd602)[(uint32_t)iVar8] == '\0')) &&
                ((int16_t)(&DAT_00bcd652)[sVar21 * 0xc2] == iVar11)))) break;
            (&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)(0);
          }
          uVar13 = _DAT_00d3b4e4;
          if ((&DAT_00799a0a)[(uint32_t)iVar22] != '\0') {
            iVar9 = CITY_MUL32(DAT_00eaac28,0x49d8);
            iVar8 = *(volatile int32_t *)(&DAT_00ea5ff0 + iVar9);
            (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(0);
            (&DAT_00799a3c)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)(0);
            *(volatile int32_t *)(&DAT_00ea5ff0 + iVar9) = CITY_ADD32(iVar8,100);
            uVar31 = _DAT_00d3b4e0;
            (&DAT_00799a1b)[(uint32_t)iVar22] = (uint8_t)(7);
            iVar8 = FUN_004b4800((&DAT_007999d9)[(uint32_t)iVar22],4,uVar13,uVar31,4);
            iVar22 = DAT_00eaac28;
            iVar9 = CITY_MUL32(iVar8,0x184);
            sVar21 = (int16_t)((&DAT_00ea5ce8)[(uint32_t)CITY_MUL32(DAT_00eaac28,0x24ec)]);
            (&DAT_00bcd630)[(uint32_t)iVar9] = (uint8_t)(8);
            *(volatile uint16_t *)(&DAT_00bcd66c + iVar9) = (uint16_t)(100);
            if (((sVar21 != 0) &&
                (iVar22 = FUN_004c5120((&DAT_00ea5f9c)[(uint32_t)CITY_MUL32(iVar22,0x1276)]), iVar22 == 0)) &&
               (cVar2 = (int8_t)(FUN_004c52d0((&DAT_00ea5f9c)[(uint32_t)CITY_MUL32(DAT_00eaac28,0x1276)],2)), cVar2 != '\0')) {
              *(volatile int16_t *)(&DAT_00bcd66c + iVar9) = (int16_t)(*(volatile int16_t *)(&DAT_00bcd66c + iVar9) << 1);
            }
            (&DAT_00bcd603)[(uint32_t)iVar9] = (uint8_t)(7);
            (&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)((int16_t)iVar8);
            (&DAT_00bcd652)[(uint32_t)CITY_MUL32(iVar8,0xc2)] = (uint16_t)((uint16_t)local_44);
            *(volatile uint16_t *)(&DAT_00bcd62e + iVar9) = (uint16_t)(0x1e);
          }
        }
        break;
      case 0x50:
        sVar21 = (int16_t)((&DAT_00799a02)[(uint32_t)CITY_MUL32(local_44,0x84)]);
        if (0 < sVar21) {
          iVar8 = sVar21 * 0x184;
          if ((((&DAT_00bcd606)[(uint32_t)iVar8] != '\0') &&
              (((&DAT_00bcd602)[(uint32_t)iVar8] == '@' || ((&DAT_00bcd602)[(uint32_t)iVar8] == '\0')))) &&
             ((int16_t)(&DAT_00bcd652)[sVar21 * 0xc2] == local_44)) break;
          (&DAT_00799a02)[(uint32_t)CITY_MUL32(local_44,0x84)] = (uint16_t)(0);
        }
        iVar8 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                             (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
        if ((iVar8 != 0) && (0 < *(volatile int32_t *)(&DAT_00ea1894 + CITY_MUL32(DAT_00eaac28,0x49d8)))) {
          cVar2 = (int8_t)((&DAT_00799a0a)[(uint32_t)iVar22]);
          (&DAT_00ea5e0c)[(uint32_t)CITY_MUL32(DAT_00eaac28,0x1276)] = 1;
          (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(cVar2 + 1U);
          uVar31 = _DAT_00d3b4e4;
          uVar13 = _DAT_00d3b4e0;
          if (1 < (uint8_t)(cVar2 + 1U)) {
            (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(0);
            uVar30 = 0x40;
            goto LAB_00466268;
          }
        }
        break;
      case 0x51:
        if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(local_44,0x84)] < 1) {
          *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1) = (uint8_t)(2);
        }
        sVar21 = (int16_t)((&DAT_00799a02)[(uint32_t)CITY_MUL32(local_44,0x84)]);
        if (0 < sVar21) {
          iVar8 = sVar21 * 0x184;
          if ((((&DAT_00bcd606)[(uint32_t)iVar8] != '\0') &&
              (((&DAT_00bcd602)[(uint32_t)iVar8] == '\b' || ((&DAT_00bcd602)[(uint32_t)iVar8] == '\0')))) &&
             ((int16_t)(&DAT_00bcd652)[sVar21 * 0xc2] == local_44)) break;
          (&DAT_00799a02)[(uint32_t)CITY_MUL32(local_44,0x84)] = (uint16_t)(0);
        }
        iVar8 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                             (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
        if (iVar8 != 0) {
          if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 0x65) {
            restore_city_ensure_labor_seeker(iVar11);
          }
          iVar8 = FUN_004cde60((int32_t)(int16_t)(&DAT_00799a18)[(uint32_t)CITY_MUL32(iVar11,0x84)],
                               *(volatile uint32_t *)
                                (&DAT_00705eb8 + (int16_t)(&DAT_007999e8)[(uint32_t)CITY_MUL32(iVar11,0x84)] * 0x28));
          if (iVar8 < 100) {
            if (iVar8 < 0x4b) {
              if (iVar8 < 0x32) {
                if (iVar8 < 0x19) {
                  if (iVar8 < 1) break;
                  bVar20 = (uint8_t)(0xf);
                }
                else {
                  bVar20 = (uint8_t)(7);
                }
              }
              else {
                bVar20 = (uint8_t)(3);
              }
            }
            else {
              bVar20 = (uint8_t)(1);
            }
          }
          else {
            bVar20 = (uint8_t)(0);
          }
          cVar2 = (int8_t)((&DAT_00799a0a)[(uint32_t)iVar22]);
          (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(cVar2 + 1U);
          uVar31 = _DAT_00d3b4e4;
          uVar13 = _DAT_00d3b4e0;
          if (bVar20 < (uint8_t)(cVar2 + 1U)) {
            (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(0);
            uVar30 = 8;
LAB_00463bf3:
            uVar3 = (uint8_t)((&DAT_007999d9)[(uint32_t)iVar22]);
            goto LAB_00463bfc;
          }
        }
        break;
      case 0x54:
      case 0x55:
      case 0x56:
      case 0x57:
      case 0xbb:
      case 0xbc:
      case 0xbd:
        if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(local_44,0x84)] < 1) {
          *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1) = (uint8_t)(2);
        }
        sVar21 = (int16_t)((&DAT_00799a02)[(uint32_t)CITY_MUL32(local_44,0x84)]);
        if (0 < sVar21) {
          iVar8 = sVar21 * 0x184;
          if ((((&DAT_00bcd606)[(uint32_t)iVar8] != '\0') &&
              (((&DAT_00bcd602)[(uint32_t)iVar8] == '\a' || ((&DAT_00bcd602)[(uint32_t)iVar8] == '\0')))) &&
             ((int16_t)(&DAT_00bcd652)[sVar21 * 0xc2] == local_44)) break;
          (&DAT_00799a02)[(uint32_t)CITY_MUL32(local_44,0x84)] = (uint16_t)(0);
        }
        iVar8 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                             (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
        if (iVar8 != 0) {
          if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 0x33) {
            restore_city_ensure_labor_seeker(iVar11);
          }
          iVar8 = FUN_004cde60((int32_t)(int16_t)(&DAT_00799a18)[(uint32_t)CITY_MUL32(iVar11,0x84)],
                               *(volatile uint32_t *)
                                (&DAT_00705eb8 + (int16_t)(&DAT_007999e8)[(uint32_t)CITY_MUL32(iVar11,0x84)] * 0x28));
          if (iVar8 < 100) {
            if (iVar8 < 0x4b) {
              if (iVar8 < 0x32) {
                if (iVar8 < 0x19) {
                  if (iVar8 < 1) break;
                  bVar20 = (uint8_t)(0xf);
                }
                else {
                  bVar20 = (uint8_t)(7);
                }
              }
              else {
                bVar20 = (uint8_t)(3);
              }
            }
            else {
              bVar20 = (uint8_t)(1);
            }
          }
          else {
            bVar20 = (uint8_t)(0);
          }
          iVar8 = FUN_004cde60((int32_t)(int16_t)(&DAT_00799a18)
                                           [*(volatile int32_t *)(&DAT_00ea4000 + (uint32_t)CITY_MUL32(DAT_00eaac28,0x49d8)) * 0x84],
                               *(volatile uint32_t *)
                                (&DAT_00705eb8 +
                                (int16_t)(&DAT_007999e8)
                                       [*(volatile int32_t *)(&DAT_00ea4000 + (uint32_t)CITY_MUL32(DAT_00eaac28,0x49d8)) * 0x84] *
                                0x28));
          cVar2 = (int8_t)((&DAT_00799a0a)[(uint32_t)iVar22]);
          (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(cVar2 + 1U);
          if (bVar20 < (uint8_t)(cVar2 + 1U)) {
            (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(0);
            if ((((&DAT_007999e8)[(uint32_t)CITY_MUL32(iVar11,0x84)] == 0x56) ||
                ((&DAT_007999e8)[(uint32_t)CITY_MUL32(iVar11,0x84)] == 0x57)) && (0 < iVar8)) {
              uVar3 = (uint8_t)((&DAT_007999d9)[(uint32_t)iVar22]);
              uVar30 = 7;
              uVar31 = _DAT_00d3b4e4;
              uVar13 = _DAT_00d3b4e0;
              goto LAB_00463bfc;
            }
          }
        }
        break;
      case 0x58:
        (&DAT_00b7482c)[*(volatile int32_t *)(&DAT_007999e4 + (uint32_t)iVar22)] =
             ((uint8_t)(&DAT_00acf85c)[*(volatile int32_t *)(&DAT_007999e4 + (uint32_t)iVar22)] & 1) + DAT_00f569d4;
        sVar21 = (int16_t)((&DAT_00799a02)[(uint32_t)CITY_MUL32(local_44,0x84)]);
        if (0 < sVar21) {
          iVar8 = sVar21 * 0x184;
          if (((&DAT_00bcd606)[(uint32_t)iVar8] != '\0') &&
             ((((&DAT_00bcd602)[(uint32_t)iVar8] == ')' || ((&DAT_00bcd602)[(uint32_t)iVar8] == '\0')) &&
              ((int16_t)(&DAT_00bcd652)[sVar21 * 0xc2] == local_44)))) break;
          (&DAT_00799a02)[(uint32_t)CITY_MUL32(local_44,0x84)] = (uint16_t)(0);
        }
        if (((0 < (int16_t)(&DAT_007999ea)[(uint32_t)CITY_MUL32(local_44,0x84)]) &&
            (iVar8 = FUN_004807d0((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                                  (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]),
            iVar8 != 0)) &&
           (cVar2 = (int8_t)((&DAT_00799a0a)[(uint32_t)iVar22]), (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(cVar2 + 1U),
           uVar31 = _DAT_00d3b4e4, uVar13 = _DAT_00d3b4e0, 4 < (uint8_t)(cVar2 + 1U))) {
          (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(0);
          uVar3 = (uint8_t)((&DAT_007999d9)[(uint32_t)iVar22]);
          uVar30 = 0x29;
LAB_00466e15:
          iVar22 = FUN_004b4800(uVar3,uVar30,uVar31,uVar13,0);
          (&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)((int16_t)iVar22);
          (&DAT_00bcd630)[(uint32_t)CITY_MUL32(iVar22,0x184)] = (uint8_t)(10);
          (&DAT_00bcd652)[(uint32_t)CITY_MUL32(iVar22,0xc2)] = (uint16_t)(uVar4);
          local_44 = iVar11;
        }
        break;
      case 0x59:
        FUN_0047bf50((uint16_t)local_44,(int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                     (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),2,2,CITY_ADD32(DAT_00f569d4,2),8);
        if (0 < (int32_t)(&DAT_00ea5e0c)[(uint32_t)CITY_MUL32(DAT_00eaac28,0x1276)]) {
          sVar21 = (int16_t)((&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)]);
          if (0 < sVar21) {
            iVar8 = sVar21 * 0x184;
            if ((((&DAT_00bcd606)[(uint32_t)iVar8] != '\0') &&
                (((&DAT_00bcd602)[(uint32_t)iVar8] == ':' || ((&DAT_00bcd602)[(uint32_t)iVar8] == '\0')))) &&
               ((int16_t)(&DAT_00bcd652)[sVar21 * 0xc2] == iVar11)) break;
            (&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)(0);
          }
          iVar8 = FUN_004807d0((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                               (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
          if ((iVar8 != 0) &&
             (cVar2 = (int8_t)((&DAT_00799a0a)[(uint32_t)iVar22]), (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(cVar2 + 1U),
             uVar31 = _DAT_00d3b4e4, uVar13 = _DAT_00d3b4e0, 8 < (uint8_t)(cVar2 + 1U))) {
            (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(0);
            uVar3 = (uint8_t)((&DAT_007999d9)[(uint32_t)iVar22]);
            uVar30 = 0x3a;
            goto LAB_00466e15;
          }
        }
        break;
      case 0x5a:
      case 0x5e:
      case 0xb9:
      case 0xba:
        if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(local_44,0x84)] < 1) {
          *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1) = (uint8_t)(2);
        }
        iVar22 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                              (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
        if ((iVar22 != 0) && ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 0x65)) {
          restore_city_ensure_labor_seeker(iVar11);
          local_44 = iVar11;
        }
        break;
      case 0x5d:
        FUN_0047c260(local_44);
        local_44 = iVar11;
        break;
      case 0x5f:
        if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(local_44,0x84)] < 1) {
          *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1) = (uint8_t)(2);
        }
        iVar8 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                             (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
        if (iVar8 != 0) {
          if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 0x65) {
            restore_city_ensure_labor_seeker(iVar11);
          }
          iVar8 = FUN_004cde60((int32_t)(int16_t)(&DAT_00799a18)[(uint32_t)CITY_MUL32(iVar11,0x84)],
                               *(volatile uint32_t *)
                                (&DAT_00705eb8 + (int16_t)(&DAT_007999e8)[(uint32_t)CITY_MUL32(iVar11,0x84)] * 0x28));
          if (iVar8 < 100) {
            if (iVar8 < 0x4b) {
              if (iVar8 < 0x32) {
                if (iVar8 < 0x19) {
                  if (iVar8 < 1) break;
                  bVar20 = (uint8_t)(0x30);
                }
                else {
                  bVar20 = (uint8_t)(0x20);
                }
              }
              else {
                bVar20 = (uint8_t)(0x10);
              }
            }
            else {
              bVar20 = (uint8_t)(0xc);
            }
          }
          else {
            bVar20 = (uint8_t)(8);
          }
          cVar2 = (int8_t)((&DAT_00799a0a)[(uint32_t)iVar22]);
          (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(cVar2 + 1U);
          if (bVar20 < (uint8_t)(cVar2 + 1U)) {
            (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(0);
            FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                         (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
            iVar22 = FUN_004b7400(iVar11);
            if (iVar22 == 0) {
              FUN_004b70c0(iVar11);
              local_44 = iVar11;
            }
          }
        }
        break;
      case 100:
      case 0x65:
      case 0x66:
      case 0x67:
      case 0x68:
      case 0x69:
      case 0x6a:
      case 0x6b:
      case 0x6d:
      case 0x6e:
      case 0x6f:
      case 0x70:
      case 0x71:
      case 0x72:
      case 0xa1:
      case 0xa2:
      case 0xc2:
      case 0xc4:
      case 0xcb:
      case 0xcc:
      case 0xcd:
      case 0xd8:
      case 0xd9:
      case 0xdd:
      case 0xe0:
      case 0xe8:
      case 0xe9:
        if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(local_44,0x84)] < 1) {
          *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1) = (uint8_t)(2);
        }
        iVar8 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                             (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
        if (iVar8 != 0) {
          if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 0x33) {
            restore_city_ensure_labor_seeker(iVar11);
          }
          sVar21 = (int16_t)((&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)]);
          if (0 < sVar21) {
            iVar8 = sVar21 * 0x184;
            if ((((&DAT_00bcd606)[(uint32_t)iVar8] != '\0') &&
                (((&DAT_00bcd602)[(uint32_t)iVar8] == '\x04' || ((&DAT_00bcd602)[(uint32_t)iVar8] == '\0')))) &&
               ((int16_t)(&DAT_00bcd652)[sVar21 * 0xc2] == iVar11)) break;
            (&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)(0);
          }
          cVar2 = (int8_t)(FUN_00488030(iVar11));
          if (cVar2 == '\0') {
            iVar8 = FUN_00455bf0(iVar11);
            if (iVar8 != 0) {
              iVar8 = 100;
              goto LAB_00462838;
            }
          }
          else {
            iVar8 = FUN_00488400((void *)(uintptr_t)0x00bca350u,iVar11);
            if (iVar8 != 0) {
              local_47 = (int8_t)('\x01');
LAB_00462838:
              restore_city_reset_dispatch_reservations(iVar11);
              iVar9 = FUN_004b4800((&DAT_007999d9)[(uint32_t)iVar22],4,_DAT_00d3b4e4,_DAT_00d3b4e0,4);
              uVar3 = (uint8_t)((&DAT_00799a1b)[(uint32_t)iVar22]);
              (&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)((int16_t)iVar9);
              iVar15 = CITY_MUL32(iVar9,0x184);
              (&DAT_00bcd630)[(uint32_t)iVar15] = (uint8_t)(8);
              *(volatile int16_t *)(&DAT_00bcd66c + iVar15) = (int16_t)((int16_t)iVar8);
              (&DAT_00bcd603)[(uint32_t)iVar15] = (uint8_t)(uVar3);
              (&DAT_00bcd652)[(uint32_t)CITY_MUL32(iVar9,0xc2)] = (uint16_t)(uVar4);
              *(volatile uint16_t *)(&DAT_00bcd62e + iVar15) = (uint16_t)(0x1e);
              if (((local_47 != '\0') && ((&DAT_007999e8)[(uint32_t)CITY_MUL32(iVar11,0x84)] == 0x66)) &&
                 (*(volatile int16_t *)(&DAT_00ea4138 + CITY_MUL32(DAT_00eaac28,0x1276)) == 0)) {
                iVar8 = FUN_004b4800((&DAT_007999d9)[(uint32_t)iVar22],4,_DAT_00d3b4e4,_DAT_00d3b4e0,4);
                *(volatile int16_t *)(&DAT_00799a8e + iVar22) = (int16_t)((int16_t)iVar8);
                iVar22 = CITY_MUL32(iVar8,0x184);
                (&DAT_00bcd630)[(uint32_t)iVar22] = (uint8_t)(8);
                *(volatile uint16_t *)(&DAT_00bcd66c + iVar22) = (uint16_t)(100);
                (&DAT_00bcd603)[(uint32_t)iVar22] = (uint8_t)(9);
                (&DAT_00bcd652)[(uint32_t)CITY_MUL32(iVar8,0xc2)] = (uint16_t)(uVar4);
                *(volatile uint16_t *)(&DAT_00bcd62e + iVar22) = (uint16_t)(0x1e);
                local_44 = iVar11;
              }
            }
          }
        }
        break;
      case 0x6c:
        if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(local_44,0x84)] < 1) {
          *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1) = (uint8_t)(2);
        }
        iVar8 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                             (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
        if (iVar8 != 0) {
          if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 0x33) {
            restore_city_ensure_labor_seeker(iVar11);
          }
          iVar8 = FUN_00459240(iVar11);
          if (iVar8 == 0) {
            iVar8 = FUN_004cde60((int32_t)(int16_t)(&DAT_00799a18)[(uint32_t)CITY_MUL32(iVar11,0x84)],
                                 *(volatile uint32_t *)
                                  (&DAT_00705eb8 + (int16_t)(&DAT_007999e8)[(uint32_t)CITY_MUL32(iVar11,0x84)] * 0x28));
            if (iVar8 < 100) {
              if (iVar8 < 0x4b) {
                if (iVar8 < 0x32) {
                  if ((iVar8 < 0x19) && (iVar8 < 1)) break;
                  iVar8 = 1;
                }
                else {
                  iVar8 = 2;
                }
              }
              else {
                iVar8 = 2;
              }
            }
            else {
              iVar8 = 3;
            }
            sVar21 = (int16_t)((&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)]);
            local_38 = iVar8;
            if (sVar21 < 1) {
LAB_00462a31:
              if (99 < (int16_t)(&DAT_00799a14)[(uint32_t)CITY_MUL32(iVar11,0x84)]) {
                iVar9 = FUN_004b4800((&DAT_007999d9)[(uint32_t)iVar22],4,_DAT_00d3b4e4,_DAT_00d3b4e0,4);
                (&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)((int16_t)iVar9);
                iVar15 = CITY_MUL32(iVar9,0x184);
                (&DAT_00799a14)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)((&DAT_00799a14)[(uint32_t)CITY_MUL32(iVar11,0x84)] + -100);
                (&DAT_00799a1b)[(uint32_t)iVar22] = (uint8_t)(0x14);
                (&DAT_00bcd630)[(uint32_t)iVar15] = (uint8_t)(8);
                (&DAT_00bcd603)[(uint32_t)iVar15] = (uint8_t)(0x14);
                (&DAT_00bcd652)[(uint32_t)CITY_MUL32(iVar9,0xc2)] = (uint16_t)(uVar4);
                *(volatile uint16_t *)(&DAT_00bcd62e + iVar15) = (uint16_t)(0x1e);
                *(volatile uint16_t *)(&DAT_00bcd66c + iVar15) = (uint16_t)(100);
              }
            }
            else {
              iVar9 = sVar21 * 0x184;
              if (((&DAT_00bcd606)[(uint32_t)iVar9] == '\0') ||
                 ((((&DAT_00bcd602)[(uint32_t)iVar9] != '\x04' && ((&DAT_00bcd602)[(uint32_t)iVar9] != '\0')) ||
                  ((int16_t)(&DAT_00bcd652)[sVar21 * 0xc2] != iVar11)))) {
                (&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)(0);
                goto LAB_00462a31;
              }
            }
            if (500 - (int16_t)(&DAT_00799a14)[(uint32_t)CITY_MUL32(iVar11,0x84)] < CITY_MUL32(iVar8,0x19)) {
              iVar8 = (500 - (int16_t)(&DAT_00799a14)[(uint32_t)CITY_MUL32(iVar11,0x84)]) / 0x19;
              local_38 = iVar8;
            }
            if (5 < *(volatile uint8_t *)(&DAT_00799a7e + CITY_MUL32(iVar11,0x84))) {
              uVar3 = (uint8_t)(FUN_00462140(iVar11,0x4b));
              *(volatile uint8_t *)(&DAT_00799a7e + CITY_MUL32(iVar11,0x84)) = (uint8_t)(uVar3);
            }
            if ((((int32_t)(uint32_t)*(volatile uint8_t *)(&DAT_00799a7e + CITY_MUL32(iVar11,0x84)) < iVar8) &&
                (DAT_00978d98 != '\0')) &&
               ((&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(0),
               (int32_t)(uint32_t)*(volatile uint8_t *)(&DAT_00799a7e + CITY_MUL32(iVar11,0x84)) < iVar8)) {
              do {
                iVar8 = FUN_004b4800((&DAT_007999d9)[(uint32_t)iVar22],0x4b,_DAT_00d3b4e4,_DAT_00d3b4e0,0);
                (&DAT_00bcd630)[(uint32_t)CITY_MUL32(iVar8,0x184)] = (uint8_t)(8);
                (&DAT_00bcd652)[(uint32_t)CITY_MUL32(iVar8,0xc2)] = (uint16_t)((uint16_t)local_44);
                (&DAT_00d3b1c8)[(uint32_t)DAT_00d3b1c4] = DAT_00d3b374;
                DAT_00d3b1c4 = CITY_ADD32(DAT_00d3b1c4,1);
                if (99 < DAT_00d3b1c4) {
                  DAT_00d3b1c4 = 0;
                }
                local_40 = 0x1f;
                do {
                  uVar10 = DAT_00d3b384 >> 4;
                  uVar26 = DAT_00d3b384 & 1;
                  uVar27 = DAT_00d3b380 >> 4;
                  uVar24 = DAT_00d3b380 & 1;
                  DAT_00d3b384 = DAT_00d3b384 >> 1;
                  DAT_00d3b380 = DAT_00d3b380 >> 1;
                  if ((uVar10 & 1) != uVar26) {
                    DAT_00d3b384 = DAT_00d3b384 | 0x40000000;
                  }
                  if ((uVar27 & 1) != uVar24) {
                    DAT_00d3b380 = DAT_00d3b380 | 0x40000000;
                  }
                  local_40 = CITY_ADD32(local_40,-1);
                } while (local_40 != 0);
                _DAT_00d3b378 = DAT_00d3b384 & 7;
                _DAT_00d3b364 = DAT_00d3b380 & 0x7fff;
                _DAT_00d3b370 = DAT_00d3b384 & 0x7fff;
                _DAT_00d3b368 = DAT_00d3b380 & 0x7f;
                _DAT_00d3b36c = DAT_00d3b380 & 7;
                cVar2 = (int8_t)(*(volatile int8_t *)(&DAT_00799a7e + CITY_MUL32(iVar11,0x84)));
                uVar1 = (uint64_t)_DAT_00d3b370;
                DAT_00d3b374 = DAT_00d3b384 & 0x7f;
                *(volatile uint8_t *)(&DAT_00799a7e + CITY_MUL32(iVar11,0x84)) = (uint8_t)(cVar2 + 1U);
                *(volatile int16_t *)(&DAT_00bcd62e + CITY_MUL32(iVar8,0x184)) = (int16_t)((int16_t)(uVar1 % 0x1e));
                local_40 = 0;
              } while ((int32_t)(uint32_t)(uint8_t)(cVar2 + 1U) < local_38);
            }
          }
        }
        break;
      case 0x73:
        if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(local_44,0x84)] < 1) {
          *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1) = (uint8_t)(2);
        }
        iVar8 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                             (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
        if (iVar8 != 0) {
          if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 0x65) {
            restore_city_ensure_labor_seeker(iVar11);
          }
          iVar8 = FUN_004cde60((int32_t)(int16_t)(&DAT_00799a18)[(uint32_t)CITY_MUL32(iVar11,0x84)],
                               *(volatile uint32_t *)
                                (&DAT_00705eb8 + (int16_t)(&DAT_007999e8)[(uint32_t)CITY_MUL32(iVar11,0x84)] * 0x28));
          if (iVar8 < 100) {
            if (iVar8 < 0x4b) {
              if (iVar8 < 0x32) {
                if (iVar8 < 0x19) {
                  if (iVar8 < 1) break;
                  iVar8 = 0;
                }
                else {
                  iVar8 = 1;
                }
              }
              else {
                iVar8 = 1;
              }
            }
            else {
              iVar8 = 2;
            }
          }
          else {
            iVar8 = 3;
          }
          sVar21 = (int16_t)((&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)]);
          local_38 = iVar8;
          if (sVar21 < 1) {
LAB_004651c3:
            if (0 < (int16_t)(&DAT_00799a14)[(uint32_t)CITY_MUL32(iVar11,0x84)]) {
              iVar16 = FUN_004b4800((&DAT_007999d9)[(uint32_t)iVar22],4,_DAT_00d3b4e4,_DAT_00d3b4e0,4);
              iVar15 = DAT_00eaac28;
              iVar19 = CITY_MUL32(DAT_00eaac28,0x49d8);
              iVar9 = *(volatile int32_t *)(&DAT_00ea5fe4 + iVar19);
              iVar28 = CITY_MUL32(iVar16,0x184);
              (&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)((int16_t)iVar16);
              *(volatile int32_t *)(&DAT_00ea5fe4 + iVar19) = CITY_ADD32(iVar9,100);
              sVar21 = (int16_t)((&DAT_00ea5ce8)[(uint32_t)CITY_MUL32(iVar15,0x24ec)]);
              (&DAT_00bcd630)[(uint32_t)iVar28] = (uint8_t)(8);
              (&DAT_00bcd603)[(uint32_t)iVar28] = (uint8_t)(8);
              (&DAT_00bcd652)[(uint32_t)CITY_MUL32(iVar16,0xc2)] = (uint16_t)((uint16_t)local_44);
              *(volatile uint16_t *)(&DAT_00bcd62e + iVar28) = (uint16_t)(0x1e);
              *(volatile uint16_t *)(&DAT_00bcd66c + iVar28) = (uint16_t)(100);
              if (((sVar21 != 0) &&
                  (iVar9 = FUN_004c5120((&DAT_00ea5f9c)[(uint32_t)CITY_MUL32(iVar15,0x1276)]), iVar9 == 0)) &&
                 (cVar2 = (int8_t)(FUN_004c52d0((&DAT_00ea5f9c)[(uint32_t)CITY_MUL32(DAT_00eaac28,0x1276)],2)), cVar2 != '\0')) {
                *(volatile int16_t *)(&DAT_00bcd66c + iVar28) = (int16_t)(*(volatile int16_t *)(&DAT_00bcd66c + iVar28) << 1);
              }
              (&DAT_00799a14)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)((&DAT_00799a14)[(uint32_t)CITY_MUL32(iVar11,0x84)] + -100);
              (&DAT_00799a1b)[(uint32_t)iVar22] = (uint8_t)(8);
            }
          }
          else {
            iVar9 = sVar21 * 0x184;
            if (((&DAT_00bcd606)[(uint32_t)iVar9] == '\0') ||
               ((((&DAT_00bcd602)[(uint32_t)iVar9] != '\x04' && ((&DAT_00bcd602)[(uint32_t)iVar9] != '\0')) ||
                ((int16_t)(&DAT_00bcd652)[sVar21 * 0xc2] != local_44)))) {
              (&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)(0);
              goto LAB_004651c3;
            }
          }
          if (500 - (int16_t)(&DAT_00799a14)[(uint32_t)CITY_MUL32(iVar11,0x84)] < CITY_MUL32(iVar8,100)) {
            iVar8 = (500 - (int16_t)(&DAT_00799a14)[(uint32_t)CITY_MUL32(iVar11,0x84)]) / 100;
            local_38 = iVar8;
          }
          if (5 < *(volatile uint8_t *)(&DAT_00799a7e + CITY_MUL32(iVar11,0x84))) {
            uVar3 = (uint8_t)(FUN_00462140(local_44,0x49));
            *(volatile uint8_t *)(&DAT_00799a7e + CITY_MUL32(iVar11,0x84)) = (uint8_t)(uVar3);
          }
          if (((int32_t)(uint32_t)*(volatile uint8_t *)(&DAT_00799a7e + CITY_MUL32(iVar11,0x84)) < iVar8) &&
             ((&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(0),
             (int32_t)(uint32_t)*(volatile uint8_t *)(&DAT_00799a7e + CITY_MUL32(iVar11,0x84)) < iVar8)) {
            do {
              iVar8 = FUN_004b4800((&DAT_007999d9)[(uint32_t)iVar22],0x49,_DAT_00d3b4e4,_DAT_00d3b4e0,0);
              (&DAT_00d3b1c8)[(uint32_t)DAT_00d3b1c4] = DAT_00d3b374;
              DAT_00d3b1c4 = CITY_ADD32(DAT_00d3b1c4,1);
              (&DAT_00bcd630)[(uint32_t)CITY_MUL32(iVar8,0x184)] = (uint8_t)(0xe);
              (&DAT_00bcd652)[(uint32_t)CITY_MUL32(iVar8,0xc2)] = (uint16_t)((uint16_t)local_44);
              if (99 < DAT_00d3b1c4) {
                DAT_00d3b1c4 = 0;
              }
              local_40 = 0x1f;
              do {
                uVar10 = DAT_00d3b384 >> 4;
                uVar26 = DAT_00d3b384 & 1;
                uVar27 = DAT_00d3b380 >> 4;
                uVar24 = DAT_00d3b380 & 1;
                DAT_00d3b384 = DAT_00d3b384 >> 1;
                DAT_00d3b380 = DAT_00d3b380 >> 1;
                if ((uVar10 & 1) != uVar26) {
                  DAT_00d3b384 = DAT_00d3b384 | 0x40000000;
                }
                if ((uVar27 & 1) != uVar24) {
                  DAT_00d3b380 = DAT_00d3b380 | 0x40000000;
                }
                local_40 = CITY_ADD32(local_40,-1);
              } while (local_40 != 0);
              _DAT_00d3b378 = DAT_00d3b384 & 7;
              cVar2 = (int8_t)(*(volatile int8_t *)(&DAT_00799a7e + CITY_MUL32(iVar11,0x84)));
              _DAT_00d3b370 = DAT_00d3b384 & 0x7fff;
              _DAT_00d3b364 = DAT_00d3b380 & 0x7fff;
              DAT_00d3b374 = DAT_00d3b384 & 0x7f;
              _DAT_00d3b368 = DAT_00d3b380 & 0x7f;
              _DAT_00d3b36c = DAT_00d3b380 & 7;
              *(volatile uint16_t *)(&DAT_00bcd62e + CITY_MUL32(iVar8,0x184)) = (uint16_t)((uint16_t)DAT_00d3b384 & 0x1f);
              *(volatile uint8_t *)(&DAT_00799a7e + CITY_MUL32(iVar11,0x84)) = (uint8_t)(cVar2 + 1U);
              local_40 = 0;
            } while ((int32_t)(uint32_t)(uint8_t)(cVar2 + 1U) < local_38);
          }
        }
        break;
      case 0x88:
        iVar8 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                             (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
        if (iVar8 != 0) {
          if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 0x33) {
            restore_city_ensure_labor_seeker(iVar11);
          }
          iVar9 = FUN_004cde60((int32_t)(int16_t)(&DAT_00799a18)[(uint32_t)CITY_MUL32(iVar11,0x84)],
                               *(volatile uint32_t *)
                                (&DAT_00705eb8 + (int16_t)(&DAT_007999e8)[(uint32_t)CITY_MUL32(iVar11,0x84)] * 0x28));
          iVar8 = (&DAT_00799a9c)[(uint32_t)CITY_MUL32(iVar11,0x42)];
          FUN_004cde60((int32_t)(int16_t)(&DAT_00799a18)[(uint32_t)CITY_MUL32(iVar8,0x84)],
                       *(volatile uint32_t *)(&DAT_00705eb8 + (int16_t)(&DAT_007999e8)[(uint32_t)CITY_MUL32(iVar8,0x84)] * 0x28))
          ;
          if ((-1 < iVar8) && (iVar9 != 0)) {
            if (iVar9 < 100) {
              if (iVar9 < 0x4b) {
                if (iVar9 < 0x32) {
                  if (iVar9 < 0x19) {
                    if (iVar9 < 1) break;
                    iVar8 = -1;
                  }
                  else {
                    iVar8 = 0x1d;
                  }
                }
                else {
                  iVar8 = 0xf;
                }
              }
              else {
                iVar8 = 7;
              }
            }
            else {
              iVar8 = 3;
            }
            cVar2 = (int8_t)((&DAT_00799a0a)[(uint32_t)iVar22]);
            (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(cVar2 + 1U);
            if (iVar8 < (int32_t)(uint32_t)(uint8_t)(cVar2 + 1U)) {
              (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(0);
              sVar21 = (int16_t)((&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)]);
              if (0 < sVar21) {
                iVar8 = sVar21 * 0x184;
                if ((((&DAT_00bcd606)[(uint32_t)iVar8] != '\0') &&
                    (((&DAT_00bcd602)[(uint32_t)iVar8] == 'L' || ((&DAT_00bcd602)[(uint32_t)iVar8] == '\0')))) &&
                   ((int16_t)(&DAT_00bcd652)[sVar21 * 0xc2] == local_44)) break;
                (&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)(0);
              }
              iVar8 = FUN_004806c0((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                                   (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
              if (iVar8 != 0) {
                iVar22 = FUN_004b4800((&DAT_007999d9)[(uint32_t)iVar22],0x4c,_DAT_00d3b4e4,_DAT_00d3b4e0,0);
                iVar8 = FUN_0046f140();
                (&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)((int16_t)iVar22);
                *(volatile int32_t *)(&DAT_00bcd714 + CITY_MUL32(iVar22,0x184)) = iVar8;
                (&DAT_00bcd630)[(uint32_t)CITY_MUL32(iVar22,0x184)] = (uint8_t)(8);
                puVar12 = (volatile uint32_t *)(uintptr_t)(0x009784dcu + (uint32_t)iVar8 * 44u);
                for (iVar11 = 10; iVar11 != 0; iVar11 = iVar11 + -1) {
                  *puVar12 = 0xffffffff;
                  puVar12 = puVar12 + 1;
                }
                (&DAT_00bcd652)[(uint32_t)CITY_MUL32(iVar22,0xc2)] = (uint16_t)((uint16_t)local_44);
              }
            }
          }
        }
        break;
      case 0xa7:
        if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(local_44,0x84)] < 1) {
          *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1) = (uint8_t)(2);
        }
        sVar21 = (int16_t)((&DAT_00799a02)[(uint32_t)CITY_MUL32(local_44,0x84)]);
        if (0 < sVar21) {
          iVar8 = sVar21 * 0x184;
          if (((&DAT_00bcd606)[(uint32_t)iVar8] != '\0') &&
             ((((&DAT_00bcd602)[(uint32_t)iVar8] == '\n' || ((&DAT_00bcd602)[(uint32_t)iVar8] == '\0')) &&
              ((int16_t)(&DAT_00bcd652)[sVar21 * 0xc2] == local_44)))) break;
          (&DAT_00799a02)[(uint32_t)CITY_MUL32(local_44,0x84)] = (uint16_t)(0);
        }
        iVar8 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                             (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
        if (iVar8 != 0) {
          if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 0x65) {
            restore_city_ensure_labor_seeker(iVar11);
          }
          iVar8 = FUN_004cde60((int32_t)(int16_t)(&DAT_00799a18)[(uint32_t)CITY_MUL32(iVar11,0x84)],
                               *(volatile uint32_t *)
                                (&DAT_00705eb8 + (int16_t)(&DAT_007999e8)[(uint32_t)CITY_MUL32(iVar11,0x84)] * 0x28));
          if (iVar8 < 100) {
            if (iVar8 < 0x4b) {
              if (iVar8 < 0x32) {
                if (iVar8 < 0x19) {
                  if (iVar8 < 1) break;
                  bVar20 = (uint8_t)(0xf);
                }
                else {
                  bVar20 = (uint8_t)(7);
                }
              }
              else {
                bVar20 = (uint8_t)(3);
              }
            }
            else {
              bVar20 = (uint8_t)(1);
            }
          }
          else {
            bVar20 = (uint8_t)(0);
          }
          cVar2 = (int8_t)((&DAT_00799a0a)[(uint32_t)iVar22]);
          (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(cVar2 + 1U);
          uVar31 = _DAT_00d3b4e4;
          uVar13 = _DAT_00d3b4e0;
          if (bVar20 < (uint8_t)(cVar2 + 1U)) {
            (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(0);
            uVar30 = 10;
            goto LAB_00463bf3;
          }
        }
        break;
      case 0xac:
        if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(local_44,0x84)] < 1) {
          *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1) = (uint8_t)(2);
        }
        iVar8 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                             (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
        if (iVar8 != 0) {
          if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 0x33) {
            restore_city_ensure_labor_seeker(iVar11);
          }
          if (0 < (int16_t)(&DAT_00799a18)[(uint32_t)CITY_MUL32(iVar11,0x84)]) {
            if ((*(volatile int16_t *)(&DAT_00799a08 + iVar22) == 0) && ((&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)] != 0))
            {
              iVar8 = FUN_004b4800((&DAT_007999d9)[(uint32_t)iVar22],0x3e,
                                   (int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                                   (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),0);
              *(volatile int16_t *)(&DAT_00799a08 + iVar22) = (int16_t)((int16_t)iVar8);
              (&DAT_00bcd652)[(uint32_t)CITY_MUL32(iVar8,0xc2)] = (uint16_t)(uVar4);
              (&DAT_00bcd630)[(uint32_t)CITY_MUL32(iVar8,0x184)] = (uint8_t)(8);
            }
            sVar21 = (int16_t)((&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)]);
            if (0 < sVar21) {
              iVar22 = sVar21 * 0x184;
              if ((((&DAT_00bcd606)[(uint32_t)iVar22] == '\0') ||
                  (((&DAT_00bcd602)[(uint32_t)iVar22] != '*' && ((&DAT_00bcd602)[(uint32_t)iVar22] != '\0')))) ||
                 ((int16_t)(&DAT_00bcd652)[sVar21 * 0xc2] != iVar11)) {
                (&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)(0);
              }
            }
            if ((int16_t)(&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 1) {
              DAT_00bcb9ac = 2;
              local_44 = iVar11;
            }
          }
        }
        break;
      case 0xad:
        if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(local_44,0x84)] < 1) {
          *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1) = (uint8_t)(2);
        }
        iVar8 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                             (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
        if (iVar8 != 0) {
          if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 0x33) {
            restore_city_ensure_labor_seeker(iVar11);
          }
          if (0 < (int16_t)(&DAT_00799a18)[(uint32_t)CITY_MUL32(iVar11,0x84)]) {
            if ((*(volatile int16_t *)(&DAT_00799a08 + iVar22) == 0) && ((&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)] != 0))
            {
              iVar8 = FUN_004b4800((&DAT_007999d9)[(uint32_t)iVar22],0x3e,
                                   (int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                                   (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),0);
              *(volatile int16_t *)(&DAT_00799a08 + iVar22) = (int16_t)((int16_t)iVar8);
              (&DAT_00bcd652)[(uint32_t)CITY_MUL32(iVar8,0xc2)] = (uint16_t)(uVar4);
              (&DAT_00bcd630)[(uint32_t)CITY_MUL32(iVar8,0x184)] = (uint8_t)(8);
            }
            sVar21 = (int16_t)((&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)]);
            if (0 < sVar21) {
              iVar22 = sVar21 * 0x184;
              if (((&DAT_00bcd606)[(uint32_t)iVar22] == '\0') ||
                 ((((&DAT_00bcd602)[(uint32_t)iVar22] != '*' && ((&DAT_00bcd602)[(uint32_t)iVar22] != '\0')) ||
                  ((int16_t)(&DAT_00bcd652)[sVar21 * 0xc2] != iVar11)))) {
                (&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)(0);
              }
            }
            if ((int16_t)(&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 1) {
              DAT_00bcb9ac = 2;
              local_44 = iVar11;
            }
          }
        }
        break;
      case 0xb1:
        restore_city_update_delayed_stock_worker(local_44);
        local_44 = iVar11;
        break;
      case 0xb2:
        restore_city_update_stock_reset_worker(local_44);
        local_44 = iVar11;
        break;
      case 0xb3:
        restore_city_update_service_worker(local_44);
        local_44 = iVar11;
        break;
      case 0xb4:
        restore_city_refresh_masked_building_image(local_44);
        if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 1) {
          *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1) = (uint8_t)(2);
        }
        sVar21 = (int16_t)((&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)]);
        if (0 < sVar21) {
          iVar8 = sVar21 * 0x184;
          if (((&DAT_00bcd606)[(uint32_t)iVar8] != '\0') &&
             ((((&DAT_00bcd602)[(uint32_t)iVar8] == 'W' || ((&DAT_00bcd602)[(uint32_t)iVar8] == '\0')) &&
              ((int16_t)(&DAT_00bcd652)[sVar21 * 0xc2] == iVar11)))) break;
          (&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)(0);
        }
        iVar8 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                             (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
        if ((iVar8 != 0) && ((&DAT_00799a0d)[(uint32_t)iVar22] != '\0')) {
          if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 0x33) {
            restore_city_ensure_labor_seeker(iVar11);
          }
          iVar8 = FUN_004cde60((int32_t)(int16_t)(&DAT_00799a18)[(uint32_t)CITY_MUL32(iVar11,0x84)],
                               *(volatile uint32_t *)
                                (&DAT_00705eb8 + (int16_t)(&DAT_007999e8)[(uint32_t)CITY_MUL32(iVar11,0x84)] * 0x28));
          if ((int8_t)(&DAT_00ac2d4c)[*(volatile int32_t *)(&DAT_007999e4 + (uint32_t)iVar22)] < '\x1e') {
            iVar8 = iVar8 / 2;
          }
          if (iVar8 < 100) {
            if (iVar8 < 0x4b) {
              if (iVar8 < 0x32) {
                if (iVar8 < 0x19) {
                  if (iVar8 < 1) break;
                  bVar20 = (uint8_t)(0x1d);
                }
                else {
                  bVar20 = (uint8_t)(0xf);
                }
              }
              else {
                bVar20 = (uint8_t)(7);
              }
            }
            else {
              bVar20 = (uint8_t)(3);
            }
          }
          else {
            bVar20 = (uint8_t)(1);
          }
          cVar2 = (int8_t)((&DAT_00799a0a)[(uint32_t)iVar22]);
          (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(cVar2 + 1U);
          uVar31 = _DAT_00d3b4e4;
          uVar13 = _DAT_00d3b4e0;
          if (bVar20 < (uint8_t)(cVar2 + 1U)) {
            (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(0);
            uVar3 = (uint8_t)((&DAT_007999d9)[(uint32_t)iVar22]);
            uVar30 = 0x57;
            goto LAB_00466271;
          }
        }
        break;
      case 0xb5:
        if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(local_44,0x84)] < 1) {
          *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1) = (uint8_t)(2);
        }
        if ((*(volatile int16_t *)(&DAT_00799a8c + iVar22) != 0) &&
           ((iVar8 = *(volatile int16_t *)(&DAT_00799a8c + iVar22) * 0x184, (&DAT_00bcd606)[(uint32_t)iVar8] != '\x01' ||
            ((&DAT_00bcd602)[(uint32_t)iVar8] != 'M')))) {
LAB_004664b0:
          *(volatile uint16_t *)(&DAT_00799a8c + iVar22) = (uint16_t)(0);
        }
        goto LAB_004664b7;
      case 0xb6:
        if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(local_44,0x84)] < 1) {
          *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1) = (uint8_t)(2);
        }
        if ((*(volatile int16_t *)(&DAT_00799a8c + iVar22) != 0) &&
           ((iVar8 = *(volatile int16_t *)(&DAT_00799a8c + iVar22) * 0x184, (&DAT_00bcd606)[(uint32_t)iVar8] != '\x01' ||
            ((&DAT_00bcd602)[(uint32_t)iVar8] != 'N')))) goto LAB_004664b0;
LAB_004664b7:
        iVar22 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                              (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
        if ((iVar22 != 0) && ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 0x33)) {
          restore_city_ensure_labor_seeker(iVar11);
          local_44 = iVar11;
        }
        break;
      case 0xb8:
        if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(local_44,0x84)] < 1) {
          *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1) = (uint8_t)(2);
        }
        sVar21 = (int16_t)((&DAT_00799a02)[(uint32_t)CITY_MUL32(local_44,0x84)]);
        if (0 < sVar21) {
          iVar8 = sVar21 * 0x184;
          if ((((&DAT_00bcd606)[(uint32_t)iVar8] != '\0') &&
              (((&DAT_00bcd602)[(uint32_t)iVar8] == 'Y' || ((&DAT_00bcd602)[(uint32_t)iVar8] == '\0')))) &&
             ((int16_t)(&DAT_00bcd652)[sVar21 * 0xc2] == local_44)) break;
          (&DAT_00799a02)[(uint32_t)CITY_MUL32(local_44,0x84)] = (uint16_t)(0);
        }
        iVar8 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                             (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
        if (iVar8 != 0) {
          if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 0x65) {
            restore_city_ensure_labor_seeker(iVar11);
          }
          iVar8 = FUN_004cde60((int32_t)(int16_t)(&DAT_00799a18)[(uint32_t)CITY_MUL32(iVar11,0x84)],
                               *(volatile uint32_t *)
                                (&DAT_00705eb8 + (int16_t)(&DAT_007999e8)[(uint32_t)CITY_MUL32(iVar11,0x84)] * 0x28));
          if (iVar8 < 100) {
            if (iVar8 < 0x4b) {
              if (iVar8 < 0x32) {
                if (iVar8 < 0x19) {
                  if (iVar8 < 1) break;
                  bVar20 = (uint8_t)(0xf);
                }
                else {
                  bVar20 = (uint8_t)(7);
                }
              }
              else {
                bVar20 = (uint8_t)(3);
              }
            }
            else {
              bVar20 = (uint8_t)(1);
            }
          }
          else {
            bVar20 = (uint8_t)(0);
          }
          cVar2 = (int8_t)((&DAT_00799a0a)[(uint32_t)iVar22]);
          (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(cVar2 + 1U);
          uVar31 = _DAT_00d3b4e4;
          uVar13 = _DAT_00d3b4e0;
          if (bVar20 < (uint8_t)(cVar2 + 1U)) {
            (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(0);
            uVar3 = (uint8_t)((&DAT_007999d9)[(uint32_t)iVar22]);
            uVar30 = 0x59;
            goto LAB_00463bfc;
          }
        }
        break;
      case 0xc3:
        if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(local_44,0x84)] < 1) {
          *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1) = (uint8_t)(2);
        }
        iVar8 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                             (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
        if (iVar8 != 0) {
          if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 0x33) {
            restore_city_ensure_labor_seeker(iVar11);
          }
          iVar8 = FUN_00459240(iVar11);
          if (iVar8 == 0) {
            iVar8 = FUN_004cde60((int32_t)(int16_t)(&DAT_00799a18)[(uint32_t)CITY_MUL32(iVar11,0x84)],
                                 *(volatile uint32_t *)
                                  (&DAT_00705eb8 + (int16_t)(&DAT_007999e8)[(uint32_t)CITY_MUL32(iVar11,0x84)] * 0x28));
            if (iVar8 < 100) {
              if (iVar8 < 0x4b) {
                if (iVar8 < 0x32) {
                  if ((iVar8 < 0x19) && (iVar8 < 1)) break;
                  iVar8 = 1;
                }
                else {
                  iVar8 = 2;
                }
              }
              else {
                iVar8 = 4;
              }
            }
            else {
              iVar8 = 5;
            }
            sVar21 = (int16_t)((&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)]);
            local_38 = iVar8;
            if (sVar21 < 1) {
LAB_00462d66:
              if (0x32 < (int16_t)(&DAT_00799a14)[(uint32_t)CITY_MUL32(iVar11,0x84)]) {
                iVar9 = FUN_004b4800((&DAT_007999d9)[(uint32_t)iVar22],4,_DAT_00d3b4e4,_DAT_00d3b4e0,4);
                (&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)((int16_t)iVar9);
                iVar15 = CITY_MUL32(iVar9,0x184);
                (&DAT_00799a14)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)((&DAT_00799a14)[(uint32_t)CITY_MUL32(iVar11,0x84)] + -100);
                (&DAT_00bcd630)[(uint32_t)iVar15] = (uint8_t)(8);
                (&DAT_00bcd603)[(uint32_t)iVar15] = (uint8_t)(0x16);
                (&DAT_00bcd652)[(uint32_t)CITY_MUL32(iVar9,0xc2)] = (uint16_t)(uVar4);
                *(volatile uint16_t *)(&DAT_00bcd62e + iVar15) = (uint16_t)(0x1e);
                *(volatile uint16_t *)(&DAT_00bcd66c + iVar15) = (uint16_t)(100);
              }
            }
            else {
              iVar9 = sVar21 * 0x184;
              if (((&DAT_00bcd606)[(uint32_t)iVar9] == '\0') ||
                 ((((&DAT_00bcd602)[(uint32_t)iVar9] != '\x04' && ((&DAT_00bcd602)[(uint32_t)iVar9] != '\0')) ||
                  ((int16_t)(&DAT_00bcd652)[sVar21 * 0xc2] != iVar11)))) {
                (&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)(0);
                goto LAB_00462d66;
              }
            }
            iVar9 = 500 - (int16_t)(&DAT_00799a14)[(uint32_t)CITY_MUL32(iVar11,0x84)];
            if (CITY_MUL32(iVar8,0x32) - iVar9 != 0 && iVar9 <= CITY_MUL32(iVar8,0x32)) {
              iVar8 = iVar9 / 0x32;
              local_38 = iVar8;
            }
            if (5 < *(volatile uint8_t *)(&DAT_00799a7e + CITY_MUL32(iVar11,0x84))) {
              uVar3 = (uint8_t)(FUN_00462140(iVar11,0x5a));
              *(volatile uint8_t *)(&DAT_00799a7e + CITY_MUL32(iVar11,0x84)) = (uint8_t)(uVar3);
            }
            cVar2 = (int8_t)(DAT_00978d9a);
            (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(0);
            if ((cVar2 != '\0') && ((int32_t)(uint32_t)*(volatile uint8_t *)(&DAT_00799a7e + CITY_MUL32(iVar11,0x84)) < iVar8)) {
              do {
                iVar8 = FUN_004b4800((&DAT_007999d9)[(uint32_t)iVar22],0x5a,_DAT_00d3b4e4,_DAT_00d3b4e0,0);
                (&DAT_00bcd630)[(uint32_t)CITY_MUL32(iVar8,0x184)] = (uint8_t)(8);
                (&DAT_00bcd652)[(uint32_t)CITY_MUL32(iVar8,0xc2)] = (uint16_t)((uint16_t)local_44);
                (&DAT_00d3b1c8)[(uint32_t)DAT_00d3b1c4] = DAT_00d3b374;
                DAT_00d3b1c4 = CITY_ADD32(DAT_00d3b1c4,1);
                if (99 < DAT_00d3b1c4) {
                  DAT_00d3b1c4 = 0;
                }
                local_40 = 0x1f;
                do {
                  uVar10 = DAT_00d3b384 >> 4;
                  uVar26 = DAT_00d3b384 & 1;
                  uVar27 = DAT_00d3b380 >> 4;
                  uVar24 = DAT_00d3b380 & 1;
                  DAT_00d3b384 = DAT_00d3b384 >> 1;
                  DAT_00d3b380 = DAT_00d3b380 >> 1;
                  if ((uVar10 & 1) != uVar26) {
                    DAT_00d3b384 = DAT_00d3b384 | 0x40000000;
                  }
                  if ((uVar27 & 1) != uVar24) {
                    DAT_00d3b380 = DAT_00d3b380 | 0x40000000;
                  }
                  local_40 = CITY_ADD32(local_40,-1);
                } while (local_40 != 0);
                _DAT_00d3b378 = DAT_00d3b384 & 7;
                _DAT_00d3b364 = DAT_00d3b380 & 0x7fff;
                _DAT_00d3b370 = DAT_00d3b384 & 0x7fff;
                _DAT_00d3b368 = DAT_00d3b380 & 0x7f;
                _DAT_00d3b36c = DAT_00d3b380 & 7;
                cVar2 = (int8_t)(*(volatile int8_t *)(&DAT_00799a7e + CITY_MUL32(iVar11,0x84)));
                uVar1 = (uint64_t)_DAT_00d3b370;
                DAT_00d3b374 = DAT_00d3b384 & 0x7f;
                *(volatile uint8_t *)(&DAT_00799a7e + CITY_MUL32(iVar11,0x84)) = (uint8_t)(cVar2 + 1U);
                *(volatile int16_t *)(&DAT_00bcd62e + CITY_MUL32(iVar8,0x184)) = (int16_t)((int16_t)(uVar1 % 0x1e));
                local_40 = 0;
              } while ((int32_t)(uint32_t)(uint8_t)(cVar2 + 1U) < local_38);
            }
          }
        }
        break;
      case 199:
        restore_city_dispatch_kind85(local_44);
        local_44 = iVar11;
        break;
      case 0xce:
        if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(local_44,0x84)] < 1) {
          *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1) = (uint8_t)(2);
        }
        sVar21 = (int16_t)((&DAT_00799a02)[(uint32_t)CITY_MUL32(local_44,0x84)]);
        if (0 < sVar21) {
          iVar8 = sVar21 * 0x184;
          if ((((&DAT_00bcd606)[(uint32_t)iVar8] != '\0') &&
              (((&DAT_00bcd602)[(uint32_t)iVar8] == ' ' || ((&DAT_00bcd602)[(uint32_t)iVar8] == '\0')))) &&
             ((int16_t)(&DAT_00bcd652)[sVar21 * 0xc2] == local_44)) break;
          (&DAT_00799a02)[(uint32_t)CITY_MUL32(local_44,0x84)] = (uint16_t)(0);
        }
        iVar8 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                             (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
        if (iVar8 != 0) {
          if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 0x33) {
            restore_city_ensure_labor_seeker(iVar11);
          }
          iVar8 = FUN_004cde60((int32_t)(int16_t)(&DAT_00799a18)[(uint32_t)CITY_MUL32(iVar11,0x84)],
                               *(volatile uint32_t *)
                                (&DAT_00705eb8 + (int16_t)(&DAT_007999e8)[(uint32_t)CITY_MUL32(iVar11,0x84)] * 0x28));
          if (iVar8 < 100) {
            if (iVar8 < 0x4b) {
              if (iVar8 < 0x32) {
                if (iVar8 < 0x19) {
                  if (iVar8 < 1) break;
                  bVar20 = (uint8_t)(0x2c);
                }
                else {
                  bVar20 = (uint8_t)(0x1d);
                }
              }
              else {
                bVar20 = (uint8_t)(0xf);
              }
            }
            else {
              bVar20 = (uint8_t)(7);
            }
          }
          else {
            bVar20 = (uint8_t)(3);
          }
          cVar2 = (int8_t)((&DAT_00799a0a)[(uint32_t)iVar22]);
          (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(cVar2 + 1U);
          uVar31 = _DAT_00d3b4e4;
          uVar13 = _DAT_00d3b4e0;
          if (bVar20 < (uint8_t)(cVar2 + 1U)) {
            (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(0);
            uVar30 = 0x20;
            goto LAB_00466268;
          }
        }
        break;
      case 0xe2:
        if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(local_44,0x84)] < 1) {
          *(volatile uint8_t *)((uint32_t)(uintptr_t)&DAT_00799a9a + iVar22 + 1) = (uint8_t)(2);
        }
        sVar21 = (int16_t)((&DAT_00799a02)[(uint32_t)CITY_MUL32(local_44,0x84)]);
        if (0 < sVar21) {
          iVar8 = sVar21 * 0x184;
          if (((&DAT_00bcd606)[(uint32_t)iVar8] != '\0') &&
             ((((&DAT_00bcd602)[(uint32_t)iVar8] == 'i' || ((&DAT_00bcd602)[(uint32_t)iVar8] == '\0')) &&
              ((int16_t)(&DAT_00bcd652)[sVar21 * 0xc2] == local_44)))) break;
          (&DAT_00799a02)[(uint32_t)CITY_MUL32(local_44,0x84)] = (uint16_t)(0);
        }
        iVar8 = FUN_00480180((int32_t)*(volatile int16_t *)((uint32_t)(uintptr_t)&DAT_007999dc + iVar22 + 2),
                             (int32_t)*(volatile int16_t *)(&DAT_007999e0 + iVar22),(&DAT_007999db)[(uint32_t)iVar22]);
        if (iVar8 != 0) {
          if ((int16_t)(&DAT_007999f0)[(uint32_t)CITY_MUL32(iVar11,0x84)] < 0x33) {
            restore_city_ensure_labor_seeker(iVar11);
          }
          iVar8 = FUN_004cde60((int32_t)(int16_t)(&DAT_00799a18)[(uint32_t)CITY_MUL32(iVar11,0x84)],
                               *(volatile uint32_t *)
                                (&DAT_00705eb8 + (int16_t)(&DAT_007999e8)[(uint32_t)CITY_MUL32(iVar11,0x84)] * 0x28));
          if (iVar8 < 100) {
            if (iVar8 < 0x4b) {
              if (iVar8 < 0x32) {
                if (iVar8 < 0x19) {
                  if (iVar8 < 1) break;
                  bVar20 = (uint8_t)(0x50);
                }
                else {
                  bVar20 = (uint8_t)(0x32);
                }
              }
              else {
                bVar20 = (uint8_t)(0x1e);
              }
            }
            else {
              bVar20 = (uint8_t)(0xf);
            }
          }
          else {
            bVar20 = (uint8_t)(7);
          }
          cVar2 = (int8_t)((&DAT_00799a0a)[(uint32_t)iVar22]);
          (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(cVar2 + 1U);
          if (((bVar20 < (uint8_t)(cVar2 + 1U)) &&
              (DAT_00708214 <= (int16_t)(&DAT_00799a14)[(uint32_t)CITY_MUL32(iVar11,0x84)])) &&
             (DAT_00708214 <= *(volatile int16_t *)(&DAT_00799aa4 + CITY_MUL32(iVar11,0x42)))) {
            sVar21 = (int16_t)((int16_t)DAT_00708214);
            (&DAT_00799a14)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)((&DAT_00799a14)[(uint32_t)CITY_MUL32(iVar11,0x84)] - sVar21);
            *(volatile int16_t *)(&DAT_00799aa4 + CITY_MUL32(iVar11,0x42)) =
                 (int16_t)(*(volatile int16_t *)(&DAT_00799aa4 + CITY_MUL32(iVar11,0x42)) - sVar21);
            uVar31 = _DAT_00d3b4e4;
            uVar13 = _DAT_00d3b4e0;
            (&DAT_00799a0a)[(uint32_t)iVar22] = (uint8_t)(0);
            iVar8 = FUN_004b4800((&DAT_007999d9)[(uint32_t)iVar22],0x69,uVar31,uVar13,0);
            (&DAT_00799a02)[(uint32_t)CITY_MUL32(iVar11,0x84)] = (uint16_t)((int16_t)iVar8);
            iVar22 = CITY_MUL32(iVar8,0x184);
            (&DAT_00bcd630)[(uint32_t)iVar22] = (uint8_t)(0xc);
            goto LAB_0046463b;
          }
        }
        break;
      case 0xe7:
        restore_city_update_dual_stock_worker(local_44);
        local_44 = iVar11;
      }
switchD_004623ca_caseD_26:
      local_44 = CITY_ADD32(local_44,1);
    } while (local_44 <= DAT_0078d790);
  }
  pcVar25 = timeGetTime;
  (*pcVar25)();
  if (DAT_00bca348 != '\0') {
    *(volatile uint32_t *)(&DAT_00ea6204 + CITY_MUL32(DAT_00eaac28,0x49d8)) = 0;
  }
  return;
}
