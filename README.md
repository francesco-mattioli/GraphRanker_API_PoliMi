<p align="center">
  <img width="100%" src="https://www.symbola.net/wp-content/uploads/2020/07/Polimi.jpg" alt="header" />
</p>

# GraphRanker-API
## Project Assignment
The assignment consists of writing a C program that manages a ranking of 
[weighted](https://en.wikipedia.org/wiki/Graph_(discrete_mathematics)#Weighted_graph) 
[directed graphs](https://en.wikipedia.org/wiki/Graph_(discrete_mathematics)#Directed_graph). 
Given two blank-separated integers `d` and `k` which are the number of nodes in a graph and the ranking size, respectively, 
the program has to print the top `k` graphs it has seen since the beginning of the execution, based on ranking metrics defined below.
Every graph is labeled with a number representing the amount of graphs that have been read before it, starting from `0`.

The program accepts two commands:
- `AggiungiGrafo` [adjacency matrix]: add a graph to those considered to draw up the ranking

E.g.:
```
AggiungiGrafo
0,56,9
1,0,33
6,0,0
```
- `TopK`: print the labels of the top `k` graphs received until that point, on a single line, blank-separated, in any order

Graphs have to be ranked based on the following metrics:
- Sum of all minimum-length paths from node `0` to all nodes reachable from `0` shall be minimum
- Paths to nodes not reachable from node `0` are considered of zero length
- If two graphs reach the same metric value, the oldest one stays

### Execution sample
```
Input:
3,2                       3-nodes graphs, print the 2 best graphs
AggiungiGrafo             First graph added (index 0, paths sum = 4)
0,1,3
0,2,0
9,0,0
AggiungiGrafo             Second graph added (index 1, paths sum = 4)
0,0,2
1,0,6
8,2,0
AggiungiGrafo             Third graph added (index 2, paths sum = 6)
3,1,8
0,0,5
0,9,0
TopK

Output:
0 1 Or 1 0
```
