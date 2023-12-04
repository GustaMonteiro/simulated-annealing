#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <functional>
#include <map>
#include <fstream>

#include "usage.hpp"
#include "matching.hpp"
#include "inputModes.hpp"

std::map<std::string, std::function<void(int, std::vector<std::vector<int>> &, std::istream &)>> inputModes = {
    {"--matrix", matrixModeInput},
    {"--edge", edgeModeInput},
    {"--weight", weightsModeInput}};

std::set<std::string> acceptedModes{"--manual", "--matrix", "--edge", "--weight", "--file"};

std::map<int, std::string> modeNumberToString = {
    {0, "--matrix"},
    {1, "--edge"},
    {2, "--weight"}};

int main(int argc, char **argv)
{
  if (argc < 2 || (argc == 2 && (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h")))
    printUsage(argv[0]);

  std::string mode(argv[1]);
  if (acceptedModes.find(mode) == acceptedModes.end())
    printUsage(argv[0]);

  if (mode == "--manual")
    getInputModeManually(mode);

  if (mode == "--file")
  {
    if (argc < 3)
      printUsage(argv[0]);

    std::ifstream file(argv[2]);
    if (!file.is_open())
    {
      std::cout << "Not able to open file " << argv[2] << std::endl;
      exit(-1);
    }

    int m;
    file >> m;
    if (m < 0 || m > 2)
    {
      std::cout << "Input mode not recognized in file" << std::endl;
      file.close();
      exit(-1);
    }

    mode = modeNumberToString[m];
    int n;
    file >> n;

    std::vector<std::vector<int>> weights(n, std::vector<int>(n));

    inputModes[mode](n, weights, file);

    int simulatedAnnealingMinCost = simulatedAnnealing(weights);
    std::cout << "Minimum cost matching with simulated annealing is: " << simulatedAnnealingMinCost << std::endl;

    file.close();

    return 0;
  }

  int n;
  std::cout << "Insert the number of vertices (n): ";
  std::cin >> n;

  std::vector<std::vector<int>> weights(n, std::vector<int>(n));

  inputModes[mode](n, weights, std::cin);

  int simulatedAnnealingMinCost = simulatedAnnealing(weights);
  std::cout << "Minimum cost matching with simulated annealing is: " << simulatedAnnealingMinCost << std::endl;

  return 0;
}