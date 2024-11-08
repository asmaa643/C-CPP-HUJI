// Matrix.h
#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>
#include <cmath>

#define DEFAULT_SIZE 1
#define RANGE_TO_PRINT 0.1
#define STARS "**"
#define NOTHING "  "
#define INDEX_OUT_OF_RANGE "Index out of range!\n"
#define COLUMNS_MULTI_ROWS "Columns of (1) must be equal to Rows of (2).\n"
#define COLUMNS_NOT_EQUAL_ROWS "Lengths aren't equal, PROCESS isn't possible\n"
#define LENGTHS_ARE_NOT_VALID "Can't build a matrix with these lengths!\n"
#define NOT_VALID_NUMBER_STARS "There aren't enough stars!\n"

/**
 * @struct matrix_dims
 * @brief Matrix dimensions container. Used in MlpNetwork.h and main.cpp
 */
typedef struct matrix_dims
{
    int rows, cols;
} matrix_dims;

// Insert Matrix class here...
/**
 * A class that represents a matrix.
 */
class Matrix
{
 public:
  /**
   * This constructor builds a rows X cols matrix, sets all values to 0.
   */
  Matrix (int rows, int cols);
  /**
   * This constructor builds a 1 X 1 matrix, sets all values to 0.
   */
  Matrix () : Matrix (DEFAULT_SIZE, DEFAULT_SIZE)
  {};
  /**
   * A copy constructor.
   * @param m a Matrix to copy.
   */
  Matrix (const Matrix &m);
  /**
   * Destructor: deletes the dynamic members.
   */
  ~Matrix ();
  /**
   * @return The number of the rows.
   */
  int get_rows () const;
  /**
   * @return The number of the columns.
   */
  int get_cols () const;
  /**
   * @return The transposed matrix.
   */
  Matrix &transpose ();
  /**
   * @return The vectorized matrix.
   */
  Matrix &vectorize ();
  /**
   * This method prints the matrix.
   */
  void plain_print () const;
  /**
   * @param m A matrix.
   * @return The elementwise multiplication matrix for m and this matrix.
   */
  Matrix dot (const Matrix &m) const;
  /**
   * @return  Frobenius norm of the given (this) matrix.
   */
  float norm () const;
  /**
   * Copy-assignment operator
   * @param m A matrix.
   * @return The matrix after copying m to it.
   */
  Matrix &operator= (const Matrix &m);
  /**
   * @param b A matrix.
   * @return A new matrix of the sum of this matrix an b.
   */
  Matrix operator+ (const Matrix &b) const;
  /**
   * @param b A matrix to add;
   * @return This matrix after adding b to it.
   */
  Matrix &operator+= (const Matrix &b);
  /**
   * @param b A matrix.
   * @return A new matrix of the mult of this matrix an b.
   */
  Matrix operator* (const Matrix &m) const;
  /**
   * @param b A scalar c.
   * @return A new matrix of this matrix * c.
   */
  Matrix operator* (float c) const;
  /**
   * @param b A scalar c.
   * @return A new matrix of c * this matrix.
   */
  friend Matrix operator* (float c, const Matrix &m);
  /**
   * @param i The number of row.
   * @param j The number of column.
   * @return The i,j element in the matrix.
   */
  float operator() (int i, int j) const;
  /**
   * @param i The number of row.
   * @return The i element in the vectorized matrix.
   */
  float operator[] (int i) const;
  /**
   * @param i The number of row.
   * @param j The number of column.
   * @return The i,j element in the matrix.
   */
  float &operator() (int i, int j);
  /**
   * @param i The number of row.
   * @return The i element in the vectorized matrix.
   */
  float &operator[] (int i);
  /**
   * @param os The output stream that we add to.
   * @param m A matrix.
   * @return os that contains the Matrix Output (the stars matrix).
   */
  friend std::ostream &operator<< (std::ostream &os, const Matrix &m);
  /**
   * Operator to fill the matrix from a file.
   * @param is istream to read from.
   * @param m A matrix.
   * @return is after reading.
   */
  friend std::istream &operator>> (std::istream &is, Matrix &m);
 private:
  struct matrix_dims dims{}; //The size of the matrix.
  float *matrix; //The matrix.
};

#endif //MATRIX_H