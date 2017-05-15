CC ?= g++
CFLAGS := $(CFLAGS) -Wall -Wextra -Ilib/include
LFLAGS := -Llib -lSDL2 -lm -lGL -lstdc++
OPTFLAGS := -O3 -ffast-math

all: *.cpp *.include
	$(CC) $(CFLAGS) Persons.cpp -o Persons.out $(LFLAGS)

release: *.cpp *.include
	$(CC) $(CFLAGS) $(OPTFLAGS) Persons.cpp -o Persons.out $(LFLAGS)
