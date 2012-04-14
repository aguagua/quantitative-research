# make file to compile, run and clean

# C++11 requires g++ version >= 4.3
compile:
	g++ main.cc profiler.cc sort.cc --std=c++0x -pthread -o psort

run:
	./psort > psort.log

clean:
	rm -rf run_time.txt sorted_array.txt psort.log