#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <math.h>

std::ifstream myFile;

void writeAndPrintDataFile(int numberOfCities, std::vector<std::vector<int>>& matrix, std::vector<int>& perm) {
    if (!myFile.good())
        std::cout << "Can't open file" << std::endl;
    else {
        int j = 0;
        while (myFile.good() && j != numberOfCities + 2) {
            std::string line;
            getline(myFile, line);
            std::istringstream lline (line);
            for (int i = 0; i < numberOfCities + 1; i++) {
                lline >> matrix[j][i];
            }
            j++;
        }
        for (int i = 0; i < numberOfCities + 1; i++) {
            for (int j = 0; j < numberOfCities; j++)
                std::cout << matrix[i][j] << " ";
            std::cout << std::endl;
        }
        std::cout << std::endl;
        for (unsigned long i = 0; i < perm.size(); i++)
            perm[i] = matrix[numberOfCities + 1][i];
        for (unsigned long i = 0; i < perm.size(); i++)
            std::cout << perm[i] << " ";
    }
    std::cout << std::endl << std::endl;
}

void calculatePermutationResult(std::vector<std::vector<int>>& matrix, std::vector<int>& perm, int& result) {
    result = 0;
    for (int i = 0; i < perm.size(); i++) {
        result += matrix[perm[i]+1][perm[i+1]];
    }
}

int generateRandomNumber (int from, int to) {
    int randomNumber = rand() % (to - from) + from;
    return randomNumber;
}

double generateRandomDoubleNumber (double from, double to) {
    double randomNumber = (double)rand() / RAND_MAX;
    return from + randomNumber * (from - to);
}

void checkIfEqual(int firstRandomNumber, int secondRandomNumber) {
    if (firstRandomNumber == secondRandomNumber) {
        while (firstRandomNumber == secondRandomNumber) {
            secondRandomNumber = generateRandomNumber(1, 8);
        }
    }
}

void switchPlaces(int firstPosition, int secondPosition, std::vector<int>& perm) {
    int tempPosition = perm[secondPosition];
    perm[secondPosition] = perm[firstPosition];
    perm[firstPosition] = tempPosition;
}

void objectiveFunction(std::vector<std::vector<int>>& matrix, std::vector<int>& perm, int& result) {
    int perm1Result = 0;
    int perm2Result = 0;
    perm.push_back(0);
    for (int i = 0; i < perm.size(); i++) {
        perm1Result += matrix[perm[i] + 1][perm[i + 1]];
        if (perm[i + 1] == 0) {
            for (int j = i; j < perm.size() - 2; j++) {
                perm2Result += matrix[perm[j + 1] + 1][perm[j + 2]];
            }
            break;
        }
    }
    if (perm1Result > perm2Result)
        result = perm1Result;
    else
        result = perm2Result;
}

void calculateSAResult(int numberOfCities, std::vector<std::vector<int>>& matrix, std::vector<int>& perm, int& saResult) {
    // variables initialization
    int initialResult; // x0
    calculatePermutationResult(matrix, perm, initialResult);
    int currentResult = initialResult; // x
    int bestResult = currentResult;
    int newResult = currentResult;
    double initialTemperature = 1000;
    double currentTemperature = initialTemperature;
    double finalTemperature = 0.01;
    double delta;
    double p;
    double z;
    double coolingRate = 0.995;
    // Simulated Annealing
    while (currentTemperature > finalTemperature) {
        int firstRandomNumber;
        firstRandomNumber = generateRandomNumber(1, numberOfCities + 1);
        int secondRandomNumber;
        secondRandomNumber = generateRandomNumber(1, numberOfCities + 1);
        checkIfEqual(firstRandomNumber, secondRandomNumber);
        switchPlaces(firstRandomNumber, secondRandomNumber, perm);
        objectiveFunction(matrix, perm, newResult);
        if (bestResult > newResult)
            bestResult = newResult;
        if (newResult <= currentResult)
            currentResult = newResult;
        else {
            delta = newResult - currentResult;
            p = exp(-delta/currentTemperature);
            z = -generateRandomDoubleNumber(0, 1); // TODO - erase "-"
            if (z < p)
                currentResult = newResult;
            currentTemperature *= coolingRate;
        }
    }
    saResult = bestResult;
}

int main() {
    // variables initialization
    int numberOfCities = 0;
    int saResult = 0;

    srand(time(NULL));
    myFile.open("data.txt");
    std::string s;
    myFile >> s;
    std::istringstream (s) >> numberOfCities;
    std::vector <std::vector <int>> matrix (numberOfCities+2, std::vector<int>(numberOfCities));
    std::vector <int> perm (numberOfCities + 1);

    // problem solving
    writeAndPrintDataFile(numberOfCities, matrix, perm);
    myFile.close();
    calculateSAResult(numberOfCities, matrix, perm, saResult);
    std::cout << "Simulated annealing result: " << saResult << std::endl;
}
