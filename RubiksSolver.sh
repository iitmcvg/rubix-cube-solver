#!/bin/bash
echo "Enter the path address of the 6 images:"
read addr1
read addr2
read addr3
read addr4
read addr5
read addr6
./a.out $addr1 $addr2 $addr3 $addr4 $addr5 $addr6
./Rubix $(cat input.txt)

