#include "pharaoh/simulation/finance_calendar.h"
#include "pharaoh/core/percentage_adjust.h"

static uint32_t wrap_add(uint32_t left, uint32_t right)
{
    return left + right;
}

static uint32_t wrap_sub(uint32_t left, uint32_t right)
{
    return left - right;
}

static void rollover_u32(uint32_t *current, uint32_t *previous)
{
    *previous = *current;
    *current = 0;
}

void pharaoh_finance_pay_monthly_wages(PharaohFinanceLedger *ledger)
{
    uint32_t product = ledger->worker_count * ledger->wage_rate;
    const uint32_t cost = (uint32_t)(((int32_t)product / 10) / 12);

    ledger->treasury = (int32_t)wrap_sub((uint32_t)ledger->treasury, cost);
    ledger->wages_current_year = wrap_add(ledger->wages_current_year, cost);
    ledger->wage_units_current_year =
        wrap_add(ledger->wage_units_current_year, ledger->wage_rate);
}

void pharaoh_finance_pay_monthly_salary(PharaohFinanceLedger *ledger)
{
    uint32_t salary;

    if (ledger->treasury <= -5000 || ledger->salary_enabled == 0u) {
        return;
    }
    salary = ledger->salary_value;
    ledger->salary_current_year =
        wrap_add(ledger->salary_current_year, salary);
    salary = ledger->salary_value;
    ledger->salary_total = wrap_add(ledger->salary_total, salary);
    salary = ledger->salary_value;
    ledger->treasury =
        (int32_t)wrap_sub((uint32_t)ledger->treasury, salary);
}

void pharaoh_finance_rollover_wages(PharaohFinanceLedger *ledger)
{
    rollover_u32(&ledger->wages_current_year, &ledger->wages_previous_year);
    rollover_u32(
        &ledger->wage_units_current_year, &ledger->wage_units_previous_year);
}

void pharaoh_finance_rollover_trade(PharaohFinanceLedger *ledger)
{
    rollover_u32(
        &ledger->trade_income_current_year, &ledger->trade_income_previous_year);
    rollover_u32(
        &ledger->trade_expense_current_year, &ledger->trade_expense_previous_year);
}

void pharaoh_finance_rollover_construction(PharaohFinanceLedger *ledger)
{
    rollover_u32(
        &ledger->construction_current_year, &ledger->construction_previous_year);
}

void pharaoh_finance_rollover_interest(PharaohFinanceLedger *ledger)
{
    rollover_u32(
        &ledger->interest_current_year, &ledger->interest_previous_year);
}

void pharaoh_finance_rollover_salary(PharaohFinanceLedger *ledger)
{
    rollover_u32(&ledger->salary_current_year, &ledger->salary_previous_year);
}

void pharaoh_finance_rollover_sundries(PharaohFinanceLedger *ledger)
{
    rollover_u32(
        &ledger->sundries_current_year, &ledger->sundries_previous_year);
    ledger->festival_cost_previous_year = ledger->festival_cost_current_year;
    ledger->festival_cost_current_year = 0;
    ledger->festival_events_current_year = 0;
}

void pharaoh_finance_rollover_requests_festivals(PharaohFinanceLedger *ledger)
{
    rollover_u32(&ledger->requests_festivals_current_year,
        &ledger->requests_festivals_previous_year);
}

void pharaoh_finance_estimate_wages(
    PharaohFinanceLedger *ledger, uint32_t month, uint32_t *projected_wages)
{
    const uint32_t product = ledger->worker_count * ledger->wage_rate;
    const uint32_t monthly =
        (uint32_t)(((int32_t)product / 10) / 12);
    ledger->wages_previous_year = ledger->wages_current_year;
    *projected_wages = (12u - month) * monthly + ledger->wages_current_year;
}

void pharaoh_finance_estimate_interest(PharaohFinanceLedger *ledger,
    const PharaohBuildingTypeRecord *buildings,
    uint16_t special_building_count, int32_t special_building_id)
{
    if (special_building_count != 0u) {
        (void)pharaoh_building_group_index(buildings, special_building_id);
    }
    ledger->interest_previous_year = ledger->interest_current_year;
}

void pharaoh_finance_estimate_salary(PharaohFinanceLedger *ledger)
{
    ledger->salary_current_year = ledger->salary_total;
}

void pharaoh_finance_calculate_totals(PharaohFinanceTotals *totals)
{
    uint32_t income = totals->income_components[0];
    uint32_t expense = totals->expense_components[0];
    int32_t index;
    for (index = 1; index < 4; ++index) income += totals->income_components[index];
    for (index = 1; index < 7; ++index) expense += totals->expense_components[index];
    totals->income_current_year = income;
    totals->expense_current_year = expense;
    totals->net_previous_year =
        totals->income_previous_year - totals->expense_previous_year;
    totals->net_current_year = income - expense;
    totals->projected_balance = totals->projection_base + totals->net_current_year;
}

void pharaoh_finance_calculate_tribute(int32_t treasury, int32_t population,
    int32_t net_current_year, volatile uint32_t *provisional_tribute)
{
    uint32_t amount;

    if (treasury <= 0) {
        *provisional_tribute = 0;
        *provisional_tribute = 0;
        return;
    }
    if (net_current_year <= 0) {
        if (population >= 2001) *provisional_tribute = 200;
        else if (population >= 1001) *provisional_tribute = 100;
        else *provisional_tribute = 0;
        *provisional_tribute = 0;
        return;
    }

    *provisional_tribute = population >= 5001 ? 500u :
        population >= 3001 ? 400u : population >= 2001 ? 300u :
        population >= 1001 ? 225u : population >= 501 ? 150u : 50u;
    amount = (uint32_t)pharaoh_adjust_by_percentage(net_current_year, 25);
    if ((int32_t)amount > (int32_t)*provisional_tribute) {
        *provisional_tribute = amount;
    }
    *provisional_tribute = 0;
}
