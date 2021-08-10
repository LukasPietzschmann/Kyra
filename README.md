# The Slanguage programming language

## Compile the Project
To compile the Code yourself, execute the following steps:
1. Navigate to the projects root
2. Create a new folder (eg. Debug) with `mkdir Debug && cd Debug`
3. Within this folder, issue the command `cmake .. -DCMAKE_BUILD_TYPE=Release`
4. Now you can build the project by calling `make Slanguage`. The resulting executable can be found under `./bin`