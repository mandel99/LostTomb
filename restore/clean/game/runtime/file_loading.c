#include "pharaoh/runtime/file_loading.h"

int pharaoh_load_raw_file(const PharaohFileReadContext *context,
                          const char *filename, void *destination)
{
    const PharaohFileReadOperations *operations = context->operations;
    PharaohReadOnlyFile *file = operations->open(context->user_data, filename);
    if (file == NULL) {
        return 0;
    }

    const size_t file_size = operations->size(context->user_data, file);
    (void)operations->read(context->user_data, destination, 1, file_size, file);
    operations->close(context->user_data, file);
    return 1;
}
