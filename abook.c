/*
 * abook.c
 *
 * Description: Address Book Example
 *
 */

#include "tools.h"

#define DAT_FILENAME "abook.dat"

int main(int argc, char *argv[])
{
    struct termios term;
    struct address_book *s = NULL;
    char str1[STR_LENGTH];
    char str2[STR_LENGTH];
    char str3[STR_LENGTH];
    char str4[STR_LENGTH];
    char cmd;

    printf("------------------- Address Book Example ----------------------\n");
    printf("--- L (List) * A (Add) * S (Search) * D (Delete) * Q (Exit) ---\n");
    printf("---------------------------------------------------------------\n");

    data_load(DAT_FILENAME);

    tcgetattr(fileno(stdin), &term);
    term_mode(0, &term);

    do {
	cmd = getchar();

	switch (cmd) {
	case 108:	// 'L'
	    printf("----------------------- Address Book --------------------------\n");
	    data_print();
	    break;
	case 97:	// 'A'
	    printf("------------------------ Add Record ---------------------------\n");
	    term_mode(1, &term);
	    printf("\rLast Name: ");
	    if (!fgets(str3, STR_LENGTH-1, stdin)) *str3 = '\0';
	    printf("\rFirst Name: ");
	    if (!fgets(str1, STR_LENGTH-1, stdin)) *str1 = '\0';
	    printf("\rMiddle Name: ");
	    if (!fgets(str2, STR_LENGTH-1, stdin)) *str2 = '\0';
	    printf("\rE-mail: ");
	    if (!fgets(str4, STR_LENGTH-1, stdin)) *str4 = '\0';
	    if (add_record(trim(str1), trim(str2), trim(str3), trim(str4)))
		fprintf(stderr, "Error: Not enough memory\n");
	    else
		printf("\rRecord added.\n");
	    term_mode(0, &term);
	    break;
	case 115:	// 'S'
	    printf("----------------------- Search Record -------------------------\n");
	    term_mode(1, &term);
	    printf("\rSearch Last Name: ");
	    if (!fgets(str3, STR_LENGTH-1, stdin)) *str3 = '\0';
	    printf("\rSearch First Name: ");
	    if (!fgets(str1, STR_LENGTH-1, stdin)) *str1 = '\0';
	    printf("\rSearch Middle Name: ");
	    if (!fgets(str2, STR_LENGTH-1, stdin)) *str2 = '\0';
	    printf("\rSearch E-mail: ");
	    if (!fgets(str4, STR_LENGTH-1, stdin)) *str4 = '\0';
	    s = search_record(trim(str1), trim(str2), trim(str3), trim(str4));
	    if (s) {
		printf("---------------------- Search Results -------------------------\n");
		printf("%s\t%s\t%s\t%s\n", s->fname, s->mname, s->lname, s->email);
	    } else {
		printf("\rNothing found.\n");
	    }
	    term_mode(0, &term);
	    break;
	case 100:	// 'D'
	    if (s && del_record(s)) {
		s = NULL;
		printf("\rRecord removed.\n");
	    } else {
		printf("\rPlease use 'Search' to select the record to remove.\n");
	    }
	    break;
	}
    } while (cmd != 113); // 113 = 'Q'

    term_mode(1, &term);

    data_save(DAT_FILENAME);
    free_book();

    return 0;
}

