#include "matching.hpp"
#include <iostream>

#include <algorithm>
#include <numeric>
#include <vector>
#include <random>

#include <cstdlib>
#include <ctime>
#include <cmath>

#include "defines.hpp"

std::pair<int, bool> getPermutationCost(const std::vector<int> &permutation, const std::vector<std::vector<int>> &weights)
{
  int cost = 0;
  bool validMatching = true;
  for (int i = 1; i < permutation.size(); i += 2)
  {
    int u = permutation[i - 1];
    int v = permutation[i];
    if (weights[u][v] == INF)
    {
      validMatching = false;
      break;
    }
    cost += weights[u][v];
  }
  return {cost, validMatching};
}

void testPermutation(const std::vector<int> &permutation, int *currentMinCost, std::vector<int> *currentBestPermutation, const std::vector<std::vector<int>> &weights)
{
  auto [cost, validMatching] = getPermutationCost(permutation, weights);

  if (validMatching && cost < *currentMinCost)
  {
    if (currentMinCost)
      *currentMinCost = cost;
    if (currentBestPermutation)
      *currentBestPermutation = permutation;
  }
}

int simulatedAnnealing(const std::vector<std::vector<int>> &weights)
{
  double Tm = 0.0001;
  double T0 = 100;
  double alpha = 0.95;
  int numIterations = 6000;
  std::srand(std::time(0));
  int n = weights.size();

  bool nIsOdd = false;

  if (n % 2 != 0)
    nIsOdd = true;

  std::vector<int> initialPermutation(n);
  std::iota(initialPermutation.begin(), initialPermutation.end(), 0);

  std::random_device rd;
  std::mt19937 g(rd());

  std::shuffle(initialPermutation.begin(), initialPermutation.end(), g);

  std::vector<int> bestPermutation = initialPermutation;
  std::vector<int> currentBestPermutation = initialPermutation;

  double T = T0;

  while (T > Tm)
  {
    for (int i = 0; i < numIterations; i++)
    {
      auto v = currentBestPermutation;
      int l = std::rand() % n;
      int r = std::rand() % n;

      std::iter_swap(v.begin() + l, v.begin() + r);

      int costNeighbor = getPermutationCost(v, weights).first,
          costCurrentBestPermutation = getPermutationCost(currentBestPermutation, weights).first;

      int delta = costNeighbor - costCurrentBestPermutation;

      if (delta < 0)
      {
        currentBestPermutation = v;

        int costBestPermutation = getPermutationCost(bestPermutation, weights).first;
        if (costNeighbor < costBestPermutation)
          bestPermutation = v;
      }
      else
      {
        auto exponent = ((double)-delta) / T;
        auto prob = std::exp(exponent);
        auto randomValue = (double)rand() / RAND_MAX;

        if (prob > randomValue)
          currentBestPermutation = v;
      }
    }
    T *= alpha;
  }

  if (nIsOdd)
    bestPermutation.pop_back();
  int minCost = getPermutationCost(bestPermutation, weights).first;

  std::cout << "Minimum matching:\n";

  std::cout << "(";
  for (int i = 0; i < bestPermutation.size(); i += 2)
  {
    if (n < 26)
    {
      if (i)
        std::cout << ", ";
      std::cout << (char)('A' + bestPermutation[i]) << (char)('A' + bestPermutation[i + 1]);
    }
    else
    {
      if (i)
        std::cout << " - ";
      std::cout << bestPermutation[i] << "," << bestPermutation[i + 1];
    }
  }
  std::cout << ")" << std::endl;

  return minCost;
}