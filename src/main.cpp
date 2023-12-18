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

// Map of input modes and corresponding functions for handling input
std::map<std::string, std::function<void(int, std::vector<std::vector<double>> &, std::istream &)>> inputModes = {
    {"--matrix", matrixModeInput},
    {"--edge", edgeModeInput},
    {"--weight", weightsModeInput},
    {"--coordinates", coordinatesModeInput}};

// Set of accepted input modes
std::set<std::string> acceptedModes{"--manual", "--matrix", "--edge", "--weight", "--coordinates", "--file"};

// Map to convert mode numbers to mode strings
std::map<int, std::string> modeNumberToString = {
    {0, "--matrix"},
    {1, "--edge"},
    {2, "--weight"},
    {3, "--coordinates"}};

// Main function
int main(int argc, char **argv)
{
  // Display usage information if no arguments or help flag is provided
  if (argc < 2 || (argc == 2 && (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h")))
    printUsage(argv[0]);

  // Get the input mode from the command line arguments
  std::string mode(argv[1]);

  // Check if the provided input mode is accepted
  if (acceptedModes.find(mode) == acceptedModes.end())
    printUsage(argv[0]);

  // If manual mode is selected, prompt the user for input
  if (mode == "--manual")
    getInputModeManually(mode);

  // If file mode is selected
  if (mode == "--file")
  {
    // Check if a file path is provided
    if (argc < 3)
      printUsage(argv[0]);

    // Get the file path from the command line arguments
    std::filesystem::path filePath = argv[2];

    // Open the file
    std::ifstream file(filePath);
    if (!file.is_open())
    {
      std::cout << "Not able to open file " << filePath << std::endl;
      exit(-1);
    }

    // Read the input mode and number of vertices from the file
    int m;
    file >> m;
    if (m < 0 || m > 3)
    {
      std::cout << "Input mode not recognized in file" << std::endl;
      file.close();
      exit(-1);
    }

    // Convert mode number to mode string
    mode = modeNumberToString[m];
    int n;
    file >> n;

    // Create a matrix to store weights
    std::vector<std::vector<double>> weights(n, std::vector<double>(n));

    // Call the corresponding input mode function to read data from the file
    inputModes[mode](n, weights, file);

    // Close the file
    file.close();

    // Perform simulated annealing and print result information
    ResultData result = simulatedAnnealing(weights, 6000, filePath, mode);
    result.printInfos();

    // Log the result
    Logger::log(result);

    return 0;
  }

  // If not in file mode, prompt the user for the number of vertices
  int n;
  std::cout << "Insert the number of vertices (n): ";
  std::cin >> n;

  // Create a matrix to store weights
  std::vector<std::vector<double>> weights(n, std::vector<double>(n));

  // Call the corresponding input mode function to read data from the user
  inputModes[mode](n, weights, std::cin);

  // Perform simulated annealing and print result information
  ResultData result = simulatedAnnealing(weights);
  result.printInfos();

  return 0;
}
