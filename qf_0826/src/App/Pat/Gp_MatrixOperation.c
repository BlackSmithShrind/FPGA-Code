#include "Gp_PatDef.h"

// 矩阵乘法 ：A矩阵大小 m*n,B矩阵大小 n*q;
double ** mul(double ** A, int m, int n, double ** B, int q)
{
    double **C = (double **)malloc(m * sizeof(double *));
    int i = 0;
    for(; i < m; i++)
    {
        C[i] = (double *)malloc(q * sizeof(double));
    }

    double sum1 = 0.0;
    i = 0;
	for (; i < m; i++)
    {
		int j = 0;
		for (; j < q; j++)
        {
			sum1 = 0.0;
			int k = 0;
			for (; k < n; k++)
            {
				sum1 = sum1+ A[i][k] * B[k][j];
			}
			C[i][j] = sum1;
		}
	}

	return C;
}

// 矩阵减法 ：A和B矩阵大小 m*n  A - B;
double ** sub(double ** A,int m,int n,double ** B)
{
    double **C = (double **)malloc(m * sizeof(double *));
    int i = 0;
    for(; i < m;i++)
    {
        C[i] = (double *)malloc(n * sizeof(double));
    }

    i = 0;
	for (; i < m; i++)
    {
		int j = 0;
		for (; j < n; j++)
        {
			C[i][j] = A[i][j] - B[i][j];
		}
	}

	return C;
}

Matrix* InitMatrix(int row, int line)				//初始化一个矩阵
{
	if (row > 0 && line > 0)
	{
        Matrix* matrix = (Matrix *)malloc(sizeof(Matrix));
        matrix->row = row;
        matrix->line = line;
        matrix->data = (double *)malloc(sizeof(double) * row * line);
        memset((void*)matrix->data, 0, sizeof(double) * row * line);
        return matrix;
	}
	else 
    {
        return NULL;
    }
} 

void FreeMatrix(Matrix* pMatrix)				//释放一个矩阵
{
    if (pMatrix)
    {
        if (pMatrix->data != NULL)
        {
            free(pMatrix->data);
            pMatrix->data = NULL;
        }

        free(pMatrix);
        pMatrix = NULL;
    }
}

void ValueMatrix(Matrix *matrix, double *array) 		//给矩阵赋值
{
	if (matrix->data != NULL)
	{
        memcpy(matrix->data, array, matrix->row * matrix->line * sizeof(double));
    }
}

// 矩阵相乘
Matrix* MulMatrix(Matrix* matrix_A, Matrix* matrix_B)
{
	double d1 = 0.0;
    if (matrix_A->line == matrix_B->row)		//列==行
	{
        Matrix *matrix_C = InitMatrix(matrix_A->row, matrix_B->line);
        int i = 0;
        for (; i < matrix_A->row; i++)
        {
        	int j = 0;
            for (; j < matrix_B->line; j++)
            {
            	int k = 0;
                for (; k < matrix_A->line; k++)
                {
                	d1 = matrix_A->data[i * matrix_A->line + k] * matrix_B->data[k * matrix_B->line + j];
                    matrix_C->data[i * matrix_C->line + j] += d1;
                    //printf("MulMatrix:matrix_C->data[%d]=%.16lf d1=%f matrix_A->data[%d]=%.16lf matrix_B->data[%d]=%.16lf\n", i * matrix_C->line + j, matrix_C->data[i * matrix_C->line + j], d1, i * matrix_A->line + k, matrix_A->data[i * matrix_A->line + k], k * matrix_B->line + j, matrix_B->data[k * matrix_B->line + j]);

                }
            }
		}
		return matrix_C;
	}
	else
	{
		printf("不可相乘 matrix_A->line=[%d] matrix_B->row=[%d]\n", matrix_A->line, matrix_B->row);
		return NULL;
	}
}

// 矩阵相加
Matrix* AddMatrix(Matrix *matrix_A, Matrix *matrix_B)
{
	if (matrix_A->row == matrix_B->row && matrix_A->line == matrix_B->line)
	{
        Matrix *matrix_C = InitMatrix(matrix_A->row, matrix_A->line);
        int i = 0;
        for (; i < matrix_A->row; i++)
        {
        	int j = 0;
            for (; j < matrix_A->line; j++)
            {
                matrix_C->data[i * matrix_C->line + j] = matrix_A->data[i * matrix_A->line + j] + matrix_B->data[i * matrix_A->line + j];
                printf("AddMatrix:matrix_C->data[%d]=%.16lf matrix_A->data[%d]=%.16lf matrix_B->data[%d]=%.16lf\n", i * matrix_C->line + j, matrix_C->data[i * matrix_C->line + j], i * matrix_A->line + j, matrix_A->data[i * matrix_A->line + j], i * matrix_A->line + j, matrix_B->data[i * matrix_A->line + j]);

            }
		}
		return matrix_C;
	}
	else 
	{
		printf("不可相加 matrix_A->row=[%d] matrix_B->row=[%d] matrix_A->line=[%d] matrix_B->line=[%d]\n", matrix_A->row, matrix_B->row, matrix_A->line, matrix_B->line);
		return NULL;
	}
}

// 矩阵相减
Matrix* SubMatrix(Matrix *matrix_A, Matrix *matrix_B)
{
	if (matrix_A->row == matrix_B->row && matrix_A->line == matrix_B->line)
	{
        Matrix *matrix_C = InitMatrix(matrix_A->row, matrix_A->line);
        int i = 0;
        for (; i < matrix_A->row; i++)
        {
        	int j = 0;
            for (; j < matrix_A->line; j++)
            {
                matrix_C->data[i * matrix_C->line + j] = matrix_A->data[i * matrix_A->line + j] - matrix_B->data[i * matrix_A->line + j];
                printf("SubMatrix:matrix_C->data[%d]=%.16lf matrix_A->data[%d]=%.16lf matrix_B->data[%d]=%.16lf\n", \
                		i * matrix_C->line + j, matrix_C->data[i * matrix_C->line + j], i * matrix_A->line + j, matrix_A->data[i * matrix_A->line + j], i * matrix_A->line + j, matrix_B->data[i * matrix_A->line + j]);

            }
		}
		return matrix_C;
	}
	else 
	{
		printf("不可相减 matrix_A->row=[%d] matrix_B->row=[%d] matrix_A->line=[%d] matrix_B->line=[%d]\n", matrix_A->row, matrix_B->row, matrix_A->line, matrix_B->line);
		return NULL;
	}
}

// 计算矩阵大小
int SizeMatrix(Matrix *matrix)
{
    return matrix->row * matrix->line;
}

// 打印矩阵
void PrintMatrix(Matrix *matrix)
{
	int i = 0;
    for (; i < SizeMatrix(matrix); i++)
    {
        printf("%lf\t", matrix->data[i]);
        if ((i+1)%matrix->line == 0)
            printf("\n");
    }
}

// 复制矩阵
void CopyMatrix(Matrix *matrix_A, Matrix *matrix_B)
{
	matrix_B->row = matrix_A->row;
	matrix_B->line = matrix_A->line;
    memcpy(matrix_B->data, matrix_A->data, SizeMatrix(matrix_A) * sizeof(double));
}

//矩阵转置
void TransMatrix(Matrix *matrix)			
{
    int swap;
    Matrix *matrixTemp = InitMatrix(matrix->row, matrix->line);       	//创建一个临时矩阵
    CopyMatrix(matrix, matrixTemp);                                                 // 将目标矩阵的data复制给临时矩阵

    int i = 0;
    for (; i < matrix->line; i++)
    {
    	int j = 0;
        for (; j < matrix->row; j++)
        {
            matrix->data[i * matrix->row + j] = matrixTemp->data[j * matrix->line + i];
        }
    }
    swap = matrix->row;
    matrix->row = matrix->line;
    matrix->line = swap;
}

 //测试接口是否正常函数
// int main()
// {
//     double value[] = {1,2,3,4,5,6,7,8,9};
//     double value2[] = {5,3,7};
//     Matrix *matrix1 = InitMatrix(matrix1,3,3);
// 	Matrix *matrix2 = InitMatrix(matrix2,3,1);

// 	ValueMatrix(matrix1,value);
// 	ValueMatrix(matrix2,value2);
//     PrintMatrix(matrix1);
//     PrintMatrix(matrix2);
//     Matrix *matrix3 = MulMatrix(matrix1,matrix2);	//乘法
//     PrintMatrix(matrix3);
//     TransMatrix(matrix2);	//乘法
//     PrintMatrix(matrix2);
//     printf("hello world");
//     return 0;
// }
