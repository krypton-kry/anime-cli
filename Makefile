all: build

build:
	[ -d build ] || mkdir build
	cc src/*.c -o build/anime-cli -lmpv -lcurl -lgumbo -DLOG_USE_COLOR

run:
	./build/anime-cli

.PHONY: run build