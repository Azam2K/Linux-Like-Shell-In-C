#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct {
    char *alias;
    char *command;
} Alias;

#define MAX_INPUT_LENGTH 512
char input[MAX_INPUT_LENGTH];
char aliasInput[MAX_INPUT_LENGTH];
  char *tokens[512];
  //char *token;
  int i;
  //int j;
  char* historyArray[20] = {NULL};
  int arrayIterator = 0;
  //bool isUnique = false;
  //int lastCommandIterator;
  char inputHistory[MAX_INPUT_LENGTH];
  bool toCheckHistory = true;
  char* historyTokens[512];
  char* historyToken;
  //char* aliasList[512];
  Alias aliasList[10];
  int numAliases = 0;
  char aliasinput[MAX_INPUT_LENGTH];
  //char inputcopycopy[MAX_INPUT_LENGTH];
  //char* copyTokens[512];
  //char* copyToken;
  //bool loopexec = true;
  int infinitLoopStopper = 0;


void saveHistory(char *historyArray[]);
void parseUserInput(char *input, char **tokens);
void tokenizeHistory(char *inputHistory, char **historyTokens);
void getCommandFromHistory(char *input, char **tokens, char **historyArray, int arrayIterator);
void parseUserNewLine(char *input, char **tokens);
bool checkAlias(char **tokens);
void execute(char **tokens);
void saveAliases();
void read_history_file(char *filename, char **historyArray, int max_history_size, int *arrayIterator);
void createAlias(char **tokens);
void load_aliases();


int main() {

 load_aliases();
  
  char* original_path = getenv("PATH");

 chdir(getenv("HOME"));

 read_history_file("history.hist_list", historyArray, 20, &arrayIterator);
 
  // COMMAND LOOP 
  while (1) {

    //loopexec = true;
    infinitLoopStopper = 0;

    // set toCheckHistory = true;
    toCheckHistory = true;

    // Prompt user
    printf(">");

    // Read user input
    if (fgets(input, MAX_INPUT_LENGTH, stdin)  == NULL)  {
        /* Restore the original path for the user when exiting the program
     and display the change to the user showing their new path.*/
    setenv("PATH",original_path,1);
    printf("\n\nPath Restored on Exit: %s\n",getenv("PATH"));
      saveHistory(historyArray);
      saveAliases();
      printf("Exiting Shell\n");
      exit(0);
    }

    aliasCommandTelaport:


    //copies the input to inputhistory
     strcpy(inputHistory, input);
     //strcpy(inputcopycopy, input);
     tokenizeHistory(inputHistory, historyTokens);

    // Ignore input if it only contains a newline character
    if (strlen(input) == 1 && input[0] == '\n') {
     continue;
    }  

    // Set ArrayIterator to 0 if 20, since reached maximum value in array
    if (arrayIterator == 20) {
        arrayIterator = 0;
    }

    // Check input length
    if (strlen(input) > 512) {
        printf("Input too long!\n");
        input[0] = '\0'; // clear input
        continue; // start next iteration
    }

    parseUserInput(input, tokens);

    


 if ((strcmp(tokens[0], "!!") == 0) || (strcmp(tokens[0], "!") == 0) || (strcmp(tokens[0], "!-") == 0)){
    getCommandFromHistory(input, tokens, historyArray, arrayIterator);
    toCheckHistory = false;
    //input = getCommandFromHistory();
    parseUserInput(input, tokens);
   }

    

    /*
    ----------------------
     USER COMMAND PROMPTS
    ----------------------
    */
    //aliasCommandTelaport:
    //parseUserInput(input, tokens);
    //printf("%s",input);

    // Exit shell if user types "exit" and saves history to file
    if (strcmp(tokens[0], "exit") == 0) {
       /* Restore the original path for the user when exiting the program
     and display the change to the user showing their new path.*/
    setenv("PATH",original_path,1);
    printf("\n\nPath Restored on Exit: %s\n",getenv("PATH"));
      saveHistory(historyArray);
      saveAliases();
      printf("Exiting Shell\n");
      exit(0);
    }


    else if (strcmp(tokens[0], "cd") == 0) {


        if (tokens[1] && tokens[2] != NULL){
            printf("Error: cd command only takes 1 parameter\n");
            printf("Usage : cd <directory>\n");
        }
        else if (tokens[1] == NULL) {
            // No path specified, go to home directory
            chdir(getenv("HOME"));
            }
        else if (chdir(tokens[1]) != 0) {
            // Invalid path specified, display error message
            perror("Error");
            }
        }



    // Get system path
    else if (strcmp(tokens[0], "getpath") == 0) {
        if (tokens[1] == NULL) {
            char* path = getenv("PATH");
            printf("PATH=%s\n", path);
        } 
        else {
            printf("Error: getpath command does not take any parameters\n");
        }
        
    }



else if (strcmp(tokens[0], "alias") == 0 && tokens[1] != NULL) {
    if (tokens[1] == NULL || tokens[2] == NULL) {
        printf("Usage: alias <alias name> <command you want to alias>\n");
    }

   else {
    createAlias(tokens);
}
    
}




   
else if (strcmp(tokens[0], "alias") == 0 && tokens[1] == NULL) {
    if (numAliases == 0) {
        printf("No aliases defined.\n");
    } else {
        printf("Aliases:\n");
        for (int i = 0; i < numAliases; i++) {
            printf("%s %s\n", aliasList[i].alias, aliasList[i].command);
        }
    }
}

    else if (strcmp(tokens[0], "savealiases") == 0) {
        saveAliases();
    }



    // Set system path
    else if (strcmp(tokens[0], "setpath") == 0) {
      if (tokens[1] == NULL) {
        printf("setpath: missing argument\n");
      } else if (setenv("PATH", tokens[1], 1) != 0){
          printf("setpath: error setting PATH\n");
      }
      else if (tokens[1] && tokens[2] != NULL){
         // Invalid number of parameters, print error message
        printf("Error: setpath command requires one parameter\n");
        printf("Usage: setpath <path>\n");
      }

    }



    // print history
    else if (strcmp(tokens[0], "history") == 0) {
        for (i = 0; i <= 19; i++) {
            if(historyArray[i] == NULL){
                continue;
            }
        int temp = i + 1;
        printf("%d %s", temp, historyArray[i]);
        printf("\n");
        }
        //goto historyCheck;
    }



    // save history 
    else if (strcmp(tokens[0], "saveHistory") == 0) {
        saveHistory(historyArray);
        //goto historyCheck;
    }

    else if (strcmp(tokens[0], "unalias") == 0) {
      if (tokens[1] == NULL) {
        printf("Error: unalias command requires at least one parameter\n");
        printf("Usage: alias <command name\n");
      }
      else {
        for (int i = 1; tokens[i] != NULL; i++) {
          bool aliasFound = false;
          for (int j = 0; j < numAliases; j++) {
            if (strcmp(tokens[i], aliasList[j].alias) == 0) {
              aliasFound = true;
              // Shift remaining aliases left to remove the one being unaliased
              for (int k = j; k < numAliases - 1; k++) {
                aliasList[k] = aliasList[k+1];
              }
              numAliases--;
              printf("Alias %s removed\n", tokens[i]);
              break;
            }
          }
          if (!aliasFound) {
            printf("Alias %s not found\n", tokens[i]);
          }
        }
      }
    }



    else if (checkAlias(tokens) == true){// Execute user command
        //parseUserInput(input, tokens);
        goto aliasCommandTelaport;
    }

   
   else{
    execute(tokens);
     }



    /*
    Adding command to History
    */
        
   
    // 
      if(toCheckHistory == true){
        // Check is position in list has something in it, if so, free up the memory
        historyArray[arrayIterator] = (char*) malloc(strlen(inputHistory) + 1);

        // Copy the contents of input to historyArray
        strcpy(historyArray[arrayIterator], inputHistory);

        //increment the Array Iterator
        arrayIterator++;
      }
    
    //}
     
  }
    
    return 0;
}




// HELPER FUNCTIONS

   

// save to history
void saveHistory(char *historyArray[]) {
    FILE *fp;
    fp = fopen("history.hist_list", "w");

    for (int i = 0; i <= 19; i++) {
        if (historyArray[i] == NULL) {
            break;
        }
        fprintf(fp, "%s\n", historyArray[i]);

    }
    printf("Command History Saved to file \n");
    fclose(fp);
}

void saveAliases() {
    FILE *fp;
    fp = fopen(".aliases", "w");
    printf("Aliases have been saved to file \n");
    for (int i = 0; i < numAliases; i++) {
        if (aliasList == NULL) {
            break;
        } else {
            fprintf(fp, "%s %s\n", aliasList[i].alias, aliasList[i].command);
        }
    }
    fclose(fp);               
}


void parseUserInput(char *input, char **tokens) {
    int i = 0;
    char *token = strtok(input, " \t \n ; & > < \r |");

    while (token != NULL) {
      tokens[i] = token;
      token = strtok(NULL, " \t \n ; & > < \r |");
      i++;
    }
    tokens[i] = NULL;
}

void parseUserNewLine(char *input, char **tokens) {
    int i = 0;
    char *token = strtok(input, "\n");

    while (token != NULL) {
      tokens[i] = token;
      token = strtok(NULL, "\n");
      i++;
    }
    tokens[i] = NULL;
}

void tokenizeHistory(char *inputHistory, char **historyTokens) {
  int i = 0;
  char *historyToken = strtok(inputHistory, "\n");

  while (historyToken != NULL) {
    historyTokens[i] = historyToken;
    historyToken = strtok(NULL, "\n");
    i++;
  }
  historyTokens[i] = NULL; 
}



void getCommandFromHistory(char *input, char **tokens, char **historyArray, int arrayIterator) {
    int lastCommandIterator;
    //bool toCheckHistory = true;
    
    // Get Last command in history
    // The last command iterator is calculated within this function which just finds the index of the last command in the array
    // If the user enters !! then it just executes the last command entered, whether it was valid or not
    if (strcmp(tokens[0], "!!") == 0) {
        if (arrayIterator == 0) {
            lastCommandIterator = 19;
        } else {
            lastCommandIterator = arrayIterator - 1;
        }
        if (historyArray[lastCommandIterator] == NULL) {
            printf("No Previous Command\n");
            return;
        }
        strcpy(input, historyArray[lastCommandIterator]);
        printf(">%s\n", input);
        //toCheckHistory = false;
    }

    // Get [index] command in history
    // handles if a user inputs something along the lines of !- then a number
    // it then does some checking to see if the position they entered at has a value in it
    // if valid, it re-executes the command at that index and doesn't pass the second value to the re-executed command
    if (strcmp(tokens[0], "!") == 0) {
        int x = atoi(tokens[1]);
        if (x >=1 && x <=20) {
            if (historyArray[x-1] == NULL) {
                printf("\n No command at position %d\n", x);
                return;
            } else {
                strcpy(input, historyArray[x-1]);
                // Remove argument from tokens array
                tokens[1] = NULL;
                printf(">%s\n", input);
            }
        }
        //toCheckHistory = false;
    }

    // handles if a user inputs something along the lines of !- then a number
    // this then stores the integer value they enter into a token
    // it then checks if the last index minus the inputted index is less than 0 or if it's null and deals with it appropriately
    // otherwise it re-executes the command it was supposed to and sets tokens[1] to null which doesn't pass the second value to the re-executed command
    if (strcmp(tokens[0], "!-") == 0) {
        int x = atoi(tokens[1]);
        if (arrayIterator == 0) {
            lastCommandIterator = 19;
        } else {
            lastCommandIterator = arrayIterator - 1;
        }
        if (historyArray[lastCommandIterator - x] == NULL || lastCommandIterator - x < 0) {
            printf("No Previous Command\n");
            return;
        }
        tokens[1] = NULL;
        strcpy(input, historyArray[lastCommandIterator - x]);
        printf(">%s\n", input);
        //toCheckHistory = false;
    }
    //return input;
}


bool checkAlias(char **tokens) {
    for (int i = 0; i < numAliases; i++) {
        if (strcmp(tokens[0], aliasList[i].alias) == 0) {
            tokens[0] = aliasList[i].command;
            strcpy(input, aliasList[i].command);
            return true;
        }
    }
    return false;
}


void execute(char **tokens) {
    pid_t pid;
    int status;
    pid = fork();

    if (pid == 0) {
        // child process
        execvp(tokens[0],tokens);
        // if execvp returns, it must have failed
        perror(tokens[0]);
        exit(1);
    } else if (pid < 0) {
        perror("fork");
    } else {
        // parent process
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
}

void load_aliases() {
    char filename[] = ".aliases";
    char buffer[256];
    char* alias;
    char* command;
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        printf("Error: Failed to open file %s\n", filename);
        exit(1);
    }

    while (fgets(buffer, sizeof(buffer), file)) {
        // Remove trailing newline character
        buffer[strcspn(buffer, "\n")] = '\0';

        // Split buffer into alias and command
        alias = strtok(buffer, " ");
        command = strtok(NULL, "");

        // Print out the results for debugging purposes
        // printf("Alias: %s\n", alias);
        //printf("Command: %s\n", command);
        tokens[1] = alias;
        tokens[2] = command;
        createAlias(tokens);
    }
    tokens[1] = NULL;
    tokens[2] = NULL;

    fclose(file);

    
}

void read_history_file(char *filename, char **historyArray, int max_history_size, int *arrayIterator) {
    FILE *file = fopen(filename, "r");
    char line[20];
    int index = 0;

    while (fgets(line, 20, file) != NULL && index < max_history_size) {
        if (line[0] == '\n') {
            break; // stop reading when a null line is encountered
        }
        int pos = strcspn(line, "\r\n"); // find position of '\r' or '\n' character
        line[pos] = '\0'; // replace it with a null terminator
        historyArray[index] = strdup(line); // allocate memory for the line and store it in the array
        (*arrayIterator)++;
        index++;
    }

    fclose(file);
}

void createAlias(char **tokens) {
    Alias newAlias;
    newAlias.alias = strdup(tokens[1]);
    char aliasCommand[512] = "";
    int i = 2;
    while (tokens[i] != NULL) {
        // skip over the "alias_name" argument
        if (strcmp(tokens[i], tokens[1]) != 0) {
            strcat(aliasCommand, tokens[i]);
            if (tokens[i+1] != NULL){
                strcat(aliasCommand, " ");
            }
        }
        i++;
    }
    newAlias.command = strdup(aliasCommand);
    aliasList[numAliases] = newAlias;
    numAliases++;    
}