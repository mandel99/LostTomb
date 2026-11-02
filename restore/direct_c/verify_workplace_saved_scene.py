"""Offline workplace layers using the existing save's complete footprints.

The save and licensed assets are read only. Native DCL decoding supplies the
stored building/grid bytes. Food enumeration alone is controlled to resource8;
the stock, animation, food, clipping and raster providers execute together.
With --ground, the real ground pass runs before the upper layers as well.
The isolated saved footprints exclude surrounding terrain and actor occlusion.
"""
from verify_workplace_pixels import *


def saved_chunks():
    source = ROOT/'tools/verify_compression_emulation.py'
    decoder = {'__file__': str(source), '__name__': 'saved_scene_decoder'}
    exec(compile(source.read_text().split('sample=bytes.fromhex', 1)[0],
                 str(source), 'exec'), decoder)
    path = ROOT/'build/debug-startup-game/Save/Restore/autosave.sav'
    saved = path.read_bytes()
    descriptors = [struct.unpack_from('<5I', saved, 12+i*20)
                   for i in range(struct.unpack_from('<I', saved, 8)[0])]
    schema = next(i for i, d in enumerate(descriptors)
                  if d[0] == 0 and d[2:4] == (20, 300))
    wanted = {0x7999d8, 0xb67d1c, 0xb285cc, 0xb4e6fc,
              0xb7482c, 0xadc36c, 0xaf598c}
    chunks, position = {}, 6012
    for codec, address_, size, count, _ in descriptors[schema+1:]:
        length = size*count
        if codec:
            marker = struct.unpack_from('<I', saved, position)[0]
            position += 4
            stored = length if marker == 0x80000000 else marker
        else:
            marker, stored = None, length
        payload = saved[position:position+stored]
        position += stored
        if address_ not in wanted:
            continue
        if codec and marker != 0x80000000:
            result = decoder['native'](0, length, 0xffffffff, payload)
            assert result['result'] == 1, (hex(address_), result['result'])
            payload = bytes.fromhex(result['output'])[:length]
        assert len(payload) == length
        chunks[address_] = payload
    assert position == len(saved) and set(chunks) == wanted
    print('Save SHA256:', hashlib.sha256(saved).hexdigest(), flush=True)
    return chunks


def verify():
    from verify_world_detail import prepare
    chunks = saved_chunks()
    general, pixels = archive('Pharaoh_General', 11907,
        [*range(12757, 12790), *range(13198, 13203), 13991,
         *range(13994, 14002)])
    ambient, worker_pixels = archive('SprAmbient', 15831, range(18477, 18501))
    shift = 0xc00000
    ambient = {image: struct.pack('<I', struct.unpack_from('<I', record)[0]+shift)+record[4:]
               for image, record in ambient.items()}
    for c, u, state in machines:
        u.mem_map(0x2fe00000, 0x200000)
        u.mem_map(PIXELS, 0x1000000)
        u.mem_map(SCREEN, 0x200000)
    cases = 0
    captures = []
    for building, (width, height) in itertools.product(
            [8, 9, 10], [(640, 480), (800, 600), (1024, 768)]):
        buildings = chunks[0x7999d8]
        kind = struct.unpack_from('<H', buildings, building*264+16)[0]
        tiles = [tile for tile in range(228*228)
                 if struct.unpack_from('<H', chunks[0xb4e6fc], tile*2)[0] == building]
        root = min(tiles)
        results = []
        for c, u, state in machines:
            prepare(u, state, 0, kind, 0, building=building, image=0,
                    width=width, height=height, origin=(88, 42))
            # Remove the synthetic one-cell scene produced by prepare().
            u.mem_write(0x70e494, struct.pack('<I', 0xfff0000)*(459*231))
            for at, data in chunks.items():
                u.mem_write(at, data)
            for tile in tiles:
                dy, dx = divmod(tile-root, 228)
                row = 80+dx+dy
                col = 40+(dx-dy+(row&1))//2
                put(u, 0x70e494+(row*231+col)*4, tile)
            u.mem_write(PIXELS, pixels)
            u.mem_write(PIXELS+shift, worker_pixels)
            u.mem_write(SCREEN, b'\x5a'*(width*height*2))
            for image, record in {**general, **ambient}.items():
                u.mem_write(0x10d69f8+image*64, record)
            for at, value in [(0xf56a84, 13198), (0xf56f34, 18477),
                              (0xf5648c, 13994), (0xe92e18, SCREEN),
                              (0xe38d94, width*2), (0xe38d9c, width),
                              (0xe38d98, height), (0xd3b550, height),
                              (0xd3b554, 0), (0xd3b558, width), (0xd3b55c, 0)]:
                put(u, at, value)
            byte(u, 0xe38dda, 0)
            def resources(cpu, args):
                put(cpu, 0xf53dd8, 1)
                put(cpu, 0xf53ddc, 8)
                return 0
            hook(c, state, 0x5596d0, 0, resources)
            if '--ground' in sys.argv:
                run(c, u, 0x430ef0)
            run(c, u, 0x436b60, [0])
            results.append(bytes(u.mem_read(SCREEN, width*height*2)))
        assert results[0] == results[1], ('saved scene', building, width, height,
            [(i, a, b) for i, (a, b) in enumerate(zip(*results)) if a != b][:12])
        assert results[1] != b'\x5a'*(width*height*2)
        if '--capture' in sys.argv and width == 800:
            from PIL import Image, ImageChops
            rgb = bytearray(width*height*3)
            for index, (pixel,) in enumerate(struct.iter_unpack('<H', results[1])):
                rgb[index*3:index*3+3] = bytes(
                    (((pixel >> 10) & 31)*255//31,
                     ((pixel >> 5) & 31)*255//31, (pixel & 31)*255//31))
            image = Image.frombytes('RGB', (width, height), bytes(rgb))
            background = Image.new('RGB', image.size, image.getpixel((0, 0)))
            bounds = ImageChops.difference(image, background).getbbox()
            assert bounds
            captures.append((building, image.crop(bounds)))
        cases += 1
    if captures:
        from PIL import Image, ImageDraw
        sheet = Image.new('RGB', (sum(im.width+16 for _, im in captures),
                                  max(im.height for _, im in captures)+38), (245, 245, 245))
        draw = ImageDraw.Draw(sheet)
        left = 8
        for building, image in captures:
            draw.text((left, 6), 'Building %d' % building, fill=(20, 20, 20))
            sheet.paste(image, (left, 28))
            left += image.width+16
        output = ROOT/('artifacts/workplace-saved-ground-scene.png' if '--ground' in sys.argv
                       else 'artifacts/workplace-saved-scene.png')
        sheet.save(output)
        print('Offline saved building layers; ground pass:', '--ground' in sys.argv, output)
    print('PASS complete saved footprints, food/worker/stock layers and raster; ground pass:',
          '--ground' in sys.argv, 'cases:', cases)
    print('Verified target SHA256:', target_sha)


if __name__ == '__main__':
    verify()
