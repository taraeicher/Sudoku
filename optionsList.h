//Entry in a list of options for a given box

typedef struct option
{
	char current;
	char *previous;
	char* next;
}Option;

//Pointer to a list of options for a given box

typedef option* OptionList;

//Add an option to the end of the list. Do not permit duplicates.

void appendOption(Option optionToAppend, OptionList list);

//Find an option in a list.

option findOption(char content, OptionList list);

//Delete an option from the list.

void deleteOption(Option optionToDelete, OptionList list);