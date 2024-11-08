#include "Dense.h"

Dense::Dense (const Matrix &weights, const Matrix &bias, mat_func activation)
{
  weights_mat = weights;
  bias_vec = bias;
  activate = activation;
}

Matrix Dense::get_weights () const
{
  return weights_mat;
}
Matrix Dense::get_bias () const
{
  return bias_vec;
}
mat_func Dense::get_activation () const
{
  return activate;
}

Matrix Dense::operator() (const Matrix &x)
{
  Matrix mat = weights_mat * x;
  return activate (mat += bias_vec);
}