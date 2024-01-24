XFT_CFLAGS=$(shell pkg-config --cflags freetype2)
LDFLAGS=-lX11
XFT_LDFLAGS=$(shell pkg-config --libs freetype2) -lXft

build: ubar.cpp
	sed -i 's@~@'"${HOME}"'@g' config.h
	g++ $(XFT_CFLAGS) $(LDFLAGS) $(XFT_LDFLAGS) ubar.cpp -o ubar
