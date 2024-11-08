#include "Activation.h"

Matrix activation::relu (const Matrix &x)
{
  Matrix m (x);
  for (int i = 0; i < x.get_rows () * x.get_cols (); i++)
  {
    if (m[i] < 0)
    {
      m[i] = 0;
    }
  }
  return m;
}

Matrix activation::softmax (const Matrix &x)
{
  Matrix m (x);
  float sum = 0;
  for (int i = 0; i < x.get_rows () * x.get_cols (); i++)
  {
    m[i] = std::exp (m[i]);
    sum += m[i];
  }
  float soft = 1 / sum;
  m = m * soft;
  return m;
}