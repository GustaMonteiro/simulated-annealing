#include "inputModes.hpp"

#include <vector>
#include <iostream>
#include <string>

void printWeightsMatrix(const std::vector<std::vector<int> > &weights)
{
  int n = weights.size();

  const int maxRowsCols = 18;

  bool dots = n > maxRowsCols ? true : false;
  if (n > maxRowsCols)
    n = maxRowsCols;

  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
    {
      std::cout << weights[i][j] << " ";
    }
    if (dots)
      std::cout << " ...";
    std::cout << std::endl;
  }
  if (dots)
  {
    std::cout << ".    .    .    .\n";
    std::cout << ".    .    .    .\n";
    std::cout << ".    .    .    .\n\n";
  }
}

void matrixModeInput(int n, std::vector<std::vector<int> > &weights, std::istream &stream)
{
  std::cout << "Insert each weight for edge Mij | for i, j = 1..n:" << std::endl;

  int expectedEdges = (n * (n - 1)) / 2;

  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      stream >> weights[i][j];

  printWeightsMatrix(weights);
}

void edgeModeInput(int n, std::vector<std::vector<int> > &weights, std::istream &stream)
{
  std::cout << "Insert each edge and its weights (xi yi wi) | for i = 1..n:" << std::endl;

  int expectedEdges = (n * (n - 1)) / 2;

  for (int i = 0; i < expectedEdges; i++)
  {
    int x, y, w;
    stream >> x >> y >> w;

    weights[x - 1][y - 1] = w;
    weights[y - 1][x - 1] = w;
  }

  for (int i = 0; i < n; i++)
    weights[i][i] = 0;

  printWeightsMatrix(weights);
}

void weightsModeInput(int n, std::vector<std::vector<int> > &weights, std::istream &stream)
{
  std::cout << "Insert each weight of edge ij | for i = 1..n-1 and for j = i+1..n:" << std::endl;

  int expectedEdges = (n * (n - 1)) / 2;

  for (int i = 0; i < n - 1; i++)
  {
    for (int j = i + 1; j < n; j++)
    {
      int w;
      stream >> w;

      weights[i][j] = w;
      weights[j][i] = w;
    }
  }

  for (int i = 0; i < n; i++)
    weights[i][i] = 0;

  printWeightsMatrix(weights);
}

void getInputModeManually(std::string &mode)
{
  while (true)
  {
    int m;
    std::cout << "Chose the input mode:\n0 - Matrix\n1 - Edge\n2 - Weight\n\n";
    std::cin >> m;

    if (m == 0)
    {
      mode = "--matrix";
      break;
    }
    else if (m == 1)
    {
      mode = "--edge";
      break;
    }
    else if (m == 2)
    {
      mode = "--weight";
      break;
    }
    else
      std::cout << "Invalid mode, try again\n";
  }
}
