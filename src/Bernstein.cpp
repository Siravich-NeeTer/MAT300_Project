#include "Bernstein.h"

float BernsteinPolynomial(int i, int degree, float t)
{
	return Pascal::GetValue(degree, i) * std::pow(1.0f - t, degree - i) * std::pow(t, i);
}