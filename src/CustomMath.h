#pragma once

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

#include <glm/glm.hpp>
#include <Eigen/Dense>

class Pascal
{
	public:
		static double GetValue(int n, int i);
		static void PrintPascal();

	private:
		// Use "double" to hold large number
		static std::vector<std::vector<double>> m_PascalValue;
};

float Lerp(float a, float b, float t);
float Clamp(float val, float min, float max);

// Bi(t) = dPi * (1-t)^(d-i) * t^i
float BernsteinPolynomial(int i, int degree, float t);
// NLI Method
float NestedLinearInterpolation(std::vector<float> coefficientList, float t);
glm::vec2 NestedLinearInterpolation(std::vector<glm::vec2> positionList, float t);
glm::vec2 NestedLinearInterpolation(std::vector<glm::vec2> positionList, float t, std::vector<std::vector<glm::vec2>>& shellPosition);
glm::vec3 NestedLinearInterpolation(std::vector<glm::vec3> positionList, float t);
glm::vec3 NestedLinearInterpolation(std::vector<glm::vec3> positionList, float t, std::vector<std::vector<glm::vec3>>& shellPosition);
// Mid Point (Divide-Conquer)
std::vector<glm::vec2> MidPointSubDivision(const std::vector<glm::vec2>& positionList);
std::vector<glm::vec2> MidPointSubDivision(const std::vector<glm::vec2>& positionList, int division);

std::vector<glm::vec3> MidPointSubDivision(const std::vector<glm::vec3>& positionList);
std::vector<glm::vec3> MidPointSubDivision(const std::vector<glm::vec3>& positionList, int division);

// Newton Form
// -Use double variable type since when we substitute there are a chance that can overflow (in case)
// -- p(t) = [...]g + [...]gt + [...]gt(t-1) + ... + [...]g(t!)
enum NewtonFormType
{
	X, Y
};
// -CreateNewtonForm use when initialize the graph
void CreateNewtonFormTable(const std::vector<glm::vec2>& positionList, NewtonFormType type, std::vector<std::vector<double>>& coeffTable);
// -Update use when drag point or add/remove point
void UpdateNewtonFormTable(const std::vector<glm::vec2>& positionList, NewtonFormType type, std::vector<std::vector<double>>& coeffTable, const std::vector<int>& tList, int startUpdateFromIndex = 0);
// -Use the find final value aka. interpolated with t
double SubstituteNewtonForm(float t, std::vector<std::vector<double>>& coeffTable, const std::vector<int> &tList);

// Interpolating Cubic Spline
double Truncate(double t, double c, int p);
Eigen::MatrixXf InterpolatingCubic(const std::vector<glm::vec2>& positionList);
float SolveInterpolationCubic(const Eigen::VectorXf& resultCoeff, float t);

// B-Spline Function
float NestedLinearInterpolation_DeBoor(std::vector<float> coefficientList, float t, int degree, int N);
float DividedDifference_BSpline(float t, int degree, int N, int i);
float Cramer_BSpline(float t, int degree, int N, int i);

// Polar form
glm::vec2 PolarForm(const std::vector<glm::vec2>& basePoint, const std::vector<float>& coefficientList, int startIdx, int endIdx);
glm::vec2 NestedLinearInterpolation_DeBoor(const std::vector<glm::vec2>& basePoint, std::vector<float>& coefficientList, float t);

// DeBoor B-Spline
glm::vec2 DeBoor_BSpline(const std::vector<glm::vec2>& basePoint, std::vector<float>& coefficientList, int degree, float t); glm::vec2 DeBoor_BSpline(const std::vector<glm::vec2>& basePoint, std::vector<float>& coefficientList, int degree, float t);
glm::vec2 DeBoor_BSpline(const std::vector<glm::vec2>& basePoint, std::vector<float>& coefficientList, int degree, float t); glm::vec2 DeBoor_BSpline(const std::vector<glm::vec2>& basePoint, std::vector<float>& coefficientList, int degree, float t, std::vector<std::vector<glm::vec2>>& shellPosition);