
typedef struct 
{
    int row;//行
    int line;//列
    double *data;
}Matrix;

// 矩阵X
double **mul(double **A, int m, int n, double **B, int q);

// 矩阵
double **sub(double **A, int m, int n, double **B);
//初始化矩阵
Matrix* InitMatrix(int row, int line);
//释放矩阵
void FreeMatrix(Matrix* pMatrix);
//给一个矩阵赋值
void ValueMatrix(Matrix* pMatrix, double *array);
//获得一个矩阵的大小
int SizeMatrix(Matrix* pMatrix);
//释放一个矩阵
void FreeMatrix(Matrix* pMatrix);
//释放一个矩阵
void CopyMatrix(Matrix* pMatrix_A, Matrix* pMatrix_B);
//复制一个矩阵的值
void PrintMatrix(Matrix* pMatrix);
//打印一个矩阵
//矩阵的基本运算
//矩阵的加法
Matrix* AddMatrix(Matrix *matrix_A, Matrix *matrix_B);
//矩阵的减法
Matrix* SubMatrix(Matrix *matrix_A, Matrix *matrix_B);	
//矩阵的乘法
Matrix* MulMatrix(Matrix *matrix_A, Matrix *matrix_B);		
//矩阵转置 条件为方阵
void TransMatrix(Matrix *matrix);			                

