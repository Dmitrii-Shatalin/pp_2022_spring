// Copyright 2022 Gordey Maria
#ifndef MODULES_TASK_1_GORDEY_M_MATRIX_MULT_MATRIX_MULT_H_
#define MODULES_TASK_1_GORDEY_M_MATRIX_MULT_MATRIX_MULT_H_

#include <ctime>
#include <random>
#include <vector>
#include <queue>
#include <iostream>
class SparseM {
 public:
  int rows, cols;
  std::vector<int> col_idx;
  std::vector<int> row;
  std::vector<double> value;
  SparseM():rows(0), cols(0) {}
  SparseM(double* arr, int rows, int cols):rows(rows), cols(cols) {
      int k = 0;
      bool flag = true;
      for (int i = 0; i < cols; i++) {
          for (int j = 0; j < rows; j++) {
              if (arr[j*rows + i] != 0) {
                  if (flag) {
                      col_idx.push_back(k);
                      flag = false;
                  }
                  value.push_back(arr[j*rows + i]);
                  row.push_back(j);
                  k++;
              }
          }
          if (flag)  col_idx.push_back(k);
          flag = true;
      }
      col_idx.push_back(k);
  }
  SparseM(const SparseM& lhs) : rows(lhs.rows), cols(lhs.cols), col_idx(lhs.col_idx), row(lhs.row), value(lhs.value) {}
  SparseM& operator = (const SparseM& lhs) {
      if (this == &lhs) return (*this);
      rows = lhs.rows;
      cols = lhs.cols;
      col_idx = lhs.col_idx;
      row = lhs.row;
      value = lhs.value;
      return (*this);
  }
  friend SparseM operator *(const SparseM& A, const SparseM& B);
  friend bool operator == (const SparseM& A, const SparseM& B) {
      if (A.col_idx == B.col_idx && A.rows == B.rows
       && A.value == B.value && A.cols == B.cols && A.row == B.row) return true;
      return false;
  }
  friend std::ostream& operator <<(std::ostream& out, const SparseM& m) {
      out << "rows: " << m.rows << "\n";
      out << "cols: " << m.cols << "\ncol_idx: ";
      for (size_t i = 0; i < m.col_idx.size(); i++)out << m.col_idx[i] << " ";
      out << "\nrows: ";
      for (size_t i = 0; i < m.row.size(); i++)out << m.row[i] << " ";
      out << "\nvalues: ";
      for (size_t i = 0; i < m.value.size(); i++)out << m.value[i] << " ";
      out << "\n";
      return out;
  }
};
void transport(SparseM* ptr);
SparseM Mult_parallel(const SparseM& A, const SparseM& B);
double* create_random_matrix(int size_n);
#endif  // MODULES_TASK_1_GORDEY_M_MATRIX_MULT_MATRIX_MULT_H_
