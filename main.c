#include <shlobj.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <libgen.h>

int main() {
    char actualDir[1024];
    if (SHGetFolderPathA(NULL, CSIDL_PROFILE, NULL, 0, actualDir) != S_OK) {
        printf("Error getting user profile directory\n");
        return 1;
    }

    char *cmdList [] = {
        "ls",
        "cd",
        "echo",
        "exit",
        "clear"
    };

    while(1) {
        //Getting user Input & Replacing New Line
        char cmdName[256];
        printf("%s> ", actualDir);
        fgets(cmdName,256,stdin);
        cmdName[strcspn(cmdName, "\n")] = '\0';

        //Split user input to cmd & args
        char *cmd = strtok(cmdName, " ");
        char *arg = strtok(NULL, " ");

        int i;
        for(i = 0; i < sizeof(cmdList)/sizeof(char*); i++) {
            if(strcmp(cmd, cmdList[i]) == 0) {
                if(arg != NULL) {
                    if(strcmp(cmd, "echo") == 0) {
                        printf("%s\n", arg);
                        break;
                    }else if(strcmp(cmd, "cd") == 0) {
                        char newPath[1024];
                        if(strcmp(arg, "..") == 0 || strcmp(arg, "../") == 0) {
                            char* parentDir = dirname(actualDir);
                            sprintf(newPath, "%s", parentDir);
                        } else {
                            sprintf(newPath, "%s\\%s", actualDir, arg);
                        }
                        DIR* dir = opendir(newPath);
                        if (dir) {
                            sprintf(actualDir, "%s", newPath);
                            closedir(dir);
                        } else if (ENOENT == errno) {
                            printf("Directory does not exist\n");
                        } else {
                            printf("Invalid New Path\n");
                        }
                    }
                } else {
                    if(strcmp(cmd, "ls") == 0) {
                        DIR *d;
                        struct dirent *dir;
                        d = opendir(actualDir);
                        if (d) {
                            while ((dir = readdir(d)) != NULL) {
                                printf("%s\n", dir->d_name);
                            }
                            closedir(d);
                        } else {
                            printf("Error opening directory: %s\n", strerror(errno));
                        }
                        break;
                    }else if(strcmp(cmd, "clear") == 0) {
                        system("cls");
                    }else if(strcmp(cmd, "exit") == 0) {
                        exit(0);
                    }else {
                        printf("Missing Argument.\n");
                    }
                }
                break;
            }
        }

        // Command not found == Error
        if(i == sizeof(cmdList)/sizeof(char*)) {
            printf("Cmd %s not found.\n", cmdName);
        }
    }
    return 0;
}