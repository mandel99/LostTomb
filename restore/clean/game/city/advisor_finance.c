#include "pharaoh/city/advisor_finance.h"

static int32_t signed_bits(uint32_t bits)
{
    return bits<=INT32_MAX?(int32_t)bits:
        (int32_t)((int64_t)bits-INT64_C(0x100000000));
}

void pharaoh_city_finance_estimate_wages(const PharaohAdvisorFinanceContext *c)
{
    const uint32_t month=*c->month;
    PharaohFinanceLedger view;
    view.wage_rate=*c->wage_rate;
    view.worker_count=*c->employed;
    view.wages_current_year=*c->wages_actual;
    uint32_t forecast;
    pharaoh_finance_estimate_wages(&view,month,&forecast);
    *c->wages_copy=view.wages_previous_year;
    *c->wages_forecast=forecast;
}

void pharaoh_city_finance_estimate_interest(const PharaohAdvisorFinanceContext *c)
{
    const uint16_t count=*c->temple_count;
    const int32_t building=count!=0?*c->temple_building:0;
    PharaohFinanceLedger view;
    view.interest_current_year=*c->interest_actual;
    /* The actual group query is pure, so this selected bank is stable. */
    pharaoh_finance_estimate_interest(&view,c->buildings,count,building);
    *c->interest_copy=view.interest_previous_year;
}

void pharaoh_city_finance_estimate_salary(const PharaohAdvisorFinanceContext *c)
{
    PharaohFinanceLedger view;
    view.salary_total=*c->salary_source;
    pharaoh_finance_estimate_salary(&view);
    *c->salary_copy=view.salary_current_year;
}

void pharaoh_city_finance_calculate_totals(const PharaohAdvisorFinanceContext *c)
{
    PharaohFinanceTotals view;
    for(int32_t i=0;i<4;++i) view.income_components[i]=*c->income[i];
    for(int32_t i=0;i<7;++i) view.expense_components[i]=*c->expense[i];
    view.income_previous_year=*c->income_previous;
    view.expense_previous_year=*c->expense_previous;
    view.projection_base=*c->projection_base;
    pharaoh_finance_calculate_totals(&view);
    *c->income_total=view.income_current_year;
    *c->expense_total=view.expense_current_year;
    *c->net_previous=view.net_previous_year;
    *c->net_current=view.net_current_year;
    *c->balance=view.projected_balance;
}

void pharaoh_city_finance_calculate_tribute(const PharaohAdvisorFinanceContext *c)
{
    const int32_t treasury=*c->treasury;
    if(treasury<=0){
        /* These unused arguments are not read by this native branch. */
        pharaoh_finance_calculate_tribute(treasury,0,0,c->tribute);
        return;
    }
    const int32_t net=signed_bits(*c->net_current);
    const int32_t population=*c->population;
    pharaoh_finance_calculate_tribute(treasury,population,net,c->tribute);
}
