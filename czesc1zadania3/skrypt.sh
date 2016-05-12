#!/bin/bash
if [ $# -lt 1 ]
then 
   echo "Except executable program name. If second argument is passed and it is neg, then excepted resault is negated."
   exit
fi
a1=( 20 5 5 5 5 5 5 2 2 2)
a2=( 0  3 2 4 2 0 0 0 0 0)
a3=( 0  0 0 0 0 1 1 2 3 1)
b1=( 10 5 5 5 5 5 5 6 6 6)
b2=( 0  2 3 2 4 0 0 0 0 0)
b3=( 0  0 0 0 0 1 2 3 3 2)
r=(  1  0 0 1 1 0 1 1 0 0)
if [ $# -gt 1 ]
then
   if [ $2 == "neg" ]
   then
      r=( 0 1 1 0 0 1 0 0 1 1)
   fi
fi

for ind in `seq 0 9`; do
  out=`$1 ${a1[$ind]} ${a2[$ind]} ${a3[$ind]} ${b1[$ind]} ${b2[$ind]} ${b3[$ind]}`
  if [ "$out" != "${r[$ind]}" ]
  then
    echo "in column " $ind
    echo "input" ${n[$ind]} ${m[$ind]} 
    echo "output actual" $out
    echo "output expected" ${r[$ind]}
  fi
done
