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
	enum MenuType {NORMAL_MENU, PRETTY_MENU, SIMPLE_MENU, SELECT_MENU };
	MenuType  menu_type = NORMAL_MENU;
	Person* people = malloc(64*sizeof(Person));
	int num_people = 0, capacity = 64;
	while (true) {
		switch (menu_type) {
			case NORMAL_MENU:		
				puts(
					"Hello welcome to the personprogram.\n"
					"Enter q to quit\n"
					"Enter p to print persons\n"
					"Enter a to add person\n"
					"Enter d to delete a person by ssn\n"
					"Enter m to switch menu\n"
				);
				break;
			case PRETTY_MENU:
				puts(
					"<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3\n"
					"<3 Hello welcome to the personprogram.<3\n"
					"<3          Enter q to quit           <3\n"
					"<3      Enter p to print persons      <3\n"
					"<3        Enter a to add person       <3\n"
					"<3  Enter d to delete a person by ssn <3\n"
					"<3     Enter m to switch menu         <3\n"
					"<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3\n"
				);
				break;
			case SIMPLE_MENU:
				puts("Enter m to switch menu\n");
				break;
			case SELECT_MENU:
				puts(
					"Enter n for NORMAL\n"
					"Enter p for PRETTY\n"
					"Enter s for SIMPLE\n"
				);
				break;
		}
		char command;
		while((command = getchar()) == '\n');
		while(getchar() != '\n');
		switch (menu_type) {
			case NORMAL_MENU: 
			case PRETTY_MENU:
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
						if (num_people == capacity) {
							capacity *= 2;
							people = realloc(people, capacity*sizeof(Person));
						}
						people[num_people++] = p;
						break;
					case 'd':
						char ssn[32];
						get_string("Enter social security number", ssn, sizeof(ssn));
						for (int i = 0; i < num_people; ++i)
							if (strcmp(people[i].ssn, ssn) == 0)
								people[i] = people[--num_people];
						break;
					case 'm':
						menu_type = SELECT_MENU;
						break;
					default:
						puts("Not a valid option");
						break;
				}
				break;
			case SIMPLE_MENU:
				if (command== 'm')
					menu_type = SELECT_MENU;
				else 
					puts("Not valid option");
				break;
			case SELECT_MENU:
				switch (command) {
					case 'n': menu_type = NORMAL_MENU; break;
					case 'p': menu_type = PRETTY_MENU; break;
					case 's': menu_type = SIMPLE_MENU; break;
				}
				break;

		}
	}
	return 0;
}
