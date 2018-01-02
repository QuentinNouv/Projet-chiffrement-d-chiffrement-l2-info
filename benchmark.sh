#!/bin/sh
cd tests
ls | grep "^keys[0-3]*$" | xargs rm -rfmkdir tests/keys0
mkdir tests/keys0
mkdir tests/keys1
mkdir tests/keys2
mkdir tests/keys3
cd ..
if [ $1 -eq 1 ]
then
	bash test_script.sh
	bash test_crack.sh
else
	i=0
	sum=0
	min=0
	max=0
	if [ $# -eq 1 ]
	then
		echo
		echo "  | Pass |  Time  |"
		echo "  |------|--------|"
	fi
	while [ $i -ne $1 ]
	do
		start=`gdate +%s%N | cut -b1-13`
		bash test_crack_wo_echo.sh
		end=`gdate +%s%N | cut -b1-13`
		timer=`expr $end - $start`
		i=`expr $i + 1`
		sum=`expr $sum + $timer`
		if [ $i -eq 1 ]
		then
			min=$timer
			max=$timer
		elif [ $timer -gt $max ]
		then
			max=$timer
		elif [ $timer -lt $min ]
		then
			min=$timer
		fi
		if [ $# -eq 1 ]
		then
			printf "  | %04d | %d ms |\n" $i $timer
		fi
	done
	moy=`expr $sum / $1`
	echo
	echo "Average : $moy ms | Min : $min ms (-$(( $moy - $min  ))) | Max : $max ms (+$(( $max - $moy )))"
	fi
exit 0
