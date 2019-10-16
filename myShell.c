#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define delimiter " \n\r\t"
#define line "======================================\n"

void allocationError()
{
	fprintf(stderr, "Allocation Error!");
	exit(EXIT_FAILURE);
}

char *getUsername()
{
    	char *username = getenv("USERNAME");
	return username;
}

int exitShell()
{
	return 0;
}

char *promptOption()
{
	printf("Please choose/customise prompt:    \n");
	char *username = getUsername();
	printf("[1] %s>    \n", username);
	printf("[2] Create your own...\n%s", line);
	char option[1024];
	char *prompt = malloc(20 * sizeof(char));
	fgets(option, 1024, stdin);
	if (atoi(option) == 1)
	{
		prompt = strcat(username, "> ");
		printf("%sPrompt set to: %s\n%s", line, prompt, line);
	}
	else if (atoi(option) == 2)
	{
		printf("%sWrite your prompt below followed by [ENTER]...\n", line);
		fgets(prompt, 20, stdin);
		prompt[strcspn(prompt, "\n")] = 0;
		printf("%sPrompt set to: %s\n%s", line, prompt, line);
	}
	else
	{
		printf("%sIncorrect Option Selected\n%s", line, line);
		prompt = promptOption();
	}
	return prompt;
}

char *readLine()
{
	int buffSize = 1024, position = 0;
	char *buffer = malloc(buffSize * sizeof(char));
    	char c;

	if (!buffer)
	{
		allocationError();
	}

	while (1)
	{
		c = getchar();
		if (c == EOF || c == '\n')
		{
			buffer[position] = '\0';
			return buffer;
		}
		else
		{
			buffer[position] = c;
		}
		position++;
	}

	if (position >= buffSize)
	{
		buffSize += 1024;
		buffer = realloc(buffer, buffSize);

		if (!buffer)
		{
			allocationError();
		}
	}
}

int runse(char **args)
{
	pid_t pid;
	int status;

	if (strcmp(args[0], "exit") == 0)
	{
		return exitShell();
	}

	pid = fork();

	if (pid < 0)
	{
		printf("COULD NOT FORK SUCCESSFULLY...\n");
	}
	else if (pid == 0)
	{
		if (execvp(args[0], args) < 0)
		{
	    		printf("COMMAND NOT FOUND...\n");
	    		exit(EXIT_FAILURE);	
		}
	}
	else
	{
		waitpid(pid, &status, WUNTRACED);
	}
}

char **tokenize(char *input)
{
	int buffSize = 1024, position = 0;
	char **tokens = malloc(buffSize * sizeof(char *));
	char *token;

	if (!tokens)
	{
		allocationError();
	}

	token = strtok(input, delimiter);
	while (token != NULL)
	{
		tokens[position] = token;
		position++;

		if (position >= buffSize)
		{
			buffSize += 1024;
			tokens = realloc(tokens, buffSize * sizeof(char *));

			if (!tokens)
			{
				allocationError();
			}
		}

		tokens[position] = NULL;
		return tokens;
	}
}

void welcome()
{
	printf("%s          WELCOME TO myShell          \n%s",line, line);
}

int main(void)
{
	welcome();
	printf("USERNAME = %s\n", getUsername());
	char *prompt = promptOption();
	char *input;
	char **args;
	int status = 1;

	do
	{
		printf("%s", prompt);
		input = readLine();
		args = tokenize(input);
		status = runse(args);
		free(input);
		free(args);
	} while (status);
}

