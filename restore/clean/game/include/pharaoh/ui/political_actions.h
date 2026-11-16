#ifndef PHARAOH_UI_POLITICAL_ACTIONS_H
#define PHARAOH_UI_POLITICAL_ACTIONS_H
#include <stdint.h>
enum PharaohRequestPopupKind {
  PHARAOH_REQUEST_POPUP_MESSAGE,
  PHARAOH_REQUEST_POPUP_RELIEF_FORCE,
  PHARAOH_REQUEST_POPUP_GOODS
};
typedef struct PharaohPoliticalState {
  int32_t popup_answer, current_request, button_parameter,
      empire_service_legions, mission_continue_months_left, salary_rank,
      salary_amount, selected_gift_size, personal_savings, window_id,
      redraw_request;
  const int32_t *salary_by_rank, *gift_cost;
} PharaohPoliticalState;
typedef struct PharaohPoliticalRules {
  int32_t request_none, no_waterborne_companies, no_waterborne_in_service,
      no_companies, no_companies_in_service, request_troops, request_short;
  int32_t popup_no_waterborne, popup_no_waterborne_in_service,
      popup_no_companies, popup_no_companies_in_service, popup_relief_force,
      popup_not_enough, popup_dispatch_goods;
  int32_t gift_sizes, window_set_salary, window_advisors, window_building_info;
} PharaohPoliticalRules;
typedef struct PharaohPoliticalContext {
  PharaohPoliticalState *state;
  const PharaohPoliticalRules *rules;
  int32_t (*request_status)(void *, int32_t, int32_t *);
  void (*dispatch_request)(void *, int32_t);
  void (*show_popup)(void *, int32_t, enum PharaohRequestPopupKind);
  void (*estimate_salary)(void *);
  void (*update_kingdom_explanation)(void *);
  void (*send_gift)(void *);
  void *user_data;
} PharaohPoliticalContext;
void pharaoh_political_answer_request(PharaohPoliticalContext *);
void pharaoh_political_request_button(PharaohPoliticalContext *);
void pharaoh_political_set_salary_rank(PharaohPoliticalContext *);
void pharaoh_political_select_gift(PharaohPoliticalContext *);
void pharaoh_political_send_gift(PharaohPoliticalContext *);
#endif
