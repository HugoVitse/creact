creact:
	mkdir -p build
	mkdir -p release
	emcc -Wall -Wextra -g -o build/creact.js src/*.c -I./lib/include -L./lib/lib -I./include -lraylib.web -s USE_GLFW=3 -s FETCH=1 -s WASM=1 -s FORCE_FILESYSTEM=1 -s EXPORTED_RUNTIME_METHODS='["ccall", "cwrap", "HEAPU8"]' -s EXIT_RUNTIME=0
	cp static/* release
	cp build/* release

creact-library:
	mkdir -p build/lib
	mkdir -p build/tmp
	emcc -c src/creact.c -I./lib/include -I./include
	mv *.o build/tmp
	ar rcs build/lib/libcreact.a build/tmp/creact.o
	rm -r build/tmp
	cp build/lib/libcreact.a myApp/lib/lib/libcreact.a
	cp include/creact.h myApp/lib/include/creact.h
	cp lib/include/* myApp/lib/include
	cp lib/lib/* myApp/lib/lib
	zip -r creact.zip myApp
