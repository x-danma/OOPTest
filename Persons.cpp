#include <cstdio>
#include <cstring>
using namespace std;
struct Person
{
	char firstName[32];
	char surName[32];
	int age;
	char socialNumber[32];
};
int main(int argc, char const *argv[])
{
	Person people[64];
	int numPeople;
	while (true) 
	{
		puts(
			"Hello welcome to the personprogram.\n"
			"Enter q to quit\n"
			"Enter p to print persons\n"
			"Enter a to add person\n"
			"Enter d to delete a person by ssn\n"
			);
		while((command = getchar()) == '\n');
		while(getchar() != '\n');
		switch (command)
		{
			case 'q':return 0;
			case 'p':
			{
				printf("Your people are:\n");
				for (int i = 0; i < numPeople; ++i)
					printf("- %s %s %i %s \n",people[i].firstName,people[i].surName, people[i].age, people[i].socialNumber);
			}break;
			case 'a':
			{
				puts("Enter firstName");
				scanf("%32s", &people[numPeople].firstName);
				puts("Enter surName");
				scanf("%32s", &people[numPeople].surName);
				puts("Enter age");
				scanf("%i", &people[numPeople].age);
				puts("Enter socialNumber");
				scanf("%32s", &people[numPeople].socialNumber);
				numPeople++;
			}break;
			case 'd':
			{
				puts("Enter ssn");
				char ssn[32];
				scanf("%32s", ssn);
				for (int i = 0; i < numPeople; ++i)
					if (strcmp(people[i].socialNumber, ssn) == 0)
						people[i] = people[--numPeople];
			}break;
		}
	}
	return 0;
}