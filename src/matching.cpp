#include "matching.hpp"
#include <iostream>

#include <chrono>
#include <algorithm>
#include <numeric>
#include <vector>
#include <random>

#include <cstdlib>
#include <ctime>
#include <cmath>

#include "defines.hpp"

std::pair<double, bool> getPermutationCost(const std::vector<int> &permutation, const std::vector<std::vector<double>> &weights)
{
  double cost = 0;
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

void testPermutation(const std::vector<int> &permutation, double *currentMinCost, std::vector<int> *currentBestPermutation, const std::vector<std::vector<double>> &weights)
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

double simulatedAnnealing(const std::vector<std::vector<double>> &weights, int numIterations)
{
  auto start = std::chrono::high_resolution_clock::now();

  double Tm = 0.0001;
  double T0 = 100;
  double alpha = 0.95;
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

  double bestPermutationCost = getPermutationCost(bestPermutation, weights).first;
  double currentBestPermutationCost = bestPermutationCost;
  double firstPermutationCost = bestPermutationCost;

  std::vector<double> allCosts;
  allCosts.push_back(firstPermutationCost);

  double T = T0;

  while (T > Tm)
  {
    for (int i = 0; i < numIterations; i++)
    {
      auto neighbor = currentBestPermutation;
      int l = std::rand() % n;
      int r = std::rand() % n;

      std::iter_swap(neighbor.begin() + l, neighbor.begin() + r);

      double neighborCost = getPermutationCost(neighbor, weights).first;
      allCosts.push_back(neighborCost);

      double delta = neighborCost - currentBestPermutationCost;

      if (delta < 0)
      {
        currentBestPermutation = neighbor;
        currentBestPermutationCost = neighborCost;

        if (neighborCost < bestPermutationCost)
        {
          bestPermutation = neighbor;
          bestPermutationCost = neighborCost;
        }
      }
      else
      {
        double exponent = ((double)-delta) / T;
        double prob = std::exp(exponent);
        double randomValue = (double)rand() / RAND_MAX;

        if (prob > randomValue)
        {
          currentBestPermutation = neighbor;
          currentBestPermutationCost = neighborCost;
        }
      }
    }
    T *= alpha;
  }

  if (nIsOdd)
    bestPermutation.pop_back();

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

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration = end - start;
  std::cout << "Execution time: " << duration.count() << " seconds." << std::endl;
  std::cout << "First matching cost: " << firstPermutationCost << std::endl;

  return bestPermutationCost;
}