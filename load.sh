#!/bin/bash

# Function to perform calculations
generate_load() {
    while true; do
        echo "sqrt(100)" | bc > /dev/null
        sleep 0.001  # Adjust sleep duration to control the load
    done
}

# Generate load in the background
generate_load &

# Store the background job's PID
LOAD_PID=$!

# Output instructions
echo "Generating controlled CPU load. Press [ENTER] to stop."

# Wait for ENTER key to stop the script
read -r -d '' _ </dev/tty

# Kill the background job
kill "$LOAD_PID"
echo "CPU load stopped."
