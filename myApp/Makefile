SRC = $(shell find src -name '*.c')

creact:
	mkdir -p build
	mkdir -p release
	emcc -o build/creact.js $(SRC) -I./include -I./lib/include -L./lib/lib -lraylib.web -lcreact -s USE_GLFW=3 -s FETCH=1 -s WASM=1 -s FORCE_FILESYSTEM=1 -s EXPORTED_RUNTIME_METHODS='["ccall", "cwrap", "HEAPU8"]' -s EXIT_RUNTIME=0
	cp static/* release
	cp build/* release
