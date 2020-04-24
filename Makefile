all: parallel serial

parallel: merge.c
	gcc -fopenmp merge.c -o parallel

serial: merge_serial.c
	gcc merge_serial.c -o serial

run_pl: parallel
	./parallel $n

run_sl: serial
	./serial $n

clean:
	rm parallel serial
