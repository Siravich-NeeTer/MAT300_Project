#pragma once

#include <iostream>
#include <cmath>
#include <string>
#include <vector>

#include "CustomMath.h"

// Bi(t) = dPi * (1-t)^(d-i) * t^i
float BernsteinPolynomial(int i, int degree, float t);