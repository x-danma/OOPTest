#include <cstdio>
#include <cstdlib>
#include <ctime>

int main(int argc, char const *argv[])
{
	char firstname[32];
	char lastname[32];
	int age;
	char ssn[32];

	srand(time(0));

	for (int num_people = 0; num_people < 100000; ++num_people) {
		int i,end;
		for (i = 0, end = 5 + rand()%10; i < end; ++i) firstname[i] = 'a' + rand()%('z'+1-'a');
		firstname[end] = '\0';
		for (i = 0, end = 5 + rand()%10; i < end; ++i) lastname[i] = 'a' + rand()%('z'+1-'a');
		lastname[end] = '\0';
		for (i = 0; i < 13; ++i) ssn[i] = '0' + rand()%('9'+1-'0');
		ssn[8] = '-';
		ssn[13] = '\0';
		age = rand()%100;
		printf("%s\t%s\t%i\t%s\n", firstname, lastname, age, ssn);
	}
	return 0;
}