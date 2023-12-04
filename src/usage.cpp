#include "usage.hpp"

#include <iostream>

void printUsage(char *executable)
{
  std::cout << "USAGE:\n\n";
  std::cout << executable << " <input mode> [file]\n\n"
                             "Input Modes:\n\n"
                             "--manual: Chose in runtime which input format to use\n\n"
                             "--matrix: Input each weight for edge Mij | for i, j = 1..n\n"
                             "\t0 X X X X\n"
                             "\tX 0 X X X\n"
                             "\tX X 0 X X\n"
                             "\tX X X 0 X\n"
                             "\tX X X X 0\n\n"
                             "--edge: Input each edge and its weitght (x y w) | x = origin, y = destination, w = weigth\n"
                             "\tX1 Y1 W1\n"
                             "\tX2 Y2 W2\n"
                             "\t...\n\n"
                             "--weight: Input only the weights of edge ij | for i = 1..n-1 and for j = i+1..n\n"
                             "\tW1,2 W1,3 W1,4 ... W1,n\n"
                             "\tW2,3 W2,4 ... W2,n\n"
                             "\t...\n"
                             "\tWn-1,n\n\n"
                             "--file <file_path>: Read input using a file\n"
                             "\tFile format:\n"
                             "\t\tmode (0 = matrix, 1 = edge, 2 = weitght)\n"
                             "\t\tn (number of vertices)\n"
                             "\t\t[depends on the mode chosen]\n"
            << std::endl;

  exit(-1);
}