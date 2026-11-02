#include "actor_render.h"

void FUN_00439740(uint32_t selection)
{
    uint32_t counter,row,y,rows=0,parity=0,x,column,columns,width,tile,offset,remaining;
    int32_t actor;
    if(AR_B(0x605b3d)==1){counter=AR_U(0x78d5ec)+1u;AR_U(0x78d5ec)=counter;if(AR_S(counter)>=5)AR_U(0x78d5ec)=0;}
    counter=AR_U(0x70e44c);row=AR_U(0x70e474);y=AR_U(0x70e48c)-counter*9u;
    counter=AR_U(0x70e460)+14u;row-=8u;AR_U(0x70e47c)=row;
    if(AR_S(counter)<=0)return;
    do{
        if(AR_S(row)>=0&&AR_S(row)<457){
            x=AR_U(0x70e490);if(parity)x=x-AR_U(0x70e45c)-1u;
            x+=(0xfffffffeu-AR_U(0x70e458))*4u;
            column=AR_U(0x70e478)-4u;columns=0;width=AR_U(0x70e464)+6u;AR_U(0x70e480)=column;
            if(AR_S(width)>=0){
                do{
                    if(AR_S(column)>=0&&AR_S(column)<231){
                        tile=AR_U(0x70e494u+(row*231u+column)*4u);AR_U(0x70e488)=tile;
                        if(AR_S(tile)<0x0fff0000){
                            actor=AR_SW(0xb350dcu+tile*2u);
                            while(actor>0){
                                offset=(uint32_t)actor*388u;
                                if(AR_B(0xbcd66fu+offset)!=0){FUN_00438e50(actor,x,y,selection);row=AR_U(0x70e47c);column=AR_U(0x70e480);}
                                actor=AR_SW(0xbcd600u+offset);
                            }
                        }
                    }
                    x+=AR_U(0x70e458)+2u;++columns;++column;width=AR_U(0x70e464)+6u;AR_U(0x70e480)=column;
                }while(AR_S(columns)<=AR_S(width));
            }
            /* The native second traversal retains a local column, writes the
               global cursor only at its ends, and captures its length once. */
            x=AR_U(0x70e490);if(parity)x=x-AR_U(0x70e45c)-1u;
            x+=(0xfffffffeu-AR_U(0x70e458))*4u;
            column=AR_U(0x70e478)-4u;width=AR_U(0x70e464)+6u;AR_U(0x70e480)=column;
            if(AR_S(width)>=0){
                remaining=width+1u;
                do{
                    if(AR_S(column)>=0&&AR_S(column)<231){
                        counter=AR_U(0xe94e0c);
                        if(counter==0||counter==51||AR_B(0x6de515)!=0)
                            AR_U(0x70e488)=AR_U(0x70e494u+(row*231u+column)*4u);
                    }
                    ++column;--remaining;x+=AR_U(0x70e458)+2u;
                }while(remaining!=0);
                AR_U(0x70e480)=column;
            }
        }
        parity=!parity;y+=AR_U(0x70e44c);++rows;++row;AR_U(0x70e47c)=row;
    }while(AR_S(rows)<AR_S(AR_U(0x70e460)+14u));
}
