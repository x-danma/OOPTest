#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include "Persons.include"
#include <limits.h>

#define LEN(a) (sizeof(a)/sizeof(*a))
#define MAX(a,b) ((a) < (b) ? (b) : (a))
#define MIN(a,b) ((b) < (a) ? (b) : (a))

#if 1
#define DEBUG(stmt) stmt
#else
#define DEBUG(stmt)
#endif

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

struct PersonList {
  int count, capacity;
  Person* people;
};

int location_of_person_in_persist_file(int i) {
  return sizeof(int) + i*sizeof(Person);
}

void delete_person(PersonList* l, int i, FILE *persist_file) {
  /* TODO: do file first, then do in memory list. */
  --l->count;
  l->people[i] = l->people[l->count];

  /* update number of people in file */
  fseek(persist_file, 0, SEEK_SET);
  fwrite(&l->count, sizeof(l->count), 1, persist_file);

  /* read in last person, and insert it at position i */
  /* TODO: check return values */
  Person p;
  fseek(persist_file, location_of_person_in_persist_file(l->count), SEEK_SET);
  fread(&p, sizeof(p), 1, persist_file);
  fseek(persist_file, location_of_person_in_persist_file(i), SEEK_SET);
  fwrite(&p, sizeof(p), 1, persist_file);

  if (ferror(persist_file)) {perror("Failed to persist to data.bin"); exit(1);}
  fflush(persist_file);
}

void push_person(PersonList* l, Person p, FILE *file) {
  /* check if person already exist */
  bool exists = false;
  for (int i = 0; i < l->count; ++i) {
    if (strcmp(p.ssn, l->people[i].ssn) == 0) {
      exists = true;
      break;
    }
  }

  if (exists) {
    printf("\nPerson with social security number %s already exists!\n\n", p.ssn);
    return;
  }

  /* TODO: try writing to file first, then do in memory list */

  if (l->count == l->capacity) {
    l->capacity *= 2;
    l->people = (Person*) realloc(l->people, l->capacity*sizeof(Person));
  }
  l->people[l->count++] = p;

  /* update number of people in file */
  fseek(file, 0, SEEK_SET);
  fwrite(&l->count, sizeof(l->count), 1, file);

  /* append person at the end of the file */
  fseek(file, location_of_person_in_persist_file(l->count - 1), SEEK_SET);
  int err = fwrite(&p, sizeof(p), 1, file);
  if (err < 0) {perror("Failed to persist to data.bin"); exit(1);}
  fflush(file);
}

/* use only if you know what you are doing. Otherwise, use push_person, which persists to file */
void push_person__nopersist(PersonList* l, Person p) {
  if (l->count == l->capacity) {
    l->capacity *= 2;
    l->people = (Person*) realloc(l->people, l->capacity*sizeof(Person));
  }
  l->people[l->count++] = p;
}

PersonList personlist_create(int capacity = 64) {
  PersonList result = {0, capacity, (Person*) malloc(capacity*sizeof(Person))};
  return result;
}

enum MenuType {NORMAL_MENU, PRETTY_MENU, SIMPLE_MENU, SELECT_MENU};

void print_person(Person person, MenuType menu_type) {
  printf("%s%s %s %i %s \n", menu_type == NORMAL_MENU ? " - " : " <3 ", person.first_name, person.surname, person.age, person.ssn);
}

void load_people_from_text_file(FILE *input, PersonList *person_list, FILE *persist_file) {
  clock_t t = clock();

  for (int row = 0; !feof(input); ++row) {
    Person p = {};
    int r = fscanf(input, "%s\t%s\t%i\t%s\n", p.first_name, p.surname, &p.age, p.ssn);
    if (r != 4) {printf("Invalid person on row %i\n", row); return;}
    if (persist_file)
      push_person(person_list, p, persist_file);
    else
      push_person__nopersist(person_list, p);
  }

  printf("Completed in %f seconds\n", (float)(clock()-t)/CLOCKS_PER_SEC);
}

void load_people_from_binary_file(FILE *input, PersonList *person_list, FILE *persist_file) {
  clock_t t = clock();

  int num_people;
  fread(&num_people, sizeof(num_people), 1, input);
  DEBUG(printf("Number of people in binary file: %i\n", num_people);)

  for (int i = 0; i < num_people; ++i) {
    Person p = {};
    int r = fread(&p, sizeof(p), 1, input);
    if (r < 0) {printf("File corruption detected. Please contact your administrator\n"); exit(1);}
    if (persist_file)
      push_person(person_list, p, persist_file);
    else
      push_person__nopersist(person_list, p);
  }

  printf("Completed in %f seconds\n", (float)(clock()-t)/CLOCKS_PER_SEC);
}

/* maybe you should use the load_people_from_file instead, this does not persist to persistent storage ! */
void load_people_from_binary_file__nopersist(FILE *input, PersonList *person_list) {
  load_people_from_binary_file(input, person_list, NULL);
}

int main(int, char const **) {
  MenuType  menu_type = NORMAL_MENU;
  PersonList person_list = personlist_create();

  /* Read people from persistent storage */
  FILE *persist_file = fopen("data.bin", "r");
  if (persist_file) {
    load_people_from_binary_file__nopersist(persist_file, &person_list);
    fclose(persist_file);
  }

  /* Append new people to this persistent storage */
  persist_file = fopen("data.bin", "r+");
  if (!persist_file) {
    persist_file = fopen("data.bin", "w+");
    if (!persist_file) {perror("Could not create data.bin"); return 1;}

    /* write num people */
    int num_people = 0;
    fwrite(&num_people, sizeof(num_people), 1, persist_file);
  }

  while (true) {
    /* Print menu */
    switch (menu_type) {
      case NORMAL_MENU:   
        puts(
          "Hello welcome to the personprogram.\n"
          "Enter q to quit\n"
          "Enter p to print persons\n"
          "Enter a to add person\n"
          "Enter d to delete a person by ssn\n"
          "Enter m to switch menu\n"
          "Enter f to read from file\n"
          "Enter s to search for person"
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
          "<3     Enter f to read from file      <3\n"
          "<3    Enter s to search for person    <3\n"
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

    /* Get input */
    char command;
    while((command = getchar()) == '\n');
    while(getchar() != '\n');

    /* Process input */
    switch (menu_type) {
      case NORMAL_MENU:
      case PRETTY_MENU:
        switch (command) {
          case 'q':
            return 0;

          case 'p':
            puts("Your people are:");
            for (int i = 0; i < person_list.count; ++i)
              print_person(person_list.people[i], menu_type);
            break;

          case 'a': {
            Person p = {};
            get_string("Enter first name", p.first_name, sizeof(p.first_name));
            get_string("Enter surname", p.surname, sizeof(p.surname));
            get_int("Enter age", &p.age);
            get_string("Enter social security number", p.ssn, sizeof(p.ssn));

            push_person(&person_list, p, persist_file);
          } break;

          case 'd': {
            char ssn[32];
            get_string("Enter social security number", ssn, sizeof(ssn));
            for (int i = 0; i < person_list.count; ++i)
              if (strcmp(person_list.people[i].ssn, ssn) == 0)
                delete_person(&person_list, i--, persist_file);
          } break;

          case 'm':
            menu_type = SELECT_MENU;
            break;

          case 'f': {
            char filename[256];
            get_string("Enter filename", filename, sizeof(filename));

            FILE* f = fopen(filename, "r");
            if (!f) {printf("Could not open file: %s\n", strerror(errno)); break;}

            load_people_from_text_file(f, &person_list, persist_file);
            fclose(f);
          } break;

          case 's': {
            char substr[128];
            get_string("Please enter firstname", substr, sizeof(substr));
            for (int i = 0; i < person_list.count; ++i)
              if (strstr(person_list.people[i].first_name, substr) || strstr(person_list.people[i].surname, substr) || strstr(person_list.people[i].ssn, substr))
                  print_person(person_list.people[i], menu_type);                  
          } break;

          default:
            puts("Not valid option");
            break;
        }
        break;
      case SIMPLE_MENU:
        if (command == 'm')
          menu_type = SELECT_MENU;
        else 
          puts("Not valid option");
        break;
      case SELECT_MENU:
        switch (command) {
          case 'n': menu_type = NORMAL_MENU; break;
          case 'p': menu_type = PRETTY_MENU; break;
          case 's': menu_type = SIMPLE_MENU; break;
          default: puts("Not valid option");
        }
        break;
    }
  }
  return 0;
}
