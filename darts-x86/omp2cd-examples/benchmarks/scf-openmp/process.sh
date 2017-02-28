#!/bin/sh


if [ $# -eq 0 ]
then
	echo "Usage: $0 NameOfCSV"
	exit
fi

begin=$1

if [ $# -gt 1 ]
then
	end=$2
else
	end=$begin
fi




echo "Implementation, Version, Threads/node, Nodes, atoms,layout,init,onel,twoel,diag,dens,eigen,total,iterations,compute,fraction" > $1.csv

for file in *.txt
do

     if [ -s $file ]

     then
	#scheduling=(basename $file)
	#version=(basename $file)
	#threads=(basename $file)

        scheduling=`echo $file| cut -f 2  -d"."`
	version=`echo $file | cut -f 3  -d"."  | cut -f 2 -d"n"`
	threads=`echo $file | cut -f 4  -d"."  | cut -f 2 -d"s"`

	atoms=`grep atoms $file | sed -e 's/.* //' -e 's/[^0-9]//g'`
	iterations=$(( 1 + `grep iter= $file |tail -n1 | sed -e 's/ iter=  //' |cut -f1 -d,` ))
	mem=`grep -A1 cache $file| tail -n1 | sed -e 's/[^0-9]//g'`
	mb=$(( mem / (1024 * 1024) ))
	
	total=`grep elapsed $file | sed -e 's/.* //' -e 's/[^.0-9]//g'`
	times=`grep -A2 diag $file |tail -n1 | sed -r -e 's/ +/ /g' -e 's/^ //'`

      
	init=`echo $times | cut -f 1 -d" "`
	onel=`echo $times | cut -f 2 -d" "`
	twoel=`echo $times | cut -f 3 -d" "`
	diag=`echo $times | cut -f 4 -d" "`
	dens=`echo $times | cut -f 5 -d" "`
	eigen=`echo $times | cut -f 7 -d" " | sed -e 's/[^.0-9]//g'`
	
	integrals=`grep -A2 fraction $file |tail -n1 | sed -r -e 's/ +/ /g' -e 's/^ //'`
	compute=`echo $integrals | cut -f4 -d" "`
	fraction=`echo $integrals | cut -f6 -d" "`
	
	echo openmp_$scheduling, $version, $threads,1,$atoms,linear,$iterations,$init,$onel,$twoel,$diag,$dens,$eigen,$total,$(( compute / $iterations)),$fraction>>$1.csv
     fi

done
