#!/bin/sh

battery() {
		echo $(acpi | grep 'Battery 0' | cut -d',' -f2 | sed 's/ //g')
}

echo "$(date) | $(battery)" > ~/.config/ubar/info.txt