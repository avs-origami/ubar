build:
	sed -i 's@~@'"$$HOME"'@' config.h
	g++ ubar.cpp -o ubar -lX11
