#ifndef DENSE_H
#define DENSE_H

#include "Activation.h"

// Insert Dense class here...
/**
 * A class that represents a layer in the network.
 */
class Dense
{
 public:
  /**
   * Constructor with given values to each member.
   * @param weights The current layer's matrix.
   * @param bias The current layer's bias.
   * @param activation The current layer's activation function.
   */
  Dense (const Matrix &weights, const Matrix &bias, mat_func activation);
  /**
   * @return The current layer's matrix.
   */
  Matrix get_weights () const;
  /**
   * @return The current layer's bias.
   */
  Matrix get_bias () const;
  /**
   * @return The current layer's activation function.
   */
  mat_func get_activation () const;
  /**
   * @param x A matrix for the process.
   * @return The output matrix for this layer.
   */
  Matrix operator() (const Matrix &x);
 private:
  Matrix weights_mat;
  Matrix bias_vec;
  mat_func activate;
};

#endif //DENSE_H
