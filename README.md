# Simulated Annealing (SA) Metaheuristic

This is a project that implements the SA metaheuristic for the matching problem in graphs.

## Building the project
The project uses CMake as building system.

Once you have CMake installed in your system just run the following commands:

```
git clone https://github.com/GustaMonteiro/simulated-annealing
cd simulated-annealing
mkdir build
cd build
cmake ..
cmake --build .
```

With that done you should see a 'simulated_annealing' executable.

## Running the project

There is two main ways to run the program:
1. Using input files (preferred)
2. Manually

To use the program with input files, that is the preferred way to use, just run:
```
./simulated_annealing --file <path_to_file>
```

E.g.:
```
./simulated_annealing --file given/Rinaldi_2392.txt
```

## Creating new instances

- The **first line** should be the input format. 

- The **second line** should be the number of vertices in the graph.

- The **rest of the lines** depends on the mode chosen.

The input format can be one of the following values:
- 0: matrix mode
- 1: edge mode
- 2: weight mode
- 3: coordinates mode

### Matrix mode:
```
Input each weight for edge Mij | for i, j = 1..n
	0 X X X X
	X 0 X X X
	X X 0 X X
	X X X 0 X
	X X X X 0
```

### Edge mode:
```
Input each edge and its weitght (x y w) | x = origin, y = destination, w = weigth
	X1 Y1 W1
	X2 Y2 W2
	...
```

### Weight mode:
```
Input only the weights of edge ij | for i = 1..n-1 and for j = i+1..n
	W1,2 W1,3 W1,4 ... W1,n
	W2,3 W2,4 ... W2,n
	...
	Wn-1,n
```

### Coordinates mode:
```
Input the coordinates of each vertice. Then the weights will be calculated
	X1 Y1
	X2 Y2
	X3 Y3
	...
	Xn Yn
```

## Example of instances

### Matrix

```
0
5
0 1 2 3 4
1 0 2 3 4
1 2 0 3 4
1 2 3 0 4
1 2 3 4 0
```

### Edge

```
1
4
1 2 3
1 3 3
1 4 3
2 3 3
2 4 3
3 4 3
```

### Weight

```
2
4
4 2 8
5 3
1
```

### Coordinates

```
3
5
1 4
5 3
4 1
9 6
10 4
```