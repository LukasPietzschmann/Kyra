# The Kyra programming language


[![Build (MacOS, Ubuntu, Windows)](https://github.com/LukasPietzschmann/Kyra/actions/workflows/build.yml/badge.svg?branch=master)](https://github.com/LukasPietzschmann/Kyra/actions/workflows/build.yml)
[![SonarCloud](https://github.com/LukasPietzschmann/Kyra/actions/workflows/sonarcloud.yml/badge.svg?branch=master)](https://github.com/LukasPietzschmann/Kyra/actions/workflows/sonarcloud.yml)
[![Lines of Code](https://sonarcloud.io/api/project_badges/measure?project=LukasPietzschmann_Kyra&metric=ncloc)](https://sonarcloud.io/dashboard?id=LukasPietzschmann_Kyra)
[![Code Smells](https://sonarcloud.io/api/project_badges/measure?project=LukasPietzschmann_Kyra&metric=code_smells)](https://sonarcloud.io/dashboard?id=LukasPietzschmann_Kyra)

<img src="https://github.com/LukasPietzschmann/Kyra/blob/master/Logo.png" style="display: block; margin-left: auto; margin-right: auto; width: 200px" />

```
print "Hello, World";
```

## What is Kyra?

### For me:

Kyra is just a Hobby Project of mine. Don't expect to come across any new industry-disrupting features. I'm trying to
implement a basic language to become more familiar with the magic 🪄 behind compilers and interpreters.

### For others:

Kyra is a **very** basic statically typed language, currently supporting:

- classes
- first-class functions
- mutable and immutable variables
    - strings, signed integers, booleans and empty (nil) values
- closures
- basic arithmetic expressions
- classic while loop

See [example.ky](https://github.com/LukasPietzschmann/Kyra/blob/master/example.ky) for a simple example.

For more details visit the [Wiki](https://github.com/LukasPietzschmann/Kyra/wiki). There you can also see some ideas I
have or what I'm currently implementing.

## Contribute

If you want to hack on Kyra yourselfe, please
read [the contribution guide](https://github.com/LukasPietzschmann/Kyra/blob/master/CONTRIBUTING.md) first. But in
general, I would love other people to join this project 😆

## Compile the Project

To compile the Code yourself, execute the following steps:

1. Navigate to the projects root
2. Create a new folder (e.g. Debug) with `mkdir Debug && cd Debug`
3. Within this folder, issue the command `cmake .. -DCMAKE_BUILD_TYPE=Release`
4. Now you can build the project by calling `make Kyra`. The resulting executable can be found under `./bin`

## Run Tests

The project uses [GoogleTest](https://github.com/google/googletest). To run all test follow these instructions:

1. Navigate to the projects root
2. Create a new folder (e.g. Debug) with `mkdir Debug && cd Debug`
3. Within this folder, issue the command `cmake .. -DCOMPILE_TESTS=ON`
4. Build the `Tests` target with `make Tests`. This will automatically run all tests

> If you already build the target `Tests` you can always rerun the tests using `ctest` or by executing the binary under
> `Debug/bin/Tests`

> To disable automatic testing after building, invoke cmake with `-DRUN_TESTS_AFTER_BUILD=OFF`

## Some additional stuff

- When using ninja as a build system (like I do :satisfied:), GCC and (Apple)Clang won't add ANSI escape sequences for
  colored output. To _fix_ this behaviour you can invoke cmake with `-DFORCE_COLORED_OUTPUT`. If you want to print the
  output to a text file, your probably want to disable colors again by using `-DFORCE_COLORED_OUTPUT=OFF`.
