#include "pharaoh/runtime/status_log_timestamp.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string.h>

static const char status_log_weekday_names[7][3] = {
    { 'S', 'u', 'n' }, { 'M', 'o', 'n' }, { 'T', 'u', 'e' },
    { 'W', 'e', 'd' }, { 'T', 'h', 'u' }, { 'F', 'r', 'i' },
    { 'S', 'a', 't' }
};

static const char status_log_month_names[12][3] = {
    { 'J', 'a', 'n' }, { 'F', 'e', 'b' }, { 'M', 'a', 'r' },
    { 'A', 'p', 'r' }, { 'M', 'a', 'y' }, { 'J', 'u', 'n' },
    { 'J', 'u', 'l' }, { 'A', 'u', 'g' }, { 'S', 'e', 'p' },
    { 'O', 'c', 't' }, { 'N', 'o', 'v' }, { 'D', 'e', 'c' }
};

static void write_two_digits(char *destination, uint32_t value)
{
    destination[0] = (char)('0' + (value / 10) % 10);
    destination[1] = (char)('0' + value % 10);
}

void pharaoh_status_log_refresh_timestamp_text(void *user_data,
    char timestamp_text[PHARAOH_STATUS_LOG_TIMESTAMP_TEXT_LENGTH])
{
    SYSTEMTIME local_time;
    (void)user_data;
    GetLocalTime(&local_time);

    memcpy(timestamp_text, status_log_weekday_names[local_time.wDayOfWeek], 3);
    timestamp_text[3] = ' ';
    memcpy(timestamp_text + 4,
        status_log_month_names[local_time.wMonth - 1], 3);
    timestamp_text[7] = ' ';
    write_two_digits(timestamp_text + 8, local_time.wDay);
    timestamp_text[10] = ' ';
    write_two_digits(timestamp_text + 11, local_time.wHour);
    timestamp_text[13] = ':';
    write_two_digits(timestamp_text + 14, local_time.wMinute);
    timestamp_text[16] = ':';
    write_two_digits(timestamp_text + 17, local_time.wSecond);
    timestamp_text[19] = ' ';
    write_two_digits(timestamp_text + 20, (uint32_t)local_time.wYear / 100);
    write_two_digits(timestamp_text + 22, (uint32_t)local_time.wYear % 100);
    timestamp_text[24] = '\n';
    timestamp_text[25] = '\0';
}
