#ifndef PHARAOH_RUNTIME_FORMAT_UNBOUNDED_H
#define PHARAOH_RUNTIME_FORMAT_UNBOUNDED_H

/* Native 0x00561A36 variadic formatter. The caller owns a destination buffer
   large enough for the formatted result and its terminating NUL byte. */
int pharaoh_format_unbounded(char *destination, const char *format, ...);

#endif
