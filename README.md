# System Programming Lab 11 Multiprocessing

## Implementation 

For my implementation, I let the number of children processes be command line argument and had it become a variable in the actual program. Then, the number of forks were dependent on the number of children. When creating the 50 images, I only changed the scale variable, so it looks like the video is zooming in on a specific put of the mandel. 

## Graph

[Link to Graph](https://github.com/MSOE-CPE2600/multiprocessing-turney-mahneric53394/blob/main/graph.png)

## Discussion

The graph contains the time the program took to complete for 1, 2, 5, 10, and 20 children processes. As one can see from the graph, as the processes increased from 1-20, the time decreased drastically. When more processes are used, the time decreases. For 1 child, the time is around 151 seconds, however 20 cihldren decreases the time to around 17 seconds.


# System Programming Lab 12 Multithreading

## Implementation

For my implementation, I let the number of threads be a command line argument and had it become a variable in the program. The default number of threads is set to be 10 if there is no command line argument for it or if it is an invalid number. The threads are then used to compute separate regions of each image, with higher number of threads meaning each thread computes a smaller region, and a lower number of threads mean each thread computes a larger region (so that takes longer). 

## Table

[Link to Table](https://github.com/MSOE-CPE2600/multiprocessing-turney-mahneric53394/blob/lab12dev/table.png)

## Discussion

The table contains the time the program took to complete the image processing for 1, 2, 5, 10, 20 children and 1, 2, 5, 10, 20 threads, and every match of them in between. Typically, as the children and threads increased, the time decreased which was expected. However, as the children and threads increased, the amount the time decreased by started to go down, so the children and threads were making a lot less of an impact on the runtime of the program. 

i. Multithreading seemed to impact the runtime more than multiprocessing, but they both significantly impacted the runtime. Threading had more of an impact probably because they have shared memory, so all the threads are done at the same time with a shared memory.

ii. The optimal runtime was 20 children and 20 threads for my system, but it seems like a lot of people's systems has a different optimal runtime. Also, a lot of my runtimes were very similar and did not make super significant impacts at higher children and threads.