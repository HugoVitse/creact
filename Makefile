creact:
	mkdir -p build
	mkdir -p release
	emcc -Wall -Wextra -g -o build/creact.js src/*.c -I./lib/raylib-6.0_webassembly/include -L./lib/raylib-6.0_webassembly/lib -lraylib.web -s USE_GLFW=3 -s FETCH=1 -s WASM=1 -s FORCE_FILESYSTEM=1 -s EXPORTED_RUNTIME_METHODS='["ccall", "cwrap", "HEAPU8"]' -s EXIT_RUNTIME=0
	cp static/* release
	cp build/* release
