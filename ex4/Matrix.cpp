#include "Matrix.h"

Matrix::Matrix (int rows, int cols)
{
  if (rows <= 0 || cols <= 0)
  {
    throw std::length_error (LENGTHS_ARE_NOT_VALID);
  }
  dims = {rows, cols};
  matrix = new float[rows * cols];
  for (int i = 0; i < rows * cols; i++)
  {
    matrix[i] = 0;
  }
}

Matrix::Matrix (const Matrix &m) : Matrix (m.dims.rows, m.dims.cols)
{
  for (int i = 0; i < dims.cols * dims.rows; i++)
  {
    this->matrix[i] = m.matrix[i];
  }
}

Matrix::~Matrix ()
{
  delete[] matrix;
}

int Matrix::get_rows () const
{
  return dims.rows;
}

int Matrix::get_cols () const
{
  return dims.cols;
}

Matrix &Matrix::transpose ()
{
  auto *temp = new float[dims.cols * dims.rows];
  for (int i = 0; i < dims.rows; ++i)
  {
    for (int j = 0; j < dims.cols; ++j)
    {
      temp[j * dims.rows + i] = matrix[i * dims.cols + j];
    }
  }
  for (int i = 0; i < dims.rows; i++)
  {
    for (int j = 0; j < dims.cols; j++)
    {
      matrix[i * dims.cols + j] = temp[i * dims.cols + j];
    }
  }
  delete[] temp;
  int swap = dims.cols;
  dims.cols = dims.rows;
  dims.rows = swap;
  return *this;
}

Matrix &Matrix::vectorize ()
{
  dims.rows = dims.rows * dims.cols;
  dims.cols = 1;
  return *this;
}

void Matrix::plain_print () const
{
  for (int i = 0; i < dims.rows; i++)
  {
    for (int j = 0; j < dims.cols; j++)
    {
      std::cout << matrix[i * dims.cols + j] << " ";
    }
    std::cout << "\n";
  }
}

Matrix Matrix::dot (const Matrix &m) const
{
  if (dims.rows != m.dims.rows || dims.cols != m.dims.cols)
  {
    throw std::length_error (COLUMNS_NOT_EQUAL_ROWS);
  }
  Matrix mult (dims.rows, dims.cols);
  for (int i = 0; i < dims.rows * dims.cols; ++i)
  {
    mult.matrix[i] = matrix[i] * m.matrix[i];
  }
  return mult;
}

float Matrix::norm () const
{
  float sum_norm = 0;
  Matrix m = this->dot (*this);
  for (int i = 0; i < dims.rows * dims.cols; i++)
  {
    sum_norm += m.matrix[i];
  }
  return sqrtf (sum_norm);
}
Matrix &Matrix::operator= (const Matrix &m)
{
  if (&m != this)
  {
    delete[] this->matrix;
    dims.rows = m.dims.rows;
    dims.cols = m.dims.cols;
    this->matrix = new float[dims.cols * dims.rows];
    for (int i = 0; i < dims.rows * dims.cols; i++)
    {
      this->matrix[i] = m.matrix[i];
    }
  }
  return *this;
}

Matrix Matrix::operator+ (const Matrix &b) const
{
  if (dims.rows != b.dims.rows || dims.cols != b.dims.cols)
  {
    throw std::length_error (COLUMNS_NOT_EQUAL_ROWS);
  }
  Matrix m (*this);
  m += b;
  return m;
}

Matrix &Matrix::operator+= (const Matrix &b)
{
  if (dims.rows != b.dims.rows || dims.cols != b.dims.cols)
  {
    throw std::length_error (COLUMNS_NOT_EQUAL_ROWS);
  }
  for (int i = 0; i < dims.rows; i++)
  {
    for (int j = 0; j < dims.cols; j++)
    {
      matrix[i * dims.cols + j] += b.matrix[i * dims.cols + j];
    }
  }
  return *this;
}

Matrix Matrix::operator* (const Matrix &m) const
{
  if (dims.cols != m.dims.rows)
  {
    throw std::length_error (COLUMNS_MULTI_ROWS);
  }
  Matrix mult (dims.rows, m.dims.cols);
  for (int i = 0; i < dims.rows; ++i)
  {
    for (int j = 0; j < m.dims.cols; ++j)
    {
      for (int k = 0; k < dims.cols; ++k)
      {
        mult.matrix[i * mult.dims.cols] +=
            matrix[i * dims.cols + k] * m.matrix[k * m.dims.cols + j];
      }
    }
  }
  return mult;
}

Matrix Matrix::operator* (float c) const
{
  Matrix multc (dims.rows, dims.cols);
  for (int i = 0; i < dims.rows * dims.cols; ++i)
  {
    multc.matrix[i] = matrix[i] * c;
  }
  return multc;
}
Matrix operator* (float c, const Matrix &m)
{
  return m * c;
}

float Matrix::operator() (int i, int j) const
{
  return (*this)[i * dims.cols + j];
}

float Matrix::operator[] (int i) const
{
  if (i >= dims.cols * dims.rows || i < 0)
  {
    throw std::out_of_range (INDEX_OUT_OF_RANGE);
  }
  return matrix[i];
}

float &Matrix::operator() (int i, int j)
{
  return (*this)[i * dims.cols + j];
}

float &Matrix::operator[] (int i)
{
  if (i >= dims.cols * dims.rows || i < 0)
  {
    throw std::out_of_range (INDEX_OUT_OF_RANGE);
  }
  return matrix[i];
}

std::ostream &operator<< (std::ostream &os, const Matrix &m)
{
  for (int i = 0; i < m.dims.rows; i++)
  {
    for (int j = 0; j < m.dims.cols; j++)
    {
      if (m.matrix[i * m.dims.cols + j] > RANGE_TO_PRINT)
      {
        os << STARS;
      }
      else
      {
        os << NOTHING;
      }
    }
    os << "\n";
  }
  return os;
}

std::istream &operator>> (std::istream &is, Matrix &m)
{
  auto array_size_bytes =
      (long long int) m.get_cols () * m.get_rows () * sizeof (float);
  is.seekg (0, std::ios_base::end);
  int file_size_bytes = (int)is.tellg ();
  if (file_size_bytes !=(int) array_size_bytes)
  {
    throw std::runtime_error (NOT_VALID_NUMBER_STARS);
  }
  is.seekg (0, std::ios_base::beg);
  if (!is.read ((char *) m.matrix, (long int)array_size_bytes))
  {
    is.clear ();
    throw std::runtime_error (NOT_VALID_NUMBER_STARS);
  }
  return is;
}
