for i in 8 16 24 32 40 48 56 64; do
	res=`cpuset -l 0 ./encoding 1000000 100 10 $i`
	echo $i $res
done
