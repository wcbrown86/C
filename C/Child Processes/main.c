//
// Assignment 3 version 1
//
// By William Chad Brown
// 
// Description: This program is intended to work with multiple processes. The program will
//              preform a few simple opperations with a child process. Each time listing the
//              child process ID and the parent process ID. The programm will give the user 
//              four choices, to show the current date and time, show a calender, show the 
//              current working directory, and to exit the program. Each opperation is preformed 
//              with child process, and this information is shown. 
//


#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

// Declaring funtion for process selection
void processChoice(int userInput);

int main() {
    
    // Holds the user input
    int choice = 0;
    
    // Force start the loop and ask the user for there choice
    do{
        
        // Main menu is shown to the user. 
        printf("Parent process ID: %d, Parent's parent ID: %d \n",
               getpid(), getppid());
        printf("Choose the function to be preformed by the child. \n");
        printf("\t 1. Display the current date and time, \n");
        printf("\t 2. Display the calendar for the current month, \n");
        printf("\t 3. List files in the current direvtory, \n");
        printf("\t 4. Quit the program. \n");
        
        // Fomating
        printf("\n\n");
        
        //Asks for choice baised off of above options
        printf("Enter your choice: ");
        
        //Grabs user input
        scanf("%d", &choice);
        
        // Formating
        printf("\n\n");
        
        // Calls funtion to process the users choice
        processChoice(choice);
        
        // Check to see if the program will continue to run.
    }while(choice != 4);
    
    //End of program print out.
    printf("Current process ID: %ld \n", (long)getpid());
    printf("Program Terminated. \n");
    
    return 0;
}

// Thakes in user inpput from the main method
void processChoice(int userInput){
    
    //Creates child varaible
    pid_t child_pid;
    
    // Creates the child process
    child_pid = fork();
    
    //Checks to see if the child was created
    if(child_pid < 0){
        
        printf("Failed to fork. \n");
        exit(-1);
    }
    
    //Switch that uses the user input to run the right system call
    switch(userInput){
            
        case 1:
            
            //Checks to see if the process is the child
            if(child_pid == 0){
                //prints the process ID's
                printf("Child process ID: %ld; Parent ID: %ld",
                       (long)getpid(), (long)getppid());

                //Formating
                printf("\n");

                //System call for date
                system("date");

                //formating
                printf("\n\n");

                //Ends the child process
                exit(0);
            }
            //Parent process to wait for the child.
            else
                wait(NULL);
            
            break;
            
        case 2:
            
            //Checks to see if the process is the child
            if(child_pid == 0){

                //prints the process ID's
                printf("Child process ID: %ld; Parent ID: %ld",
                       (long)getpid(), (long)getppid());

                //Formating
                printf("\n");

                //System call for date
                system("cal");

                //formating
                printf("\n\n");

                //Ends the child process
                exit(0);
            }
            //Parent process to wait for the child.
            else
                wait(NULL);
            
            break;
            
        case 3:
            
            //Checks to see if the process is the child
            if(child_pid == 0){

                //prints the process ID's
                printf("Child process ID: %ld; Parent ID: %ld",
                       (long)getpid(), (long)getppid());

                //Formating
                printf("\n");

                //System call for date
                system("ls -l");

                //formating
                printf("\n\n");
                
                //Ends the child process
                exit(0);
            }
            //Parent process to wait for the child.
            else
                wait(NULL);
            
            break;
            
        case 4:
            
            //Does nothing
            break;
            
        default:
            
            //for invalid input and error message is printed.
            printf("Your input is invalid please try again. \n");
            break;
    }
}