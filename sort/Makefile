# make file to compile, run and clean
compile:
	g++ main.cc profiler.cc sort.cc --std=c++0x -pthread -o psort

run:
	./psort > psort.log

clean:
	rm -rf run_time.txt sorted_array.txt psort.log