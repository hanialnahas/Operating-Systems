## About
Linux GNU env utility
execute a command in an edited environment.

## Compiling and Running:
```console
$ make
$ ./env [command-options] [NAME=VALUE] .. -- [command] [command-options]
```
- pass a list of name=value pairs to be added to the environment.
- write a command and execute it with the new environment.

### Command line options: 

| Option    | description                           |
|:---------:|:-------------------------------------:|
| `-v`      | enable trace to stderr                |
| `-u`      | remove a variable from the environment|