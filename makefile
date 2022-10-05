
np= 4
all: compile
	rm time_result.csv
	touch time_result.csv
	echo  "tempo" >>  time_result.csv
	mpiexec -np $(np) main
	echo "."
	mpiexec -np $(np) main
	echo "."
	mpiexec -np $(np) main
	echo "."
	mpiexec -np $(np) main
	echo "."
	mpiexec -np $(np) main
	echo "."
	mpiexec -np $(np) main
	echo "."
	mpiexec -np $(np) main
	echo "."
	mpiexec -np $(np) main
	echo "."
	mpiexec -np $(np) main
	echo "."
	mpiexec -np $(np) main
	echo "."
	python3 compute.py

compile:
	mpicc -o main main.c -lm