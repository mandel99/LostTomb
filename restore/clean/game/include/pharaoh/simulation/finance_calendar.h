#ifndef PHARAOH_SIMULATION_FINANCE_CALENDAR_H
#define PHARAOH_SIMULATION_FINANCE_CALENDAR_H

#include <stdint.h>
#include "pharaoh/building/type_queries.h"

typedef struct PharaohFinanceLedger {
    int32_t treasury;
    uint32_t worker_count;
    uint32_t wage_rate;
    uint32_t wages_current_year;
    uint32_t wages_previous_year;
    uint32_t wage_units_current_year;
    uint32_t wage_units_previous_year;
    uint32_t trade_income_current_year;
    uint32_t trade_income_previous_year;
    uint32_t trade_expense_current_year;
    uint32_t trade_expense_previous_year;
    uint32_t construction_current_year;
    uint32_t construction_previous_year;
    uint32_t interest_current_year;
    uint32_t interest_previous_year;
    uint32_t salary_value;
    uint32_t salary_enabled;
    uint32_t salary_current_year;
    uint32_t salary_previous_year;
    uint32_t salary_total;
    uint32_t sundries_current_year;
    uint32_t sundries_previous_year;
    uint16_t festival_cost_current_year;
    uint16_t festival_cost_previous_year;
    uint32_t festival_events_current_year;
    uint32_t requests_festivals_current_year;
    uint32_t requests_festivals_previous_year;
} PharaohFinanceLedger;

/* Original addresses 0x0045A2E0 and 0x0045A3F0. */
void pharaoh_finance_pay_monthly_wages(PharaohFinanceLedger *ledger);
void pharaoh_finance_pay_monthly_salary(PharaohFinanceLedger *ledger);

/* Original annual rollover addresses 0x0045A450 through 0x0045A5A0. */
void pharaoh_finance_rollover_wages(PharaohFinanceLedger *ledger);
void pharaoh_finance_rollover_trade(PharaohFinanceLedger *ledger);
void pharaoh_finance_rollover_construction(PharaohFinanceLedger *ledger);
void pharaoh_finance_rollover_interest(PharaohFinanceLedger *ledger);
void pharaoh_finance_rollover_salary(PharaohFinanceLedger *ledger);
void pharaoh_finance_rollover_sundries(PharaohFinanceLedger *ledger);
void pharaoh_finance_rollover_requests_festivals(PharaohFinanceLedger *ledger);

typedef struct PharaohFinanceTotals {
    uint32_t income_components[4];
    uint32_t expense_components[7];
    uint32_t income_previous_year;
    uint32_t expense_previous_year;
    uint32_t projection_base;
    uint32_t income_current_year;
    uint32_t expense_current_year;
    uint32_t net_previous_year;
    uint32_t net_current_year;
    uint32_t projected_balance;
} PharaohFinanceTotals;

void pharaoh_finance_estimate_wages(
    PharaohFinanceLedger *ledger, uint32_t month, uint32_t *projected_wages);

void pharaoh_finance_estimate_interest(PharaohFinanceLedger *ledger,
    const PharaohBuildingTypeRecord *buildings,
    uint16_t special_building_count, int32_t special_building_id);

void pharaoh_finance_estimate_salary(PharaohFinanceLedger *ledger);

void pharaoh_finance_calculate_totals(PharaohFinanceTotals *totals);

void pharaoh_finance_calculate_tribute(int32_t treasury, int32_t population,
    int32_t net_current_year, volatile uint32_t *provisional_tribute);

#endif
