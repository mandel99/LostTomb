/* Unrelated row/input functions share load_save_menu.c. These link sentinels
   are never part of the eleven-entry proof graph and abort if reached. */
#include "load_save_menu.h"
#include "file_list.h"
#include "menu_input.h"
#include <windows.h>
#include <stdio.h>
__declspec(noreturn) static void outside(void){fputs("Unproved row/input path reached\n",stderr);ExitProcess(5);}
#pragma warning(push)
#pragma warning(disable:4100)
void FUN_00402720(void){outside();}
uint32_t FUN_004029d0(void){outside();}
char *FUN_0041e010(RestoreFileList *self,int index){outside();}
int FUN_004261d0(int x,int y,RestoreMenuButton *table,int count){outside();}
int FUN_00426450(int x,int y,const RestoreMenuButton *table,int count){outside();}
uint32_t FUN_004d6740(int x,int y,const void *table,int count,uint32_t hit){outside();}
void FUN_00425bc0(int x,int y,const void *table,int count,int frames){outside();}
int FUN_00425990(int x,int y,const void *table,int count){outside();}
#pragma warning(pop)
