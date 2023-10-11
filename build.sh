#!/bin/sh
echo "Starting build script..."
gcc src/main.c src/output_spooler.c src/input.c -o bin/main
echo "... Build complete"
