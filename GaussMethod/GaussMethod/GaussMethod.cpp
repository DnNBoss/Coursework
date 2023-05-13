#include <iostream>
#include <sstream>
#include<fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include "GaussMethod.h"

std::vector<std::vector<double>> ReadFromFile(std::string filePath)
{
    std::vector<std::vector<double>> matrix;
    std::ifstream inputFile(filePath);

    if (inputFile.is_open())
    {
        std::string fileLine;
        double number;

        while (getline(inputFile, fileLine))
        {
            std::vector<double> equation;

            std::replace(fileLine.begin(), fileLine.end(), '=', ' ');
            std::istringstream stringLine(fileLine);

            while (stringLine >> number)
            {
                equation.push_back(number);
            }
            if (!equation.empty())
            {
                matrix.emplace_back(equation);
            }
        }
    }
    else
    {
        std::cout << "Файл «input.txt» не найден!" << std::endl;
    }
    inputFile.close();
    return matrix;
}

void TransformMatrix(std::vector<std::vector<double>>& matrix, std::vector<double>& freeOddsColumn)
{
    for (size_t i = 0; i < matrix.size(); i++)
    {
        freeOddsColumn.push_back(matrix[i][matrix.size()]);
        matrix[i].pop_back();
    }
}

bool CheckCorrectData(std::vector<std::vector<double>>& matrix)
{
    if (matrix.empty() || matrix.front().size() < 2 || matrix.size() + 1 != matrix.front().size())
    {
        return false;
    }

    for (size_t i = 1; i < matrix.size(); i++)
    {
        if (matrix[i].size() != matrix.front().size())
        {
            return false;
        }
    }
    return true;
}

void PrintEquations(std::vector<std::vector<double>> matrix, std::vector<double> freeOddsColumn)
{
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix.size(); j++)
        {
            std::cout << matrix[i][j] << " * X[" << j + 1 << "]";
            if (j < matrix.size() - 1)
                if (matrix[i][j + 1] > 0)
                {
                    std::cout << " + ";
                }
                else std::cout << " ";
                
        }
        std::cout << " = " << freeOddsColumn[i] << std::endl;
    }
}

std::vector<double> Gauss(std::vector<std::vector<double>>& matrix, std::vector<double> freeOddsColumn, int &flag)
{
    std::vector<double> resultX(matrix.size());
    double multiplier;

    for (int i = 0; i < matrix.size(); i++)
    {
        if (matrix[i][i] != 0)
        {
            for (int j = i + 1; j < matrix.size(); j++)
            {

                multiplier = matrix[j][i] / matrix[i][i];
                for (int k = i; k < matrix.size(); k++)
                {
                    matrix[j][k] = round((matrix[j][k] - multiplier * matrix[i][k]) * 100) / 100;
                }
                freeOddsColumn[j] = freeOddsColumn[j] - multiplier * freeOddsColumn[i];
                
                std::cout << "\nОт строки №" << j + 1 << " отнимаем строку №" << i + 1 << " умноженную на " << multiplier << std::endl << std::endl;
                PrintEquations(matrix, freeOddsColumn);
            }
           
        }
    }

    double sum = 0;
    if (std::accumulate(matrix[matrix.size() - 1].begin(), matrix[matrix.size() - 1].end(), sum) == 0)
    {
        if (freeOddsColumn[matrix.size() - 1] == 0)
        {
            flag = 1;
        }
        else
            flag = 2;
    }
    else
    {
        for (int i = matrix.size() - 1; i >= 0; i--)
        {
            multiplier = 0;
            for (int j = i + 1; j < matrix.size(); j++)
            {
                multiplier += matrix[i][j] * resultX[j];
            }
            
            resultX[i] = round((freeOddsColumn[i] - multiplier) / matrix[i][i] * 100) / 100;

            if (resultX[i] == 0)
                resultX[i] = abs(resultX[i]);
        }
        flag = 0;
    }
    return resultX;
}

void PrintAnswer(int flag, std::vector<double>& resultX)
{
    std::cout << std::endl << "Ответ:" << std::endl;

    switch (flag)
    {
    case 0:
        PrintResultX(resultX);
        break;
    case 1:
        std::cout << "Бесконечное множество решений" << std::endl;
        break;
    case 2:
        std::cout << "Нет решений" << std::endl;
        break;
    }
}

void PrintResultX(std::vector<double>& resultX)
{
    for (int i = 0; i < resultX.size(); i++)
        std::cout << "X[" << i + 1 << "] = " << resultX[i] << std::endl;
}

int main()
{
    system("chcp 1251");
    system("cls");

    std::vector<std::vector<double>> matrix;
    std::vector<double> freeOddsColumn;

    matrix = ReadFromFile("input.txt");

    if (CheckCorrectData(matrix))
    {   
        TransformMatrix(matrix, freeOddsColumn);

        PrintEquations(matrix, freeOddsColumn);
        
        std::vector<double> resultX;
        int flag = 0;

        resultX = Gauss(matrix, freeOddsColumn, flag);

        PrintAnswer(flag, resultX);
    }
    else
        std::cout << "Данные введены неверно!\nПример ввода:\n2-1+5=10\n1+1-3=-2\n2+4+1=1" << std::endl;

    system("pause");
    return 0;
}
