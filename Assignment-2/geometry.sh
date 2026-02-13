#!/bin/bash
# Geometry Calculator using 'bc' for floating point math

while true; do
    echo "--------------------------"
    echo "   GEOMETRY CALCULATOR    "
    echo "--------------------------"
    echo "1. Area of Circle"
    echo "2. Circumference of Circle"
    echo "3. Area of Rectangle"
    echo "4. Area of Square"
    echo "5. Exit"
    echo -n "Select operation: "
    read choice

    case $choice in
        1)
            echo -n "Enter Radius: "
            read r
            # scale=2 sets decimal precision. 3.14159 is Pi.
            area=$(echo "scale=2; 3.14159 * $r * $r" | bc)
            echo "Area of Circle: $area"
            ;;
        2)
            echo -n "Enter Radius: "
            read r
            circ=$(echo "scale=2; 2 * 3.14159 * $r" | bc)
            echo "Circumference: $circ"
            ;;
        3)
            echo -n "Enter Length: "
            read l
            echo -n "Enter Width: "
            read w
            area=$(echo "scale=2; $l * $w" | bc)
            echo "Area of Rectangle: $area"
            ;;
        4)
            echo -n "Enter Side: "
            read s
            area=$(echo "scale=2; $s * $s" | bc)
            echo "Area of Square: $area"
            ;;
        5)
            echo "Exiting..."
            exit 0
            ;;
        *)
            echo "Invalid choice."
            ;;
    esac
    echo "" # Empty line for readability
done
