#include "CustomMath.h"

// ---------------------- Class Pascal ----------------------
std::vector<std::vector<double>> Pascal::m_PascalValue;

// Get PascalValue using iterative method
double Pascal::GetValue(int n, int i)
{
	// If m_PascalValue already contain solutions -> return (no need to compute anymore)
	if (m_PascalValue.size() > n)
		return m_PascalValue[n][i];

	int row = m_PascalValue.size();
	for (int r = row; r <= n; r++)
	{
		std::vector<double> currentRow;
		for (int c = 0; c <= r; c++)
		{
			if (c == 0 || c == r)
				currentRow.push_back(1);
			else
				currentRow.push_back(m_PascalValue[r - 1][c] + m_PascalValue[r - 1][c - 1]);
		}
		m_PascalValue.push_back(currentRow);
	}
	return m_PascalValue[n][i];
}

// Print PascalValue for debugging
void Pascal::PrintPascal()
{
	for (int r = 0; r < m_PascalValue.size(); r++)
	{
		for (int space = 0; space < m_PascalValue.size() - r - 1; space++)
		{
			std::cout << " ";
		}
		for (int c = 0; c <= r; c++)
		{
			std::cout << m_PascalValue[r][c] << " ";
		}
		std::cout << "\n";
	}
}
// ----------------------------------------------------------

float BernsteinPolynomial(int i, int degree, float t)
{
	return Pascal::GetValue(degree, i) * std::pow(1.0f - t, degree - i) * std::pow(t, i);
}

float NestedLinearInterpolation(std::vector<float> coefficientList, float t)
{
	int degree = coefficientList.size() - 1;
	for (int i = 1; i <= degree; i++)
	{
		for (int j = 0; j <= degree - i; j++)
		{
			// Calculate new Coefficients using previous coefficients
			coefficientList[j] = (1.0f - t) * coefficientList[j] + t * coefficientList[j + 1];
		}
		coefficientList.pop_back();
	}
	return coefficientList.back();
}
glm::vec2 NestedLinearInterpolation(std::vector<glm::vec2> positionList, float t)
{
	int degree = positionList.size() - 1;
	for (int i = 1; i <= degree; i++)
	{
		for (int j = 0; j <= degree - i; j++)
		{
			// Calculate new Coefficients using previous coefficients
			positionList[j] = (1.0f - t) * positionList[j] + t * positionList[j + 1];
		}
		positionList.pop_back();
	}
	return positionList.back();
}
glm::vec2 NestedLinearInterpolation(std::vector<glm::vec2> positionList, float t, std::vector<std::vector<glm::vec2>>& shellPosition)
{
	int degree = positionList.size() - 1;
	for (int i = 1; i <= degree; i++)
	{
		for (int j = 0; j <= degree - i; j++)
		{
			// Calculate new Coefficients using previous coefficients
			positionList[j] = (1.0f - t) * positionList[j] + t * positionList[j + 1];
		}
		shellPosition.push_back(positionList);
		positionList.pop_back();
	}
	return positionList.back();
}

std::vector<glm::vec2> MidPointSubDivision(const std::vector<glm::vec2>& positionList)
{
	if (positionList.size() == 1)
		return positionList;

	// Initialize Left Side (contain P0)
	std::vector<glm::vec2> left;
	left.push_back(positionList.front());

	// Initialize Right Side (contain Pn)
	std::vector<glm::vec2> right;
	right.push_back(positionList.back());

	std::vector<glm::vec2> curPoint(positionList);
	int degree = positionList.size() - 1;
	for (int i = 0; i < degree; i++)
	{
		std::vector<glm::vec2> tmp;
		for (int idx = 0; idx < curPoint.size() - 1; idx++)
		{
			tmp.push_back(0.5f * (curPoint[idx] + curPoint[idx + 1]));

			if (idx == 0)
				left.push_back(tmp.back());
			else if (idx == curPoint.size() - 2)
				right.push_back(tmp.back());

		}
		curPoint = tmp;
	}
	std::reverse(right.begin(), right.end());

	std::vector<glm::vec2> result(left.begin(), left.end());
	result.insert(result.end(), right.begin(), right.end());
	return result;
}
std::vector<glm::vec2> MidPointSubDivision(const std::vector<glm::vec2>& positionList, int division)
{
	if (division == 0)
		return positionList;
	if (division == 1)
		return MidPointSubDivision(positionList);

	std::vector<glm::vec2> ref = MidPointSubDivision(positionList);

	// Divide
	int halfSize = ref.size() / 2;
	std::vector<glm::vec2> left(ref.begin(), ref.begin() + halfSize + 1);
	left = MidPointSubDivision(left, division - 1);
	std::vector<glm::vec2> right(ref.begin() + halfSize, ref.end());
	right = MidPointSubDivision(right, division - 1);

	// Conquer Merge Left & Right Vector together
	left.pop_back();
	std::vector<glm::vec2> result(left.begin(), left.end());
	result.insert(result.end(), right.begin(), right.end());
	return result;
}

void CreateNewtonFormTable(const std::vector<glm::vec2>& positionList, NewtonFormType type, std::vector<std::vector<double>>& coeffTable)
{
	// Set Degree
	int degree = positionList.size() - 1;

	std::vector<double> currentCoeffList;
	// Clear coeffTable & Set size
	coeffTable.clear();
	coeffTable.resize(degree + 1);

	// Set Value on first layer
	for (int i = 0; i <= degree; i++)
	{
		if (type == NewtonFormType::X)
			currentCoeffList.push_back(positionList[i].x);
		else
			currentCoeffList.push_back(positionList[i].y);
	}

	// Each layer interpolate and push into coeffTable
	for (int i = 0; i <= degree; i++)
	{
		std::vector<double> tmp(degree - i + 1);
		if (i == 0)
		{
			for (int j = 0; j <= degree; j++)
			{
				tmp[j] = currentCoeffList[j];
			}
		}
		else
		{
			for (int j = 0; j <= degree - i; j++)
			{
				tmp[j] = (currentCoeffList[j + 1] - currentCoeffList[j]) / i;
			}
		}

		currentCoeffList = tmp;
		coeffTable[i] = tmp;
	}
}
void UpdateNewtonFormTable(const std::vector<glm::vec2>& positionList, NewtonFormType type, std::vector<std::vector<double>>& coeffTable, const std::vector<int>& tList, int startUpdateFromIndex)
{
	// Set Degree
	int degree = positionList.size() - 1;

	// If Size of CoefficientTable < degree + 1 -> Allocate More Memory
	if (coeffTable.size() < degree + 1)
		coeffTable.resize(degree + 1);

	// If Size of CoefficientTable > degree + 1 -> pop the exceed coefficient
	while (coeffTable.size() > degree + 1)
	{
		for (int i = 0; i < coeffTable.size(); i++)
			coeffTable[i].pop_back();
		coeffTable.pop_back();
	}

	// Update the newValue from [startUpdateFromIndex, degree]
	for (int i = startUpdateFromIndex; i <= degree; i++)
	{
		if (type == NewtonFormType::X)
		{
			if (i < coeffTable[0].size())
				coeffTable[0][i] = positionList[i].x;
			else
				coeffTable[0].push_back(positionList[i].x);
		}
		else
		{
			if (i < coeffTable[0].size())
				coeffTable[0][i] = positionList[i].y;
			else
				coeffTable[0].push_back(positionList[i].y);
		}
	}

	// Start interpolating value
	for (int i = 1; i <= degree; i++)
	{
		for (int j = std::max(startUpdateFromIndex - i, 0); j <= degree - i; j++)
		{
			if (j < coeffTable[i].size())
				coeffTable[i][j] = (coeffTable[i - 1][j + 1] - coeffTable[i - 1][j]) / (tList[j + i] - tList[j]);
			else
				coeffTable[i].push_back((coeffTable[i - 1][j + 1] - coeffTable[i - 1][j]) / (tList[j + i] - tList[j]));
		}
	}
}
double SubstituteNewtonForm(float t, std::vector<std::vector<double>>& coeffTable, const std::vector<int>& tList)
{
	// Set Degree 
	int degree = coeffTable[0].size() - 1;

	// Create result for returning value
	double result = 0.0f;
	// Loop through all first element of each layer 
	for (int i = 0; i <= degree; i++)
	{
		double tmp = coeffTable[i][0];
		// Loop (t - t0)(t - t1) ... (t - tn)
		for (int j = 0; j < i; j++)
		{
			tmp *= (t - tList[j]);
		}
		result += tmp;
	}
	return result;
}

float Truncate(float t, float c, int p)
{
	if (t < c)
		return 0.0f;
	else
	{
		// No use "pow" since the power(p) is an int
		float result = 1.0f;
		float t_c = t - c;
		for (int i = 0; i < p; i++)
		{
			result *= t_c;
		}
		return result;
	}
}
Eigen::MatrixXf InterpolatingCubic(const std::vector<glm::vec2>& positionList)
{
	size_t n = positionList.size();
	// Declare Matrix[n + 2][n + 2] since we have free coefficient
	Eigen::MatrixXf coeff(n + 2, n + 2);
	for (int t = 0; t < n; t++)
	{
		for (int i = 0; i < n + 2; i++)
		{
			// Standard Basis
			if (i < 4)
				coeff(t, i) = pow(t, i);
			// Truncated Basis
			else
				// i - 3 to make sure it start at 1
				coeff(t, i) = Truncate(t, i - 3, 3);
		}
	}

	int freeCoeff[] = { 0, n };
	for (int idx = 0; idx < 2; idx++)
	{
		int t = freeCoeff[idx];
		for (int i = 0; i < n + 2; i++)
		{
			switch (i)
			{
				// 2nd Derivative of Standard Basis
				case 0: coeff(n + idx, i) = 0.0f; break;
				case 1: coeff(n + idx, i) = 0.0f; break;
				case 2: coeff(n + idx, i) = 2.0f; break;
				case 3: coeff(n + idx, i) = 6.0f * t; break;
				// 2nd Derivative of Truncated Basis
				// i - 3 to make sure it start at 1
				default: coeff(n + idx, i) = 6.0f * Truncate(t, i - 3, 1); break;
			}
		}
	}

	//std::cout << coeff << "\n";
	return coeff;
}
float SolveInterpolationCubic(const Eigen::VectorXf& resultCoeff, float t)
{
	float result = 0.0f;
	for (int i = 0; i < resultCoeff.size(); i++)
	{
		// Standard Basis
		if (i < 4)
			result += resultCoeff(i) * pow(t, i);
		// Truncated Basis
		else
			// i - 3 to make sure it start at 1
			result += resultCoeff(i) * Truncate(t, i - 3, 3);
	}
	return result;
}