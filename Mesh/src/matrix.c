/**
* @Author: Taylor Shin <kshin>
* @Date:   2017-02-23T08:34:27-06:00
* @Email:  kshin@aps.anl.gov
* @Last modified by:   kshin
* @Last modified time: 2017-02-23T08:35:26-06:00
*/

/***************************************

  Matrix library for graph

  Implementation file

  Taylor Shin, Feb. 23 2017

 ***************************************/

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "matrix.h"

/* Static function definitions */
/* make a new regular matrix node */
static MatrixNode NewNode(unsigned long row, unsigned long col, matrix_data_t data);
/* make a new sparse matrix node */
static SMatrixNode NewSNode(unsigned long row, unsigned long col, matrix_data_t data);
/* Destroy a regular matrix node */
static void DestroyNode(MatrixNode mn);
/* Destroy a sparse matrix node */
static void DestroySNode(SMatrixNode mn);
/* Keygen for binary tree */
static unsigned int Keygen(unsigned long A, unsigned long B);

/* Add/Sub/Multiply/Divide matrix data */
static void MAddSc(matrix_data_t* r, matrix_data_t a, matrix_data_t b);
static void MSubSc(matrix_data_t* r, matrix_data_t a, matrix_data_t b);
static void MMulSc(matrix_data_t* r, matrix_data_t a, matrix_data_t b);
static void MDivSc(matrix_data_t* r, matrix_data_t a, matrix_data_t b);
static void MRemSc(matrix_data_t* r, matrix_data_t a, matrix_data_t b);
/* Zero */
static void MSetZero(matrix_data_t* r);


/* Regular Matrix part */
/* Constructors and destructors */
/* empty matrix */
Matrix InitMatrix(unsigned long rows, unsigned long cols)
{
    assert(rows > 0 && cols > 0);
    Matrix m = (Matrix)malloc(sizeof(matrix_root));
    assert(m);
    m->rows = rows;
    m->cols = cols;
    m->matrix = NULL;
    MatrixNode tmp_node;
    matrix_data_t tmp_data;

    unsigned long i, j;
    m->matrix = (MatrixNode**)malloc(sizeof(MatrixNode*)*rows);
    assert(m->matrix);
    for (i=0; i<rows; ++i) {
        m->matrix[i] = (MatrixNode*)malloc(sizeof(MatrixNode)*cols);
        assert(m->matrix[i]);
    }
    for (i=0; i<rows; ++i) {
        for (j=0; j<cols; ++j) {
            tmp_data = NULL;
            tmp_node = NewNode(i+1, j+1, tmp_data);
            m->matrix[i][j] = tmp_node;
            ++m->cols;
        }
        ++m->rows;
    }

    return m;
}
/* Initialize zero matrix */
Matrix InitZeroMatrix(unsigned long rows, unsigned long cols)
{
    assert(rows > 0 && cols > 0);
    Matrix e = InitMatrix(rows, cols);

    matrix_data_t tmp_data;

    unsigned long i, j;
    for (i=0; i<rows; ++i) {
        for (j=0; j<cols; ++j) {
            tmp_data = (matrix_data_t)malloc(sizeof(MATRIX_D_T));
            assert(tmp_data);
            MSetZero(&tmp_data);
            MatrixSet(e, i+1, j+1, tmp_data);
        }
    }

    return e;
}
/* Destroy regular matrix (also the data) */
void DestroyMatrix(Matrix M)
{
    assert(M);

    unsigned long i, j;

    for (i=0; i<M->rows; ++i)
        for (j=0; j<M->cols; ++j)
            DestroyNode(M->matrix[i][j]);

    for (i=0; i<M->rows; ++i)
        free(M->matrix[i]);
    free(M->matrix);
    free(M);
}

/* Free up Matrix only, leaves data. */
void FreeMatrix(Matrix M)
{
    assert(M);

    unsigned long i, j;

    for (i=0; i<M->rows; ++i)
        for(j=0; j<M->cols; ++j)
            free(M->matrix[i][j]);

    for(i=0; i<M->rows; ++i)
        free(M->matrix[i]);
    free(M->matrix);
    free(M);
}

/* Set and get */
int MatrixSet(Matrix M, unsigned long row, unsigned long col, matrix_data_t data)
{
    assert(row > 0 && col > 0);
    assert(data);
    assert(M);
    /* If the given index are off the scope */
    assert(row <= M->rows && col <= M->cols);

    assert(M->matrix[row-1][col-1]);
    if (M->matrix[row-1][col-1]->data)
        free(M->matrix[row-1][col-1]->data);

    M->matrix[row-1][col-1]->data = data;

    return 0;
}
matrix_data_t MatrixGet(Matrix M, unsigned long row, unsigned long col)
{
    assert(row > 0 && col > 0);
    assert(M);
    assert(row <= M->rows && col <= M->cols);
    assert(M->matrix[row-1][col-1]);

    return M->matrix[row-1][col-1]->data;
}

/* Find stuff... not so useful in matrix but well... */
MatrixNode MatrixFind(Matrix M, matrix_data_t data)
{
    assert(M);
    /* some shortcuts */
    if (!(M->rows && M->cols)) return NULL;
    if (M->rows==1 && M->cols==1) return M->matrix[0][0];

    /* Now scan the regular matrix */
    unsigned long i, j;
    for (i=0; i<M->rows; ++i) {
        for (j=0; j<M->cols; ++j) {
            if (M->matrix[i][j]->data == data)
                return M->matrix[i][j];
        }
    }

    /* If nothing found, return NULL */
    return NULL;
}
/* Find all */
int MatrixFindAll(Matrix M, matrix_data_t data, MatrixNode** found)
{
    assert(M);
    assert(*found == NULL);
    MatrixNode* flist;
    MatrixNode* flist_tmp;
    MatrixNode* swap_tmp;

    unsigned long i, j, fli;
    unsigned long flist_len = 0;
    for (i=0; i<M->rows; ++i) {
        for (j=0; j<M->cols; ++j) {
            if (M->matrix[i][j]->data == data) {
                if (flist_len == 0) {
                    flist = (MatrixNode*)malloc(sizeof(MatrixNode));
                    assert(flist);
                    flist[0] = M->matrix[i][j];
                    flist_len++;
                }
                else {
                    flist_tmp = (MatrixNode*)malloc(sizeof(MatrixNode)*(flist_len+1));
                    assert(flist_tmp);
                    for (fli=0; fli<flist_len; ++fli) {
                        flist_tmp[fli] = flist[fli];
                    }
                    flist_tmp[flist_len] = M->matrix[i][j];

                    /* swap flist_tmp and flist */
                    swap_tmp = flist;
                    flist = flist_tmp;
                    free(swap_tmp);
                }
            }
        }
    }

    (*found) = flist;

    return 0;
}


/* Resize */
int MatrixResize(Matrix* M, unsigned long nrow, unsigned long ncol)
{
    assert(*M);
    assert(nrow > 0 && ncol > 0);

    Matrix tmpM = (*M);
    Matrix newM, tmp;
    unsigned long m_nrow = tmpM->rows;
    unsigned long m_ncol = tmpM->cols;
    unsigned long i, j;

    /* Same size? trivial. return 0 and be done with it */
    if (m_nrow == nrow && m_ncol == ncol) return 0;

    /* Assign a new matrix */
    newM = InitMatrix(nrow, ncol);

    /* expanding */
    if (m_nrow < nrow && m_ncol < ncol) {
        for (i=0; i<nrow; ++i) {
            for (j=0; j<ncol; ++j) {
                if (i<m_nrow && j<m_ncol) newM->matrix[i][j] = tmpM->matrix[i][j];
                else MSetZero(&newM->matrix[i][j]->data);
            }
        }
    }

    /* Shrinking */
    else if (m_nrow > nrow && m_ncol > ncol) {
        for (i=0; i<m_nrow; ++i) {
            for (j=0; j<m_ncol; ++j) {
                newM->matrix[i][j] = tmpM->matrix[i][j];
            }
        }
    }

    /* Overlapping cases */
    else if (m_nrow >= nrow && m_ncol <= ncol) {
        for (i=0; i<m_nrow; ++i) {
            for (j=0; j<ncol; ++j) {
                if (i>=nrow) MSetZero(&newM->matrix[i][j]->data);
                else newM->matrix[i][j] = tmpM->matrix[i][j];
            }
        }
    }
    else if (m_nrow <= nrow && m_ncol >= ncol) {
        for (i=0; i<nrow; ++i) {
            for (j=0; j<m_ncol; ++j) {
                if (j>=ncol) MSetZero(&newM->matrix[i][j]->data);
                else newM->matrix[i][j] = tmpM->matrix[i][j];
            }
        }
    }

    /* swapping!! */
    tmp = tmpM;
    (*M) = newM;
    DestroyMatrix(tmp);
    return 0;
}

/* Some arithmatic operations for regular matrix */
Matrix MatrixAdd(Matrix A, Matrix B)
{
    assert(A&&B);

    if (A->rows != B->rows || A->cols != B->cols)
        return NULL;

    Matrix C = InitZeroMatrix(A->rows, A->cols);
    matrix_data_t result = NULL;

    unsigned long i, j;
    for (i=0; i<A->rows; ++i) {
        for (j=0; j<A->cols; ++j) {
            result = (matrix_data_t)malloc(sizeof(MATRIX_D_T));
            assert(result);
            MAddSc(&result, A->matrix[i][j]->data, B->matrix[i][j]->data);
            MatrixSet(C, i+1, j+1, result);
        }
    }

    return C;
}
Matrix MatrixSub(Matrix A, Matrix B)
{
    assert(A&&B);

    if (A->rows != B->rows || A->cols != B->cols)
        return NULL;

    Matrix C = InitMatrix(A->rows, A->cols);
    matrix_data_t result = NULL;

    unsigned long i, j;
    for (i=0; i<A->rows; ++i) {
        for (j=0; j<A->cols; ++j) {
            result = (matrix_data_t)malloc(sizeof(MATRIX_D_T));
            assert(result);
            MSubSc(&result, A->matrix[i][j]->data, B->matrix[i][j]->data);
            MatrixSet(C, i+1, j+1, result);
        }
    }

    return C;
}
Matrix MatrixMul(Matrix A, Matrix B)
{
    assert(A&&B);
    if (A->cols != B->rows) return NULL;

    Matrix C = InitMatrix(A->rows, B->cols);
    matrix_data_t res, res_tmp;

    unsigned long i, j;

    for (i=0; i<A->rows; ++i) {
        res_tmp = (matrix_data_t)malloc(sizeof(MATRIX_D_T));
        assert(res_tmp);
        MSetZero(&res_tmp);
        for (j=0; j<B->cols; ++j) {
            res = (matrix_data_t)malloc(sizeof(MATRIX_D_T));
            assert(res);
            MMulSc(&res, A->matrix[i][j]->data, B->matrix[j][i]->data);
            MAddSc(&res_tmp, res_tmp, res);
        }
        MatrixSet(C, i+1, j+1, res_tmp);
    }

    return C;
}

/* Scalar operations for regular matrix */
Matrix MatrixAddSc(Matrix A, matrix_data_t sc)
{
    assert(A);
    assert(sc);

    Matrix C = InitZeroMatrix(A->rows, A->cols);
    matrix_data_t result = NULL;

    unsigned long i, j;
    for (i=0; i<A->rows; ++i) {
        for (j=0; j<A->cols; ++j) {
            result = (matrix_data_t)malloc(sizeof(MATRIX_D_T));
            assert(result);
            MAddSc(&result, A->matrix[i][j]->data, sc);
            MatrixSet(C, i+1, j+1, result);
        }
    }

    return C;
}

Matrix MatrixSubSc(Matrix A, matrix_data_t sc)
{
    assert(A);
    assert(sc);

    Matrix C = InitZeroMatrix(A->rows, A->cols);
    matrix_data_t result = NULL;

    unsigned long i, j;
    for (i=0; i<A->rows; ++i) {
        for (j=0; j<A->cols; ++j) {
            result = (matrix_data_t)malloc(sizeof(MATRIX_D_T));
            assert(result);
            MSubSc(&result, A->matrix[i][j]->data, sc);
            MatrixSet(C, i+1, j+1, result);
        }
    }

    return C;
}
Matrix MatrixMulSc(Matrix A, matrix_data_t sc)
{
    assert(A);
    assert(sc);

    Matrix C = InitZeroMatrix(A->rows, A->cols);
    matrix_data_t result = NULL;

    unsigned long i, j;
    for (i=0; i<A->rows; ++i) {
        for (j=0; j<A->cols; ++j) {
            result = (matrix_data_t)malloc(sizeof(MATRIX_D_T));
            assert(result);
            MMulSc(&result, A->matrix[i][j]->data, sc);
            MatrixSet(C, i+1, j+1, result);
        }
    }

    return C;
}

Matrix MatrixDivSc(Matrix A, matrix_data_t sc)
{
    assert(A);
    assert(sc);

    Matrix C = InitZeroMatrix(A->rows, A->cols);
    matrix_data_t result = NULL;

    unsigned long i, j;
    for (i=0; i<A->rows; ++i) {
        for (j=0; j<A->cols; ++j) {
            result = (matrix_data_t)malloc(sizeof(MATRIX_D_T));
            assert(result);
            MDivSc(&result, A->matrix[i][j]->data, sc);
            MatrixSet(C, i+1, j+1, result);
        }
    }

    return C;
}
Matrix MatrixRemSc(Matrix A, matrix_data_t sc)
{
    assert(A);
    assert(sc);

    Matrix C = InitZeroMatrix(A->rows, A->cols);
    matrix_data_t result = NULL;

    unsigned long i, j;
    for (i=0; i<A->rows; ++i) {
        for (j=0; j<A->cols; ++j) {
            result = (matrix_data_t)malloc(sizeof(MATRIX_D_T));
            assert(result);
            MRemSc(&result, A->matrix[i][j]->data, sc);
            MatrixSet(C, i+1, j+1, result);
        }
    }

    return C;
}

/* Sparse matrix part */
/* Sparse matrix */ /* Not yet implemented */
/* Constructors and destructors */
/* Make an empty matrix */
SMatrix InitSMatrix(unsigned long rows, unsigned long cols)
{
    assert(rows > 0 && cols > 0);
    SMatrix sm = (SMatrix)malloc(sizeof(smatrix_root));
    assert(sm);
    SMatrixNode smn;
    unsigned int key;

    sm->rows = rows;
    sm->cols = cols;
    sm->root = bt_node_init();

    unsigned long i, j;
    for (i=0; i<rows; ++i) {
        for (j=0; j<cols; ++j) {
            smn = NewSNode(i+1, j+1, NULL);
            key = Keygen(i, j);
            bt_insert(sm->root, smn, key);
        }
    }

    return sm;
}
/* Make a zero matrix with size (Use this...)*/
SMatrix InitZeroSMatrix(unsigned long rows, unsigned long cols)
{
    assert(rows > 0 && cols > 0);
    SMatrix sm = InitSMatrix(rows, cols);

    unsigned int key;
    unsigned long i, j;
    BTNode tmp_bt;
    matrix_data_t zero;

    for (i=0; i<rows; ++i) {
        for (j=0; j<cols; ++j) {
            key = Keygen(i, j);
            tmp_bt = bt_search(sm->root, key);
            zero = (matrix_data_t)malloc(sizeof(MATRIX_D_T));
            assert(zero);
            MSetZero(&zero);
            ((SMatrixNode)tmp_bt->stuff)->data = zero;
        }
    }

    return sm;
}
void DestroySMatrix(SMatrix M)
{
    assert(M);

    unsigned long i, j;
    unsigned int key;
    BTNode tmp_bt;
    SMatrixNode tmp_m;

    for (i=0; i<M->rows; ++i) {
        for (j=0; j<M->cols; ++j) {
            key = Keygen(i, j);
            tmp_bt = bt_search(M->root, key);
            tmp_m = (SMatrixNode)tmp_bt->stuff;
            DestroySNode(tmp_m);
        }
    }
    bt_node_destroy(M->root);

    free(M);
}


/* Set and get */
int SMatrixSet(SMatrix M, unsigned long row, unsigned long col, matrix_data_t data)
{
    assert(data);
    assert(M);
    assert(row > 0 && col > 0);

    unsigned int key = Keygen(row-1, col-1);

    BTNode tmp_bt = bt_search(M->root, key);
    SMatrixNode tmp_m = (SMatrixNode)tmp_bt->stuff;

    if (tmp_m) tmp_m->data = data;
    else return -1;

    return 0;
}
matrix_data_t SMatrixGet(SMatrix M, unsigned long row, unsigned long col)
{
    assert(M);
    assert(row > 0 && col > 0);

    unsigned int key = Keygen(row-1, col-1);

    BTNode tmp_bt = bt_search(M->root, key);
    return ((SMatrixNode)(tmp_bt->stuff))->data;
}

/* Find stuff */
SMatrixNode SMatrixFind(SMatrix M, matrix_data_t data)
{
    assert(M);
    assert(data);

    unsigned long i, j;
    unsigned int key;
    BTNode tmp_bt;
    SMatrixNode tmp_sm;

    for (i=0; i<M->rows; ++i) {
        for (j=0; j<M->cols; ++j) {
            key = Keygen(i, j);
            tmp_bt = bt_search(M->root, key);
            tmp_sm = (SMatrixNode)(tmp_bt->stuff);
            if (data == tmp_sm->data) return tmp_sm;
        }
    }

    return NULL;
}

/* Resize */
int SMatrixResize(SMatrix* M, unsigned long nrow, unsigned long ncol)
{
    assert(*M);

    /* if zero nrow or ncol are given
       free up the matrix...*/
    if (!nrow || !ncol) {
        DestroySMatrix(*M);
        return 1;
    }

    /* If the new size is the same, do nothing */
    if (nrow == (*M)->rows && ncol == (*M)->cols) return 0;

    SMatrixNode sm_tmp;
    matrix_data_t zero;
    matrix_data_t tmp_data;

    /* Loop loop */
    unsigned long i, j;
    for (i=0; i<nrow; ++i) {
        for (j=0; j<ncol; ++j) {
            /* Passing expanded region */
            if (i >= (*M)->rows || j >= (*M)->cols) {
                /* Assign zero to new nodes */
                zero = (matrix_data_t)malloc(sizeof(MATRIX_D_T));
                assert(zero); MSetZero(&zero);
                sm_tmp = NewSNode(i+1, j+1, zero);
            }
        }
    }

    /* now free up remaining stuff */
    if (nrow < (*M)->rows && ncol >= (*M)->cols) {
        for (i=nrow; i<(*M)->rows; ++i) {
            for (j=0; j<(*M)->cols; ++j) {
                tmp_data = SMatrixGet((*M), i+1, j+1);
                sm_tmp = SMatrixFind((*M), tmp_data);
                free(tmp_data);
                bt_remove((*M)->root, sm_tmp, Keygen(i,j));
            }
        }
    }
    else if (ncol < (*M)->cols && nrow >= (*M)->rows) {
        for (i=0; i<(*M)->rows; ++i) {
            for (j=ncol; (*M)->cols; ++j) {
                tmp_data = SMatrixGet((*M), i+1, j+1);
                sm_tmp = SMatrixFind((*M), tmp_data);
                free(tmp_data);
                bt_remove((*M)->root, sm_tmp, Keygen(i,j));
            }
        }
    }

    return 0;
}



/* Some arithmatic operations */
SMatrix SMatrixAdd(SMatrix A, SMatrix B)
{
    assert(A&&B);
    assert(A->rows == B->rows && A->cols == B->cols);
    SMatrix C = InitSMatrix(A->rows, A->cols);

    unsigned long i, j;
    matrix_data_t tmp_d;
    matrix_data_t a_elem;
    matrix_data_t b_elem;

    for (i=0; i<A->rows; ++i) {
        for (j=0; j<A->cols; ++j) {
            tmp_d = (matrix_data_t)malloc(sizeof(MATRIX_D_T));
            a_elem = SMatrixGet(A, i+1, j+1);
            b_elem = SMatrixGet(B, i+1, j+1);
            MAddSc(&tmp_d, a_elem, b_elem);
            SMatrixSet(C, i+1, j+1, tmp_d);
        }
    }

    return C;
}
SMatrix SMatrixSub(SMatrix A, SMatrix B)
{
    assert(A&&B);
    assert(A->rows == B->rows && A->cols == B->cols);
    SMatrix C = InitSMatrix(A->rows, A->cols);

    unsigned long i, j;
    matrix_data_t tmp_d;
    matrix_data_t a_elem;
    matrix_data_t b_elem;

    for (i=0; i<A->rows; ++i) {
        for (j=0; j<A->cols; ++j) {
            tmp_d = (matrix_data_t)malloc(sizeof(MATRIX_D_T));
            a_elem = SMatrixGet(A, i+1, j+1);
            b_elem = SMatrixGet(B, i+1, j+1);
            MSubSc(&tmp_d, a_elem, b_elem);
            SMatrixSet(C, i+1, j+1, tmp_d);
        }
    }

    return C;
}
SMatrix SMatrixMul(SMatrix A, SMatrix B)
{
    assert(A&&B);
    assert(A->cols == B->rows);
    SMatrix C = InitSMatrix(A->rows, B->cols);

    matrix_data_t res, res_tmp;
    unsigned long i, j;
    matrix_data_t a_elem, b_elem;

    for (i=0; i<A->rows; ++i) {
        res_tmp = (matrix_data_t)malloc(sizeof(MATRIX_D_T));
        assert(res_tmp);
        MSetZero(&res_tmp);
        for (j=0; j<B->cols; ++j) {
            res = (matrix_data_t)malloc(sizeof(MATRIX_D_T));
            assert(res);

            a_elem = SMatrixGet(A, i, j);
            b_elem = SMatrixGet(B, i, j);

            MMulSc(&res, a_elem, b_elem);
            MAddSc(&res_tmp, res_tmp, res);

        }
        SMatrixSet(C, i+1, j+1, res_tmp);
    }

    return C;
}
/* Scalar stuff */
SMatrix SMatrixAddSc(SMatrix A, matrix_data_t sc)
{
    assert(A);
    assert(sc);

    SMatrix C = InitSMatrix(A->rows, A->cols);

    unsigned long i, j;
    matrix_data_t tmp_d;
    matrix_data_t a_elem;

    for (i=0; i<A->rows; ++i) {
        for (j=0; j<A->cols; ++j) {
            tmp_d = (matrix_data_t)malloc(sizeof(MATRIX_D_T));
            a_elem = SMatrixGet(A, i+1, j+1);
            MAddSc(&tmp_d, a_elem, sc);
            SMatrixSet(C, i+1, j+1, tmp_d);
        }
    }

    return C;
}
SMatrix SMatrixSubSc(SMatrix A, matrix_data_t sc)
{
    assert(A);
    assert(sc);

    SMatrix C = InitSMatrix(A->rows, A->cols);

    unsigned long i, j;
    matrix_data_t tmp_d;
    matrix_data_t a_elem;

    for (i=0; i<A->rows; ++i) {
        for (j=0; j<A->cols; ++j) {
            tmp_d = (matrix_data_t)malloc(sizeof(MATRIX_D_T));
            a_elem = SMatrixGet(A, i+1, j+1);
            MSubSc(&tmp_d, a_elem, sc);
            SMatrixSet(C, i+1, j+1, tmp_d);
        }
    }

    return C;
}
SMatrix SMatrixMulSc(SMatrix A, matrix_data_t sc)
{
    assert(A);
    assert(sc);

    SMatrix C = InitSMatrix(A->rows, A->cols);

    unsigned long i, j;
    matrix_data_t tmp_d;
    matrix_data_t a_elem;

    for (i=0; i<A->rows; ++i) {
        for (j=0; j<A->cols; ++j) {
            tmp_d = (matrix_data_t)malloc(sizeof(MATRIX_D_T));
            a_elem = SMatrixGet(A, i+1, j+1);
            MMulSc(&tmp_d, a_elem, sc);
            SMatrixSet(C, i+1, j+1, tmp_d);
        }
    }

    return C;
}
SMatrix SMatrixDivSc(SMatrix A, matrix_data_t sc)
{
    assert(A);
    assert(sc);

    SMatrix C = InitSMatrix(A->rows, A->cols);

    unsigned long i, j;
    matrix_data_t tmp_d;
    matrix_data_t a_elem;

    for (i=0; i<A->rows; ++i) {
        for (j=0; j<A->cols; ++j) {
            tmp_d = (matrix_data_t)malloc(sizeof(MATRIX_D_T));
            a_elem = SMatrixGet(A, i+1, j+1);
            MDivSc(&tmp_d, a_elem, sc);
            SMatrixSet(C, i+1, j+1, tmp_d);
        }
    }

    return C;
}
SMatrix SMatrixRemSc(SMatrix A, matrix_data_t sc)
{
    assert(A);
    assert(sc);

    SMatrix C = InitSMatrix(A->rows, A->cols);

    unsigned long i, j;
    matrix_data_t tmp_d;
    matrix_data_t a_elem;

    for (i=0; i<A->rows; ++i) {
        for (j=0; j<A->cols; ++j) {
            tmp_d = (matrix_data_t)malloc(sizeof(MATRIX_D_T));
            a_elem = SMatrixGet(A, i+1, j+1);
            MRemSc(&tmp_d, a_elem, sc);
            SMatrixSet(C, i+1, j+1, tmp_d);
        }
    }

    return C;
}








/* Static functions */
/* Make a regular matrix node */
static MatrixNode NewNode(unsigned long row, unsigned long col, matrix_data_t data)
{
    MatrixNode mn = (MatrixNode)malloc(sizeof(matrix_node));
    assert(mn);

    mn->ir = row;
    mn->ic = col;
    mn->data = data;

    return mn;
}

/* Make a new sparse matrix node */
static SMatrixNode NewSNode(unsigned long row, unsigned long col, matrix_data_t data)
{
    SMatrixNode mn = (SMatrixNode)malloc(sizeof(smatrix_node));
    assert(mn);

    mn->ir = row;
    mn->ic = col;
    mn->data = data;

    return mn;
}

/* Delete regular matrix node */
static void DestroyNode(MatrixNode mn)
{
    free(mn->data);
    free(mn);
}

/* Delete sparse matrix node */
static void DestroySNode(SMatrixNode mn)
{
    free(mn->data);
    free(mn);
}

/* Keygen for binary tree... kinda prone to overflow */
/* Cantor pairing function */
static unsigned int Keygen(unsigned long A, unsigned long B)
{
    unsigned int a, b;
    a = (unsigned int)A;
    b = (unsigned int)B;

    unsigned int key = (a+b)*(a+b+1)/2+a;

    return key;
}









/* Add/Sub/Multiply/Divide matrix data. We'll use integer.. So yea.. */
static void MAddSc(matrix_data_t* r, matrix_data_t a, matrix_data_t b)
{
    assert(a);
    assert(b);
    assert(*r);
    MATRIX_D_T result = (*a) + (*b);
    (**r) = result;
}
static void MSubSc(matrix_data_t* r, matrix_data_t a, matrix_data_t b)
{
    assert(a);
    assert(b);
    assert(*r);
    MATRIX_D_T result = (*a) - (*b);
    (**r) = result;
}
static void MMulSc(matrix_data_t* r, matrix_data_t a, matrix_data_t b)
{
    assert(a);
    assert(b);
    assert(*r);
    MATRIX_D_T result = (*a) * (*b);
    (**r) = result;
}
static void MDivSc(matrix_data_t* r, matrix_data_t a, matrix_data_t b)
{
    assert(a);
    assert(b);
    assert(*r);
    MATRIX_D_T result = (*a) / (*b);
    (**r) = result;
}
static void MRemSc(matrix_data_t* r, matrix_data_t a, matrix_data_t b)
{
    assert(a);
    assert(b);
    assert(*r);
    MATRIX_D_T result = (*a) % (*b);
    (**r) = result;
}
static void MSetZero(matrix_data_t* r)
{
    assert(*r);
    (**r) = (MATRIX_D_T)0;
}
