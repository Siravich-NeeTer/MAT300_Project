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