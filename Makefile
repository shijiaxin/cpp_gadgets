mc_throughput:
	g++ -lpthread mc_throughput.cpp -o mc_throughput.out
file_ops:
	g++ file_ops.cpp -o file_ops.out
zipf:
	g++ zipf.cpp -o zipf.out
sort:
	g++ sort.cpp -o sort.out
clean:
	rm *.out
