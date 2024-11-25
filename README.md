# System Programming Lab 11 Multiprocessing

## Implementation 

For my implementation, I let the number of children processes be command line argument and had it become a variable in the actual program. Then, the number of forks were dependent on the number of children. When creating the 50 images, I only changed the scale variable, so it looks like the video is zooming in on a specific put of the mandel. 

## Graph

[Link to Graph](https://github.com/MSOE-CPE2600/multiprocessing-turney-mahneric53394/blob/main/graph.png)

## Discussion

The graph contains the time the program took to complete for 1, 2, 5, 10, and 20 children processes. As one can see from the graph, as the processes increased from 1-20, the time decreased drastically. When more processes are used, the time decreases. For 1 child, the time is around 151 seconds, however 20 cihldren decreases the time to around 17 seconds.

