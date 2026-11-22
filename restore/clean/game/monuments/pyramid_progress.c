#include "pharaoh/monuments/progress_reports.h"
#include <limits.h>
#include <stddef.h>
#include <stdlib.h>

_Static_assert(offsetof(PharaohBuildingTypeRecord,pyramid_prep_mask)==0x5b,"native preparation mask");

_Static_assert(sizeof(PharaohPyramidProgressReport)==112 && offsetof(PharaohPyramidProgressReport,percent)==52 && offsetof(PharaohPyramidProgressReport,materialCasing)==108,"native28-DWORD progress report");

/* Complete PyramidSummary54.c reuse, native004F1310. */
enum Pyramid54Metric {
    Pyramid54FullLayerUnits=6,
    Pyramid54HalfLayerUnits=3,
    Pyramid54BuildingPercent=95,
    Pyramid54Level1Work=5,
    Pyramid54Level2Work=9,
    Pyramid54Level3Work=13,
    Pyramid54Level4Work=17,
    Pyramid54Level5Work=21
};
static int32_t Pyramid54Add(int32_t a,int32_t b) { return (int32_t)((uint32_t)a+(uint32_t)b); }
static int32_t Pyramid54Sub(int32_t a,int32_t b) { return (int32_t)((uint32_t)a-(uint32_t)b); }
static int32_t Pyramid54Multiply(int32_t a,int32_t b) { return (int32_t)((uint32_t)a*(uint32_t)b); }
/* Native CDQ/IDIV consumes the sign extension of the wrapped DWORD numerator.
 * Keep native division failures explicit instead of clamping the report. */
static int32_t Pyramid54Divide(int32_t numerator,int32_t denominator)
{
    if (!denominator || (numerator==INT32_MIN && denominator==-1)) abort();
    return numerator/denominator;
}
static PharaohBuildingTypeRecord *Pyramid54Building(PharaohPyramidSummaryContext *c,int32_t id) { return &c->buildings[id]; }
static const PharaohWalkerQueryRecord *Pyramid54Walker(PharaohPyramidSummaryContext *c,int32_t id) { return &c->walkers[id]; }
static int32_t Pyramid54CountParts(PharaohPyramidSummaryContext *c,uint32_t size)
{
    return (*c->style)==5?c->mastaba_part_counts[size]:c->part_counts[size];
}
static int32_t Pyramid54Layers(PharaohPyramidSummaryContext *c,uint32_t size,int32_t index)
{
    return (*c->style)==5?c->mastaba_layout[size][index].layers:c->pyramid_layout[size][index].layers;
}
static int32_t Pyramid54BentLayerUnits(PharaohPyramidSummaryContext *c,uint32_t size,uint32_t layer)
{
    return c->bent_half_layer_bytes[size*6+layer]?
        Pyramid54HalfLayerUnits:Pyramid54FullLayerUnits;
}
static int32_t Pyramid54BentUnits(PharaohPyramidSummaryContext *c,uint32_t size,int32_t layers)
{
    int32_t layer,total=0;
    for (layer=0;layer<layers;++layer) total=Pyramid54Add(total,Pyramid54BentLayerUnits(c,size,(uint32_t)layer));
    return total;
}
static int32_t Pyramid54AllBlocks(PharaohPyramidSummaryContext *c,uint32_t size)
{
    switch ((*c->style)) {
    case 1:return c->bent_block_totals[size];
    case 2:case 4:return c->block_totals[size];
    case 3:return c->stepped_block_totals[size];
    case 5:return c->mastaba_block_totals[size];
    default:return 0;
    }
}
static int32_t Pyramid54CasingBlocks(PharaohPyramidSummaryContext *c,uint32_t size)
{
    switch ((*c->style)) {
    case 1:return c->bent_casing_totals[size];
    case 2:case 4:
        return Pyramid54Multiply(Pyramid54CountParts(c,size),Pyramid54FullLayerUnits);
    default:return 0;
    }
}
static void Pyramid54Totals(PharaohPyramidSummaryContext *c,volatile PharaohPyramidProgressReport *out,PharaohBuildingTypeRecord *part,uint32_t size,int32_t index,int32_t layers,int32_t casing)
{
    if ((*c->style)==5 && index==0) {
        out->mainTotal=Pyramid54Add(out->mainTotal,1);
    } else if ((*c->style)==1) {
        out->mainTotal=Pyramid54Add(out->mainTotal,Pyramid54BentUnits(c,size,layers));
        out->CasingTotal=Pyramid54Add(out->CasingTotal,Pyramid54BentLayerUnits(c,size,((uint8_t)part->monument_stage)));
    } else {
        out->mainTotal=Pyramid54Add(out->mainTotal,Pyramid54Multiply(layers,Pyramid54FullLayerUnits));
        if (casing) out->CasingTotal=Pyramid54Add(out->CasingTotal,Pyramid54FullLayerUnits);
        else out->mainTotal=Pyramid54Add(out->mainTotal,Pyramid54FullLayerUnits);
    }
}
static void Pyramid54WaitingWorker(PharaohPyramidSummaryContext *c,volatile PharaohPyramidProgressReport *out,int32_t partId,int32_t workerId,int32_t casing)
{
    const int32_t leader=Pyramid54Walker(c,workerId)->leading_walker_id;
    if (leader && pharaoh_pyramid_linked_worker_active(c->walkers,leader,workerId)) return;
    out->waiting=Pyramid54Add(out->waiting,1);
    if (!pharaoh_pyramid_piece_has_pending_work(c->buildings,partId,(uint32_t)*c->style,c->bent_half_layer_bytes) && casing)
        out->waitingCasing=Pyramid54Add(out->waitingCasing,1);
    else out->waitingMain=Pyramid54Add(out->waitingMain,1);
}
static void Pyramid54Workers(PharaohPyramidSummaryContext *c,volatile PharaohPyramidProgressReport *out,PharaohBuildingTypeRecord *part,int32_t partId,int32_t casing)
{
    const int32_t id=part->pyramid_worker_id_64;
    uint8_t kind;
    if (!id) return;
    kind=Pyramid54Walker(c,id)->type;
    switch (kind) {
    case 79:
        if (pharaoh_pyramid_worker_claim_active(c,partId)) out->carpenters=Pyramid54Add(out->carpenters,1);
        break;
    case 80:
        if (pharaoh_pyramid_worker_claim_active(c,partId)) {
            out->bricklayers=Pyramid54Add(out->bricklayers,1);
            out->blockWorkers=Pyramid54Add(out->blockWorkers,1);
            if (Pyramid54Walker(c,id)->action_state==11) Pyramid54WaitingWorker(c,out,partId,id,casing);
        }
        break;
    case 81:
        if (pharaoh_pyramid_worker_claim_active(c,partId)) {
            out->stonemasons=Pyramid54Add(out->stonemasons,1);
            out->blockWorkers=Pyramid54Add(out->blockWorkers,1);
            if (Pyramid54Walker(c,id)->action_state==10) Pyramid54WaitingWorker(c,out,partId,id,casing);
        }
        break;
    case 85:
        if (pharaoh_pyramid_worker_claim_active(c,partId)) out->laborers=Pyramid54Add(out->laborers,1);
        break;
    }
}
static void Pyramid54Preparation(PharaohPyramidSummaryContext *c,volatile PharaohPyramidProgressReport *out,int32_t root,uint32_t size,int32_t casing)
{
    static const int32_t quadrantWeights[15+1]={0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4};
    int32_t index;
    out->phase=1;
    out->prepMinStage=999;
    for (index=0;index<Pyramid54CountParts(c,size);++index) {
        PharaohBuildingTypeRecord *part=Pyramid54Building(c,root+index);
        const uint8_t before=part->pyramid_work_phase;
        const int32_t id=part->pyramid_worker_id_64;
        int32_t work=0;
        if (before>out->prepMaxStage) out->prepMaxStage=before;
        if (before<out->prepMinStage) out->prepMinStage=before;
        if (id && Pyramid54Walker(c,id)->type==85 && pharaoh_pyramid_worker_claim_active(c,root+index))
            out->laborers=Pyramid54Add(out->laborers,1);
        /* prepMask is the four native quadrant bits; initialization and native
         * tile completion preserve0..15. No caller-stack bytes are invented. */
        switch (part->pyramid_work_phase) {
        case 4:work=quadrantWeights[part->pyramid_prep_mask];break;
        case 5:work=Pyramid54Level1Work;break;
        case 6:work=Pyramid54Add(quadrantWeights[part->pyramid_prep_mask],Pyramid54Level1Work);break;
        case 7:work=Pyramid54Add(quadrantWeights[part->pyramid_prep_mask],Pyramid54Level2Work);break;
        case 8:work=Pyramid54Add(quadrantWeights[part->pyramid_prep_mask],Pyramid54Level3Work);break;
        case 9:work=Pyramid54Add(quadrantWeights[part->pyramid_prep_mask],Pyramid54Level4Work);break;
        case 10:work=Pyramid54Add(quadrantWeights[part->pyramid_prep_mask],Pyramid54Level5Work);break;
        }
        out->prepDone=Pyramid54Add(out->prepDone,work);
    }
    if (out->prepMaxStage==11) out->prepDone=Pyramid54Add(out->prepDone,1);
    out->phasePercent=Pyramid54Divide(Pyramid54Multiply(out->prepDone,100),out->prepTotal);
    out->percent=Pyramid54Divide(Pyramid54Multiply(out->phasePercent,5),100);
    for (index=0;index<Pyramid54CountParts(c,size);++index)
        Pyramid54Totals(c,out,Pyramid54Building(c,root+index),size,index,Pyramid54Layers(c,size,index),casing);
}
static void Pyramid54Construction(PharaohPyramidSummaryContext *c,volatile PharaohPyramidProgressReport *out,int32_t root,uint32_t size,int32_t blocks,int32_t casing,int32_t preparationTotal)
{
    int32_t index,frontier,remaining,phaseBlocks,phaseTotal;
    PharaohBuildingTypeRecord *first;
    /* Native4F171E copies captured EBP, before a child can change the report. */
    out->prepDone=preparationTotal;
    pharaoh_pyramid_collect_work_pieces(c->buildings,root,(uint32_t)*c->style,c->part_counts,c->mastaba_part_counts,c->frontier_parts,c->frontier_count);
    frontier=c->frontier_parts[0];
    if ((*c->frontier_count)<1) frontier=root;
    first=Pyramid54Building(c,frontier);
    out->height=first->pyramid_work_column+first->pyramid_work_row*6;
    out->heightNext=Pyramid54Add(out->height,1);
    if ((*c->style)==1) out->layerCount=c->bent_layer_counts[size];
    else if ((*c->style)==5) out->layerCount=c->mastaba_layer_counts[size];
    else out->layerCount=c->layer_counts[size];
    if ((*c->frontier_count)<1) {
        out->phase=4;out->phasePercent=100;out->percent=100;
    } else {
        remaining=Pyramid54Building(c,root)->monument_remaining_blocks;
        if (!first->pyramid_work_phase) {
            out->phase=2;phaseBlocks=blocks;phaseTotal=Pyramid54Sub(blocks,casing);
        } else { out->phase=3;phaseBlocks=casing;phaseTotal=casing; }
        out->phasePercent=Pyramid54Divide(Pyramid54Multiply(Pyramid54Sub(phaseBlocks,Pyramid54Building(c,root)->monument_remaining_blocks),100),phaseTotal);
        out->percent=Pyramid54Add(Pyramid54Divide(Pyramid54Multiply(Pyramid54Sub(blocks,remaining),Pyramid54BuildingPercent),blocks),5);
    }
    for (index=0;index<Pyramid54CountParts(c,size);++index) {
        const int32_t partId=root+index;
        const int32_t layers=Pyramid54Layers(c,size,index);
        PharaohBuildingTypeRecord *part=Pyramid54Building(c,partId);
        if (!part->pyramid_work_phase) {
            int32_t completed=part->pyramid_work_row;
            if (layers<completed) completed=layers;
            if (completed<0) completed=0;
            if ((*c->style)==1)
                out->mainPlaced=Pyramid54Add(out->mainPlaced,Pyramid54BentUnits(c,size,completed));
            else if ((*c->style)!=5 || index!=0)
                out->mainPlaced=Pyramid54Add(out->mainPlaced,Pyramid54Multiply(completed,Pyramid54FullLayerUnits));
            if (part->pyramid_work_row<((uint8_t)part->monument_stage) || !casing)
                out->mainPlaced=Pyramid54Add(out->mainPlaced,part->pyramid_work_column);
            else out->casingPlaced=Pyramid54Add(out->casingPlaced,part->pyramid_work_column);
        } else if ((*c->style)==1) {
            out->mainPlaced=Pyramid54Add(out->mainPlaced,Pyramid54BentUnits(c,size,layers));
            out->casingPlaced=Pyramid54Add(out->casingPlaced,Pyramid54BentLayerUnits(c,size,part->pyramid_work_row));
        } else if ((*c->style)==5 && index==0)
            out->mainPlaced=Pyramid54Add(out->mainPlaced,1);
        else {
            out->mainPlaced=Pyramid54Add(out->mainPlaced,Pyramid54Multiply(layers,Pyramid54FullLayerUnits));
            if (casing) out->casingPlaced=Pyramid54Add(out->casingPlaced,Pyramid54FullLayerUnits);
            else out->mainPlaced=Pyramid54Add(out->mainPlaced,Pyramid54FullLayerUnits);
        }
        Pyramid54Totals(c,out,part,size,index,layers,casing);
        Pyramid54Workers(c,out,part,partId,casing);
    }
    if (out->phase==2) for (index=0;index<(*c->frontier_count);++index) {
        if (!pharaoh_pyramid_piece_has_pending_work(c->buildings,c->frontier_parts[index],(uint32_t)*c->style,c->bent_half_layer_bytes) && casing)
            out->frontierCasing=Pyramid54Add(out->frontierCasing,1);
        else out->frontierMain=Pyramid54Add(out->frontierMain,1);
    }
}
void pharaoh_pyramid_build_progress_report(PharaohPyramidSummaryContext *c,int32_t buildingId,PharaohPyramidProgressReport *output)
{
    volatile PharaohPyramidProgressReport *out=output;
    PharaohBuildingTypeRecord *main;
    uint32_t size;
    int32_t root,blocks,casing,style,preparationTotal=0;
    *output=(PharaohPyramidProgressReport){0};
    if (Pyramid54Building(c,buildingId)->building_type!=183) return;
    root=buildingId-Pyramid54Building(c,buildingId)->subtype;
    main=Pyramid54Building(c,root);
    size=main->pyramid_size_variant;
    style=(*c->style);
    if (style>=1 && style<=4) preparationTotal=c->prep_totals[size];
    else if (style==5) preparationTotal=c->mastaba_prep_totals[size];
    out->prepTotal=preparationTotal;
    blocks=Pyramid54AllBlocks(c,main->pyramid_size_variant);
    casing=Pyramid54CasingBlocks(c,size);
    if (main->pyramid_work_phase>=2) Pyramid54Preparation(c,out,root,size,casing);
    else Pyramid54Construction(c,out,root,size,blocks,casing,preparationTotal);
    out->mainRemaining=Pyramid54Sub(out->mainTotal,out->mainPlaced)/2;
    out->casingRemaining=Pyramid54Sub(out->CasingTotal,out->casingPlaced)/2;
    out->mainTotal/=2;out->CasingTotal/=2;
    out->mainPlaced/=2;out->casingPlaced/=2;
    out->frontierMain/=2;out->frontierCasing/=2;
    out->materialMain=Pyramid54Multiply(out->waitingMain,200);
    out->materialCasing=Pyramid54Multiply(out->waitingCasing,200);
}
