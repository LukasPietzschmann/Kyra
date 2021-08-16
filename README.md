# The Slanguage programming language

## What is Slanguage?
### For me:
Slanguage is just a Hobby Project of mine. Don't expect to come across any new industry-disrupting features. I'm trying to implement a basic language to become more familiar with C++ and the magic behind compilers and interpreters.

### For others:
Well, I think I'll describe this when I have convinced the interpreter to function :)
Until then, many concepts of the language might change.

## Compile the Project
To compile the Code yourself, execute the following steps:
1. Navigate to the projects root
2. Create a new folder (eg. Debug) with `mkdir Debug && cd Debug`
3. Within this folder, issue the command `cmake .. -DCMAKE_BUILD_TYPE=Release`
4. Now you can build the project by calling `make Slanguage`. The resulting executable can be found under `./bin`

## Run Tests
The project uses [GoogleTest](https://github.com/google/googletest). To run all test follor these instructions:
1. Navigate to the projects root
2. Create a new folder (eg. Debug) with `mkdir Debug && cd Debug`
3. Within this folder, issue the command `cmake .. -DCOMPILE_TESTS=ON`
4. Build the `Tests` target with `make Tests`. This will automatically run all tests
> If you already build the target `Tests` you can always rerun the tests using `ctest` or by executing the binary under
> `Debug/bin/Tests`

> To disable automatic testing after building, invoke cmake with `-DRUN_TESTS_AFTER_BUILD=OFF`