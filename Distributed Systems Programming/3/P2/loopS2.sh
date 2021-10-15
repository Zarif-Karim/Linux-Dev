#!/bin/bash

#for looping server call of part two lab set 3

s=$1
r=$2

for l in 3 6 9 12
do
	for w in 3 6 9 12
	do
		for h in 3 6 9 12
		do
			./box_2_client $s $l $w $h 10 >> $r
		done
	done
done
