#pragma once

#include <string>
#include <vector>
#include <iostream>

void printWeightsMatrix(const std::vector<std::vector<int> > &weights);
void matrixModeInput(int n, std::vector<std::vector<int> > &weights, std::istream& stream = std::cin);
void edgeModeInput(int n, std::vector<std::vector<int> > &weights, std::istream& stream = std::cin);
void weightsModeInput(int n, std::vector<std::vector<int> > &weights, std::istream& stream = std::cin);
void getInputModeManually(std::string& mode);