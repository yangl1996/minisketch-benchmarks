for i in 2 4 6 8 10 12 14 16 18 20 30 40 50 60 70 80 90 100 200 300 400 500 600 700 800 900 1000 2000; do
	runtimes=`expr 20000000 / $i / $i / $i`
	res=`cpuset -l 0 ./decoding $i $runtimes 64`
	echo $i $res
done
