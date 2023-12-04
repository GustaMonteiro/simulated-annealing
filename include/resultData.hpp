#pragma once

#include <vector>
#include <filesystem>
#include <iostream>

struct ResultData
{
  int numberOfVertices = 0;

  std::filesystem::path instancePath = "";
  std::string inputMode = "";

  std::vector<int> bestMatching;
  std::vector<double> allCalculatedCosts;

  double bestMatchingCost = 0;
  double firstMatchingCost = 0;
  double executionTime = 0;
  double calculatedCostsMean = 0;

  void printBestMatching()
  {
    std::cout << "Minimum matching:\n";

    std::cout << "(";
    for (int i = 0; i < bestMatching.size(); i += 2)
    {
      if (numberOfVertices < 26)
      {
        if (i)
          std::cout << ", ";
        std::cout << (char)('A' + bestMatching[i]) << (char)('A' + bestMatching[i + 1]);
      }
      else
      {
        if (i)
          std::cout << " - ";
        std::cout << bestMatching[i] << "," << bestMatching[i + 1];
      }
    }
    std::cout << ")" << std::endl;
  }

  void printExecutionTime()
  {
    std::cout << "Execution time: " << executionTime << " seconds" << std::endl;
  }

  void printInfos()
  {
    std::cout << "Instance: " << instancePath.string() << std::endl;
    std::cout << "Input mode: " << inputMode << std::endl;
    std::cout << "Number of vertices: " << numberOfVertices << std::endl;
    std::cout << "First matching cost: " << firstMatchingCost << std::endl;
    std::cout << "Mean of costs: " << calculatedCostsMean << std::endl;
    std::cout << "Best matching cost: " << bestMatchingCost << std::endl;
    printExecutionTime();
    printBestMatching();
  }
};