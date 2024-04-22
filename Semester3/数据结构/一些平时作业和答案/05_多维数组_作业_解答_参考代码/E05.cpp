#include"Global.h"
//第五章练习
typedef int ElemType;
//definition of TSMatrix
const int MAXSIZE = 10;     // 选择小规模便于验证
typedef struct {
	int i, j;// 该非零元的行号和列号
	ElemType e; // 该非零元的值
} Triple;// 三元组
typedef struct {
	Triple data[MAXSIZE + 1];     // 非零元三元组表，data[0] 未用
	int mu, nu, tu; // 矩阵的行数、列数和非零元的个数
} TSMatrix;// 三元组顺序表 

//funciton of creating a TSMatrix
Status CreateTSMatrix(TSMatrix& M, const int(&param)[3 * MAXSIZE + 3])
{// 以行序为主序的次序逐个存储非零元信息到参数数组param，减少人机交互，提升速度，
	M.mu = param[0];
	M.nu = param[1];
	M.tu = param[2];
	for (int i = 1, k = 1; i <= M.tu; i++, k++) {
		M.data[k].i = param[i * 3 + 0];
		M.data[k].j = param[i * 3 + 1];
		M.data[k].e = param[i * 3 + 2];
	} // for
	return OK;
}

Status PrintTSMatrix(TSMatrix& M)
{// 以行序为主序的次序逐个存储非零元信息到参数数组param，减少人机交互，提升速度，
	printf("mu = %d\tnu = %d\ttu = %d\n", M.mu, M.nu, M.tu);
	for (int i = 1; i <= M.tu; i++) {
		printf("[%02d]%d,%d,%d\n", i, M.data[i].i, M.data[i].j, M.data[i].e);
	} // for
	return OK;
}

//function of adding by TSMatrix
Status AddTSMatrix(const TSMatrix &A, const TSMatrix &B, TSMatrix& C) {
	int i(1), j(1), k(1), ka(1), kb(1);
	
	if (A.mu != B.mu || A.nu != B.nu)
		return ERROR_INVALID_PARAM;//非法参数
	C.mu = A.mu; C.nu = B.nu;

	for (; i <= A.tu && j <= B.tu && k <= MAXSIZE;) {
		//计算矩阵A当前元素A.data[i]的坐标(i,j)以行序为主序时的一维位序，约定数组下标从1起始
		ka = (A.data[i].i - 1) * A.nu + A.data[i].j;
		//计算矩阵B当前元素B.data[j]的坐标(i,j)以行序为主序时的一维位序，约定数组下标从1起始
		kb = (B.data[j].i - 1) * B.nu + B.data[j].j;
		if (ka == kb)
		{//A,B当前非零元位置相同
			C.data[k].e = A.data[i].e + B.data[j].e;
			if (C.data[k].e != 0)
			{//注意：并不是每次循环都能产生C中的一个非零元素，所以C中非零元素的序号是否增加要结合具体情况判定，而不是每次循环都增加
				C.data[k].i = A.data[i].i; C.data[k].j = A.data[i].j; k++;
			}
			i++; j++;
		}
		else {//A,B当前非零元位置不同
			C.data[k++] = (ka < kb) ? A.data[i++] : B.data[j++];
		}
	}
	for (; i <= A.tu && k <= MAXSIZE; i++, k++)
	{
		C.data[k] = A.data[i];
	}
	for (; j <= B.tu && k <= MAXSIZE; j++, k++)
	{
		C.data[k] = B.data[j];
	}
	C.tu = k - 1;
	return 0;
}

void test_E05_AddTSMatrix() {
	TSMatrix A = { 0 }, B = { 0 }, C = { 0 };
	//验证参数设置，考虑了5种情况：同行不同列；同列不同行；同行同列且相互抵消；同行同列没有相互抵消；不同行不同列
	//param1:mu = 200, nu = 200, tu = 5, (3, 4, 5), (10, 100, -1), (101, 2, 3), (110, 199, 4), (150, 50, 6)
	int param1[3 * MAXSIZE + 3] = { 200, 200, 5, 3, 4, 5, 10, 100, -1, 101, 2, 3, 110, 199, 4, 150, 50, 6 },
		//param2:mu = 200, nu = 200, tu = 7, (3, 7, 4), (20, 100, 20), (101, 2, -3), (110, 199, 2), (151, 21, 3)
		param2[3 * MAXSIZE + 3] = { 200, 200, 5, 3, 7, 4, 20, 100, 20, 101, 2, -3, 110, 199, 2, 151, 21, 3 };

	CreateTSMatrix(A, param1);
	CreateTSMatrix(B, param2);
	PrintTSMatrix(A);
	PrintTSMatrix(B);
	AddTSMatrix(A, B, C);
	//结果应该是:mu = 100, nu = 200, tu = 5, (3, 4, 5),(3, 7, 4), (10, 100, -1), (20, 100, 20), (110, 199, 6), (150, 50, 6), (151, 21, 3)
	PrintTSMatrix(C);
}


//定义最大行数
#define MAXRC (128)
typedef struct {
	int j;// 该非零元的列号
	ElemType e; // 该非零元的值
} Tuple;// 二元组

typedef struct {
	Tuple data[MAXSIZE];     // 非零元二元组表，首个二元组存放在data[0]
	int rpos[MAXRC+1];//二元组行逻辑链接，指示每行首个非零元位置，序号从0起始，赠一个行序号，方便计算各行非零元素个数
	int mu, nu, tu; // 矩阵的行数、列数和非零元的个数
} BSMatrix;// 二元组顺序表 

//funciton of creating a BSMatrix
Status CreateBSMatrix(BSMatrix& M, const int(&param)[3 * MAXSIZE + 3])
{// 以行序为主序的次序逐个存储非零元信息到参数数组param，减少人机交互，提升速度，

	int i(0), j(0), k(0), ridx(0);

	M.mu = param[0];
	M.nu = param[1];
	M.tu = param[2];

	//初始化起始行起始位置为0
	for (i = 0; i < MAXRC; i++)
		M.rpos[i] = 0;
	//存储首个非零元信息
	M.rpos[param[3]] = 0;
	M.data[0].j = param[4];
	M.data[0].e = param[5];
	for (i = 2, k = 1, ridx = param[3]; i <= M.tu; i++, k++) {
		M.data[k].j = param[i * 3 + 1];
		M.data[k].e = param[i * 3 + 2];
		if (param[i * 3 + 0] != ridx) {
			//如果行号发生变化，保存当前行首个非零元位置
			for (j = ridx + 1; j <= param[i * 3 + 0]; j++)
				M.rpos[j] = k;
			ridx = param[i * 3 + 0];
		}
	} // for
	for (i = ridx + 1; i <= M.mu; i++) {
		M.rpos[i] = k;//剩余空行也记录起始位置，这样M.rops[i] - M.rops[i-1]就是第i-1行非零元素个数
	}
	return OK;
}

Status PrintBSMatrix(BSMatrix& M)
{// 以行序为主序的次序逐个存储非零元信息到参数数组param，减少人机交互，提升速度，
	int i(0), num(0), j(0), k(0);

	printf("mu = %d\tnu = %d\ttu = %d\n", M.mu, M.nu, M.tu);
	
	for (i = 0; i < M.mu; i++) {
		num = M.rpos[i + 1] - M.rpos[i];//计算第i行非零元个数
		if (!num)
			continue;
		for(j = 0; j < num; j++, k++)
			printf("[%02d]%d,%d,%d\n", k, i, M.data[k].j, M.data[k].e);
	} // for
	return OK;
}

Status GetEl_BSMatrix(const BSMatrix& M, int i, int j, ElemType &e)
{// 给定元素位置（i,j)，从二元组顺序表中取矩阵元素值e，行列序号从0开始
	int k(0);

	if (i < 0 || i >= M.mu || j < 0 || j >= M.nu)
		return ERROR;//元素位置非法

	e = 0;
	for (k = M.rpos[i]; k < M.rpos[i + 1]; k++) {
		if (M.data[k].j == j) {//指定元素非零
			e = M.data[k].e;
			break;
		}
	}
	return OK;
}

void test_E06_GetEl_BSMatrix() {
	BSMatrix A = { 0 };
	//验证参数设置，考虑了3种情况：首个非零行不是第0行，非零元素行不相邻，最末非零元素行不在末尾,行列序号从0开始
	//param1:mu = 100, nu = 200, tu = 5, (3, 4, 5), (10, 100, -1), (15, 2, 3), (20, 199, 4), (80, 50, 6)
	int param1[3 * MAXSIZE + 3] = { 100, 200, 5, 3, 4, 5, 10, 100, -1, 15, 2, 3, 20, 199, 4, 80, 50, 6 };
	ElemType e(-1);
	Status status;

	CreateBSMatrix(A, param1);
	PrintBSMatrix(A);

	status = GetEl_BSMatrix(A, 100, 1, e);//参数非法，status应该为ERROR, e应该为-1
	status = GetEl_BSMatrix(A, 2, 1, e);//零元，status应该为OK, e应该为0
	status = GetEl_BSMatrix(A, 15, 2, e);//非零元，status应该为OK, e应该为3
}
