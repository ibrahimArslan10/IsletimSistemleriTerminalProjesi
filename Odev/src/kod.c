#include "kod.h"

char *  satirOku(char * buffer) {
    char * ch = buffer;
    while ((*ch = getchar()) != '\n')
        ch++;
    *ch = 0;
    return buffer;
}

void ayir(char * satir, char ** argv) {
    while (*satir != '\0' && *satir != '>' && *satir != '<') {
        // boslukları kesmek
        while (*satir == ' ' || *satir == '\t' || *satir == '\n')
            *satir++ = '\0';

        *argv++ = satir;

        while (*satir != '\0' && *satir != ' ' && *satir != '\t' && *satir != '\n')
            satir++;
    }

    *argv = 0;
}

int calistir(char * komut, char ** args, int flags, char * inputFile, char * outputFile) {
    int pid = fork();
    if (pid < 0) {
        fprintf(stderr, "%s\n", strerror(errno));
        return BITIR;
    }

    int status = 0;
    if (pid != 0) {

        // bekleme islemleri ebeveyn ve process
        waitpid (pid , &status , WUNTRACED);

        return 0;
    } else {
        int fd_in = 0;
        int fd_out = 1;
        if (flags & 1) {
            if (access(inputFile, F_OK) == -1) {
                fprintf(stderr, "%s\n", strerror(errno));
                kill(getpid(), SIGTERM);
                return BITIR;
            }
            fd_in = open(inputFile, O_RDWR);
            if (fd_in == -1) {
                fprintf(stderr, "%s\n", strerror(errno));
                kill(getpid(), SIGTERM);
                return BITIR;
            }
            dup2(fd_in, STDIN_FILENO);
            close(fd_in);
        }
        if ((flags & 2)) {
            fd_out = open(outputFile, O_RDWR | O_CREAT | O_TRUNC , S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | S_IWGRP | S_IWOTH);
            if (fd_out == -1) {
                fprintf(stderr, "%s\n", strerror(errno));
                kill(getpid(), SIGTERM);
                return BITIR; 
            }
            dup2(fd_out, STDOUT_FILENO);
            close(fd_out);
        }
        if ((flags & 4)) {
            fd_out = open(outputFile, O_RDWR | O_CREAT | O_APPEND , S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | S_IWGRP | S_IWOTH);
            if (fd_out == -1) {
                fprintf(stderr, "%s\n", strerror(errno));
                kill(getpid() , SIGTERM);
                return BITIR; 
            }
            dup2(fd_out, STDOUT_FILENO);
            close (fd_out);
        }
        if (execvp(komut, args) == -1) {
            fprintf(stderr, "%s\n", strerror(errno));
            kill(getpid() , SIGTERM);
            return BITIR;
        }
    }
    return 0;
}

void komutBolCalistir(char * komut) {
    char * args[100];
    ayir(komut, args);
    char * inputFile = NULL;
    char * outputFile = NULL;
    int flags = 0;

    int len;

    for (len = 0 ; args[len] ; len++)
        ;

    int i;
    for (i = 0 ; args[i] ; i++) {

        if (args[i][0] == '<') {
            if (i + 1 <= len)
                inputFile = args[i + 1];
            else
                inputFile = NULL;
            flags |= 1;
            args[i] = NULL;
            i++;
        }

        if (!args[i]) {
            fprintf(stderr, "%s\n", "Syntax Error");
            return;
        }

        if (args[i][0] == '>') {
            flags |= 2;
            if (strlen(args[i]) == 2 && args[i][1] == '>') {
                flags |= 4;
                flags &= ~2;
            }

            if (i + 1 <= len)
                outputFile = args[i + 1];
            else
                outputFile = NULL;

            args[i] = NULL;
            i++;
        }
    }
   calistir(komut, args, flags, inputFile, outputFile);
}
