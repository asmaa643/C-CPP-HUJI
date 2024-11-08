#include "Matrix.h"
#include <cmath>
#ifndef ACTIVATION_H
#define ACTIVATION_H

typedef Matrix (*mat_func) (const Matrix &x);

// Insert Activation class here...
/**
 * A namespace for the two activation functions.
 */
namespace activation
{
    Matrix relu (const Matrix &x);
    Matrix softmax (const Matrix &x);
}

#endif //ACTIVATION_H