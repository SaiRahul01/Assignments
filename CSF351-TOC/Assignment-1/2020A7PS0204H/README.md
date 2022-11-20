# CS F351 Theory of Computation Assignment 1 Template

## General Instructions

1. (Optional) Fork this repository to your GitHub account. 
2. Clone the repository to your local machine.
    > Note: If you forked the repository, clone your fork. Otherwise clone the main repository. 
3. All your source code must be present in the `src` directory. Source code present in any other directory will not be considered or evaluated. 
4. Use `templates/template.h` and `templates/templates.c` as reference and create your own files in the `src` directory as required.
5. You might want to use [vscode](https://code.visualstudio.com/) and [Makefile Tools extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.makefile-tools) with your project.
    - Open the project folder in vscode. 
    - Set the build target as `all` and launch target to the default option.


## Build Instructions
1. Open a terminal.
2. Change your directory to the directory containing the file `Makefile`.
3. Optionally clean the project directory by running the following command:

```sh
make clean # Remove all build artifacts such as ./bin, ./lib, etc.
```

4. Run the following command:

```sh
make # Compile your project and produce a ./bin/main executable file. 
```


## Run Instructions:

```sh
./bin/main # Run the program
```