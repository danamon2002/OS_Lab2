#!/bin/bash

# Loop through all .c files that start with "test_"
for file in test_*.c; do
    # Get base file name (exclude extension).
    base_name="${file%.c}"

    # Compile with output file sharing name with .c file
    gcc "$file" -o "$base_name"
    
    # Check if compilation succeeded (return code 0?)
    if [ $? -eq 0 ]; then
        echo "Compiled $file . Running..."
        # Run the compiled executable
        ./"$base_name"
        echo "Output from $file concluded."
    # Report if compilation fails.
    else
        echo "ERROR: Compilation failed for $file."
    fi
done
