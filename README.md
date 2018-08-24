# CS 395T Final Project

## About
This is a Pin binary instrumentation tool that I wrote as part of the final project in CS 395T: Prediction Mechanisms in Computer Architecture.  

It's purpose is to count the number of unique function call histories that lead to a memory load instruction at a given address, as well as the opposite metric, the number of unique memory load addresses a given sequence of function calls leads to. The analysis was run on the PARSEC benchmarks. 

As part of the project, I examined how the above metrics changed when the length of the function call history changed, so the history length is customizable as a macro in the `CS_395T.cpp` file. Additionally, since Pin does not easily accommodate external libraries or even more recent versions of C++, I ended up implementing my own hash tables for the project (the `pin_hash` files).  

I have also included the Python script I used to generate scatterplots from raw data. Kudos to the `matplotlib` library for making this task very easy!
