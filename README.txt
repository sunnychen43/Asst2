Asst2: Plagiarism Detector
Sunny Chen and Michael Zhao

Description:
This program implements a basic plagiarism detector. When run, it scans the given directory and subdirectories for all files, and computes the distribution of words for each file. Next, our program finds the Jensen-Shannon distance for each pair of files. Finally, we print the color-coded distance for each pair, ordered from smallest to greatest number of tokens between the two files.


Included Files:
    Asst2.c         - contains main routine for program. Handles dir traversal and file tokenizing
    analyzer.c      - performs all mathematical operations: KLD, JSD, finding mean distribution
    datastructs.c   - defines data structures and related helper functions
    reader.c        - contains HashTable and helper functions for tokenizing
    
    testcases.txt   - brief description for each of our testcases
    testcases/      - folder containing our testcases to run

    and all associated .h files


Usage:
 > make && ./detector "."  (runs program on current directory)
 > make && ./detector "C:\User\Name\filepath" (runs program on indicated file path)


Operation:
Our code is split into 3 main components: dir/file handling, computing distribution, and analysis

Dir/File handling - 
Using the UNIX filesystem functions, we check the directory for all files and scan subdirectories recursively. For each file found, we spawn a new pthread and call read_file() with the file path as an argument.
NOTE: We only handle files with threads. If a directory contains a huge nest of subdirectories, that could use a bunch of threads so we try to avoid it.

read_file() will tokenize all the words in the given file, storing them in a hashtable to keep track of frequencies. We use a Token data structure defined in reader.h to store tokens which can handle words of variable size while ignoring punctuation except for hyphens. Each pthread will have a seperate HashTable, storing the frequencies of words for its respective file.


Computing Distribution - 
Once we have the hashtable of frequencies for each file, we iterate through each key/value pair to generate a TokenList. A TokenList is a linked list of tokens, sorted alphabetically, containing the probability distribution of each file. Each file will have its own TokenList containing the token distribution. We store the file's name, total number of tokens, and associated TokenList in another linked list called a FileList. This FileList is a shared data structure across all threads containing information about all the files. We use a mutex when adding elements to the FileList. Once all threads have completed, the FileList will contain the token distribution for all scanned files.


Analysis -
The analyze() function recieves a FileList of all files in the original directory and compares each pair of files. For each pair, it generates a mean distribution TokenList and uses that to compute the KLD and JSD. Since our output must be ordered from least to greatest number of tokens, we store the JSD for each pair of files and the sum of their tokens in an OutputList. OutputList is a sorted linked list, containg all pairs of files, their number of tokens, and their JSD. Once all pairs have been processed, we go through the OutputList and print the JSD and file names for each pair. Depending on how "similar" the files are, the JSD will be color coded from RED (very similar) to WHITE (not similar).



