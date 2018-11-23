/**
 * 
 * Author:      William Chad Brown
 * 
 * Description: This program takes a file and formats it so that each line only contains 
 *              the number of characters as set by the user. When the program starts the 
 *              user is asked to enter a number between 40 - 100. Then the user is prompted
 *              for a file name to read and then to format for the user. The program will  
 *              read in the information in the file and then store each word in a linked list.
 *              Then the lenght of each work is added to the linde until the next work will not
 *              fit on the current line. Any left over space that needs to be filled will be added.
 *              
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Sets up Linked List
struct listNode {  /* self-referential structure */
    char *data;
    struct listNode *nextPtr;
};

typedef struct listNode LISTNODE;
typedef LISTNODE *LISTNODEPTR;

//Sets up function
void insert(LISTNODEPTR *, char *);
char delete(LISTNODEPTR *, char *);
int isEmpty(LISTNODEPTR);
void printList(LISTNODEPTR, int);
void instructions(void);

//Global Varables
int lineLength;
char filename[100];

int main(){
    
    //Creates varables needed for the file and linkedList
    FILE *filePonter = NULL;
    LISTNODEPTR startPtr = NULL;
    
    //Prints the question to the user for the length of the line
    //and the file name
    instructions();
    
    //checks to see if the file can be open if not the program ends
    if((filePonter = fopen(filename, "r")) == NULL){
        
        printf("File can not be open. \n");
        exit(1);
    }else{
        //varables needed to hold the text from the file to be modifed
        char *item = malloc(BUFSIZ);
        char *str = malloc(BUFSIZ);
        char *data = malloc(30*sizeof(char));
        
        //gets the frist line of text from the file
        item = fgets(item, BUFSIZ,filePonter);
        //loops threw the other lines of text.
        while(item != NULL){
            //looks for new line char to seperate paragraphs
            while(item != NULL && strncmp(item, "\r\n",2)){
                //adds the lines of the same paragraph to one char array
                strcat(str, item);
                //printf("%s", str); testing
                //gets the next line
                item = fgets(item, BUFSIZ,filePonter);
            }
            //Checks for empty string
            if(strncmp(str,"",1)){
                //splits the strings at white space and newline
                data = strtok(str," \n");
                //loops threw the char array pulling out the strings
                while(str != NULL && data != NULL){
                    
                    //adds new words to the linked list
                    insert(&startPtr, data);
                    //splits at white space new line and return
                    data = strtok(NULL, " \r\n");
                    //printf("%s", data); testing
                    
                }
                //free data memory
                free(data);
                //sets str back to ""
                str = malloc(BUFSIZ);
                //prints the current paragraph
                printList(startPtr,lineLength );
                //freeMemory(startPtr); does not work
                //resets the start pointer and adds spaces after the paragraph
                startPtr = NULL;
                printf("\n\n");
                
            }
            //gets the next line from the file.
            item = fgets(item, BUFSIZ,filePonter);
        }
    }
    //exit the program
    return 0;
}

/* Print the instructions */
void instructions(void)
{
    lineLength = 0;
    while(lineLength < 40 || lineLength > 100){
        printf("Please enter the number of characters per line.\n");
        printf("Valid lengths are between 40 and 100. \n");
        scanf("%d", &lineLength);
    }
    
    printf("Please enter the file name. \n");
    scanf("%s", filename);
    
    
}

/* Insert a new value into the list in sorted order */
void insert(LISTNODEPTR *sPtr, char *value)
{
    LISTNODEPTR newPtr, previousPtr, currentPtr;
    
    newPtr = malloc(BUFSIZ);
    
    if (newPtr != NULL) {    /* is space available */
        newPtr->data = value;
        newPtr->nextPtr = NULL;
        
        previousPtr = NULL;
        currentPtr = *sPtr;
        
        while (currentPtr != NULL && value > currentPtr->data) {
            previousPtr = currentPtr;          /* walk to ...   */
            currentPtr = currentPtr->nextPtr;  /* ... next node */
        }
        
        if (previousPtr == NULL) {
            newPtr->nextPtr = *sPtr;
            *sPtr = newPtr;
        }
        else {
            previousPtr->nextPtr = newPtr;
            newPtr->nextPtr = currentPtr;
        }
    }
    else
        printf("%s not inserted. No memory available.\n", value);
}

/* Delete a list element */
char delete(LISTNODEPTR *sPtr, char *value)
{
    LISTNODEPTR previousPtr, currentPtr, tempPtr;
    
    if (value == (*sPtr)->data) {
        tempPtr = *sPtr;
        *sPtr = (*sPtr)->nextPtr;  /* de-thread the node */
        free(tempPtr);             /* free the de-threaded node */
        return *value;
    }
    else {
        previousPtr = *sPtr;
        currentPtr = (*sPtr)->nextPtr;
        
        while (currentPtr != NULL && currentPtr->data != value) {
            previousPtr = currentPtr;          /* walk to ...   */
            currentPtr = currentPtr->nextPtr;  /* ... next node */
        }
        
        if (currentPtr != NULL) {
            tempPtr = currentPtr;
            previousPtr->nextPtr = currentPtr->nextPtr;
            free(tempPtr);
            return *value;
        }
    }
    
    return '\0';
}


/* Print the list */
void printList(LISTNODEPTR currentPtr, int length)
{
    //pointers for keeping track of current position
    LISTNODEPTR end = currentPtr;
    LISTNODEPTR aPtr = currentPtr;
    LISTNODEPTR begin = currentPtr;
    
    //outer loop for pinting the current line
    while(begin->nextPtr != NULL)
    {
        //Local variables needed
        int numLineSpace[100] = {};
        int numWhiteSpace = 0;
        int charInLine = 0;
        int leftOverSpace = 0;
        int count = 0;
        int finalLine = 0;
        int j = 0;
        int i = 0;
        
        //adds to the count for chars in the current line
        charInLine = strlen(end->data)+1;
        //Tracks the number of words in the line
        count = 1;
        //moves to the next item in the linked list
        end = end->nextPtr;
        
        //Finds out how many words will fit in the current line
        while(charInLine+strlen(end->data)+1 < length)
        {
            charInLine = charInLine + strlen(end->data) + 1;
            count = count + 1;
            if(end->nextPtr != NULL)
                end = end->nextPtr;
            else
            {
                finalLine = 1;
            }
        }
        //track the number of extra white space char needed
        leftOverSpace = length - (charInLine-1);
        
        //adds white space char to an array for tracking
        while(leftOverSpace >= 0)
        {
            if(numWhiteSpace < (count - 1))
            {
                numLineSpace[numWhiteSpace] = numLineSpace[numWhiteSpace] + 1;
                leftOverSpace = leftOverSpace - 1;
                numWhiteSpace = numWhiteSpace + 1;
            }
            else
                numWhiteSpace = 0;
        }
        
        
        
        //Prints the work to the current line
        for( i = 1; i <= count; i++)
        {
            if(i < count)
            {
                printf("%s ", aPtr->data);
                //adds extra white space if needed to fill the line
                for(j = 1; j <= numLineSpace[i-1]; j++)
                {
                    printf(" ");
                }
            }
            //adds new line when needed.
            else if(i == count)
            {
                printf("%s\n", aPtr->data);
            }
            if(aPtr->nextPtr != NULL)
                aPtr = aPtr->nextPtr;
            else
                break;
        }
        
        //moves current pointers
        if(aPtr->nextPtr != NULL)
            end = aPtr;
        if(end->nextPtr == NULL)
            begin = end;
        //printf("\n");
    }
}

