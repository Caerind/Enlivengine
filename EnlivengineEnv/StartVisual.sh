#!/bin/bash

all_solutions="`find build -maxdepth 1 -name *.sln`"
for solution in $all_solutions
do
	start $solution
done