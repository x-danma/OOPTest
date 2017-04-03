//Personregister CRUD på person]
#include <cstdio>

int mySum(int* a, int b)
{
	return *a+b;
}

void whatIsHello(int* a) 
{
	printf("My hello is %lu \n", a);
}


int main(int argc, char const *argv[])
{
	/* code */

	int hello;

	hello = 2;
	mySum(&hello, 3);

	whatIsHello(&hello);

	return 0;
}


//person.cpp 2 person1 person2