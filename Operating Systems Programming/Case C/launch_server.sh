#!/bin/bash
### This script launches the logserver process in a new window.
### Magic is needed for OSX as I can't rely on xterm being installed!
### Only works when logged in via the console, not Putty/SSH
### It is not necessary to understand this script!


if [ $(uname) == "Darwin" ]
then

	osascript <<EOF
	set cmd to "cd " &  quoted form of "$PWD" & "; ./logserver; exit; "
	tell application "Terminal" to do script cmd
EOF
else
	if command -v xterm
	then
		xterm -title "Log Server" -e './logserver; \
			echo press enter to exit; read junk;' &
	elif command -v gnome-terminal
	then
		gnome-terminal -- bash -c  './logserver; \
			echo press enter to exit; read junk;' &
	else
		echo Cant launch a new window run logserver directly.
	fi

fi
