/*
 * MyShell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2017, <GROUP MEMBERS> Shreyans Rishi, Kalev Gonvick, Vicki Derbyshire
 * All rights reserved.
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include "utility.h"
#include "myshell.h"
#include <pwd.h>

// Put macros or constants here using #define
#define BUFFER_LEN 256

// Put global environment variables here
char path[BUFFER_LEN];
char origin[BUFFER_LEN];
// Define functions declared in myshell.h here

int main(int argc, char *argv[])
{
    // Input buffer and and commands
    char buffer[BUFFER_LEN] = { 0 };
    char command[BUFFER_LEN] = { 0 };
    char arg[BUFFER_LEN] = { 0 };

    
    // Parse the commands provided using argc and argv
    // Perform an infinite loop getting command input from users
    while (fgets(buffer, BUFFER_LEN, stdin) != NULL)
    {
        if(buffer[strlen(buffer)-1]=='\n'){
            buffer[strlen(buffer)-1]='\0';
        }
        arg[0] = '\0';
        // Perform string tokenization to get the command and argument
        char *token;
        token = strtok(buffer, " ");
        int i = 0;
        while( token != NULL ) {

            if(i==0){
                strcpy(command, token);
		
            }
            else{
                strcat(arg, token);
            }
            i++;
            token = strtok(NULL, " ");
	    printf("%s\n", token);
            if(token != NULL){
                strcat(arg, " ");
            } 
        }

        // Check the command and execute the operations for each command
        // cd command -- change the current directory
        if (strcmp(command, "cd") == 0)
        {

            getcwd(command, sizeof(command));
            // Check for any argument 
            if(strcmp(path,"") == 0){
                printf("Still at current path.");
            }
            else{
                //Sets the environment variable
                setenv("PWD", path, 2);
                //Checks if directory exists
                if(chdir(getenv("PWD")) != 0){
                    printf("Directory does not exist.");
                }
                else{
                    printf("Now at directory %s\n", getenv("PWD"));
                }
            }
        }
        
        // dir command -- list the contents of the requested directory
        else if (strcmp(command, "dir") == 0){
            //Required directory variables
            DIR *dir;
            struct dirent *ent;
            //Store the current path
            getcwd(origin, sizeof(origin));
            //Checks if there is an argument to change directory
            if (strcmp(arg,"") != 0){
                //Changes path to argument
                setenv("PWD", path, 2);
                //Checks if directory exists
                if(chdir(getenv("PWD")) != 0){
                    printf("Directory does not exist.");
                }
                //Get the current directory and print its contents
                getcwd(path, sizeof(path));
                dir = opendir(path);
                while ((ent = readdir(dir)) != NULL){
                    printf("%s\n", ent->d_name);
                }
                //Change path back to original directory
                setenv("PWD", origin, 3);
                chdir(getenv("PWD"));
                //Closes directory
                closedir(dir);
            }
            else{
                printf("Require argument. Please try again.\n");
            }

        }

        // echo command -- print the arguments/message
        else if (strcmp(command, "echo") == 0){
            printf("%s\n",arg);
        }
        // clr command -- clears the screen
        else if (strcmp(command, "clr") == 0){
            printf("\%c[2J", 033);
        }
        // pause command -- pauses the system until enter is pressed
        else if (strcmp(command, "pause") == 0){
            printf("Please press Enter to continue...\n");
            getchar();
        }
        // environ command -- lists the environmnent strings
        else if (strcmp(command, "environ") == 0){
            system("printenv");
        }
        // help command -- opmakens the readme file
        else if (strcmp(command, "help") == 0){
            FILE *fpt;
            char string[1000];
            //Opens the readme file
            fpt = fopen("README.md", "rw");
            fread(string, 100, 100, fpt);
            fgets(string, 100, fpt);
            //prints the readme file onto the console
            printf("%s\n", string);
            //close the file
            fclose(fpt);
        }

        // quit command -- exit the shell
        else if (strcmp(command, "quit") == 0)
        {
            return EXIT_SUCCESS;
        }

        // Unsupported command
        else
        {
            fputs("Unsupported command, use help to display the manual\n", stderr);
        }
    }
    return EXIT_SUCCESS;
}
