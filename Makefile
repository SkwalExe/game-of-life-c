all: build

build: 
	mkdir -p build
	clang src/*.c -o build/golc

install:
	[ -f build/golc ] || (echo Please build first; exit 1)
	sudo cp build/golc /usr/local/bin/

uninstall:
	sudo rm -f /usr/local/bin/golc