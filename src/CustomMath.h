#pragma once

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

#include <glm/glm.hpp>

class Pascal
{
	public:
		static double GetValue(int n, int i);
		static void PrintPascal();

	private:
		// Use "double" to hold large number
		static std::vector<std::vector<double>> m_PascalValue;
};

// Bi(t) = dPi * (1-t)^(d-i) * t^i
float BernsteinPolynomial(int i, int degree, float t);
// NLI Method
float NestedLinearInterpolation(std::vector<float> coefficientList, float t);
glm::vec2 NestedLinearInterpolation(std::vector<glm::vec2> positionList, float t);
glm::vec2 NestedLinearInterpolation(std::vector<glm::vec2> positionList, float t, std::vector<std::vector<glm::vec2>>& shellPosition);
// Mid Point (Divide-Conquer)
std::vector<glm::vec2> MidPointSubDivision(const std::vector<glm::vec2>& positionList);
std::vector<glm::vec2> MidPointSubDivision(const std::vector<glm::vec2>& positionList, int division);
// Newton Form
enum NewtonFormType
{
	X, Y
};
double NewtonForm(const std::vector<glm::vec2>& positionList, float t, NewtonFormType type);
void InitNewtonFormTable(const std::vector<glm::vec2>& positionList, NewtonFormType type, std::vector<std::vector<double>>& coeffTable);
double SubstituteNewtonForm(float t, std::vector<std::vector<double>>& coeffTable);