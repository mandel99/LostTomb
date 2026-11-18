#include "pharaoh/ui/custom_map_selection.h"

#include <string.h>

#define CHECK(condition) do { if (!(condition)) return __LINE__; } while (0)

int main(void)
{
    PharaohCampaign campaign = { 0 };
    PharaohCustomMapSelectionState selection = { 0 };
    PharaohFileList files;

    pharaoh_mission_file_list_initialize(&files);
    selection.files = &files;
    memcpy(campaign.mission_names[0], "First mission", sizeof("First mission"));
    memcpy(campaign.mission_names[1], "Second mission", sizeof("Second mission"));
    memcpy(campaign.mission_names[2], "Third mission", sizeof("Third mission"));
    campaign.mission_name_count = 3;
    selection.first_visible_file = 7;
    selection.selected_file = 2;

    CHECK(pharaoh_campaign_mission_selection_load(&selection, &campaign) == 3);
    CHECK(files.count == 3);
    CHECK(strcmp(pharaoh_file_list_name(&files, 0), "First mission") == 0);
    CHECK(strcmp(pharaoh_file_list_name(&files, 1), "Second mission") == 0);
    CHECK(strcmp(pharaoh_file_list_name(&files, 2), "Third mission") == 0);
    CHECK(selection.first_visible_file == 0);
    CHECK(selection.selected_file == 0);
    CHECK(pharaoh_custom_map_selection_select_visible(&selection, 2) != 0);
    CHECK(selection.selected_file == 2);

    pharaoh_file_list_destroy(&files);
    return 0;
}
