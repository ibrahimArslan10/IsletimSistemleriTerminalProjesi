#ifndef KOD_H
#define KOD_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BITIR 1143


char *  satirOku(char * buffer);
void ayir(char * satir, char ** argv);
int calistir(char * komut, char ** args, int flags, char * inputFile, char * outputFile);
void komutBolCalistir(char *komut);

#endif
