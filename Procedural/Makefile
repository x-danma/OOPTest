CC ?= gcc
CFLAGS := $(CFLAGS) -Wall -Wextra
OPTFLAGS := -O3 -ffast-math

all: *.cpp *.include
	$(CC) $(CFLAGS) Persons.cpp -o Persons.out

release: *.cpp *.include
	$(CC) $(CFLAGS) $(OPTFLAGS) Persons.cpp -o Persons.out
