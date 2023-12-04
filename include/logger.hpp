#pragma once

#include <fstream>
#include "resultData.hpp"

class Logger
{
private:
  static std::string createLogFile(std::string logFileName = "")
  {
    if (logFileName == "")
      logFileName = "resultLogs.csv";
    if (std::filesystem::exists(logFileName))
      return logFileName;

    std::ofstream file(logFileName);
    if (!file.is_open())
      return std::cout << "Could not create log file\n", logFileName;

    file << "Instância,n,Formato,Início,Média,Solução (Min),tempo(s)\n";

    file.close();

    return logFileName;
  }

public:
  static void log(const ResultData &result)
  {
    std::ofstream file(createLogFile(), std::ios::app);
    if (!file.is_open())
    {
      std::cout << "Could not open log file\n";
      return;
    }

    file << result.instancePath.string() << ","
         << result.numberOfVertices << ","
         << result.inputMode << ","
         << result.firstMatchingCost << ","
         << result.calculatedCostsMean << ","
         << result.bestMatchingCost << ","
         << result.executionTime << "\n";

    file.close();
  }
};