#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>

/// \brief This function will be responsible to find passed when running the program
/// \param current It's the current argument, which, in this case, contains the depth
void setDepth(char current[]);

/// \brief This function will be responsible to treat each of the paramenters passed when running the program
/// \param current It's the current argument to be held
/// \return Returns 0 upon sucess and 1 otherwise
int setParameters(char current[]);

/// \brief This function appends to the file defined in the global environment variable LOG_FILENAME
/// \brief the Execution Registers
/// \param instant Instant since the beggining of the program
/// \param pid Process identifier
/// \param action Action that's being registed
/// \param info Information about the action perfomed
void printExecutionErrors(double instant, pid_t pid, char action[], char info[]);

/// \brief This function appends to the file defined in the global environment variable LOG_FILENAME
/// \brief the Execution Registers
/// \param instant Instant since the beggining of the program
/// \param pid Process identifier
/// \param action Action that's being registed
/// \param info Information about the action perfomed
void printExecutionErrors2(double instant, pid_t pid, char action[], int info);

/// \brief Core function of the program. It's responsible for almost everything, which includes searching
/// \brief the directory, get it's size and the size of each file or symbolic link presented, and creates
/// \brief a new process responsible of make the exact same thing on a sub-directory
/// \param path The current path being searched
/// \return 0 upon sucess, -1 otherwise
int getSizeType(char path[]);

/// \brief SIGINT Handler. Responsible for handling the SIGINT signal
/// \param signo Number of the signal received 
void sigint_handler(int signo);

/// \brief Function that prints the path and it's size, according to some arguments, like -b (or --bytes)
/// \brief and -B (or --block-size).
/// \param st Struct that stores the information of some directory, file or symbolic link
/// \param path Path of the current directory, file or symbolic link
void printSizePath(struct stat st, char path[]);

/// \brief Function that prints the path and it's size, according to some arguments, like -b (or --bytes)
/// \brief and -B (or --block-size).
/// \param st Struct that stores the information of the directory
/// \param path Path of the current directory
/// \param totalExtra The total size of the files inside the directory
void printSizePathDir(struct stat st, char path[], size_t totalExtra);

/// \brief Function that gets the size of the dir passed as argument, according to the size of the files inside it
/// \param path Path of the current directory
/// \return Returns the size of the dir passed as argument
size_t getSizeOfDir(char path[]);
