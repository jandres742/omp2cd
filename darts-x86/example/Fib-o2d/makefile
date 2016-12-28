CC=g++
CFLAGS=-std=c++11 -fopenmp
TARGET=fib-o2d
SOURCE=$(TARGET).cpp

all:
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)
omp:
	$(CC) $(CFLAGS) -DOMPCODE -o $(TARGET) $(SOURCE)
run:	all
	./$(TARGET)

clean:
	rm -rf *.o $(TARGET)
