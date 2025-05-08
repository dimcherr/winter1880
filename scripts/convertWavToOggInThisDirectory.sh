#!/bin/bash

# Check if ffmpeg is installed
if ! command -v ffmpeg &> /dev/null; then
    echo "ffmpeg is not installed. Please install it first."
    exit 1
fi

# Check if directory is provided as argument, else use current directory
if [ -z "$1" ]; then
    DIR="."
else
    DIR="$1"
fi

# Verify directory exists
if [ ! -d "$DIR" ]; then
    echo "Directory $DIR does not exist."
    exit 1
fi

# Convert all .wav files to .ogg
for file in "$DIR"/*.wav; do
    if [ -f "$file" ]; then
        output="${file%.*}.ogg"
        echo "Converting $file to $output..."
        ffmpeg -i "$file" -c:a libvorbis "$output" -y
        if [ $? -eq 0 ]; then
            echo "Successfully converted $file"
        else
            echo "Error converting $file"
        fi
    fi
done

echo "Conversion complete."