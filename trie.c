#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>



//defining structure for a trie node
struct node {
	struct node *alphabetsList[26]; // array of pointers to the nodes starting with a alphabet
	struct node *digitsList[10]; // array of pointers to the nodes starting with a digit
	struct node *dot; // pointer to the node starting with char '.'
	struct node *comma;// pointer to the node starting with char ','
	struct node *dash;// pointer to the node starting with char '-'
	struct node *quote;// pointer to the node starting with char '''
	struct node *slash;
	struct node *and;
	struct node *not;
	char data; // data is set to 1 when a words ended in this node, otherwise data is 0
};
typedef struct node *trie;



//initialize a trie
trie initTrie() {
	int i;
	trie t, *list;

	t = (trie) malloc(sizeof(struct node)); // allocate meemory for a trie node

	t->data = 0;

	list = t->alphabetsList; // get the alphabet pointer array and make every elements to be null
	for(i = 0; i < 26 ; i++)
		list[i] = NULL;

	list = t->digitsList; // get the digit pointer array and make every elements to be null
	for(i = 0; i < 10 ; i++)
		list[i] = NULL;

	t->dot = NULL; // make dot pinter to be null
	t->comma = NULL;// make comma pinter to be null
	t->dash = NULL;// make dash pinter to be null
	t->quote = NULL;// make quote pinter to be null
	t->slash = NULL;
	t->and = NULL;
	t->not = NULL;

	return t;
}


// insert character 'c' in the string 's'  with length 'length' at position 'position'
void insertChar(char *s, int length, char c, int position ) {
	int i;
	for(i = length-1; i>=position; i--)
		s[i+1] = s[i];
	s[position] = c;
	s[length + 1] = '\0';
}

// delete from the string 's'  with length 'length' at position 'position', the last char in 's' is '\0'
void deleteChar(char *s, int length, int position ) {
	int i;
	for(i = position; i < length; i++)
		s[i] = s[i+1];
}


// change a string to lower case
void toLower(char *s) {
	int length, i;
	char asciiDiff = 'A' - 'a';
	length = strlen(s);
	for(i = 0; i < length; i++) {
		if(s[i] >= 'A' && s[i] <= 'Z')
			s[i] = s[i] - asciiDiff ;
	}
}

// return 1 if string 's' is found in trie 't', consider that null string can't come in this function, zero length string can come
int search(trie t, char *s) {

	int i, index, size;
	char asciia = 'a', ascii0 = '0' ;
	trie *p;
	
	if(t == NULL)
		return 0;

	size = strlen(s);
		
	// check all the characters one by one from first to last of the word
	for(i = 0; i < size ; i++) {

		if( s[i] >= asciia && s[i] <= 'z' ) { // if the char is an alphabet
			index = s[i] - asciia ; 
			p = t->alphabetsList;
			if( p[index] == NULL )
				return 0;
			t = p[index];
		}

		else if( s[i] >= ascii0 && s[i] <= '9' ) {// if the char is a digit
			index = s[i] - ascii0 ; 
			p = t->digitsList;
			if( p[index] == NULL )
				return 0;
			t = p[index];

		}

		else if (s[i] == '.') { // if the char is a dot
			if(t->dot == NULL)
				return 0;
			t = t->dot;
		}

		else if (s[i] == ',') { // if the char is an comma
			if(t->comma == NULL)
				return 0;
			t = t->comma;
		}

		else if (s[i] == '-') { //if the char is an dash
			if(t->dash == NULL)
				return 0;
			t = t->dash;
		}

		else if (s[i] == '\'') { //if the char is an quote
			if(t->quote == NULL)
				return 0;
			t = t->quote;
		}

		else if (s[i] == '/') { //if the char is an quote
			if(t->slash == NULL)
				return 0;
			t = t->slash;
		}

		else if (s[i] == '&') { //if the char is an quote
			if(t->and == NULL)
				return 0;
			t = t->and;
		}

		else if (s[i] == '!') { //if the char is an quote
			if(t->not == NULL)
				return 0;
			t = t->not;
		}

		else
			return 0;		
	}

	if(t->data == 1) // if the word exists	
		return 1;
	return 0;
}


// insert the word 's' into the trie 't'
int insertWord (trie t, char *s) {

	int i, index, size;
	char asciia = 'a', ascii0 = '0' ;
	trie *p;

	size = strlen(s);
	toLower(s);
		
	for(i = 0; i < size ; i++) {

		if( s[i] >= asciia && s[i] <= 'z' ) { // if the char is an alphabet
			index = s[i] - asciia ; 
			p = t->alphabetsList;
			if( p[index] == NULL )
				p[index] = initTrie();
			t = p[index];
		}

		else if( s[i] >= ascii0 && s[i] <= '9' ) { // if the char is a digit
			index = s[i] - ascii0 ; 
			p = t->digitsList;
			if( p[index] == NULL )
				p[index] = initTrie();
			t = p[index];

		}

		// IF the character is other than alphanumeric

		else if (s[i] == '.') {
			if(t->dot == NULL)
				t->dot = initTrie();
			t = t->dot;
		}

		else if (s[i] == ',') {
			if(t->comma == NULL)
				t->comma = initTrie();
			t = t->comma;
		}

		else if (s[i] == '-') {
			if(t->dash == NULL)
				t->dash = initTrie();
			t = t->dash;
		}

		else if (s[i] == '\'') {
			if(t->quote == NULL)
				t->quote = initTrie();
			t = t->quote;
		}

		else if (s[i] == '/') {
			if(t->slash == NULL)
				t->slash = initTrie();
			t = t->slash;
		}

		else if (s[i] == '&') {
			if(t->and == NULL)
				t->and = initTrie();
			t = t->and;
		}

		else if (s[i] == '!') {
			if(t->not == NULL)
				t->not = initTrie();
			t = t->not;
		}

		else
			return 0;		
	}
	if(t->data == 1)
		return 0;

	t->data = 1;
	return 1;	
}

// Delete the word 's' into the trie 't'
int deleteWord (trie t, char *s) {

	int i, index, size;
	char asciia = 'a', ascii0 = '0' ;
	trie *p;

	size = strlen(s);
		
	for(i = 0; i < size ; i++) {

		if( s[i] >= asciia && s[i] <= 'z' ) {
			index = s[i] - asciia ; 
			p = t->alphabetsList;
			if( p[index] == NULL )
				return 0;
			t = p[index];
		}

		else if( s[i] >= ascii0 && s[i] <= '9' ) {
			index = s[i] - ascii0 ;
			p =  t->digitsList;
			if( p[index] == NULL )
				return 0;
			t = p[index];

		}

		else if (s[i] == '.') {
			if(t->dot == NULL)
				return 0;
			t = t->dot;
		}

		else if (s[i] == ',') {
			if(t->comma == NULL)
				return 0;
			t = t->comma;
		}

		else if (s[i] == '-') {
			if(t->dash == NULL)
				return 0;
			t = t->dash;
		}

		else if (s[i] == '\'') {
			if(t->quote == NULL)
				return 0;
			t = t->quote;
		}

		else if (s[i] == '/') {
			if(t->slash == NULL)
				return 0;
			t = t->slash;
		}

		else if (s[i] == '&') {
			if(t->and == NULL)
				return 0;
			t = t->and;
		}

		else if (s[i] == '!') {
			if(t->not == NULL)
				return 0;
			t = t->not;
		}

		else
			return 0;		
	}
	if(t->data == 0)
		return 0;

	t->data = 0;
	return 1;	
}


// find in the trie by changing one single character to the word 's'
int changeFind(trie t, char *s) {

	int i, length, j, count = 0;
	char c, temp;

	length = strlen(s);

	for(i = 0; i < length; i++) {
		temp = s[i];

		c = 'a';
		for(j = 0; j < 26; j++) { // change to alphabet
			s[i] = c + j;
			if(search(t, s) == 1) {
				printf("%s\n", s);
				count++;
			}
		}

		c = '0';
		for(j = 0; j < 10; j++) { // change to digit
			s[i] = c + j;
			if(search(t, s) == 1) {
				printf("%s\n", s);
				count++;
			}
		}


		// change to other characters
		s[i] = '.';
		if(search(t, s) == 1) {
			printf("%s\n", s);
			count++;

		}

		s[i] = '\'';
		if(search(t, s) == 1) {
			printf("%s\n", s);
			count++;
		}

		s[i] = '-';
		if(search(t, s) == 1)
			printf("%s\n", s);

		s[i] = ',';
		if(search(t, s) == 1) {
			printf("%s\n", s);
			count++;
		}

		s[i] = '&';
		if(search(t, s) == 1) {
			printf("%s\n", s);
			count++;
		}

		s[i] = '/';
		if(search(t, s) == 1) {
			printf("%s\n", s);
			count++;
		}

		s[i] = '!';
		if(search(t, s) == 1) {
			printf("%s\n", s);
			count++;
		}


		s[i] = temp;
	}

	if(count > 0)
		printf("\nTotal %d words found in the trie changing one character\n\n", count);

	else
		printf("\nNo word found changing one character in the input word\n\n");

	return count;
}

// find words by inserting any one character to the input word
int insertFind(trie t, char *s) {

	int i, length, j, count = 0;
	char c;

	length = strlen(s);

	for(i = 0; i <= length; i++) {

		insertChar(s, length,'!', i);
		if(search(t, s) == 1) {
			printf("%s\n", s);
			count++;
		}

		c = 'a';
		for(j = 0; j < 26; j++) {
			s[i] = c + j;
			if(search(t, s) == 1) {
				printf("%s\n", s);
				count++;
			}
		}

		c = '0';
		for(j = 0; j < 10; j++) {
			s[i] = c + j;
			if(search(t, s) == 1) {
				printf("%s\n", s);
				count++;
			}
		}

		s[i] = '.';
		if(search(t, s) == 1) {
			printf("%s\n", s);
			count++;
		}

		s[i] = '-';
		if(search(t, s) == 1) {
			printf("%s\n", s);
			count++;
		}

		s[i] = ',';
		if(search(t, s) == 1) {
			printf("%s\n", s);
			count++;
		}

		s[i] = '\'';
		if(search(t, s) == 1) {
			printf("%s\n", s);
			count++;
		}

		s[i] = '/';
		if(search(t, s) == 1) {
			printf("%s\n", s);
			count++;
		}

		s[i] = '&';
		if(search(t, s) == 1) {
			printf("%s\n", s);
			count++;
		}
	
		deleteChar(s, length +1 , i);
	}

	if(count > 0)
		printf("\nTotal %d words found in the trie inserting one character\n\n", count);

	else
		printf("\nNo word found inserting one character in the input word\n\n");

	return count;
}

// load words in a file to the trie 't'
void loadWord(trie t) {

	char s[100], c;
	FILE *fp = fopen("/usr/share/dict/words", "r");
	int count = 0;

	if(fp == NULL) {
		printf("File can not be opened\n");
		return;
	}

	while( ( c=getc(fp)) != EOF ) {
		if(c == '\n') {
			s[count] = '\0';
			count = 0;
			insertWord(t, s);	
		}
		else {
			s[count] = c;
			count++;
		}
	}

	if(count > 0) {
		s[count] = '\0';
		insertWord(t, s);	
	}

	fclose(fp);
}


// free memory allocated for a trie
void freeTrie(trie t) {
	int i;

	if(t == NULL)
		return;

	for(i = 0; i < 26 ;i++)
		freeTrie((t->alphabetsList)[i]);

	for(i = 0; i < 10 ;i++)
		freeTrie((t->digitsList)[i]);

	freeTrie(t->dot);
	freeTrie(t->comma);
	freeTrie(t->dash);
	freeTrie(t->quote);
	freeTrie(t->slash);
	freeTrie(t->and);
	freeTrie(t->not);

	free(t);
}




// delete any character from search word and check if resulted word exists in the trie
int deleteFind(trie t, char *s) {
	int length, i, find, count = 0;
	char c;
	length = strlen(s);

	for(i = 0; i < length; i++) {
		c = s[i];
		deleteChar(s, length , i);
		find = search(t, s);
		if(find == 1) {
			printf("%s\n", s);
			count++;
		}
		insertChar(s, length-1, c, i);
	}

	if(count > 0)
		printf("\nTotal %d words found in the trie deleting one character\n\n", count);

	else
		printf("\nNo word found deleting a character in the input word\n\n");
	
	return count;
}


// Swap any  two characters from search word and check if resulted word exists in the trie
int swapFind(trie t, char *s) {
	int length, i, j, find, count = 0;
	char c;
	length = strlen(s);

	for(i = 0; i < length; i++)
		for(j = i+1; j < length ; j++) {
			s[i] = s[i] ^ s[j] ;
			s[j] = s[i] ^ s[j] ;
			s[i] = s[i] ^ s[j] ;

			find = search(t, s);

			if(find == 1) {
				printf("%s\n", s);
				count++;
			}

			s[i] = s[i] ^ s[j] ;
			s[j] = s[i] ^ s[j] ;
			s[i] = s[i] ^ s[j] ;
	}

	if(count > 0)
		printf("\nTotal %d words found in the trie swapping two characters\n\n", count);

	else
		printf("\nNo word found swapping two characters in the input word\n\n");
	
	return count;
}


// main function
int main() {
	char s[100], enter;
	int choice, output, count = 0;
	trie t = NULL;
	clock_t begin, end;
	double time_spent;

	t = initTrie();
	loadWord(t);
	printf("\nAll the words in the file \'/usr/share/dict/words\' has been loaded successfully to the trie\n");

	do {	
		printf("\nEnter Your Choice Number from the list below and press enter key:\n\n");
		printf("1. Create a trie by loading words from the given file\n");
		printf("2. Create an empty trie\n");
		printf("3. Search a word in the trie\n");
		printf("4. Insert a word in the trie\n");
		printf("5. Delete a word from the trie\n");
		printf("0. Exit\n\n");
		printf("Choice Number :$");
		
		if( (scanf("%d%c", &choice, &enter)) != 2 || enter != '\n' || choice < 0 || choice > 5 ) {
			printf("\nYour choice is not valid for any type of operations !\n\n");
			exit(1);
		}

		switch(choice) {

			case 1:
				freeTrie(t); // free memory allocated for a trie
				t = initTrie(); // initialize the tire to be empty

				loadWord(t); // load the words from the given file
				printf("All the words in the file \'/usr/share/dict/words\' has been loaded successfully to the trie\n");
				break;

			case 2:
				freeTrie(t); // free memory allocated for the trie if it is not empty
				t = initTrie();
				break;

			case 3:
				printf("Enter the word you want to search:$");
				scanf("%s", s);

				toLower(s); // make the lower case

				begin = clock(); // start counting execution time

				output = search(t, s); // check if the word is present in the trie

				if(output == 1)
					printf("word \'%s\' present in the trie\n", s);

				else { // if the word does not present in the trie
					printf("word \'%s\' does not present in the trie\n\n", s);

					count = deleteFind(t, s); // words obtained from Trie by delete any one character from the word

					count += insertFind(t, s); // words obtained from Trie by adding one character at any position

					count += changeFind(t, s); // words obtained from Trie by changing any single character;

					count += swapFind(t, s); // words obtained from Trie by swapping any two characters

					if(count > 0)
						printf("Total %d Similar words found\n", count);
					else
						printf("No Similar words found\n");
				}
				end = clock();
				time_spent = ( (double)(end - begin)) / ( (double)CLOCKS_PER_SEC);

				printf("Total time taken in searching operation is %e seconds\n", time_spent);
	
				break;

			case 4:
				printf("Enter the word you want to insert:$");
				scanf("%s", s);

				output = insertWord(t, s); // insert the word

				if(output == 1) // if the insertion is successfull
					printf("word inserted successfully in the trie\n");
				else
					printf("word can not be inserted !\n");
				break;

			case 5:
				printf("Enter the word you want to delete:$");
				scanf("%s", s);

				toLower(s);

				output = deleteWord(t, s); // delete the word

				if(output == 1)
					printf("word deleted successfully from the trie\n");
				else
					printf("word is not present in the trie !\n");
				break;

			case 0:	//exit from program
				freeTrie(t); // free memory allocated for a trie
				printf("Memory allocated for the trie has been deallocated\n");
				printf("\nProgram terminates successfully\n\n");
				exit(0); // if choice is '0' program terminates
		}
		printf("------------------------\n");

	}while(1);
}
