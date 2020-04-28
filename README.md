# OS Project 1

## Compilation
Compile the project with command `make`, which will execute the `Makefile` in folder `src` and create executables `./bin/main` and `./bin/job`.

## Execution
To run the scheduler, use the command

```bash
sudo dmesg --clear
sudo ./bin/main < input-file
```

The scheduler reads the descriptions of processes from standard input and will schedule the jobs according to the policy read. To run the scheduler on all input files in the `test` folder, execute `./run.sh`. The standard output and dmesg will be stored in the `output` folder.

## Demo
`demo.sh` is similar to `run.sh`, but it only runs the scheduler on five input files, and the outputs will be stored in `demo/output`. The demo video is located at the `demo` folder as well.
