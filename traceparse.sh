#!/bin/bash

if [ $# -lt 2 ]; then
    echo "Usage: $0 <filename> <application> [additional parameters for the application]"
    exit 1
fi

filename="$1"
app="$2"
file_res="res_${filename}"

if [ ! -e "$file_res" ]; then
	#Run strace and store in file filename
	strace -o "$filename" "${@:2}"

	#Run ./sparse on generated strace data and store in file filename_res
	./sparse "$filename" "$file_res"

	#Remove strace output file
	rm "$filename"
else
	echo "File '$file_res' already exists."
	exit 1
fi
