#pragma once

#include <vector>
#include "resultData.hpp"

ResultData simulatedAnnealing(const std::vector<std::vector<double>> &weights, int numIterations = 6000, std::string instancePath = "", std::string inputMode = "");