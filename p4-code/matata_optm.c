// optimized versions of matrix A^T*A operation
#include "matvec.h"
#include "string.h"
int matata_VER1(matrix_t mat, matrix_t ans) { // Simple optimization
  // YOUR CODE HERE

  memset(ans.data, 0, sizeof(int) * ans.rows * ans.cols);

  for(int row = 0; row<mat.rows; row++){ // Reordering memory access to be sequential to favor cache, working across rows for matrices
    for(int i = 0; i<mat.rows; i++){
      int lead = MGET(mat, row, i);
      for(int j = 0; j<mat.cols; j++){
        int val = MGET(ans, i, j) + lead * MGET(mat, row, j);
        MSET(ans, i, j, val);
      }
    }
  }
  return 0;
}

int matata_VER2(matrix_t mat, matrix_t ans) { // Current best version
  int lead = 0;   // Intializing variables for later use
  int val0 = 0;
  int val1 = 0;
  int val2 = 0;
  
  int row;
  int i;
  int j;
  
  int mat_rows = mat.rows;
  int mat_cols = mat.cols;

  memset(ans.data, 0, sizeof(int) * mat_rows * mat_cols); // Using memset to bulk change the answer matrix data to 0
 
  for(row = 0; row<mat_rows; row++){
    for(i = 0; i<mat_rows; i++){
      lead = mat.data[row*mat_cols + i];  // Reordering memory access to be sequential to favor cache, working across rows for matrices

      for(j = 0; j<mat_cols-3; j+=3){ // Loop unrolling to favor cache
        val0 = ans.data[i*mat_cols + j] + lead * mat.data[row*mat_cols + j];  // Inline functions instead of macros
        val1 = ans.data[i*mat_cols + j+1] + lead * mat.data[row*mat_cols + j+1];
        val2 = ans.data[i*mat_cols + j+2] + lead * mat.data[row*mat_cols + j+2];

        ans.data[i*mat_cols + j] = val0;
        ans.data[i*mat_cols + j+1] = val1;
        ans.data[i*mat_cols + j+2] = val2;
      }

      for(; j<mat_cols; j++){ // Secondary for loop to deal with the excess
        val0 = ans.data[i*mat_cols + j] + lead * mat.data[row*mat_cols + j];
        ans.data[i*mat_cols + j] = val0;
      }
    }
  }
  return 0;
}

int matata_OPTM(matrix_t mat, matrix_t ans) {
  if(mat.rows != mat.cols ||                    // must be a square matrix for A^T*A
     mat.rows != ans.rows ||
     mat.cols != ans.cols)
  {
    printf("matata_OPTM: dimension mismatch\n");
    return 1;
  }

  // Call to some version of optimized code
  return matata_VER2(mat, ans);
  // return matata_VER2(mat, ans);
}
