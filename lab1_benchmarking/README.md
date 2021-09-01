# Lab 1: Performance comparison of C++ tools

## Team

 - [Maksym Protsyk](https://github.com/maksprotsyk)

## Usage
To build executable 
```bash
mkdir build
cd build
cmake ..
make
```
To run program once for one method
```bash
cd bin
 ./tester "number of method" "input_file" "output_file"
```

To run program many times for all methods
```bash
python3 script.py "number of testing cycles" "input_file" "output_file"
```


## Prerequisites

 - C++17
 - CMake 3.15+
 - Boost library installed


## Results

- Running the binary program will write the sum, the average and the time taken to the given file

- Running the script will create an output file for every method with the results of every program execute.
    The output file will contain best measured time for every method.
   
Results for file of size 60 Mb are in "output.txt"

Results for file of size 600 Mb are in "output_old.txt"

## Important

When I executed the program with input file
 of size around 60 Mb the taken time was just
 a few seconds, so I also executed the program using the file
 of size 600 Mb (However, it can't be uploaded to github). All results for
 this input are in files which are marked as "old".

