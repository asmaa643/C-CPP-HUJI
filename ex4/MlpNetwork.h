//MlpNetwork.h

#ifndef MLPNETWORK_H
#define MLPNETWORK_H

#include "Dense.h"
#define LAST_ARRAY_SIZE 10
#define MLP_SIZE 4

/**
 * @struct digit
 * @brief Identified (by Mlp network) digit with
 *        the associated probability.
 * @var value - Identified digit value
 * @var probability - identification probability
 */
typedef struct digit
{
    unsigned int value;
    float probability;
} digit;

const matrix_dims img_dims = {28, 28};
const matrix_dims weights_dims[] = {{128, 784},
                                    {64,  128},
                                    {20,  64},
                                    {10,  20}};
const matrix_dims bias_dims[] = {{128, 1},
                                 {64,  1},
                                 {20,  1},
                                 {10,  1}};

// Insert MlpNetwork class here...
/**
 * A class that represent the MlpNetwork.
 */
class MlpNetwork
{
 public:
  /**
   * Constructor that prepares the matrix weights and the biases.
   */
  MlpNetwork (Matrix weights[4], Matrix biases[4]);
  /**
   *
   * @param img The image to guess.
   * @return A digit contains the number it guessed and the probability of it.
   */
  digit operator() (const Matrix &img);
 private:
  Matrix weights_array[4];
  Matrix biases_array[4];

};

#endif // MLPNETWORK_H