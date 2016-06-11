#include <iostream>
using namespace std;
#define MAXPROCESS 50                          /*��������*/
#define MAXRESOURCE 100                          /*�����Դ��*/
int AVAILABLE[MAXRESOURCE];                      /*������Դ����*/
int MAX[MAXPROCESS][MAXRESOURCE];              /*����������*/
int ALLOCATION[MAXPROCESS][MAXRESOURCE];      /*�������*/
int NEED[MAXPROCESS][MAXRESOURCE];              /*�������*/
int REQUEST[MAXPROCESS][MAXRESOURCE];          /*������Ҫ��Դ��*/
bool FINISH[MAXPROCESS];                          /*ϵͳ�Ƿ����㹻����Դ����*/
int p[MAXPROCESS];                               /*��¼����*/
int m, n;                                      /*m������,n����Դ*/
void Init();
bool Safe();
void Bank();
int main()
{
	Init();
	Safe();
	Bank();
}
void Init()                  /*��ʼ���㷨*/
{
	int i, j;

	cout << "��������̵���Ŀ:";
	cin >> m;
	cout << "��������Դ������:";
	cin >> n;
	cout << "������ÿ�������������ĸ���Դ��,����" << m << "x" << n << "�������룬�м��Կո����" << endl;
	for (i = 0; i<m; i++)
		for (j = 0; j<n; j++)
			cin >> MAX[i][j];
	cout << "������ÿ�������ѷ���ĸ���Դ��,����" << m << "x" << n << "�������룬�м��Կո����" << endl;
	for (i = 0; i<m; i++)
	{
		for (j = 0; j<n; j++)
		{
			cin >> ALLOCATION[i][j];
			NEED[i][j] = MAX[i][j] - ALLOCATION[i][j];
			if (NEED[i][j]<0)
			{
				cout << "������ĵ�" << i + 1 << "��������ӵ�еĵ�" << j + 1 << "����Դ������,����������:" << endl;
				j--;
				continue;
			}
		}
	}
	cout << "�����������Դ���е���Ŀ:" << endl;
	for (i = 0; i<n; i++)
	{
		cin >> AVAILABLE[i];
	}
}
void Bank()                  /*���м��㷨*/
{
	int i, cusneed;
	char again;
	while (1)
	{
		cout << "������Ҫ������Դ�Ľ��̺�(ע:��1�����̺�Ϊ0,��������)" << endl;
		cin >> cusneed;
		cout << "���������������ĸ���Դ������" << endl;
		for (i = 0; i<n; i++)
		{
			cin >> REQUEST[cusneed][i];
		}
		for (i = 0; i<n; i++)
		{
			if (REQUEST[cusneed][i]>NEED[cusneed][i])
			{
				cout << "��������������������̵�������!����������!" << endl;
				continue;
			}
			if (REQUEST[cusneed][i]>AVAILABLE[i])
			{
				cout << "�����������������ϵͳ�е���Դ��!����������!" << endl;


				continue;
			}
		}
		for (i = 0; i<n; i++)
		{
			AVAILABLE[i] -= REQUEST[cusneed][i];
			ALLOCATION[cusneed][i] += REQUEST[cusneed][i];
			NEED[cusneed][i] -= REQUEST[cusneed][i];
		}
		if (Safe())
		{
			cout << "ͬ���������!" << endl;
		}
		else
		{
			cout << "�������󱻾ܾ�!" << endl;
			for (i = 0; i<n; i++)
			{
				AVAILABLE[i] += REQUEST[cusneed][i];
				ALLOCATION[cusneed][i] -= REQUEST[cusneed][i];
				NEED[cusneed][i] += REQUEST[cusneed][i];
			}
		}
		for (i = 0; i<m; i++)
		{
			FINISH[i] = false;
		}
		cout << "�������������䣬�밴y" << endl;
		cin >> again;
		if (again == 'y' || again == 'Y')
		{
			continue;
		}
		break;
	}
}
bool Safe()                                      /*��ȫ���㷨*/
{
	int i, j, k, l = 0;
	int Work[MAXRESOURCE];                      /*��������*/
	for (i = 0; i < n; i++)
		Work[i] = AVAILABLE[i];
	for (i = 0; i < m; i++)
	{
		FINISH[i] = false;
	}
	for (i = 0; i < m; i++)
	{
		if (FINISH[i] == true)
		{
			continue;
		}
		else
		{
			for (j = 0; j < n; j++)
			{
				if (NEED[i][j] > Work[j])
				{
					break;
				}
			}
			if (j == n)
			{
				FINISH[i] = true;
				for (k = 0; k < n; k++)
				{
					Work[k] += ALLOCATION[i][k];
				}
				p[l++] = i;
				i = -1;
			}
			else
			{
				continue;
			}
		}
		if (l == m)
		{
			cout << "ϵͳ��ȫ��" << endl;
			cout << "��ȫ����:" << endl;
			for (i = 0; i < l; i++)
			{
				cout << p[i];
				if (i != l - 1)
				{
					cout << "-->";
				}
			}
			cout << "" << endl;
			return true;
		}
	}
	cout << "ϵͳ�ǲ���ȫ�ģ�" << endl;
	return false;
}
