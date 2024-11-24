# System Programming Lab 11 Multiprocessing

## Implementation 

For my implementation, I let the number of children processes be command line argument and had it become a variable in the actual program. Then, the number of forks were dependent on the number of children. When creating the 50 images, I only changed the scale variable, so it looks like the video is zooming in on a specific put of the mandel. 

## Graph

[Link to Graph](https://github.com/MSOE-CPE2600/multiprocessing-turney-mahneric53394/blob/labWeek11dev/graph.png)

## Discussion

The graph contains the time the program took to complete for 1, 2, 5, 10, and 20 children processes. As one can see from the graph, as the processes increased from 1-10, the time decreased drastically. However, there is a certain point where with more children the time starts to increase again, which is shown by how the time increases with 20 children. This is likely because there is a point where the number of children becomes redundant, and including too many children is useless and just takes the program longer. This means that when using multiprocessing, there is a number of processes that makes the time the lowest, which means finding that number is essential for the most efficient program.


