# CS 395T Final Project

## About
This is a Pin binary instrumentation tool that I wrote as part of the final project in the graduate class CS 395T: Prediction Mechanisms in Computer Architecture. The tool was used to instrument the PARSEC benchmarks to guage the viability of using function call histories as a metric in a prefetcher.    

It's purpose is to count the number of unique function call histories that lead to a memory load instruction at a given address, as well as the opposite metric, the number of unique memory load addresses a given sequence of function calls leads to. I also kept track of the number of times a given function call history was observed as well as the number of times memory was loaded from a particular address to see if there were any relationships between the frequency of a history or load instruction and the number of unique subseqent load addresses (in the former case) or the number of unique function call histories leading up to a particular load (in the latter case).  

As part of the project, I examined how the above metrics changed when the length of the tracked function call history changed, so the history length is customizable as a macro in the `CS_395T.cpp` file. Additionally, since Pin does not easily accommodate external libraries or even more recent versions of C++, I ended up implementing my own hash tables for the project (the `pin_hash` and `pin_hash2` files).  

I have also included the Python script I used to generate scatterplots from raw data. Kudos to the `matplotlib` library for making this task very easy! You can also see some of the generated scatterplots themselves in the `CS_395T_data` directory.  

## Results
My results show that there are a few load instructions that make up the majority of loads as well as a few function call histories that make up the majority of observed histories (I refer to these as _critical_ loads and histories). This initially looks promising, but further examination shows that the number of possible load instructions following a critical history exceeds the number of unique histories leading up to a critical load. This means that predicting which addresses to prefetch from is not feasible by just using function call history as the sole metric.