#!/bin/bash
# Menu-driven Address Book
# Data is stored in 'contacts.txt'

DB_FILE="contacts.txt"

while true; do
    echo "---------------------------"
    echo "    ADDRESS BOOK MENU      "
    echo "---------------------------"
    echo "1. Create/Reset Address Book"
    echo "2. View Records"
    echo "3. Insert Record"
    echo "4. Delete Record"
    echo "5. Modify Record"
    echo "6. Exit"
    echo -n "Enter choice: "
    read choice

    case $choice in
        1) 
            > "$DB_FILE" # Truncates/Creates empty file
            echo "Address Book created/reset." 
            ;;
        2) 
            if [ -s "$DB_FILE" ]; then
                # Formatting output with column for readability
                echo -e "Name\tPhone\tEmail"
                echo "--------------------------------"
                # Translate ':' to tabs for display
                tr ':' '\t' < "$DB_FILE" 
            else
                echo "Address Book is empty."
            fi
            ;;
        3) 
            echo -n "Enter Name: "; read name
            echo -n "Enter Phone: "; read phone
            echo -n "Enter Email: "; read email
            echo "${name}:${phone}:${email}" >> "$DB_FILE"
            echo "Record saved."
            ;;
        4) 
            echo -n "Enter Name to delete: "; read search_name
            # grep -v excludes the line, output saved to tmp, then moved back
            grep -v "^$search_name:" "$DB_FILE" > temp.txt && mv temp.txt "$DB_FILE"
            echo "Record deleted (if it existed)."
            ;;
        5) 
            echo -n "Enter Name to modify: "; read mod_name
            # Check if exists
            if grep -q "^$mod_name:" "$DB_FILE"; then
                # Delete old
                grep -v "^$mod_name:" "$DB_FILE" > temp.txt && mv temp.txt "$DB_FILE"
                # Ask for new
                echo "Enter New Details:"
                echo -n "Name: "; read n
                echo -n "Phone: "; read p
                echo -n "Email: "; read e
                echo "${n}:${p}:${e}" >> "$DB_FILE"
                echo "Record updated."
            else
                echo "Record not found."
            fi
            ;;
        6) 
            echo "Exiting..."
            exit 0 
            ;;
        *) 
            echo "Invalid option." 
            ;;
    esac
done
