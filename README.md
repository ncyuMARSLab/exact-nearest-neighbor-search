## Introduction ##
This repository finds the exact nearest neighbors from SIFT1B.  
It works on Windows with Visual Studio C++ project.  

## Input Data ##
It requires 2 input data, query and SIFT1B.  
SIFT1B is divided into 10 segments(SIFT100M).  

## Usage ##
Building the project with Visual Studio on Windows.

It finds the nearest neighbors of 5 queries per time.  
To compute the distances between 5 queries and SIFT1B, it requires at least 128G RAM.  

In line 173:  

    for (int q = 6415; q < QUERY_NUM; q += QUERY_PER_TIME) {
    
The process starts with the index q of query, the q+1th query.  
When the process stops anomalously.  
Removing the latest 5 results from the output file and modifying 'q' into the index of the 1st removed result.  
Then changing the ouput filename and restarting the process.  
After all queries are completely processed, merging all output files.  
