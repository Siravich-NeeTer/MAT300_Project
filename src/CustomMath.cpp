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