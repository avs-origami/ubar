#!/bin/bash

battery() {
	echo $(acpi | grep 'Battery 0' | cut -d',' -f2 | sed 's/ //g')
}

datetime() {
	echo $(date | cut -d':' -f1,2)
}

workspaces() {
	desk=$(($(xdotool get_desktop)+1))
	desk_sel="[$desk]"
	desks=$(wmctrl -d | awk '{print $1}' | tr '\n' ' ' | sed 's/ //g')

	desky=$(for (( i=0; i<${#desks}; i++)); do
		echo "$((${desks:$i:1}+1))" | tr '\n' ' '
	done)

	echo $desky | sed 's/ /  /g' | sed "s/$desk/$desk_sel/g" | sed 's/]  /] /g' | sed 's/  \[/ \[/g' | sed 's/1 / 1 /g' | sed 's/ 9/ 9 /g'
}

echo " $(workspaces) |  $(datetime)  |  :D" > ~/.config/ubar/info.txt
