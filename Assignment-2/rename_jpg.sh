#!/bin/bash
# Script to prefix current date to all .jpg files

# Get current date in YYYY-MM-DD format
current_date=$(date +%F)

# Loop through all jpg files
for file in *.jpg; do
    # Check if file exists to handle cases where no jpgs are found
    if [ -f "$file" ]; then
        new_name="${current_date}-${file}"
        mv "$file" "$new_name"
        echo "Renamed: $file -> $new_name"
    else
        echo "No .jpg files found."
        break # Exit loop if no files match
    fi
done
