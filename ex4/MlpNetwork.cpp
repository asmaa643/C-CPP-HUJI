#include "MlpNetwork.h"

MlpNetwork::MlpNetwork (Matrix weights[4], Matrix biases[4])
{
  for (int i = 0; i < 4; i++)
  {
    weights_array[i] = weights[i];
    biases_array[i] = biases[i];
  }
}

digit MlpNetwork::operator() (const Matrix &img)
{
  Dense layer1 (weights_array[0], biases_array[0], activation::relu);
  Matrix r1 = layer1 (img);
  Dense layer2 (weights_array[1], biases_array[1], activation::relu);
  Matrix r2 = layer2 (r1);
  Dense layer3 (weights_array[2], biases_array[2], activation::relu);
  Matrix r3 = layer3 (r2);
  Dense layer4 (weights_array[3], biases_array[3], activation::softmax);
  Matrix r4 = layer4 (r3);
  digit d;
  d.probability = r4[0];
  d.value = 0;
  for (int i = 1; i < LAST_ARRAY_SIZE; i++)
  {
    if (r4[i] > d.probability)
    {
      d.probability = r4[i];
      d.value = i;
    }
  }
  return d;
}
