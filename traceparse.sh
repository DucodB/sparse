#!/bin/bash

if [ $# -lt 2 ]; then
    echo "Usage: $0 <filename> <application> [additional parameters for the application]"
    exit 1
fi

filename="$1"
app="$2"
temp_filename="temp_${filename}"

if [ ! -e "$temp_filename" ]; then
	#Run strace and store in file temp_filename
	strace -o "$temp_filename" "${@:2}"

	#Run ./sparse on generated strace data and store in file filename
	./sparse "$temp_filename" "$filename"

	#Remove strace output file
	rm "$temp_filename"
else
	echo "File '$temp_filename' already exists. Execution stopped to avoid unexpected file deletion."
	exit 1
fi
