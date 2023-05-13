#pragma once

std::vector<std::vector<double>> ReadFromFile(std::string filePath);

bool CheckCorrectData(std::vector<std::vector<double>>& matrix);

void TransformMatrix(std::vector<std::vector<double>>& matrix, std::vector<double>& freeOddsColumn);

void PrintEquations(std::vector<std::vector<double>> matrix, std::vector<double> freeOddsColumn);

std::vector<double> Gauss(std::vector<std::vector<double>>& matrix, std::vector<double> freeOddsColumn, int &flag);

void PrintAnswer(int flag, std::vector<double>& resultX);

void PrintResultX(std::vector<double>& resultX);
