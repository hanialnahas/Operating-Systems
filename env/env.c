#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern char **environ;

int _TRACE_FLAG = 0;

void printEnvVariables();
void addVariableToEnv(char*, char*);
int addIfValid(char*);
void removeVarFromEnv(char*);
int execCommand(char*, char**);

void printEnvVariables() {
	int i = 0;
	while(environ[i])
		printf("%s\n", environ[i++]);
}

int main(int argc, char *argv[]) {
	int status = EXIT_SUCCESS;
	if(argc == 1) printEnvVariables(); // if no other args
	else {
		char c;
		while((c = getopt(argc, argv, ":vu:x:")) != -1) {
			switch(c) {
				case 'v':
					_TRACE_FLAG = 1;
					break;
				case 'u':
					removeVarFromEnv(optarg);
					break;
				case ':':
					printf("option requires an argument -- '%c'\n", optopt);
					return EXIT_FAILURE;
				case '?':
					printf("invalid option -- '%c'\n", optopt);
					return EXIT_FAILURE;
			}
		}

		while((optind < argc) && (addIfValid(argv[optind]) != -1)) {
			optind++; // add pairs to env.
		}
		if(optind < argc) {
			int ret = execCommand(argv[optind], argv+optind); // pass first non 'name=value' string as command and rest as args.
			fprintf(stderr, "Exited with status code %d\n", ret);
			return EXIT_FAILURE;
		}
		printEnvVariables();
	}

	if(fflush(stdout) || ferror(stdout)) {
		perror("Writing to stdout failed.\n");
		status = EXIT_FAILURE;
	}

	if(fflush(stderr) || ferror(stderr)) {
		perror("Writing to stderr failed.\n");
		status = EXIT_FAILURE;
	}

	return status;
}

int addIfValid(char *var) {
	char *equal;
	if((equal = strchr(var, '=')) != NULL) {
		size_t len = equal-var; // first character to one character before '=' sign.
		char *before = strndup(var, len);
		addVariableToEnv(before, equal+1); // send everything before and after '=' sign.
		free(before);
	}
	else return -1;
}

void addVariableToEnv(char *var, char *val) {
	if(_TRACE_FLAG) fprintf(stderr, "setenv:	%s=%s\n", var, val);
	if(setenv(var, val, 1) != 0) {
		fprintf(stderr, "error setting the environment variable: %s\n", var);
	}
}

void removeVarFromEnv(char *var) {
	if(_TRACE_FLAG) fprintf(stderr, "unsetenv:	%s\n", var);
	if(unsetenv(var) != 0) {
		fprintf(stderr, "error unsetting the environment variable: %s\n", var);
	}
}

int execCommand(char *command, char **args) {
	if(_TRACE_FLAG) {
		fprintf(stderr, "executing: %s\n", command);
		int i = 0;
		while(args[i]) {
			fprintf(stderr, "\targ[%d]= '%s'\n", i, args[i]);
			i++;
		}
	}
	return execvp(command, args);
}
