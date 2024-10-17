source = $(wildcard *.ino)
target = build/$(source).with_bootloader.hex
opt = -t 1

ALL: $(target)

$(target): $(source)
ifdef color
	./am2 $(opt) $<
else
	GCC_COLORS="" ./am2 --no-color $(opt) $<
endif

clean:
	rm -rf build

mrproper:
	rm -rf build out
