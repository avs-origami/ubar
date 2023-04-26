#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <X11/Xlib.h>
#include <assert.h>
#include <unistd.h>
#include "config.h"
using namespace std;

#define NIL (0)

int main() {
    // Open the display
    Display *dpy = XOpenDisplay(NIL);
    assert(dpy);
    
	// Get the foreground and background colors
    XColor fg_color;
    XParseColor(dpy, DefaultColormap(dpy, 0), fg_col, &fg_color);
    XAllocColor(dpy, DefaultColormap(dpy, 0), &fg_color);
    
    XColor bg_color;
    XParseColor(dpy, DefaultColormap(dpy, 0), bg_col, &bg_color);
    XAllocColor(dpy, DefaultColormap(dpy, 0), &bg_color);

	// Set window attributes
    XSetWindowAttributes attributes;
    attributes.override_redirect = True;
    attributes.background_pixel = bg_color.pixel;
    
	// Create the window
    Window w = XCreateWindow(dpy, XDefaultRootWindow(dpy), x_pos, y_pos, width, height, 0, CopyFromParent, InputOutput, CopyFromParent, CWBackPixel | CWOverrideRedirect, &attributes);
    XSelectInput(dpy, w, StructureNotifyMask);
    XMapWindow(dpy, w);
    
	// Create a graphics context and set the font and text color
    GC gc = XCreateGC(dpy, w, 0, NIL);
    XSetForeground(dpy, gc, fg_color.pixel);
    XFontStruct* font_info = XLoadQueryFont(dpy, font_name);
    
    if (!font_info) {
		fprintf(stderr, "XLoadQueryFont: failed loading font '%s'\n", font_name);
		exit(-1);
    }
    
    XSetFont(dpy, gc, font_info->fid);
    
	// Wait for the window to be mapped
    for(;;) {
		XEvent e;
		XNextEvent(dpy, &e);
		if (e.type == MapNotify) {
				break;
		}
    }
    
	// Main event loop
    for(;;) {
        char* text_string;
		string line;

		ifstream myfile (info_file);
	
		// Load the bar text from the specified file
		if (myfile.is_open()) {
			getline (myfile, line);
			myfile.close();
			text_string = const_cast<char*>(line.c_str());
		} else {
			text_string = "ubar v1.0.0";
		}
        
		// Draw the info to the bar
		XDrawString(dpy, w, gc, 3, 11, text_string, strlen(text_string));
        XFlush(dpy);
		sleep(1);
		XClearWindow(dpy, w);
    }
}