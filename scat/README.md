## About
Different implementations of the linux cat utility.

## Compiling and Running:
```console
$ make
$ ./scat [command-options] < [input] > [output]
```
- leaving the input or the output empty uses the stdin/stdout respectively.
- when using multiple command option only the last one wins.

### Command line options: 

| Option    | description                           |
|:---------:|:-------------------------------------:|
| `-l`      | use C library getc/putc calls         |
| `-s`      | use read/write system calls           |
| `-p`      | use linux specific *sendfile* command |