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
#include "resultData.hpp"
#include "logger.hpp"

std::map<std::string, std::function<void(int, std::vector<std::vector<double>> &, std::istream &)>> inputModes = {
    {"--matrix", matrixModeInput},
    {"--edge", edgeModeInput},
    {"--weight", weightsModeInput},
    {"--coordinates", coordinatesModeInput}};

std::set<std::string> acceptedModes{"--manual", "--matrix", "--edge", "--weight", "--coordinates", "--file"};

std::map<int, std::string> modeNumberToString = {
    {0, "--matrix"},
    {1, "--edge"},
    {2, "--weight"},
    {3, "--coordinates"}};

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

    std::filesystem::path filePath = argv[2];

    std::ifstream file(filePath);
    if (!file.is_open())
    {
      std::cout << "Not able to open file " << filePath << std::endl;
      exit(-1);
    }

    int m;
    file >> m;
    if (m < 0 || m > 3)
    {
      std::cout << "Input mode not recognized in file" << std::endl;
      file.close();
      exit(-1);
    }

    mode = modeNumberToString[m];
    int n;
    file >> n;

    std::vector<std::vector<double>> weights(n, std::vector<double>(n));

    inputModes[mode](n, weights, file);

    file.close();

    ResultData result = simulatedAnnealing(weights, 6000, filePath, mode);
    result.printInfos();

    Logger::log(result);

    return 0;
  }

  int n;
  std::cout << "Insert the number of vertices (n): ";
  std::cin >> n;

  std::vector<std::vector<double>> weights(n, std::vector<double>(n));

  inputModes[mode](n, weights, std::cin);

  ResultData result = simulatedAnnealing(weights);
  result.printInfos();

  return 0;
}