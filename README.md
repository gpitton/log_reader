Log File Reader
===============

This repository contains the source code for the `lread` utility, useful to parse efficiently the (possibly very large) log data coming from the cluster's computations.

In particular, the tool is useful to read a log file and keep only the messages coming from a certain computing node.

Installation
------------

Make sure you have a C compiler (such as gcc or clang).
Then, execute the following commands from the terminal:
```
gcc -std=c99 -o lread && cp lread /usr/bin
```
the second command may require administrator privileges. In this case, run instead:
```
gcc -std=c99 -o lread && sudo cp lread /usr/bin
```
You may want to install the utility to some other directory than /usr/bin. In this case, make sure that the appropriate directory is sourced by introducing the following line in your .bashrc:
```
export PATH=$PATH:/path/to/lread/location
```


Usage
-----
The typical call to this utility is:
```
lread --file log.txt --node node-356.master1 --output mess.356.1.txt
```

- the flag `--file` specifies the path to a log file;
- the flag `--node` specifies the name of the node whose output messages should be saved;
- the flag `--output` specifies the text file to which the messages extracted from the log file will be saved.


A brief recap on how the tool works can be found by typing:
```
lread --help
```
