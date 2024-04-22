#include"Global.h"
//��������ϰ
typedef int ElemType;
//definition of TSMatrix
const int MAXSIZE = 10;     // ѡ��С��ģ������֤
typedef struct {
	int i, j;// �÷���Ԫ���кź��к�
	ElemType e; // �÷���Ԫ��ֵ
} Triple;// ��Ԫ��
typedef struct {
	Triple data[MAXSIZE + 1];     // ����Ԫ��Ԫ���data[0] δ��
	int mu, nu, tu; // ����������������ͷ���Ԫ�ĸ���
} TSMatrix;// ��Ԫ��˳��� 

//funciton of creating a TSMatrix
Status CreateTSMatrix(TSMatrix& M, const int(&param)[3 * MAXSIZE + 3])
{// ������Ϊ����Ĵ�������洢����Ԫ��Ϣ����������param�������˻������������ٶȣ�
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
{// ������Ϊ����Ĵ�������洢����Ԫ��Ϣ����������param�������˻������������ٶȣ�
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
		return ERROR_INVALID_PARAM;//�Ƿ�����
	C.mu = A.mu; C.nu = B.nu;

	for (; i <= A.tu && j <= B.tu && k <= MAXSIZE;) {
		//�������A��ǰԪ��A.data[i]������(i,j)������Ϊ����ʱ��һάλ��Լ�������±��1��ʼ
		ka = (A.data[i].i - 1) * A.nu + A.data[i].j;
		//�������B��ǰԪ��B.data[j]������(i,j)������Ϊ����ʱ��һάλ��Լ�������±��1��ʼ
		kb = (B.data[j].i - 1) * B.nu + B.data[j].j;
		if (ka == kb)
		{//A,B��ǰ����Ԫλ����ͬ
			C.data[k].e = A.data[i].e + B.data[j].e;
			if (C.data[k].e != 0)
			{//ע�⣺������ÿ��ѭ�����ܲ���C�е�һ������Ԫ�أ�����C�з���Ԫ�ص�����Ƿ�����Ҫ��Ͼ�������ж���������ÿ��ѭ��������
				C.data[k].i = A.data[i].i; C.data[k].j = A.data[i].j; k++;
			}
			i++; j++;
		}
		else {//A,B��ǰ����Ԫλ�ò�ͬ
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
	//��֤�������ã�������5�������ͬ�в�ͬ�У�ͬ�в�ͬ�У�ͬ��ͬ�����໥������ͬ��ͬ��û���໥��������ͬ�в�ͬ��
	//param1:mu = 200, nu = 200, tu = 5, (3, 4, 5), (10, 100, -1), (101, 2, 3), (110, 199, 4), (150, 50, 6)
	int param1[3 * MAXSIZE + 3] = { 200, 200, 5, 3, 4, 5, 10, 100, -1, 101, 2, 3, 110, 199, 4, 150, 50, 6 },
		//param2:mu = 200, nu = 200, tu = 7, (3, 7, 4), (20, 100, 20), (101, 2, -3), (110, 199, 2), (151, 21, 3)
		param2[3 * MAXSIZE + 3] = { 200, 200, 5, 3, 7, 4, 20, 100, 20, 101, 2, -3, 110, 199, 2, 151, 21, 3 };

	CreateTSMatrix(A, param1);
	CreateTSMatrix(B, param2);
	PrintTSMatrix(A);
	PrintTSMatrix(B);
	AddTSMatrix(A, B, C);
	//���Ӧ����:mu = 100, nu = 200, tu = 5, (3, 4, 5),(3, 7, 4), (10, 100, -1), (20, 100, 20), (110, 199, 6), (150, 50, 6), (151, 21, 3)
	PrintTSMatrix(C);
}


//�����������
#define MAXRC (128)
typedef struct {
	int j;// �÷���Ԫ���к�
	ElemType e; // �÷���Ԫ��ֵ
} Tuple;// ��Ԫ��

typedef struct {
	Tuple data[MAXSIZE];     // ����Ԫ��Ԫ����׸���Ԫ������data[0]
	int rpos[MAXRC+1];//��Ԫ�����߼����ӣ�ָʾÿ���׸�����Ԫλ�ã���Ŵ�0��ʼ����һ������ţ����������з���Ԫ�ظ���
	int mu, nu, tu; // ����������������ͷ���Ԫ�ĸ���
} BSMatrix;// ��Ԫ��˳��� 

//funciton of creating a BSMatrix
Status CreateBSMatrix(BSMatrix& M, const int(&param)[3 * MAXSIZE + 3])
{// ������Ϊ����Ĵ�������洢����Ԫ��Ϣ����������param�������˻������������ٶȣ�

	int i(0), j(0), k(0), ridx(0);

	M.mu = param[0];
	M.nu = param[1];
	M.tu = param[2];

	//��ʼ����ʼ����ʼλ��Ϊ0
	for (i = 0; i < MAXRC; i++)
		M.rpos[i] = 0;
	//�洢�׸�����Ԫ��Ϣ
	M.rpos[param[3]] = 0;
	M.data[0].j = param[4];
	M.data[0].e = param[5];
	for (i = 2, k = 1, ridx = param[3]; i <= M.tu; i++, k++) {
		M.data[k].j = param[i * 3 + 1];
		M.data[k].e = param[i * 3 + 2];
		if (param[i * 3 + 0] != ridx) {
			//����кŷ����仯�����浱ǰ���׸�����Ԫλ��
			for (j = ridx + 1; j <= param[i * 3 + 0]; j++)
				M.rpos[j] = k;
			ridx = param[i * 3 + 0];
		}
	} // for
	for (i = ridx + 1; i <= M.mu; i++) {
		M.rpos[i] = k;//ʣ�����Ҳ��¼��ʼλ�ã�����M.rops[i] - M.rops[i-1]���ǵ�i-1�з���Ԫ�ظ���
	}
	return OK;
}

Status PrintBSMatrix(BSMatrix& M)
{// ������Ϊ����Ĵ�������洢����Ԫ��Ϣ����������param�������˻������������ٶȣ�
	int i(0), num(0), j(0), k(0);

	printf("mu = %d\tnu = %d\ttu = %d\n", M.mu, M.nu, M.tu);
	
	for (i = 0; i < M.mu; i++) {
		num = M.rpos[i + 1] - M.rpos[i];//�����i�з���Ԫ����
		if (!num)
			continue;
		for(j = 0; j < num; j++, k++)
			printf("[%02d]%d,%d,%d\n", k, i, M.data[k].j, M.data[k].e);
	} // for
	return OK;
}

Status GetEl_BSMatrix(const BSMatrix& M, int i, int j, ElemType &e)
{// ����Ԫ��λ�ã�i,j)���Ӷ�Ԫ��˳�����ȡ����Ԫ��ֵe��������Ŵ�0��ʼ
	int k(0);

	if (i < 0 || i >= M.mu || j < 0 || j >= M.nu)
		return ERROR;//Ԫ��λ�÷Ƿ�

	e = 0;
	for (k = M.rpos[i]; k < M.rpos[i + 1]; k++) {
		if (M.data[k].j == j) {//ָ��Ԫ�ط���
			e = M.data[k].e;
			break;
		}
	}
	return OK;
}

void test_E06_GetEl_BSMatrix() {
	BSMatrix A = { 0 };
	//��֤�������ã�������3��������׸������в��ǵ�0�У�����Ԫ���в����ڣ���ĩ����Ԫ���в���ĩβ,������Ŵ�0��ʼ
	//param1:mu = 100, nu = 200, tu = 5, (3, 4, 5), (10, 100, -1), (15, 2, 3), (20, 199, 4), (80, 50, 6)
	int param1[3 * MAXSIZE + 3] = { 100, 200, 5, 3, 4, 5, 10, 100, -1, 15, 2, 3, 20, 199, 4, 80, 50, 6 };
	ElemType e(-1);
	Status status;

	CreateBSMatrix(A, param1);
	PrintBSMatrix(A);

	status = GetEl_BSMatrix(A, 100, 1, e);//�����Ƿ���statusӦ��ΪERROR, eӦ��Ϊ-1
	status = GetEl_BSMatrix(A, 2, 1, e);//��Ԫ��statusӦ��ΪOK, eӦ��Ϊ0
	status = GetEl_BSMatrix(A, 15, 2, e);//����Ԫ��statusӦ��ΪOK, eӦ��Ϊ3
}
