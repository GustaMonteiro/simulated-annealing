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

// Function to calculate the cost of a permutation and check if it's a valid matching
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

// Simulated Annealing algorithm for finding an optimal matching
ResultData simulatedAnnealing(const std::vector<std::vector<double>> &weights, int numIterations, std::string instancePath, std::string inputMode)
{
  ResultData result;

  // Parameters for Simulated Annealing
  double Tm = 0.0001;
  double T0 = 100;
  double alpha = 0.95;

  // Seed for random number generation
  std::srand(std::time(0));

  // Number of vertices in the graph
  int n = weights.size();

  // Check if the number of vertices is odd
  bool nIsOdd = false;
  if (n % 2 != 0)
    nIsOdd = true;

  // Initialize the current best permutation randomly
  std::vector<int> currentBestPermutation(n);
  std::iota(currentBestPermutation.begin(), currentBestPermutation.end(), 0);
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(currentBestPermutation.begin(), currentBestPermutation.end(), g);

  // Initialize the best permutation and its cost
  std::vector<int> bestPermutation = currentBestPermutation;
  double bestPermutationCost = getPermutationCost(bestPermutation, weights).first;
  double currentBestPermutationCost = bestPermutationCost;

  // Initialize the ResultData structure
  result.instancePath = instancePath;
  result.inputMode = inputMode;
  result.allCalculatedCosts.push_back(bestPermutationCost);
  result.firstMatchingCost = bestPermutationCost;
  result.numberOfVertices = n;

  // Initialize temperature
  double T = T0;

  // Start the timer
  auto start = std::chrono::high_resolution_clock::now();

  // Simulated Annealing loop
  while (T > Tm)
  {
    for (int i = 0; i < numIterations; i++)
    {
      // Generate a neighboring permutation by swapping two elements
      auto neighbor = currentBestPermutation;
      int l = std::rand() % n;
      int r = std::rand() % n;
      std::iter_swap(neighbor.begin() + l, neighbor.begin() + r);

      // Calculate the cost of the neighboring permutation
      double neighborCost = getPermutationCost(neighbor, weights).first;
      result.allCalculatedCosts.push_back(neighborCost);

      // Calculate the change in cost
      double delta = neighborCost - currentBestPermutationCost;

      // Accept the neighboring permutation if it improves the cost
      if (delta < 0)
      {
        currentBestPermutation = neighbor;
        currentBestPermutationCost = neighborCost;

        // Update the best permutation if needed
        if (neighborCost < bestPermutationCost)
        {
          bestPermutation = neighbor;
          bestPermutationCost = neighborCost;
        }
      }
      else
      {
        // Accept the neighboring permutation with a certain probability
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
    // Reduce temperature
    T *= alpha;
  }

  // If the number of vertices is odd, remove the last vertex from the best permutation
  if (nIsOdd)
    bestPermutation.pop_back();

  // Stop the timer
  auto end = std::chrono::high_resolution_clock::now();

  // Calculate execution time
  std::chrono::duration<double> duration = end - start;
  result.executionTime = duration.count();
  result.bestMatching = bestPermutation;
  result.bestMatchingCost = bestPermutationCost;

  // Calculate the mean of all calculated costs
  double sum = 0.0;
  for (auto cost : result.allCalculatedCosts)
    sum += cost;
  result.calculatedCostsMean = sum / result.allCalculatedCosts.size();

  // Return the ResultData structure
  return result;
}
