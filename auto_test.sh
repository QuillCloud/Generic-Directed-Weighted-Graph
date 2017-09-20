#!/bin/bash
compiler="g++ -std=c++14 -Wall -Werror -O2 -o"
num=1
curl='/usr/bin/curl'
url="http://www.cse.unsw.edu.au/~abadim/CS6771_Ass3/"
CURLARGS="-f -s -S -k"

rm -rf TEST
mkdir TEST
cp Graph.h Graph.tem ./TEST
cd TEST

echo "Downloading test files....."
for num in {1,2,3,4,5,6,7,8m,8c,9m,9c,10,11,12,13}
do
	downloadTest="$($curl $CURLARGS $url"test"$num.cpp > test$num.cpp)"
	downloadResult="$($curl $CURLARGS $url"result"$num.txt > result$num.txt)"
	$downloadTest
	$downloadResult
done
echo "Download completed!"

for file in test{1,2,3,4,5,6,7,8m,8c,9m,9c,10,11,12,13}.cpp
do
	filename=`basename $file`
	echo "------------------Testing case is $filename------------------"

	target=`echo "$filename" | cut -d '.' -f1|sed 's/test/result/g'`
	test_file="$target.txt"
	command_line="$($compiler $target $filename)"
	val="valgrind --log-file=$target.val ./$target"
	$val > $target.out
	if diff -w "$target.out" "$test_file"
	then
		echo "Test case $filename passed!!!!!"
	else
		echo "Test case $filename fail :("
	fi
	echo "Memory Usage"
	cat $target.val |  egrep 'heap usage'
	cat $target.val |  egrep 'lost'
done

cd ..