#include "pharaoh/ui/political_actions.h"
void pharaoh_political_answer_request(PharaohPoliticalContext *c) {
  if (c->state->popup_answer == 1)
    c->dispatch_request(c->user_data, c->state->current_request);
}
void pharaoh_political_request_button(PharaohPoliticalContext *c) {
  PharaohPoliticalState *s = c->state;
  const PharaohPoliticalRules *r = c->rules;
  int32_t event_id,
      status = c->request_status(c->user_data, s->button_parameter, &event_id),
      text = 0;
  if (status == r->request_none)
    return;
  s->empire_service_legions = 0;
  if (status == r->no_waterborne_companies)
    text = r->popup_no_waterborne;
  else if (status == r->no_waterborne_in_service)
    text = r->popup_no_waterborne_in_service;
  else if (status == r->no_companies)
    text = r->popup_no_companies;
  else if (status == r->no_companies_in_service)
    text = r->popup_no_companies_in_service;
  else if (status == r->request_short)
    text = r->popup_not_enough;
  if (text) {
    c->show_popup(c->user_data, text, PHARAOH_REQUEST_POPUP_MESSAGE);
    return;
  }
  if (status == r->request_troops) {
    s->current_request = event_id;
    c->show_popup(c->user_data, r->popup_relief_force,
                  PHARAOH_REQUEST_POPUP_RELIEF_FORCE);
    return;
  }
  s->current_request = status - 1;
  c->show_popup(c->user_data, r->popup_dispatch_goods,
                PHARAOH_REQUEST_POPUP_GOODS);
  s->redraw_request = 1;
}
void pharaoh_political_set_salary_rank(PharaohPoliticalContext *c) {
  PharaohPoliticalState *s = c->state;
  int32_t old;
  if (s->mission_continue_months_left > 0)
    return;
  s->salary_rank = s->button_parameter;
  s->salary_amount = s->salary_by_rank[s->salary_rank];
  c->estimate_salary(c->user_data);
  c->update_kingdom_explanation(c->user_data);
  old = s->window_id;
  s->redraw_request = 1;
  s->window_id = old == c->rules->window_set_salary
                     ? c->rules->window_advisors
                     : c->rules->window_building_info;
}
void pharaoh_political_select_gift(PharaohPoliticalContext *c) {
  PharaohPoliticalState *s = c->state;
  int32_t gift = s->button_parameter;
  if (gift >= 1 && gift <= c->rules->gift_sizes &&
      s->gift_cost[gift - 1] > s->personal_savings)
    return;
  s->selected_gift_size = gift - 1;
  s->redraw_request = 1;
}
void pharaoh_political_send_gift(PharaohPoliticalContext *c) {
  PharaohPoliticalState *s = c->state;
  if (s->gift_cost[0] > s->personal_savings)
    return;
  c->send_gift(c->user_data);
  s->window_id = c->rules->window_advisors;
  s->redraw_request = 1;
}
