# 15 puzzle
The 15 puzzle is a sliding puzzle having 15 square tiles numbered 1–15 in a frame that is 4 tiles high and 4 tiles wide, leaving one unoccupied tile position.
Tiles in the same row or column of the open position can be moved by sliding them horizontally or vertically, respectively.
The goal of the puzzle is to place the tiles in numerical order.

![](https://github.com/RezaAbbaszadeh/AI-15puzzle-Tree-Search/blob/master/game-board.jpg)


## About project
implementation of blind (BFS,DFS,UCS,BDS) and heuristic (A*,IDA*) search strategies for solving 15 (also 24) puzzle

### Blind strategies
- Breadth first search (BFS)
- Depth First Search (DFS)
- Uniform-cost search (UCS)
- Bidirectional search (BDS)

### Heuristic strategies
- A*
- Iterative Deepening A* (IDA*)

### Heuristic evaluation functions
- Manhatan distance: sum of the distances of each tile from where it belongs
- Linear Conflict: count horizontal and vertical conflicts(two tiles that are in wrong order in a row or column)
- Disjoint database: Create a database of real cost of subproblems. In this case i splited tiles to 3 groups: {1,5,6,9,10,13} , {7,8,11,12,14,15} , {2,3,4}.
For each initial state a game might have i solved the puzzle ignoring other tiles location and saved its cost in database.
<br>
In all search algorithms above, repeated nodes creation avoided using an unordered_map as visited list.

## 24 puzzle
Beacause of large state space and memory problems I used IDA* for 24 puzzle.
I merged several heuristics as mentioned in this [article](https://courses.cs.washington.edu/courses/csep573/10wi/korf96.pdf)
