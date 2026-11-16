#include "pharaoh/simulation/finance_calendar.h"

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
