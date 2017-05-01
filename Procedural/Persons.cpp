#include <stdio.h>
#include <string.h>
#include "Persons.include"

struct Person {
	char first_name[32];
	char surname[32];
	int age;
	char ssn[32];
};

void get_string(const char* msg, char* buf, int num) {
	puts(msg);
	getline(buf, num);
}
void get_int(const char* msg, int* result) {
	int success;
	while (true) {
		puts(msg);
		success = scanf("%i", result); while(getchar() != '\n');
		if (success) return;
		puts("Please enter a number\n----------------");
	}
}

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
			case 'q':
				return 0;
			case 'p':
				puts("Your people are:");
				for (int i = 0; i < num_people; ++i)
					printf("- %s %s %i %s \n",people[i].first_name,people[i].surname, people[i].age, people[i].ssn);
				break;
			case 'a':
				Person p = {0};
				get_string("Enter first name", p.first_name, sizeof(p.first_name));
				get_string("Enter surname", p.surname, sizeof(p.surname));
				get_int("Enter age", &p.age);
				get_string("Enter social security number", p.ssn, sizeof(p.ssn));
				people[num_people++] = p;
				break;
			case 'd':
				char ssn[32];
				get_string("Enter social security number", ssn, sizeof(ssn));
				for (int i = 0; i < num_people; ++i)
					if (strcmp(people[i].ssn, ssn) == 0)
						people[i] = people[--num_people];
				break;
			default:
				puts("Not a valid option");
				break;
		}
	}
	return 0;
}
