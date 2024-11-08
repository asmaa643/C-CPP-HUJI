/*****************************************************************************/
/*                                INCLUDES                                   */
/*****************************************************************************/
// standard libs
#include <iostream>
#include <string>
#include <istream>
#include <fstream>
#include <cmath>
#include <random>
#include <cassert>

// project headers
#include "Matrix.h"
#include "Dense.h"
#include "MlpNetwork.h"
#include "Activation.h"
#define REAL_BINARY_FILE_PATH "./images/im0"
#define FAKE_BINARY_FILE_PATH "./fake_path"


// usage
using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using namespace activation;


/*****************************************************************************/
/*                                MACROS                                     */
/*****************************************************************************/
#define DIVIDE_LINE cout << "====================" << endl
#define START_TEST cout << "RUNNING TEST: " << __func__ << endl
#define PASSED_TEST cout << "TEST PASSED: " << __func__  << endl
#define EPSILON 0.00001
#define CMP_FLOATS(a, b) abs((a)-(b)) < EPSILON





/*****************************************************************************/
/*                             HELPER FUNCTIONS                              */
/*****************************************************************************/
void is_same_size (Matrix &a, Matrix &b)
{
  assert(a.get_rows () == b.get_rows () && a.get_cols () == b.get_cols ());
}

void cmp_matrices (Matrix a, Matrix b)
{
  assert (a.get_rows () == b.get_rows () && a.get_cols () == b.get_cols ());
  for (int i = 0; i < a.get_rows () * a.get_cols (); i++)
    assert(a[i] == b[i]);
}

Matrix generate_random_matrix (int rows, int cols)
{
  Matrix matrix  (rows, cols);
  std::random_device rd;
  std::mt19937 mt (rd ());
  std::uniform_real_distribution<float> dist (-10.0, 10.0);
  for (int i = 0; i < rows * cols; i++)
    matrix[i] = dist (mt);
  return matrix;
}

/*****************************************************************************/
/*                           MATRIX CONSTRUCTORS                             */
/*****************************************************************************/

void test_constructor_default ()
{
  START_TEST;
  Matrix m1;
  assert(m1.get_rows () == 1 && m1.get_cols () == 1);
  assert(m1[0] == 0);
  try {
    Matrix m2 = Matrix(2,-1);
    assert(false);
  }
  catch(std::length_error & e) {}
  PASSED_TEST;
}

void test_constructor_rows_cols ()
{
  START_TEST;
  Matrix m1 = Matrix (5, 4);
  assert(m1.get_rows () == 5 && m1.get_cols () == 4);
  for (int i = 0; i < 5 * 4; i++) assert(m1[i] == 0);


  PASSED_TEST;
}

void test_constructor_matrix ()
{
  START_TEST;
  Matrix m1 = generate_random_matrix (5, 4);
  Matrix m2  (m1);
  assert(m2.get_rows () == 5 && m2.get_cols () == 4);
  for (int i = 0; i < 5 * 4; i++) assert(m1[i] == m2[i]);
  PASSED_TEST;
}

/*****************************************************************************/
/*                             MATRIX METHODS                                */
/*****************************************************************************/

void is_transposed (Matrix &a, Matrix &b)
{
  assert(a.get_rows () == b.get_cols () && a.get_cols () == b.get_rows ());
  for (int r = 0; r < a.get_rows (); r++)
  {
    for (int c = 0; c < a.get_cols (); c++)
    {
      assert(a (r, c) == b (c, r));
    }
  }
}
void test_transpose ()
{
  START_TEST;
  Matrix m1, m2;
  m1 = generate_random_matrix (1, 1);
  m2 = Matrix (m1);
  is_transposed (m1, m2.transpose ());

  m1 = generate_random_matrix (1, 2);
  m2  = Matrix(m1);
  is_transposed (m1, m2.transpose ());

  m1 = generate_random_matrix (30, 10000);
  m2  = Matrix(m1);
  is_transposed (m1, m2.transpose ());

  m1 = generate_random_matrix (3, 3);
  m2  = Matrix(m1);
  is_transposed (m1, m2.transpose ());

  m1 = generate_random_matrix (4, 3);
  m2 = Matrix (m1);
  is_transposed (m1, m2.transpose ());

  m1 = generate_random_matrix (3, 4);
  m2 = Matrix (m1);
  is_transposed (m1, m2.transpose ());

  m1 = generate_random_matrix (10, 15);
  m2 = Matrix (m1);
  is_transposed (m1, m2.transpose ());

  m1 = generate_random_matrix (100, 100);
  m2 = Matrix (m1);
  is_transposed (m1, m2.transpose ());

  PASSED_TEST;
}

void is_vectorized (Matrix &a, Matrix &b)
{
  assert(b.get_cols () == 1 && b.get_rows () == a.get_rows () * a.get_cols ());
  for (int i = 0; i < a.get_rows () * a.get_cols (); i++)
  {
    assert(a[i] == b[i]);
  }
}
void test_vectorize ()
{
  START_TEST;
  Matrix m1, m2;
  m1 = generate_random_matrix (1, 1);
  m2 = Matrix (m1);
  is_vectorized (m1, m2.vectorize ());

  m1 = generate_random_matrix (1, 5);
  m2 = Matrix (m1);
  is_vectorized (m1, m2.vectorize ());

  m1 = generate_random_matrix (5, 1);
  m2 = Matrix (m1);
  is_vectorized (m1, m2.vectorize ());

  m1 = generate_random_matrix (5, 5);
  m2 = Matrix (m1);
  is_vectorized (m1, m2.vectorize ());

  m1 = generate_random_matrix (10, 10);
  m2 = Matrix (m1);
  is_vectorized (m1, m2.vectorize ());

  PASSED_TEST;
}

void is_dotted (Matrix &a, Matrix &b, Matrix a_dot_b)
{
  is_same_size (a, a_dot_b);
  for (int i = 0; i < a.get_rows () * a.get_cols (); i++)
    assert(a_dot_b[i] == a[i] * b[i]);
}
void test_dot ()
{
  START_TEST;
  Matrix m1, m2, m3;
  m1 = generate_random_matrix (1, 1);
  m2 = generate_random_matrix (1, 1);
  m3 = m1;
  is_dotted (m1, m2, m3.dot (m2));

  m1 = generate_random_matrix (1, 5);
  m2 = generate_random_matrix (1, 5);
  m3 = m1;
  is_dotted (m1, m2, m3.dot (m2));

  m1 = generate_random_matrix (5, 5);
  m2 = generate_random_matrix (5, 5);
  m3 = m1;
  is_dotted (m1, m2, m3.dot (m2));

  m1 = generate_random_matrix (100, 105);
  m2 = generate_random_matrix (100, 105);
  m3 = m1;
  is_dotted (m1, m2, m3.dot (m2));

  try
  {
    m1 = generate_random_matrix (3, 5);
    m2 = generate_random_matrix (5, 4);
    m1.dot (m2);
    assert(false);
  }
  catch (std::length_error &e)
  {};
  PASSED_TEST;
}

void test_norm ()
{
  START_TEST;

  Matrix m1;
  m1 = Matrix (1, 1);
  assert(m1.norm () == m1[0]);

  m1 = Matrix (3, 3);
  for (int i = 0; i < 9; i++) m1[i] = (float) i;
  // TODO: Check if this is valid comparison
  assert(CMP_FLOATS (m1.norm (), sqrt (
      0 * 0 + 1.0 * 1.0 + 2.0 * 2.0 + 3.0 * 3.0 + 4.0 * 4.0 + 5.0 * 5.0 + 6.0 *
                                                                          6.0
      + 7.0 * 7.0 + 8.0 * 8.0)));
  PASSED_TEST;
}


/*****************************************************************************/
/*                           MATRIX OPERATORS                                */
/*****************************************************************************/
// Matrix Matrix::operator+ (const Matrix &rhs) const
void test_matrix_plus_matrix_op ()
{
  START_TEST;
  Matrix m1, m2, res;
  m1 = generate_random_matrix (5, 5);
  m2 = generate_random_matrix (5, 5);
  res = m1 + m2;
  for (int i = 0; i < m1.get_rows () * m1.get_cols (); i++)
    assert(res[i] == m1[i] + m2[i]);

  try
  {
    m1 = generate_random_matrix (3, 5);
    m2 = generate_random_matrix (5, 4);
    m1 + m2;
    assert(false);
  }
  catch (std::length_error &e)
  {};

  PASSED_TEST;
}

void is_valid_matrix_mult (Matrix &a, Matrix &b, Matrix &ab)
{
  assert(ab.get_rows () == a.get_rows () && ab.get_cols () == b.get_cols ());
  for (int r = 0; r < ab.get_rows (); r++)
  {
    for (int c = 0; c > ab.get_cols (); c++)
    {
      float sum = 0;
      for (int k = 0; k < a.get_cols (); k++)
      {
        sum += a (r, c) * b (r, c);
      }
      assert(sum == ab (r, c));
    }
  }
}

// Matrix Matrix::operator* (const Matrix &rhs) const
void test_matrix_mult_matrix_op ()
{
  START_TEST;
  Matrix m1, m2, res;
  m1 = generate_random_matrix (5, 4);
  m2 = generate_random_matrix (4, 5);
  res = m1 * m2;
  is_valid_matrix_mult (m1, m2, res);

  try
  {
    m1 = generate_random_matrix (5, 3);
    m2 = generate_random_matrix (2, 4);
    m1 * m2;
    assert(false);
  }
  catch (std::length_error &e)
  {};

  PASSED_TEST;
}
// Matrix Matrix::operator* (const float rhs) const
void test_matrix_mult_float_op ()
{
  START_TEST;
  Matrix m1 = generate_random_matrix (5, 4);
  float c = 13.42921;
  Matrix m2 = Matrix (m1 * c);
  is_same_size (m1, m2);
  for (int i = 0; i < m1.get_rows () * m1.get_cols (); i++)
    assert((m2)[i] == m2[i]);
  PASSED_TEST;
}

// Matrix operator* (float lhs, const Matrix &rhs)
void test_float_mult_matrix_op ()
{
  START_TEST;
  Matrix m1 = generate_random_matrix (5, 4);
  float c = 13.42921;
  Matrix m2 = Matrix (c * m1);
  is_same_size (m1, m2);
  for (int i = 0; i < m1.get_rows () * m1.get_cols (); i++)
    assert((m2)[i] == m2[i]);
  PASSED_TEST;
}

// Matrix &Matrix::operator= (const Matrix &rhs)
void test_matrix_assign_matrix_op ()
{
  START_TEST;
  Matrix m1 = generate_random_matrix (5, 4);
  Matrix m2 = generate_random_matrix (3, 6);

  m1 = m2; // Both already initiated
  cmp_matrices (m1, m2);

  Matrix m3 = m1; // m3 is not initiated
  cmp_matrices (m1, m3);

  m1 = m1; // Self assignment

  m1 = generate_random_matrix (5, 4);
  m2 = generate_random_matrix (3, 7);
  m3 = generate_random_matrix (8, 3);

  m1 = m2 = m3; // Chaining
  cmp_matrices (m1, m2);
  cmp_matrices (m1, m3);

  PASSED_TEST;
}

// Matrix &Matrix::operator+= (const Matrix &rhs)
void test_matrix_plus_assign_matrix_op ()
{
  START_TEST;
  Matrix m1, m2, m3;
  m1 = generate_random_matrix (1, 1);
  m2 = generate_random_matrix (1, 1);
  m3 = m1;
  m3 += m2;
  cmp_matrices (m3, m1 + m2);

  m1 = generate_random_matrix (5, 5);
  m2 = generate_random_matrix (5, 5);
  m3 = m1;
  m3 += m2;
  cmp_matrices (m3, m1 + m2);
  try
  {
    m1 = generate_random_matrix (5, 5);
    m2 = generate_random_matrix (3, 3);
    m1 += m2;
    assert(false);
  }
  catch (std::length_error &e)
  {};
  PASSED_TEST;
}

// float &Matrix::operator() (const int r, const int c)
// float Matrix::operator() (const int r, const int c) const
// float Matrix::operator[] (const int i) const
// float &Matrix::operator[] (const int i)
void test_indexing_op ()
{
  START_TEST;
  Matrix m1 (generate_random_matrix (10, 12));
  // by value
  assert(m1 (3, 2) == m1[3 * m1.get_cols () + 2]);
  assert(m1[18] == m1 (1, 6));
  // by reference
  m1 (2, 3) = 1000.0;
  assert(m1[2 * m1.get_cols () + 3] == 1000.0);
  m1[34] = 1000.0;
  assert(m1[34] == 1000);
  m1[34] += 1;
  assert(m1[34] == 1001);

  // exceptions
  try
  {
    m1 (5000, 10);
    assert(false);
  }
  catch (std::out_of_range &e)
  {}

  try
  {
    m1[5000]; // This should raise exception
    assert(false);
  }
  catch (std::out_of_range &e)
  {}

  try
  {
    m1[-5]; // This should raise exception
    assert(false);
  }
  catch (std::out_of_range &e)
  {}

  try
  {
    m1(-3,-2); // This should raise exception
    assert(false);
  }
  catch (std::out_of_range &e)
  {}

  PASSED_TEST;

}

void test_stream_input_matrix_op ()
{
  START_TEST;
  std::ifstream stream1 ("C:\\Users\\asmaa\\CLionProjects\\ex4-asmaa.ghrayeb\\images\\im0", std::ifstream::binary); // Check if
  // the path
  // is pointing to one of images provided (im0, ... im9)
  assert(stream1.good());

  Matrix m1 (generate_random_matrix (28, 28));
  stream1 >> m1; // This should work

  Matrix m2(generate_random_matrix (4,3));
  ifstream stream2 (REAL_BINARY_FILE_PATH, ifstream::binary);
  try {
    stream2 >> m2; // This should throw an error because m2 is too small
    assert(false);
  }
  catch (std::runtime_error & e) {}

  Matrix m3(generate_random_matrix (29,29));
  try {
    stream2 >> m3; // This should throw an error because m3 is too big
    assert(false);
  }
  catch (std::runtime_error & e) {}


  ifstream stream3 (FAKE_BINARY_FILE_PATH, ifstream::binary);
  try {
    stream3 >> m2; // This should throw error because stream open failed
    assert(false);
  }
  catch (std::runtime_error & e) {}

  PASSED_TEST;
}

void test_stream_output_matrix_op ()
{
  START_TEST;
  PASSED_TEST;
}

/*****************************************************************************/
/*                           ACTIVATION FUNCTIONS                            */
/*****************************************************************************/

void test_relu ()
{
  START_TEST;
  Matrix m1 = Matrix (100, 120);
  Matrix m2 = activation::relu (m1);
  is_same_size (m1, m2);
  for (int i = 0; i < m2.get_cols () * m2.get_rows (); i++)
  {
    if (m1[i] >= 0)
    {
      assert(m2[i] >= 0 && m1[i] == m2[i]);
    }
  }
  PASSED_TEST;
}

void test_softmax ()
{
  START_TEST;
  Matrix m1 = Matrix (10, 20);
  Matrix m2 = relu (m1); // Non-negative matrix
  Matrix m3 = activation::softmax (m2);
  float sum = 0;
  for (int i = 0; i < m3.get_cols () * m3.get_rows (); i++)
  {
    sum += m3[i];
    assert(m3[i] >= 0 && m3[i] <= 1);
  }
  assert(CMP_FLOATS (sum, 1.0));
  PASSED_TEST;
}

/*****************************************************************************/
/*                              MLPNETWORK TESTS                             */
/*****************************************************************************/

void test_mlp ()
{
  START_TEST;
  PASSED_TEST;
}


/*****************************************************************************/
/*                                  MAIN                                     */
/*****************************************************************************/
int main ()
{
  void (*tests[]) () = {
      test_constructor_default,
      test_constructor_rows_cols,
      test_constructor_matrix,
      test_transpose,
      test_vectorize,
      test_dot,
      test_norm,
      test_matrix_plus_matrix_op,
      test_matrix_mult_matrix_op,
      test_matrix_mult_float_op,
      test_float_mult_matrix_op,
      test_matrix_assign_matrix_op,
      test_matrix_plus_assign_matrix_op,
      test_indexing_op,
      test_stream_input_matrix_op,
      test_relu,
      test_softmax,

  };
  cout << "RUNNING TESTS" << endl;
  DIVIDE_LINE;
  for (auto &test: tests)
  {
    test ();
    DIVIDE_LINE;
  }
  cout << "PASSED ALL TESTS!" << endl;

}