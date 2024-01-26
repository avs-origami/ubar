#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <memory>
#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>
#include <assert.h>
#include <unistd.h>
#include "config.h"

using namespace std;

#define NIL (0)
string exec_output(const char* cmd, const char* dummy);

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
		string bar_left = exec_output(bar_script_left, "ubar v1.4.0");
        string bar_right = exec_output(bar_script_right, "avs-origami.github.io");
        char* text_left = const_cast<char*>(bar_left.c_str());
        char* text_right = const_cast<char*>(bar_right.c_str());

        // Get the size of the right-side info in pixels
        XGlyphInfo info;
        XftTextExtentsUtf8(dpy, font, (FcChar8*)text_right, strlen(text_right), &info);
        int right_xpos = width - info.xOff - 3;

		// Draw the info to the bar
		XftDrawStringUtf8(draw, &fg_color, font, 3, 11, (const FcChar8 *)text_left, strlen(text_left));
        XftDrawStringUtf8(draw, &fg_color, font, right_xpos, 11, (const FcChar8 *)text_right, strlen(text_right));
        XFlush(dpy);
		sleep(0.25);
		XClearWindow(dpy, w);
    }
}

string exec_output(const char* cmd, const char* dummy) {
    shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) return dummy;
    
    char buffer[128];
    string result = "";
    
    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != NULL)
            result += buffer;
    }
    
    return result;
}
