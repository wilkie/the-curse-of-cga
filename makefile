CC=i586-pc-msdosdjgpp-gcc
CFLAGS=

.PHONY: game
game: src/*.c src/*.h
	$(CC) $(CFLAGS) src/*.c -o CURSECGA.exe

.PHONY: png2cga
png2cga:
	gcc ./tools/src/png2cga.c -o ./tools/$@ -lm

.PHONY: bitmaps
bitmaps:
	mkdir -p ASSETS
	./tools/png2cga dev/bitmaps/font.png ASSETS/FONT.BIN -mask
	./tools/png2cga dev/bitmaps/figure.png ASSETS/FIGURE.BIN -mask
	./tools/png2cga dev/bitmaps/tileset.png ASSETS/TILESET.BIN
	./tools/png2cga dev/bitmaps/hud.png ASSETS/HUD.BIN
	./tools/png2cga dev/bitmaps/objects.png ASSETS/OBJECTS.BIN -mask
	./tools/png2cga dev/bitmaps/logo.png ASSETS/LOGO.BIN
	./tools/png2cga dev/bitmaps/title.png ASSETS/TITLE.BIN
	./tools/png2cga dev/bitmaps/ending.png ASSETS/ENDING.BIN -mask

.PHONY: tilemaps
tilemaps:
	mkdir -p ASSETS
	./tools/tmx2bin dev/tilemaps/basemap.tmx ASSETS/MAP.BIN

assets: bitmaps tilemaps

.PHONY: tmx2bin
tmx2bin:
	(cd tools/src/tmx2bin; go build -o ../../$@)

linecount:
	(cd src; find . -name '*.c' | xargs wc -l)
	(cd src; find . -name '*.h' | xargs wc -l)
