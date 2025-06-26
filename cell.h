#ifndef CELL_H
#define CELL_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<errno.h>
#include<stdbool.h>
#include<sysexits.h>



#define Y "\033[1;33m"
#define G "\033[1;32m"
#define C "\033[1;36m"
#define RED "\033[1;31m"
#define RST "\033[0m"

#define p(...) printf(__VA_ARGS__)
#define ERROR(msg) fprintf(stderr, RED msg RST "\n")
#define DEL "\n\t \v\f\r"
#define CELL_Jr 0

enum{
	OK,
	ERROR
};

typedef struct s_builtin
{
	const char *builtin_name;
	int (*foo)(char **);
} t_builtin;

#include "utils.c"
#include "builtin.c"

int cell_exit(char **);
int cell_env(char **);
int cell_echo(char **);

void Chdir(const char *);
pid_t Fork();
void Execvp(const char* file, char *const argv[]);
pid_t Wait(int *status);
pid_t Waitpid(pid_t pid, int *status, int options);

void SpinnerLoading();
void Getcwd(char *, size_t);
void Getline(char **, size_t *, FILE *);
void printbanner();
void *Malloc(size_t);
void *Realloc(void *, size_t);

#endif
