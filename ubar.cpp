#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>
#include <assert.h>
#include <unistd.h>
#include "config.h"

using namespace std;

#define NIL (0)

int main() {
    // Open the display
    Display *dpy = XOpenDisplay(NIL);
    assert(dpy);
    
    // Get the background and border colors
    XColor bg_color;
    XParseColor(dpy, DefaultColormap(dpy, 0), bg_col, &bg_color);
    XAllocColor(dpy, DefaultColormap(dpy, 0), &bg_color);

    XColor bord_color;
    XParseColor(dpy, DefaultColormap(dpy, 0), border_col, &bord_color);
    XAllocColor(dpy, DefaultColormap(dpy, 0), &bord_color);

    // Set window attributes
    XSetWindowAttributes attributes;
    attributes.override_redirect = True;
    attributes.background_pixel = bg_color.pixel;
    attributes.border_pixel = bord_color.pixel;
    
    // Create the window
    Window w = XCreateWindow(dpy, XDefaultRootWindow(dpy), x_pos, y_pos, width, height, border_size, CopyFromParent, InputOutput, CopyFromParent, CWBackPixel | CWOverrideRedirect | CWBorderPixel, &attributes);
    XSelectInput(dpy, w, StructureNotifyMask);
    XMapWindow(dpy, w);
    
    // Set the font and text color
    XftColor fg_color;
    Visual *visual = DefaultVisual(dpy, 0);
    Colormap cmap = DefaultColormap(dpy, 0);
    XftFont *font = XftFontOpenName(dpy, 0, font_name);
    if (!font) {
        cerr << "Xft: failed to load font " << font_name << endl;
        exit(1);
    }

    if (!XftColorAllocName(dpy, visual, cmap, fg_col, &fg_color)) {
        cerr << "Xft: Failed to allocate color " << fg_col << endl;
        exit(1);
    }

    XftDraw *draw = XftDrawCreate(dpy, w, visual, cmap);
    
	// Wait for the window to be mapped
    for (;;) {
		XEvent e;
		XNextEvent(dpy, &e);
		if (e.type == MapNotify) {
				break;
		}
    }
    
	// Main event loop
    for (;;) {
		// Update the bar
		system(bar_script);

        char* text_string;
		string line;

		ifstream myfile (info_file);
	
		// Load the bar text from the specified file
		if (myfile.is_open()) {
			getline (myfile, line);
			myfile.close();
			text_string = const_cast<char*>(line.c_str());
		} else {
			text_string = "ubar v1.1.0";
		}
        
		// Draw the info to the bar
		XftDrawStringUtf8(draw, &fg_color, font, 3, 11, (const FcChar8 *)text_string, strlen(text_string));
        XFlush(dpy);
		sleep(0.25);
		XClearWindow(dpy, w);
    }
}
