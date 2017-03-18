# Turing Machine Simulator

Wikipedia definition
>  A [Turing machine](https://en.wikipedia.org/wiki/Turing_machine) is an abstract machine that manipulates symbols on a strip of tape according to a table of rules; to be more exact, it is a mathematical model of computation that defines such a device. Despite the model's simplicity, given any computer algorithm, a Turing machine can be constructed that is capable of simulating that algorithm's logic.

This program simulates such a machine. Also, there's a CLI built for it. You can use it as a standalone program, or you can use the core functionality in case you'd like to integrate it in your project.

## CLI commands

|Command |Usage| Description|
|-------|----|----|
|Clear| ```clear``` |Clear the screen |
|Help| ```help``` | Show help message|
|Info| ```info <command>``` | Get info on a command |
|Add| ```add <from state> <to state> <from symbol> <to symbol> <direction l/r/s>``` | Add an instruction|
|List| ```list``` | Show a numbered list of previously entered instructions |
|Delete| ```delete <number>``` | Delete an instruction. Number can be found by using the previous command|
|Settape| ```settape <tape>``` | Set an initial tape |
|Showtape| ```showtape``` | Show the initial tape |
|Showbeg| ```showbeg``` | Execution should start from a certain state. Show beginning state name |
|Setbeg| ```setbeg <beginning state name>``` | Set a custom beginning state name. Default is "beg"
|Showend| ```showend``` | When the machine reaches the ending state, the execution stops. Shows the ending state name |
|Setend| ```setend <ending state name>```  | Set a custom ending state name |
|Start| ```start``` | Start the simulation |

All commands with their functionalities live in `cli_commands.h`. The commands aliases are registered in by using `register_command<CommandFunctionality>("alias")` function.

## Using the core functionality

The files you are looking for are 
```
  machine_instruction.h
  machine_instruction.cpp
  machine_simulator.h
  machine_simulator.cpp
  machine_tape.h
  machine_tape.cpp
```

They contain all the logic you need for creating a custom representation of the Turing machine

### Note
I was planning to make separate interfaces for Linux and Mac, but kinda stopped after finishing the Windows one. So the CLI will only work under Windows, since it uses `Windows.h` library for animations and few other things.
