#include"cell.h"


t_builtin g_builtin[] =
{
	{"echo", cell_echo},
	{"env", cell_env},
	{"exit", cell_exit},
	{NULL}
};

int status = 0;
bool t = false;

void cell_launch(char **args)
{
	if(Fork() == CELL_Jr)
		Execvp(args[0], args);
	else
		Wait(&status);

}

void cell_exec(char **args)
{
	int i;
	const char *curr;

	if(!args || !args[0])
		return;

	i=0;
	while((curr = g_builtin[i].builtin_name))
	{
		if(!strcmp(curr, args[0]))
		{
			if((status =  g_builtin[i].foo(args)))
				p("%s failed\n", curr);
			return;
		}
		i++;
	}

	cell_launch(args);
}

char **cell_split_line(char *line)
{
	char **tokens;
	unsigned long position;
	size_t bufsize;;

	bufsize = BUFSIZ;
	tokens = Malloc(bufsize * sizeof *tokens);
	position = 0;

	for(char *token = strtok(line, DEL); token; token = strtok(NULL, DEL))
	{
		tokens[position++] = token;
		if (position >= bufsize)
		{
			bufsize *= 2;
			tokens = Realloc(tokens, bufsize * sizeof *tokens);
		}

	}
	tokens[position]=NULL;
	return tokens;
}


char* cell_read_line(void)
{
	char *buf;
	size_t bufsize;
	char cwd[BUFSIZ];

	buf = NULL;
	
	if(isatty(fileno(stdin)))
	{
		if(status)
		{
			Getcwd(cwd, sizeof(cwd));
			p(RED"\nExit status: %d"RST, status);
			p("\n\n 🦠"C"[%s]"RED"[%d]"RST" 🦠 $->",cwd, status);

			//p("$->");
		}
		else
		{
			Getcwd(cwd, sizeof(cwd));
			if(t) p(G"\nExit status: %d"RST"\n", status);
			t = true;
			p("\n 🦠"C"[%s]"RST" 🦠 $->",cwd);
		}


	}
	/* if(getline(&buf, &bufsize, stdin) == -1)
	{
		free(buf);
		buf = NULL;
		if(feof(stdin))
			p(RED"[EOF]"RST);
		else
			p(RED"Getline failed"RST);

		p("%s\n", buf);
	}
	*/
	Getline(&buf, &bufsize, stdin);
	return buf;
}


int main()
{
	p(" ✅ Compilation Ok!  ✅ \n\n");
	
	printbanner();
	char *line;
	char **args;

	while((line = cell_read_line()))
	{
		args = cell_split_line(line);

		if(args[0] && !strcmp(args[0], "cd"))
			Chdir(args[1]);

		cell_exec(args);

		free(line);
		free(args);
	}

	return EXIT_SUCCESS;
}

