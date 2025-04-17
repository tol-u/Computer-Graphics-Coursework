#!/bin/sh
bindir=$(pwd)
cd /Users/tolu/Desktop/Computer-Graphics-Coursework/source/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "x" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		GDB_COMMAND-NOTFOUND -batch -command=$bindir/gdbscript  /Users/tolu/Desktop/Computer-Graphics-Coursework/Build/Release/Computer_Graphics_Coursework 
	else
		"/Users/tolu/Desktop/Computer-Graphics-Coursework/Build/Release/Computer_Graphics_Coursework"  
	fi
else
	"/Users/tolu/Desktop/Computer-Graphics-Coursework/Build/Release/Computer_Graphics_Coursework"  
fi
