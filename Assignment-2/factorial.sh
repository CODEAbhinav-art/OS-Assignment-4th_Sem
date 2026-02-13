#!/bin/bash
#script to calculate factorial
#AUTHORS: Aman Sharma, Aditya Kumar, Aayush Praveen, Abhinav Mishra

echo "Enter a number:"
read num

factorial=1

#check the negative input
if [$num -lt 0]; then
    echo "Error: factorial ke liye 0 se badi value enter karein!"
else
    #loop from 1 to num
    for ((i=1; i<num; i++))
    do
      factorial=$((factorial * i))
    done

    echo "Factorial of $num is $factorial"
fi
