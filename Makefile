all: 
	make -C src
	mkdir -p bin
	mv src/subsonic bin/subsonic

clean:
	make -C src clean


