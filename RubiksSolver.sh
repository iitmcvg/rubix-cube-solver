#!/bin/bash
if [[ $# -eq 6 ]]; then 
./Config $1 $2 $3 $4 $5 $6; 
./Rubix $(cat input.txt); 
fi

