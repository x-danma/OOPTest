CC ?= g++
CFLAGS := $(CFLAGS) -g -Wall -Wextra -Ilib/include
LFLAGS :=
OPTFLAGS := -O3 -ffast-math

all: *.cpp *.include
	$(CC) $(CFLAGS) Persons.cpp -o Persons.out $(LFLAGS)

release: *.cpp *.include
	$(CC) $(CFLAGS) $(OPTFLAGS) Persons.cpp -o Persons.out $(LFLAGS)
