for i in 2 4 6 8 10 12 14 16 18 20 30 40 50 60 70 80 90 100 200 300 400 500 600 700 800 900 1000 2000 4000 6000 8000 10000 20000 40000 60000 80000 100000; do
	res=`cpuset -l 0 ./encoding 1000000 $i 5 64`
	echo $i $res
done