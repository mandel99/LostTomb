#ifndef PHARAOH_CITY_ADVISOR_FINANCE_H
#define PHARAOH_CITY_ADVISOR_FINANCE_H
#include "pharaoh/simulation/finance_calendar.h"

typedef struct PharaohAdvisorFinanceContext {
    const uint32_t *month; /*78D7C0*/
    const uint32_t *employed,*wage_rate,*wages_actual; /*EA4464/EA4478/EA4484*/
    volatile uint32_t *wages_copy,*wages_forecast; /*EA4488/EA5F10*/
    const uint32_t *interest_actual,*salary_source; /*EA44E4/EA4510*/
    volatile uint32_t *interest_copy,*salary_copy; /*EA44EC/EA4504*/
    const uint16_t *temple_count;
    const int32_t *temple_building;
    const PharaohBuildingTypeRecord *buildings;
    const uint32_t *income[4],*expense[7];
    /* Income:EA6020/EA5CC8/EA44BC/EA44D4.
       Expense:EA5FC8/EA44F4/EA4504/EA44EC/EA44FC/EA4488/EA44DC. */
    const uint32_t *income_previous,*expense_previous,*projection_base;
    /*EA4514/EA451C/EA452C*/
    volatile uint32_t *income_total,*expense_total,*net_previous,*net_current,*balance;
    /*EA4518/EA4520/EA4524/EA4528/EA4530*/
    const int32_t *treasury,*population;
    volatile uint32_t *tribute; /*EA5D50*/
} PharaohAdvisorFinanceContext;

/* Native0045A8C0/0045A930/0045A970/0045A990/0045A7B0. Every city
   pointer is the actual same selected49D8 bank. Existing real C kernels
   compute temporary views, followed by ordered publication to live fields. */
void pharaoh_city_finance_estimate_wages(const PharaohAdvisorFinanceContext *);
void pharaoh_city_finance_estimate_interest(const PharaohAdvisorFinanceContext *);
void pharaoh_city_finance_estimate_salary(const PharaohAdvisorFinanceContext *);
void pharaoh_city_finance_calculate_totals(const PharaohAdvisorFinanceContext *);
void pharaoh_city_finance_calculate_tribute(const PharaohAdvisorFinanceContext *);
#endif
