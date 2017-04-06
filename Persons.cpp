#include <stdio.h>
#include <string.h>
#include "Persons.include"

struct Person {
	char first_name[32];
	char surname[32];
	int age;
	char ssn[32];
};

int main(int argc, char const *argv[]) {
	Person people[64];
	int num_people;
	while (true) {
		puts(
			"Hello welcome to the personprogram.\n"
			"Enter q to quit\n"
			"Enter p to print persons\n"
			"Enter a to add person\n"
			"Enter d to delete a person by ssn\n"
			);
		char command;
		while((command = getchar()) == '\n');
		while(getchar() != '\n');
		switch (command) {
			case 'q': return 0;
			case 'p': {
				printf("Your people are:\n");
				for (int i = 0; i < num_people; ++i)
					printf("- %s %s %i %s \n",people[i].first_name,people[i].surname, people[i].age, people[i].ssn);
			} break;
			case 'a': {
				puts("Enter first_name");
				getline(people[num_people].first_name, sizeof(people[num_people].first_name));
				puts("Enter surname");
				getline(people[num_people].surname, sizeof(people[num_people].surname));
				puts("Enter age");
				scanf("%i", &people[num_people].age);
				puts("Enter social security number");
				getline(people[num_people].ssn, sizeof(people[num_people].ssn));
				++num_people;
			} break;
			case 'd': {
				puts("Enter social security number");
				char ssn[32];
        getline(ssn, sizeof(ssn));
				for (int i = 0; i < num_people; ++i)
					if (strcmp(people[i].ssn, ssn) == 0)
						people[i] = people[--num_people];
			} break;
		}
	}
	return 0;
}
