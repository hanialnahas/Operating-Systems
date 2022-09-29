#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#ifdef linux
#include <sys/sendfile.h>
#endif

void libcat() {
	/**
	 * Read a <SIZE> characters from stdin and write to stdout using C library calls
	 */
	#define SIZE 1
	char buffer[SIZE];
	while((*buffer = getc(stdin)) != EOF)
		putc(*buffer, stdout);
}

void syscat() {
	/**
	 * Read a <SIZE> characters from stdin and write to stdout using system calls
	 */
	#define SIZE 1
	char buffer[SIZE];
	while(read(STDIN_FILENO, &buffer, sizeof(buffer)) > 0)
		write(STDOUT_FILENO, buffer, sizeof(buffer));
}

void sendcat() {
	/**
	 * use linux specific sendcat call to send data from the stdin to stdout without going
	 * back and forth between user and system spaces.
	 */
	#define BUFFER_SIZE 4096
	char buffer[BUFFER_SIZE];
	while(sendfile(STDOUT_FILENO, STDIN_FILENO, 0, BUFFER_SIZE) > 0);
}

int main(int argc, char **argv) {
	int c;

	void (*cat) (void) = &libcat;

	while((c = getopt(argc, argv, "lsp")) != -1) {
		switch(c) {
			case 'l':
				cat = &libcat;
				break;
			case 's':
				cat = &syscat;
				break;
			case 'p':
				#ifdef linux
				cat = &sendcat;
				#else
				cat = &libcat;
				printf("this is not a linux system, using libcat instead.\n");
				#endif
				break;
			default:
				break;
		}
	}

	(*cat)();

	return 0;
}
