#ifndef RESTORE_PLACEMENT_PREDICATE_ENTRIES_H
#define RESTORE_PLACEMENT_PREDICATE_ENTRIES_H
#include <stdint.h>
uint8_t restore_placement_empty_mask(uint32_t terrain);
uint8_t restore_placement_piece_mask(uint32_t self,uint32_t terrain,int32_t piece);
uint8_t restore_placement_selected_mask(uint32_t self,uint32_t terrain,int32_t piece);
uint8_t restore_placement_two_piece_mask(uint32_t terrain,int32_t piece);
uint32_t restore_placement_no_existing(void);
uint32_t restore_placement_accept_empty(uint32_t terrain,uint32_t incoming_eax);
uint32_t restore_placement_accept_piece(uint32_t self,uint32_t terrain,int32_t piece,uint32_t incoming_eax);
uint32_t restore_placement_accept_selected(uint32_t self,uint32_t terrain,int32_t piece,uint32_t incoming_eax);
uint32_t restore_placement_accept_two_piece(uint32_t terrain,int32_t piece);
#endif
