#include "inputModes.hpp"

#include <vector>
#include <iostream>
#include <string>
#include <cmath>

void printWeightsMatrix(const std::vector<std::vector<double>> &weights)
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
      std::cout << weights[i][j] << "\t";
    }
    if (dots)
      std::cout << " ...";
    std::cout << std::endl;
  }
  if (dots)
  {
    for (int i = 0; i < 3; i++)
    {
      for (int j = 0; j < maxRowsCols; j++)
        std::cout << ".\t";
      std::cout << "\n";
    }
    std::cout << "\n\n";
  }
}

void matrixModeInput(int n, std::vector<std::vector<double>> &weights, std::istream &stream)
{
  std::cout << "Insert each weight for edge Mij | for i, j = 1..n:" << std::endl;

  int expectedEdges = (n * (n - 1)) / 2;

  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      stream >> weights[i][j];

  printWeightsMatrix(weights);
}

void edgeModeInput(int n, std::vector<std::vector<double>> &weights, std::istream &stream)
{
  std::cout << "Insert each edge and its weights (xi yi wi) | for i = 1..n:" << std::endl;

  int expectedEdges = (n * (n - 1)) / 2;

  for (int i = 0; i < expectedEdges; i++)
  {
    int x, y;
    double w;
    stream >> x >> y >> w;

    weights[x - 1][y - 1] = w;
    weights[y - 1][x - 1] = w;
  }

  for (int i = 0; i < n; i++)
    weights[i][i] = 0;

  printWeightsMatrix(weights);
}

void weightsModeInput(int n, std::vector<std::vector<double>> &weights, std::istream &stream)
{
  std::cout << "Insert each weight of edge ij | for i = 1..n-1 and for j = i+1..n:" << std::endl;

  int expectedEdges = (n * (n - 1)) / 2;

  for (int i = 0; i < n - 1; i++)
  {
    for (int j = i + 1; j < n; j++)
    {
      double w;
      stream >> w;

      weights[i][j] = w;
      weights[j][i] = w;
    }
  }

  for (int i = 0; i < n; i++)
    weights[i][i] = 0;

  printWeightsMatrix(weights);
}

struct Point
{
  double x, y;
};

void coordinatesModeInput(int n, std::vector<std::vector<double>> &weights, std::istream &stream)
{
  std::cout << "Insert all coordinates (xi yi) | for i = 1..n:" << std::endl;

  std::vector<Point> points(n);

  for (auto &point : points)
    stream >> point.x >> point.y;

  for (int i = 0; i < n - 1; i++)
  {
    for (int j = i + 1; j < n; j++)
    {
      double deltaX = points[i].x - points[j].x;
      double deltaY = points[i].y - points[j].y;
      double distance = std::sqrt((deltaX * deltaX) + (deltaY * deltaY));

      weights[i][j] = distance;
      weights[j][i] = distance;
    }
  }

  printWeightsMatrix(weights);
}

void getInputModeManually(std::string &mode)
{
  while (true)
  {
    int m;
    std::cout << "Chose the input mode:\n0 - Matrix\n1 - Edge\n2 - Weight\n3 - Coordinates\n\n";
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
    else if (m == 3)
    {
      mode = "--coordinates";
      break;
    }
    else
      std::cout << "Invalid mode, try again\n";
  }
}
