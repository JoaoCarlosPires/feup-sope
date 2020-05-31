#include "simpledu.h"

#define READ 0
#define WRITE 1

struct myActions {
    char CREATE[12];
    char EXIT[12];
    char RECV_SIGNAL[12]; 
    char SEND_SIGNAL[12];
    char RECV_PIPE[12];
    char SEND_PIPE[12];
    char ENTRY[12];
};

FILE *fptr;
clock_t start;
clock_t stop;

struct myActions ac = {"CREATE", "EXIT", "RECV_SIGNAL", "SEND_SIGNAL", "RECV_PIPE", "SEND_PIPE", "ENTRY"};

bool all = false; // When true, the information contains also the files
bool bytes = false; // When true, the size is the real number of bytes
bool block = false; // When true, defines the block size (in bytes)
size_t block_size = 0; // Size of the blocks (in bytes);
bool count_links = false; // When true, the same file is counted as many times as it's present
bool dereference = false; // When true, follows symbolic links
bool separate_dirs = false; // When true, the information doesn't include the sub-directory size
bool depth = false; // When true, the max depth of directories is defined in a parameter
double max_depth = INFINITY; // Max depth

char mainPath[100]; // Main Path passed as argument

void printExecutionErrors(double instant, pid_t pid, char action[], char info[]) { 
    fprintf(fptr, "%f – %d – %s – %s\n", fabs(instant), pid, action, info);
}

void printExecutionErrors2(double instant, pid_t pid, char action[], int info) { 
    fprintf(fptr, "%f – %d – %s – %i\n", fabs(instant), pid, action, info);
}

void setDepth(char current[]) {
    char sDepth[50];
    char *ps = current + 12;
    strcpy(sDepth, ps);
    max_depth = atoi(sDepth);
    stop = clock();
    double elapsed = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;
    pid_t pid = getpid();
    printExecutionErrors(elapsed, pid, ac.CREATE, current);

}

void printSizePathDir(struct stat st, char path[], size_t totalExtra) {
    char auxReg[1024];
    if (bytes) { 
        printf("%ld\t%s\n", st.st_size+totalExtra, path);
        sprintf(auxReg, "Size: %zu of: %s", st.st_size+totalExtra, path);
    } else if (block) { 
        printf("%ld\t%s\n", (1024/block_size)*(st.st_blocks/2)+totalExtra, path);
        sprintf(auxReg, "Size: %zu of: %s", (1024/block_size)*(st.st_blocks/2)+totalExtra, path);
    } else { 
        printf("%ld\t%s\n", (st.st_blocks/2)+totalExtra, path);
        sprintf(auxReg, "Size: %zu of: %s", (st.st_blocks/2)+totalExtra, path);
    }

    stop = clock();
    double elapsed = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;
    pid_t pid = getpid();
    printExecutionErrors(elapsed, pid, ac.ENTRY, auxReg);
}

void printSizePath(struct stat st, char path[]) {
    char auxReg[1024];

    if (bytes) { 
        printf("%ld\t%s\n", st.st_size, path);
        sprintf(auxReg, "Size: %zu of: %s", st.st_size, path);
    } else if (block) { 
        printf("%ld\t%s\n", (1024/block_size)*(st.st_blocks/2), path);
        sprintf(auxReg, "Size: %zu of: %s", (1024/block_size)*(st.st_blocks/2), path);
    } else { 
        printf("%ld\t%s\n", (st.st_blocks/2), path);
        sprintf(auxReg, "Size: %zu of: %s", (st.st_blocks/2), path);
    }

    stop = clock();
    double elapsed = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;
    pid_t pid = getpid();
    printExecutionErrors(elapsed, pid, ac.ENTRY, auxReg);
}

size_t getSizeOfDir(char path[]) {
    struct stat st;
    DIR *dr = opendir(path);
    struct dirent *de;
    size_t R = 0;
    while ((de = readdir(dr)) != NULL) {
        if (strstr(de->d_name, "..") == NULL && de->d_name[strlen(de->d_name)-1] != '.'){
            char auxPath[50];
            strcpy(auxPath, path);
            char bar = '/';
            strncat(auxPath, &bar, 1);
            strcat(auxPath, de->d_name);              
            if (lstat(auxPath,&st)==0) { /*get the size using stat()*/
                if (!dereference) {
                    if (((st.st_mode & S_IFMT) == S_IFREG && all) || ((st.st_mode & S_IFMT) == S_IFLNK)) {
                        if (bytes) R+=st.st_size;
                        else if (block) R+=(1024/block_size)*(st.st_blocks/2);
                        else R+=(st.st_blocks/2);
                    }
                } else {
                    if ((st.st_mode & S_IFMT) == S_IFREG && all) {
                        if (bytes) R+=st.st_size;
                        else if (block) R+=(1024/block_size)*(st.st_blocks/2);
                        else R+=(st.st_blocks/2);
                    } else if ((st.st_mode & S_IFMT) == S_IFLNK) {
                        if (bytes) R+=st.st_size;
                        else if (block) R+=(1024/block_size)*(st.st_blocks/2);
                        else R+=(st.st_blocks/2);
                    }
                }    
            }    
        }
    }
    closedir(dr);
    return R;
}

int getSizeType(char path[]) {  

    //NOTE - Some parts of this function are based on the code from:
    //https://stackoverflow.com/questions/26509974/recursive-multi-process-program-in-c

    int status;
    pid_t pid = 1;

    char auxPath[1024];
    
    struct stat st;
    
    if(lstat(path, &st) == -1)
        return -1;

    if (path == mainPath && (st.st_mode & S_IFMT) == S_IFREG) {
        printSizePath(st, path);
        return 0;
    }    

    if (!dereference) {
        if (((st.st_mode & S_IFMT) == S_IFREG && all) || ((st.st_mode & S_IFMT) == S_IFLNK))
            printSizePath(st, path);
    } else {
        if ((st.st_mode & S_IFMT) == S_IFREG && all) 
            printSizePath(st, path);
        else if ((st.st_mode & S_IFMT) == S_IFLNK) {
            printSizePath(st, path);
        }    
    }

    

    if((st.st_mode & S_IFMT) == S_IFDIR){
        int fd[2];
        pipe(fd);
        pid = fork();
        if(pid > 0) {
            waitpid(pid, &status, WUNTRACED);
            size_t totalFileSize = getSizeOfDir(path);
            if (!separate_dirs) {
                size_t extra[1];
                close(fd[WRITE]);
                read(fd[READ],extra,sizeof(size_t));
                close(fd[READ]);
                totalFileSize+=extra[0];
                extra[0] = 0;
            }
            printSizePathDir(st, path, totalFileSize);
        } else if(pid == 0){
            DIR *dir;
            struct dirent *de = NULL;
            
            if ((dir = opendir(path)) == NULL){
                printf("Cannot open %s\n", path);
                exit(EXIT_FAILURE);
            }
            else {
                if (!separate_dirs) {
                    size_t temp[1];
                    if (bytes) { 
                        temp[0] = st.st_size;
                    } else if (block) { 
                        temp[0] = (1024/block_size)*(st.st_blocks/2);
                    } else { 
                        temp[0] = (st.st_blocks/2);
                    }
                    close(fd[READ]);
                    write(fd[WRITE],temp, sizeof(size_t));
                    close(fd[WRITE]);
                    temp[0] = 0;
                }
                while((de = readdir(dir)) != NULL && max_depth != 0){
                    if(strstr(de->d_name, "..") == NULL && de->d_name[strlen(de->d_name)-1] != '.') {
                        sprintf(auxPath, "%s/%s", path, de->d_name);
                        max_depth--;
                        getSizeType(auxPath);
                        max_depth++;
                    }
                }
                closedir(dir);
                exit(EXIT_SUCCESS);
            }
        } else{
           printf("Failed to fork\n");
           exit(EXIT_FAILURE);
        }
    }
    
    return 0;
}


int setParameters(char current[]) {
    if (strstr(current, "-a") != NULL || strstr(current, "--all") != NULL) {
        all = true;
    } else if (strstr(current, "-b") != NULL || strstr(current, "--bytes") != NULL) {
        bytes = true;
    } else if (strstr(current, "-B") != NULL || strstr(current, "--block-size") != NULL) { 
        block = true;
        stop = clock();
        double elapsed = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;
        pid_t pid = getpid();
        printExecutionErrors(elapsed, pid, ac.CREATE, current);
        return 2;
    } else if (strstr(current, "-l") != NULL || strstr(current, "--count-links") != NULL) {
        count_links = true;
    } else if (strstr(current, "-L") != NULL || strstr(current, "--dereference") != NULL) {
        dereference = true;
    } else if (strstr(current, "-S") != NULL || strstr(current, "--separate-dirs") != NULL) {
        separate_dirs = true;
    } else if (strstr(current, "--max-depth=") != NULL) {
        depth = true;
        setDepth(current);
    } else if (strstr(current, ".") != NULL) {
        strcpy(mainPath, current);
    } else {
        printf("Invalid parameter: %s\n", current);
        return 1;
    }
    return 0;
}

void sigint_handler(int signo) {
    stop = clock();
    double elapsed = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;
    pid_t pid = getpid();
    printExecutionErrors(elapsed, pid, ac.RECV_SIGNAL, "SIGINT");
    char c;
    printf(" Are you sure you want to stop your program? [Y/n] ");
    scanf("%c", &c);
    if (c == 'Y' || c == 'y') {
        printf("Program stopped by CTRL-C.\n");
        kill(0, SIGKILL);
    } else if (c == 'n' || c == 'N') {
        signal(SIGINT, sigint_handler); 
        printf("\n Cannot be terminated using Ctrl+C \n"); 
        fflush(stdout);
    } else {
        printf("Invalid input.\n");
        stop = clock();
        double elapsed = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;
        printExecutionErrors2(elapsed, pid, ac.EXIT, 1);
        exit(1);
    }
}

int main(int argc, char *argv[], char * envp[]) {
    signal(SIGINT, sigint_handler);

    char current[50]; // it will save the current argument being treated
    
    start = clock();

    if (argc <= 2) {
        fprintf(stderr, "Usage: %s dir_name\n", argv[0]);
        stop = clock();
        double elapsed = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;
        pid_t pid = getpid();
        printExecutionErrors2(elapsed, pid, ac.EXIT, 1);
        exit(1);
    }

    if ((fptr = fopen(getenv("LOG_FILENAME"),"a")) == NULL){
        printf("Error opening register file. Please set the LOG_FILENAME enviroment variable.\n");
        stop = clock();
        double elapsed = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;
        pid_t pid = getpid();
        printExecutionErrors2(elapsed, pid, ac.EXIT, 1);
        exit(1);
    }
    
    for (int i = 1; i < argc; i++) {
        strcpy(current, argv[i]);
        int par = setParameters(current); 
        if (par==1) {
            stop = clock();
            double elapsed = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;
            pid_t pid = getpid();
            printExecutionErrors2(elapsed, pid, ac.EXIT, 1);
            exit(1);
        } else if (par==2) {
            i++;
            strcpy(current, argv[i]);
            block_size = atoi(current);
        }
        stop = clock();
        double elapsed = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;
        pid_t pid = getpid();
        printExecutionErrors(elapsed, pid, ac.CREATE, current);
        
    }

    if (!count_links) {
        printf("The argument -l wasn't passed.\n");
        stop = clock();
        double elapsed = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;
        pid_t pid = getpid();
        printExecutionErrors2(elapsed, pid, ac.EXIT, 1);
        exit(1);
    }    

    if (getSizeType(mainPath) == -1) {
        stop = clock();
        double elapsed = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;
        pid_t pid = getpid();
        printExecutionErrors2(elapsed, pid, ac.EXIT, 1);
        exit(1);
    }

    stop = clock();
    double elapsed = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;
    pid_t pid = getpid();
    printExecutionErrors2(elapsed, pid, ac.EXIT, 0);

    fclose(fptr);

    return 0;

}