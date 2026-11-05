#include "crt_stream_io.h"
#include "legacy_format.h"

void *FUN_00563400(void *destination, const void *source, uint32_t bytes);
void *FUN_00562880(void *destination, const void *source, uint32_t bytes);
uint32_t FUN_00563380(const char *text);
int32_t FUN_00562bea(void *stream);

#define R32(a) restore_stream_read32(a)
#define W32(a,v) restore_stream_write32((a),(v))
#define R8(a) (*(volatile const uint8_t *)(uintptr_t)(a))
#define W8(a,v) (*(volatile uint8_t *)(uintptr_t)(a) = (uint8_t)(v))

static uint32_t stream_buffer(uint32_t self)
{
    return R32(restore_stream_virtual_base(self) + 4u);
}

static void stream_error(uint32_t self, uint32_t mask)
{
    uint32_t state = restore_stream_virtual_base(self) + 8u;
    W32(state, R32(state) | mask);
}

uint32_t restore_stream_filebuf_grow(uint32_t self)
{
    uint32_t growth = R32(self + 0x34u);
    uint32_t increment = (int32_t)growth > 1 ? growth : 1u;
    uint32_t last = R32(self + 0x14u);
    uint32_t first = R32(self + 0x10u);
    uint32_t capacity = last > first ? last - first : 0u;
    uint32_t bytes;
    uint32_t live_ecx;
    uint32_t target;
    uint32_t allocation;
    uint32_t delta = 0u;
    if ((int32_t)growth > (int32_t)(capacity + increment)) {
        bytes = growth;
        live_ecx = last;
    } else {
        if ((int32_t)growth <= 1)
            growth = 1u;
        bytes = capacity + growth;
        live_ecx = bytes;
    }
    target = R32(self + 0x40u);
    allocation = target != 0u
        ? restore_stream_io_allocate_call(target, bytes, live_ecx)
        : (uint32_t)(uintptr_t)FUN_00561a1d(bytes);
    if (allocation == 0u)
        return 0xffffffffu;
    last = R32(self + 0x14u);
    first = R32(self + 0x10u);
    capacity = last > first ? last - first : 0u;
    if (capacity != 0u) {
        (void)FUN_00563400((void *)(uintptr_t)allocation,
            (const void *)(uintptr_t)first, capacity);
        first = R32(self + 0x10u);
        delta = allocation - first;
    }
    target = R32(self + 0x44u);
    if (target != 0u)
        restore_stream_io_release_call(target, first);
    else
        FUN_00561a2b((void *)(uintptr_t)first);
    (void)restore_stream_buffer_set(self, allocation, bytes + allocation, 0u);
    if (delta != 0u) {
        last = R32(self + 0x2cu);
        if (last != 0u) {
            W32(self + 0x24u, R32(self + 0x24u) + delta);
            W32(self + 0x28u, R32(self + 0x28u) + delta);
            W32(self + 0x0cu, 0xffffffffu);
            W32(self + 0x2cu, last + delta);
        }
    }
    last = R32(self + 0x20u);
    if (last != 0u) {
        uint32_t start = R32(self + 0x18u);
        uint32_t current = R32(self + 0x1cu);
        last += delta;
        W32(self + 0x20u, last);
        W32(self + 0x18u, start + delta);
        W32(self + 0x1cu, start + delta);
        if (last != 0u)
            W32(self + 0x1cu, delta + current);
    }
    return 1u;
}

uint32_t restore_stream_filebuf_overflow(uint32_t self, uint32_t character)
{
    if (R32(self + 0x1cu) >= R32(self + 0x20u)) {
        if (R32(self + 0x30u) == 0u ||
            restore_stream_filebuf_grow(self) == 0xffffffffu)
            return 0xffffffffu;
        if (R32(self + 0x20u) == 0u) {
            uint32_t current = R32(self + 0x2cu) - R32(self + 0x24u)
                + R32(self + 0x10u);
            W32(self + 0x18u, current);
            W32(self + 0x1cu, current);
            W32(self + 0x20u, R32(self + 0x14u));
        } else {
            uint32_t end = R32(self + 0x14u);
            uint32_t start = R32(self + 0x18u);
            uint32_t current = R32(self + 0x1cu);
            W32(self + 0x1cu, start);
            W32(self + 0x20u, end);
            if (end != 0u)
                W32(self + 0x1cu, current);
        }
    }
    if (character != 0xffffffffu) {
        W8(R32(self + 0x1cu), character);
        if (R32(self + 0x20u) != 0u)
            W32(self + 0x1cu, R32(self + 0x1cu) + 1u);
    }
    return 1u;
}

uint32_t restore_stream_filebuf_underflow(uint32_t self)
{
    uint32_t end = R32(self + 0x2cu);
    uint32_t current = R32(self + 0x28u);
    if (current >= end) {
        uint32_t output = R32(self + 0x1cu);
        if (end < output) {
            uint32_t first = R32(self + 0x10u);
            uint32_t old_first = R32(self + 0x24u);
            W32(self + 0x24u, first);
            W32(self + 0x2cu, output);
            W32(self + 0x0cu, 0xffffffffu);
            W32(self + 0x28u, first - old_first + current);
        }
        current = R32(self + 0x28u);
        if (current >= R32(self + 0x2cu))
            return 0xffffffffu;
    }
    return (uint32_t)R8(current);
}

uint32_t restore_stream_filebuf_seek(uint32_t self, uint32_t offset,
    uint32_t origin, uint32_t mode)
{
    uint32_t result = 0xffffffffu;
    uint32_t candidate;
    uint32_t start;
    uint32_t end;
    if ((mode & 1u) != 0u) {
        (void)restore_stream_filebuf_underflow(self);
        if (origin == 0u) candidate = R32(self + 0x24u);
        else if (origin == 1u) candidate = R32(self + 0x28u);
        else if (origin == 2u) candidate = R32(self + 0x2cu);
        else return 0xffffffffu;
        candidate += offset;
        start = R32(self + 0x24u);
        result = candidate - start;
        if (candidate < start)
            return 0xffffffffu;
        end = R32(self + 0x2cu);
        if (candidate > end)
            return 0xffffffffu;
        if (end != 0u)
            W32(self + 0x28u, candidate);
    }
    if ((mode & 2u) == 0u)
        return result;
    if (R32(self + 0x20u) == 0u &&
        restore_stream_filebuf_overflow(self, 0xffffffffu) == 0xffffffffu)
        return 0xffffffffu;
    if (origin == 0u) candidate = R32(self + 0x18u);
    else if (origin == 1u) candidate = R32(self + 0x1cu);
    else if (origin == 2u) candidate = R32(self + 0x20u);
    else return 0xffffffffu;
    candidate += offset;
    start = R32(self + 0x18u);
    result = candidate - start;
    if (candidate < start)
        return 0xffffffffu;
    if (candidate > R32(self + 0x20u)) {
        uint32_t growth;
        if (R32(self + 0x30u) == 0u)
            return 0xffffffffu;
        growth = R32(self + 0x34u);
        if ((int32_t)growth <= (int32_t)result)
            growth = result;
        W32(self + 0x34u, growth);
        if (restore_stream_filebuf_grow(self) == 0xffffffffu)
            return 0xffffffffu;
    }
    /* Native retains the pre-growth candidate, even after relocation. */
    if (R32(self + 0x20u) != 0u)
        W32(self + 0x1cu, candidate);
    return result;
}

uint32_t restore_stream_buffer_put(uint32_t self, uint32_t character)
{
    uint32_t current = R32(self + 0x1cu);
    if (current < R32(self + 0x20u)) {
        W8(current, character);
        W32(self + 0x1cu, R32(self + 0x1cu) + 1u);
        return character & 0xffu;
    }
    return restore_stream_io_virtual1(self, 0x1cu, character);
}

uint32_t restore_stream_ostream_prepare(uint32_t self)
{
    uint32_t base = restore_stream_virtual_base(self);
    uint32_t state = R32(base + 8u);
    uint32_t tie;
    if (state != 0u) {
        W32(base + 8u, state | 2u);
        return 0u;
    }
    tie = R32(base + 0x20u);
    if (tie != 0u)
        (void)restore_stream_ostream_flush(tie);
    return 1u;
}

void restore_stream_ostream_suffix(uint32_t self)
{
    uint32_t base = restore_stream_virtual_base(self);
    W32(base + 0x30u, 0u);
    base = restore_stream_virtual_base(self);
    if ((R8(base + 0x25u) & 0x20u) != 0u) {
        uint32_t buffer = R32(base + 4u);
        if (restore_stream_io_virtual0(buffer, 4u) == 0xffffffffu)
            W32(restore_stream_virtual_base(self) + 8u, 6u);
    }
    base = restore_stream_virtual_base(self);
    if ((R8(base + 0x25u) & 0x40u) != 0u) {
        if (FUN_00562bea((void *)(uintptr_t)0x6049c0u) == -1)
            stream_error(self, 2u);
        if (FUN_00562bea((void *)(uintptr_t)0x6049e0u) == -1)
            stream_error(self, 2u);
    }
}

uint32_t restore_stream_ostream_flush(uint32_t self)
{
    if (restore_stream_io_virtual0(stream_buffer(self), 4u) == 0xffffffffu)
        stream_error(self, 2u);
    return self;
}

uint32_t restore_stream_ostream_character(uint32_t self, uint32_t character)
{
    if (restore_stream_ostream_prepare(self) != 0u) {
        uint32_t base = restore_stream_virtual_base(self);
        if (R32(base + 0x30u) != 0u) {
            char text[2];
            text[0] = (char)(uint8_t)character;
            text[1] = 0;
            (void)restore_stream_ostream_format(self, 0x7027d4u,
                (uint32_t)(uintptr_t)text);
        } else {
            uint32_t byte_value = character & 0xffu;
            if (restore_stream_buffer_put(R32(base + 4u), byte_value)
                == 0xffffffffu) {
                if (restore_stream_io_virtual1(stream_buffer(self), 0x1cu,
                    byte_value) == 0xffffffffu)
                    stream_error(self, 6u);
            }
        }
        restore_stream_ostream_suffix(self);
    }
    return self;
}

uint32_t restore_stream_ostream_integer(uint32_t self, uint32_t value)
{
    char format[4];
    char prefix[4];
    char text[12];
    uint32_t i;
    for (i = 0u; i < 3u; ++i)
        format[i] = (char)R8(0x5d9a18u + i);
    format[3] = 0;
    for (i = 0u; i < 3u; ++i)
        prefix[i] = (char)R8(0x570580u + i);
    prefix[3] = 0;
    if (restore_stream_ostream_prepare(self) != 0u) {
        if (value != 0u) {
            uint32_t table = R32(self);
            uint32_t flags = R32(self + R32(table + 4u) + 0x24u);
            if ((flags & 0x60u) != 0u) {
                if ((flags & 0x40u) != 0u) {
                    char x = (char)((~(flags >> 4u) & 0x20u) | 0x58u);
                    format[1] = x;
                    prefix[1] = x;
                } else {
                    format[1] = 'o';
                }
                if ((R8(self + R32(table + 4u) + 0x24u) & 0x80u) != 0u)
                    prefix[0] = '0';
            } else if ((int32_t)value > 0 && (flags & 0x400u) != 0u) {
                prefix[0] = '+';
            }
        }
        (void)FUN_00561a36(text, format, (int32_t)value);
        (void)restore_stream_ostream_format(self,
            (uint32_t)(uintptr_t)prefix, (uint32_t)(uintptr_t)text);
        restore_stream_ostream_suffix(self);
    }
    return self;
}

uint32_t restore_stream_ostream_string(uint32_t self, uint32_t text)
{
    if (restore_stream_ostream_prepare(self) != 0u) {
        (void)restore_stream_ostream_format(self, 0x7027d4u, text);
        restore_stream_ostream_suffix(self);
    }
    return self;
}

static void stream_fill(uint32_t self, uint32_t count)
{
    while (count != 0u) {
        uint32_t base = restore_stream_virtual_base(self);
        uint32_t fill = (uint32_t)R8(base + 0x2cu);
        if (restore_stream_buffer_put(R32(base + 4u), fill) == 0xffffffffu)
            stream_error(self, 6u);
        --count;
    }
}

uint32_t restore_stream_ostream_format(uint32_t self, uint32_t prefix,
    uint32_t text)
{
    uint32_t prefix_length = FUN_00563380((const char *)(uintptr_t)prefix);
    uint32_t text_length = FUN_00563380((const char *)(uintptr_t)text);
    uint32_t base = restore_stream_virtual_base(self);
    uint32_t width = R32(base + 0x30u);
    uint32_t padding = width > text_length + prefix_length
        ? width - text_length - prefix_length : 0u;
    if ((R8(base + 0x24u) & 0x0au) == 0u) {
        uint32_t prior = padding;
        --padding;
        if ((int32_t)prior > 0) {
            stream_fill(self, prior);
            padding -= prior;
        }
    }
    if (prefix_length != 0u &&
        restore_stream_io_virtual2(stream_buffer(self), 0x14u, prefix,
            prefix_length) != prefix_length)
        stream_error(self, 6u);
    if ((R8(restore_stream_virtual_base(self) + 0x24u) & 8u) != 0u) {
        uint32_t prior = padding;
        --padding;
        if ((int32_t)prior > 0) {
            stream_fill(self, prior);
            padding -= prior;
        }
    }
    if (restore_stream_io_virtual2(stream_buffer(self), 0x14u, text,
        text_length) != text_length)
        stream_error(self, 6u);
    if ((R8(restore_stream_virtual_base(self) + 0x24u) & 2u) != 0u &&
        (int32_t)padding > 0)
        stream_fill(self, padding);
    return self;
}

uint32_t restore_stream_buffer_write(uint32_t self, uint32_t source,
    uint32_t count)
{
    uint32_t written = 0u;
    while (count != 0u) {
        uint32_t current;
        --count;
        if (R32(self + 8u) == 0u &&
            (current = R32(self + 0x1cu)) < R32(self + 0x20u)) {
            W8(current, R8(source));
            W32(self + 0x1cu, R32(self + 0x1cu) + 1u);
        } else if (restore_stream_io_virtual1(self, 0x1cu,
            (uint32_t)R8(source)) == 0xffffffffu) {
            return written;
        }
        ++source;
        ++written;
    }
    return written;
}

uint32_t restore_stream_buffer_read(uint32_t self, uint32_t destination,
    uint32_t count)
{
    uint32_t read = 0u;
    if (R32(self + 8u) != 0u) {
        if (R32(self + 0x0cu) == 0xffffffffu)
            W32(self + 0x0cu, restore_stream_io_virtual0(self, 0x20u));
        while (count != 0u) {
            --count;
            if (R32(self + 0x0cu) == 0xffffffffu)
                break;
            W8(destination, R8(self + 0x0cu));
            ++destination;
            ++read;
            W32(self + 0x0cu, restore_stream_io_virtual0(self, 0x20u));
        }
    } else {
        while (count != 0u &&
            restore_stream_io_virtual0(self, 0x20u) != 0xffffffffu) {
            uint32_t current = R32(self + 0x28u);
            uint32_t chunk = R32(self + 0x2cu) - current;
            if ((int32_t)chunk >= (int32_t)count)
                chunk = count;
            if ((int32_t)chunk > 0) {
                (void)FUN_00563400((void *)(uintptr_t)destination,
                    (const void *)(uintptr_t)current, chunk);
                destination += chunk;
                W32(self + 0x28u, R32(self + 0x28u) + chunk);
                read += chunk;
                count -= chunk;
            }
        }
    }
    return read;
}

uint32_t restore_stream_buffer_allocate(uint32_t self)
{
    uint32_t allocation = (uint32_t)(uintptr_t)FUN_00561a1d(0x200u);
    if (allocation == 0u)
        return 0xffffffffu;
    (void)restore_stream_buffer_set(self, allocation, allocation + 0x200u, 1u);
    return 1u;
}

uint32_t restore_stream_buffer_set(uint32_t self, uint32_t first,
    uint32_t last, uint32_t owned)
{
    if (R32(self + 4u) != 0u) {
        uint32_t old_first = R32(self + 0x10u);
        if (old_first != 0u)
            FUN_00561a2b((void *)(uintptr_t)old_first);
    }
    W32(self + 0x10u, first);
    W32(self + 4u, owned);
    W32(self + 0x14u, last);
    return last;
}

uint32_t restore_stream_buffer_putback(uint32_t self, uint32_t character)
{
    if (R32(self + 0x24u) < R32(self + 0x28u))
        return restore_stream_buffer_putback_direct(self, character);
    if (restore_stream_io_virtual3(self, 0x0cu, 0xffffffffu, 1u, 1u)
        == 0xffffffffu)
        return 0xffffffffu;
    if (R32(self + 8u) == 0u) {
        uint32_t end = R32(self + 0x2cu);
        if (end != 0u) {
            uint32_t current = R32(self + 0x28u);
            (void)FUN_00562880((void *)(uintptr_t)(current + 1u),
                (const void *)(uintptr_t)current, end - current - 1u);
            W8(R32(self + 0x28u), character);
        }
    }
    return character;
}

uint32_t restore_stream_buffer_putback_direct(uint32_t self,
    uint32_t character)
{
    uint32_t current = R32(self + 0x28u);
    if (R32(self + 0x24u) < current) {
        --current;
        W32(self + 0x28u, current);
        W8(current, character);
        return (uint32_t)(int32_t)(int8_t)(uint8_t)character;
    }
    return restore_stream_io_virtual1(self, 0x24u,
        (uint32_t)(int32_t)(int8_t)(uint8_t)character);
}

uint32_t restore_stream_filebuf_set_growth(uint32_t self, uint32_t growth)
{
    if (growth != 0u)
        W32(self + 0x34u, growth);
    return self;
}

uint32_t restore_stream_buffer_setbuf(uint32_t self, uint32_t buffer,
    uint32_t count)
{
    if (R32(self + 0x10u) != 0u)
        return 0u;
    if (buffer != 0u && count != 0u) {
        W32(self + 0x10u, buffer);
        W32(self + 8u, 0u);
        W32(self + 0x14u, buffer + count);
    } else {
        W32(self + 8u, 1u);
    }
    return self;
}

uint32_t restore_stream_buffer_default_seek(void)
{
    return 0xffffffffu;
}

uint32_t restore_stream_buffer_seek_position(uint32_t self, uint32_t offset,
    uint32_t mode)
{
    return restore_stream_io_virtual3(self, 0x0cu, offset, 0u, mode);
}
