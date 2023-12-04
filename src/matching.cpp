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
#include "resultData.hpp"

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

ResultData simulatedAnnealing(const std::vector<std::vector<double>> &weights, int numIterations, std::string instancePath, std::string inputMode)
{
  ResultData result;

  double Tm = 0.0001;
  double T0 = 100;
  double alpha = 0.95;
  std::srand(std::time(0));
  int n = weights.size();

  bool nIsOdd = false;

  if (n % 2 != 0)
    nIsOdd = true;

  std::vector<int> currentBestPermutation(n);
  std::iota(currentBestPermutation.begin(), currentBestPermutation.end(), 0);

  std::random_device rd;
  std::mt19937 g(rd());

  std::shuffle(currentBestPermutation.begin(), currentBestPermutation.end(), g);

  std::vector<int> bestPermutation = currentBestPermutation;

  double bestPermutationCost = getPermutationCost(bestPermutation, weights).first;
  double currentBestPermutationCost = bestPermutationCost;

  result.instancePath = instancePath;
  result.inputMode = inputMode;
  result.allCalculatedCosts.push_back(bestPermutationCost);
  result.firstMatchingCost = bestPermutationCost;
  result.numberOfVertices = n;

  double T = T0;

  auto start = std::chrono::high_resolution_clock::now();

  while (T > Tm)
  {
    for (int i = 0; i < numIterations; i++)
    {
      auto neighbor = currentBestPermutation;
      int l = std::rand() % n;
      int r = std::rand() % n;

      std::iter_swap(neighbor.begin() + l, neighbor.begin() + r);

      double neighborCost = getPermutationCost(neighbor, weights).first;
      result.allCalculatedCosts.push_back(neighborCost);

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

  auto end = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double> duration = end - start;

  result.executionTime = duration.count();
  result.bestMatching = bestPermutation;
  result.bestMatchingCost = bestPermutationCost;

  double sum = 0.0;
  for (auto cost : result.allCalculatedCosts)
    sum += cost;

  result.calculatedCostsMean = sum / result.allCalculatedCosts.size();

  return result;
}
