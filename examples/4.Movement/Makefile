CPP = g++
OPTS =  -I"../../include" -I"/usr/X11R6/include" -L"/usr/X11R6/lib" -L"../../source" -L"../../source/jpeglib" -L"../../source/zlib" -lIrrlicht -lGL -lXxf86vm -lXext -lX11 -lz -ljpeg

all:
	$(CPP) main.cpp -o example $(OPTS)

clean:
	rm example
