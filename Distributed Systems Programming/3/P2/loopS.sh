#!/bin/bash

#for looping server call of part two lab set 3

#r=$1

for l in {1..10}
do
	for w in {1..10}
	do
		for h in {1..10}
		do
			for m in {1..10}
			do
			./box_2_client watto.scem.westernsydney.edu.au $l $w $h $m >> output.txt
			done
		done
	done
done
