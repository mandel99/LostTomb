#include "pharaoh/city/finance_money.h"

#include "pharaoh/core/x87_numeric.h"

int32_t pharaoh_city_finance_money_share(int32_t money,uint32_t index,
    const PharaohFinanceMoneyShareRow table[PHARAOH_FINANCE_SHARE_ROWS],
    const int32_t *building_count_total)
{
    uint32_t buildings[PHARAOH_FINANCE_SHARE_ROWS];
    uint32_t total;
    int32_t row_index;

    if(money<1)return 0;
    for(row_index=0;row_index<PHARAOH_FINANCE_SHARE_ROWS;++row_index){
        const PharaohFinanceMoneyShareRow *row=&table[row_index];
        int32_t type_index;
        buildings[row_index]=0;
        for(type_index=0;type_index<row->building_type_count;++type_index){
            buildings[row_index]+=(uint32_t)
                building_count_total[row->building_types[type_index]];
        }
    }
    total=(uint32_t)table[2].weight*buildings[2];
    total+=(uint32_t)table[1].weight*buildings[1];
    total+=(uint32_t)table[0].weight*buildings[0];
    if(total==0)return 0;
    return pharaoh_x87_i32_ratio_multiply(table[index].weight,
        (int32_t)total,money);
}

int32_t pharaoh_city_finance_building_money_value(
    const PharaohBuildingTypeRecord *buildings,int32_t building_id,
    int32_t personal_savings,int32_t treasury,
    const PharaohFinanceMoneyShareRow mansion_shares[PHARAOH_FINANCE_SHARE_ROWS],
    const PharaohFinanceMoneyShareRow palace_shares[PHARAOH_FINANCE_SHARE_ROWS],
    const int32_t *building_count_total)
{
    const PharaohBuildingTypeRecord *building=&buildings[building_id];
    if(building->state!=1)return 0;
    switch(building->building_type){
    case 0x4d:
        return pharaoh_city_finance_money_share(personal_savings,0,
            mansion_shares,building_count_total);
    case 0x4e:
        return pharaoh_city_finance_money_share(personal_savings,1,
            mansion_shares,building_count_total);
    case 0x4f:
        return pharaoh_city_finance_money_share(personal_savings,2,
            mansion_shares,building_count_total);
    case 0x54: case 0x55: case 0xbb: case 0xbc: case 0xbd:
        return pharaoh_city_finance_money_share(treasury,2,
            palace_shares,building_count_total);
    case 0x56: case 0x57:
        return pharaoh_city_finance_money_share(treasury,0,
            palace_shares,building_count_total);
    case 0xb8:
        return pharaoh_city_finance_money_share(treasury,1,
            palace_shares,building_count_total);
    default:
        return 0;
    }
}
