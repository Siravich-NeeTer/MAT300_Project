#pragma once

#include <iostream>
#include <cmath>
#include <vector>

class Pascal
{
	public:
		static double GetValue(int n, int i);
		static void PrintPascal();

	private:
		// Use "double" to hold large number
		static std::vector<std::vector<double>> m_PascalValue;
};