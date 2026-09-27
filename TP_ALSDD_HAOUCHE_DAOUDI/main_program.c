#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//......................................... STRUCTURES .......................................
/* Brief explanation aboute the structures we used in our program:
we prefered to use the word as the simplest structure and the simpleset element in a collection,
because the functions we are going to use in the program are based on the word as a unit of operation, 
and we can easily manipulate it.
also we prefered to use the linked list as the data structure for the collection, because it allows us to 
easily add and remove words without worrying about resizing an array, and it also allows us to easily traverse 
the collection when performing set operations.
finally we used the paragraph structure to represent a paragraph in the file, which contains a collection
 of words and a pointer to the next paragraph, this allows us to easily manage multiple 
 paragraphs in a file and perform operations on them.
 */
typedef struct Node {
    char word[50];
    struct Node* next;
} Node;

typedef struct {
    Node* head;
} Collection;

typedef struct Paragraph {
    Collection words;
    struct Paragraph* next;
} Paragraph;

//.................................COLLECTION FUNCTIONS ...........................................
// Initialize an empty collection with null head.
void initCollection(Collection* C) {
    C->head = NULL;
}
// Check if a word exists in the collection, return 1 if found,if not ,return 0.
int containsWord(Collection C, char word[]) {
    Node* temp = C.head;
    while (temp) {
        if (strcmp(temp->word, word) == 0)
            return 1;
        temp = temp->next;
    }
    return 0;
}
/* Add a word to the collection if it doesn't already exist, by creating a new node and linking
 it to the head of the list.Here we can notice that it checks first if it exixts in the collection,
 because the functions we want to implement do not include duplicates .*/
void addWord(Collection* C, char word[]) {
    if (!containsWord(*C, word)) {
        Node* newNode = (Node*)malloc(sizeof(Node));
        strcpy(newNode->word, word);
        newNode->next = C->head;
        C->head = newNode;
    }
}
/* Print all words in the collection by traversing the linked list and printing each word until the end of the list
 is reached.*/
void printCollection(Collection C) {
    Node* temp = C.head;
    while (temp) {
        printf("%s ", temp->word);
        temp = temp->next;
    }
    printf("\n");
}
/* Free all nodes in the collection by traversing the linked list and freeing each node until the end
 of the list is reached, then set the head to NULL to indicate that the collection is empty.*/
void freeCollection(Collection* C) {
    Node* temp = C->head;
    while (temp) {
        Node* next = temp->next;
        free(temp);
        temp = next;
    }
    C->head = NULL;
}

//.....................SET OPERATIONS..............................
/* the union of two collections A and B is a new collection that contains all the unique words from both A and B.
 without duplicates */
Collection unionCollections(Collection A, Collection B) {
    Collection R;
    initCollection(&R);

    Node* t = A.head;
    while (t) {
        addWord(&R, t->word);
        t = t->next;
    }

    t = B.head;
    while (t) {
        addWord(&R, t->word);
        t = t->next;
    }

    return R;
}
/* the intersection of two collections A and B is a new collection that contains only the words that
 are present in both A and B. without duplicates */
Collection intersectionCollections(Collection A, Collection B) {
    Collection R;
    initCollection(&R);

    Node* t = A.head;
    while (t) {
        if (containsWord(B, t->word))
            addWord(&R, t->word);
        t = t->next;
    }

    return R;
}
/* the difference of two collections A and B (A - B) is a new collection that contains only the words that
 are present in A but not in B. without duplicates */
Collection differenceCollections(Collection A, Collection B) {
    Collection R;
    initCollection(&R);

    Node* t = A.head;
    while (t) {
        if (!containsWord(B, t->word))
            addWord(&R, t->word);
        t = t->next;
    }

    return R;
}
/* Check if all words in collection A are contained in collection B */
int isContained(Collection A, Collection B) {
    Node* t = A.head;
    while (t) {
        if (!containsWord(B, t->word))
            return 0;
        t = t->next;
    }
    return 1;
}
/* Get the inverse of collection A with respect to collection U */
Collection inverseCollection(Collection A, Collection U) {
    Collection R;
    initCollection(&R);

    Node* t = U.head;
    while (t) {
        if (!containsWord(A, t->word))
            addWord(&R, t->word);
        t = t->next;
    }

    return R;
}

// .........................................PAGRAPH FUNCTIONS.............................................
/* Create a new paragraph node with the given collection of words */
Paragraph* createParagraph(Collection C) {
    Paragraph* p = (Paragraph*)malloc(sizeof(Paragraph));
    p->words = C;
    p->next = NULL;
    return p;
}
/* Add a new paragraph node to the end of the list, this allows for dynamic addition of paragraphs */
Paragraph* addParagraphNode(Paragraph* head, Collection C) {
    Paragraph* p = createParagraph(C);
if (head == NULL)
        return p;
Paragraph* temp = head;
    while (temp->next)
        temp = temp->next;
             temp->next = p;
    return head;
}
/* Print all paragraphs in the list by traversing the linked list and printing each paragraph 
until the end of the list is reached.*/
void printParagraphs(Paragraph* head) {
    int i = 1;
    while (head) {
        printf("\nParagraph %d:\n", i++);
        printCollection(head->words);
        head = head->next;
    }
}
/* Free all paragraphs in the list by traversing the linked list and freeing each paragraph until the end
 of the list is reached.*/
void freeParagraphs(Paragraph* head) {
    Paragraph* temp;
    while (head) {
        temp = head;
        freeCollection(&head->words);
        head = head->next;
        free(temp);
    }
}
// This function traverses the linked list of paragraphs and returns
// the paragraph located at the given index position. It is used to
// allow the user to select a specific paragraph for set operations
// such as union, intersection, and difference. briefly, we use it
// when we work on the same file 
Paragraph* getParagraph(Paragraph* head, int index) {
    int i = 1;
    while (head) {
        if (i == index)
            return head;
        head = head->next;
        i++;
    }
    return NULL;
}

// Same-file operations, means that we can perform the set operations on two paragraphs from the same
// file, this function allows the user to select two paragraphs from the same file and perform the 
//desired set operation on them, then it prints the result.
void operateSameFile(Paragraph* file) {
    int p1, p2, choice;

    if (!file) {
        printf("Load file first.\n");
        return;
    }

    printf("Enter paragraph numbers (p1, p2): ");
    scanf("%d %d", &p1, &p2);

    Paragraph* A = getParagraph(file, p1);
    Paragraph* B = getParagraph(file, p2);

    if (!A || !B) {
        printf("Invalid paragraph numbers.\n");
        return;
    }

    printf("1. Union\n2. Intersection\n3. Difference (p1 - p2)\nChoice: ");
    scanf("%d", &choice);

    Collection R;
// This switch statement allows the user to choose which set operation
// will be applied on the two selected paragraph collections in the same file . According
// to the user's choice, the program performs union, intersection, or
// difference, stores the result in a new collection, displays the
// resulting set of words, and finally frees the allocated memory.

    switch (choice) {
        case 1:
            R = unionCollections(A->words, B->words);
            printf("\nResult (Union):\n");
            break;
        case 2:
            R = intersectionCollections(A->words, B->words);
            printf("\nResult (Intersection):\n");
            break;
        case 3:
            R = differenceCollections(A->words, B->words);
            printf("\nResult (Difference):\n");
            break;
        default:
            printf("Invalid choice\n");
            return;
    }

    printCollection(R);
    freeCollection(&R);
}

//........................................WORD NORMALIZATION..............................................
// This function normalizes a word by removing all non-alphabetic
// characters and converting all letters to lowercase. It is used
// during text preprocessing to ensure that words are stored in a
// consistent format, making comparisons and set operations more
// accurate and reliable.

void normalizeWord(char word[]) {
    int j = 0;
    for (int i = 0; word[i]; i++) {
        if (isalpha(word[i])) {
            word[j] = tolower(word[i]);
            j++;
        }
    }
    word[j] = '\0';
}

//...........................................FILE PROCESSING..............................................
// This function reads the content of a text file and extracts words
// from the text in order to build paragraph representations using
// dynamic data structures. Each extracted group of processed words is
// stored in a collection after normalization and preprocessing.
// The generated collections are then inserted into a linked list so
// they can later be used for set operations such as union,
// intersection, and difference.
Paragraph* readFile(char filename[]) {
    FILE* f = fopen(filename, "r");

    if (!f) {
        printf("Error opening file\n");
        return NULL;
    }

    Paragraph* head = NULL;
    char line[500];

    while (fgets(line, sizeof(line), f)) {
        Collection C;
        initCollection(&C);

        char* token = strtok(line, " \n\t");

        while (token) {
            normalizeWord(token);
            if (strlen(token) > 0)
                addWord(&C, token);
            token = strtok(NULL, " \n\t");
        }

        head = addParagraphNode(head, C);
    }

    fclose(f);
    return head;
}

// ===== OPERATIONS ON PARAGRAPHS =====
/* this function performs union operation on two paragraph collections , same principle as before */
void unionParagraphs(Paragraph* A, Paragraph* B) {
    int i = 1;
    while (A && B) {
        Collection R = unionCollections(A->words, B->words);
        printf("\nUnion Paragraph %d:\n", i++);
        printCollection(R);
        freeCollection(&R);
        A = A->next;
        B = B->next;
    }
}
/* this function performs intersection operation on two paragraph collections , same principle as before */
void intersectionParagraphs(Paragraph* A, Paragraph* B) {
    int i = 1;
    while (A && B) {
        Collection R = intersectionCollections(A->words, B->words);
        printf("\nIntersection Paragraph %d:\n", i++);
        printCollection(R);
        freeCollection(&R);
        A = A->next;
        B = B->next;
    }
}
/* this function performs difference operation on two paragraph collections , same principle as before */
void differenceParagraphs(Paragraph* A, Paragraph* B) {
    int i = 1;
    while (A && B) {
        Collection R = differenceCollections(A->words, B->words);
        printf("\nDifference Paragraph %d:\n", i++);
        printCollection(R);
        freeCollection(&R);
        A = A->next;
        B = B->next;
    }
}

// ===== MAIN =====

int main() {
    // Main function of the program. It initializes the paragraph
// structures used to store the contents of the processed text files,
// declares the variables required for user interaction, and manages
// the execution of the different operations available in the system.
    Paragraph* file1 = NULL;
    Paragraph* file2 = NULL;

    int choice;
    char path1[200];
    char path2[200];
    // Main menu loop of the program. It allows the user to interact with
// the system by loading text files, displaying stored paragraphs, and
// applying set operations such as union, intersection, and difference
// between paragraphs from different files or within the same file.
// The loop continues executing until the user chooses to exit the
// program. Memory allocated for the paragraph structures is released
// before termination to avoid memory leaks.

    do {
        printf("\n................................... MENU ................................\n");
printf("1. Load first file\n");
printf("2. Load second file\n");
printf("3. Show first file paragraphs\n");
printf("4. Show second file paragraphs\n");
printf("5. Union\n");
printf("6. Intersection\n");
printf("7. Difference\n");
printf("8. Operations within same file\n");
printf("9. Subset test\n");
printf("10. Complement\n");
printf("0. Exit\n");

        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice) {

            case 1:
                printf("Enter full path of first file:\n");
                scanf(" %[^\n]", path1);
                freeParagraphs(file1);
                file1 = readFile(path1);
                break;

            case 2:
                printf("Enter full path of second file:\n");
                scanf(" %[^\n]", path2);
                freeParagraphs(file2);
                file2 = readFile(path2);
                break;

            case 3:
                printParagraphs(file1);
                break;

            case 4:
                printParagraphs(file2);
                break;

            case 5:
                if (!file1 || !file2) {
                    printf("Please load both files first.\n");
                    break;
                }
                unionParagraphs(file1, file2);
                break;

            case 6:
                if (!file1 || !file2) {
                    printf("Please load both files first.\n");
                    break;
                }
                intersectionParagraphs(file1, file2);
                break;

            case 7:
                if (!file1 || !file2) {
                    printf("Please load both files first.\n");
                    break;
                }
                differenceParagraphs(file1, file2);
                break;

            case 8:
                printf("Choose file (1 or 2): ");
                int f;
                scanf("%d", &f);

                if (f == 1)
                    operateSameFile(file1);
                else if (f == 2)
                    operateSameFile(file2);
                else
                    printf("Invalid file choice\n");
                break;
                case 9: {
    if (!file1 || !file2) {
        printf("Please load both files first.\n");
        break;
    }

    int p1, p2;

    printf("Enter paragraph number from file1: ");
    scanf("%d", &p1);

    printf("Enter paragraph number from file2: ");
    scanf("%d", &p2);

    Paragraph* A = getParagraph(file1, p1);
    Paragraph* B = getParagraph(file2, p2);

    if (!A || !B) {
        printf("Invalid paragraph numbers.\n");
        break;
    }

    if (isContained(A->words, B->words))
        printf("Paragraph %d of file1 is a subset of paragraph %d of file2.\n", p1, p2);
    else
        printf("Paragraph %d of file1 is NOT a subset of paragraph %d of file2.\n", p1, p2);

    break;
}
case 10: {
    if (!file1 || !file2) {
        printf("Please load both files first.\n");
        break;
    }

    int p1, p2;

    printf("Enter paragraph number from file1: ");
    scanf("%d", &p1);

    printf("Enter paragraph number from file2 (Universe): ");
    scanf("%d", &p2);

    Paragraph* A = getParagraph(file1, p1);
    Paragraph* U = getParagraph(file2, p2);

    if (!A || !U) {
        printf("Invalid paragraph numbers.\n");
        break;
    }

    Collection R = inverseCollection(A->words, U->words);

    printf("\nComplement Result:\n");
    printCollection(R);

    freeCollection(&R);

    break;
}

            case 0:
                printf("thanks for using our program!\n");
                break;


            default:
                printf("Invalid choice\n");
        }

    } while (choice != 0);
/* Before freeing the paragraph structures, we verify that the pointers
are not NULL in order to avoid invalid memory operations and ensure
safe memory management. */
    if (file1 != NULL)
    freeParagraphs(file1);

if (file2 != NULL)
    freeParagraphs(file2);

    return 0;

}
/* please note madame that :
 Two simple test text files were created to demonstrate that the
program works correctly and to test all the implemented operations.
 Since we have not yet studied file manipulation, the same
 principle was applied using basic text files for testing and
validation purposes. */