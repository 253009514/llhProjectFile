#include<iostream>
#include<assert.h>
#include<stack>
using namespace std;
////////////////////////////////////////////////////////////
//��Ŀ:�Թ�������⡣
//1 1 1 1 1 1 1 1 1 1
//1 1 1 1 1 1 1 1 1 1
//2 2 2 1 1 1 1 1 1 1
//1 1 2 1 1 1 1 1 1 1
//1 1 3 2 2 2 2 1 1 1
//1 1 2 1 1 1 2 1 1 1
//1 1 3 2 2 2 2 1 1 1
//1 1 2 1 1 1 2 1 1 1
//1 1 2 1 1 1 2 1 1 1
//1 1 2 1 1 1 1 1 1 1
//��ڣ������ж�:����Ϊ�߽硣
//�߹���·���б�ǲ�����ջ��
//����ʱ���޸�Ϊ-1����ʾ��·��ͨ��
//4��������б�����˳ʱ����С�
//
//
////////////////////////////////////////////////////////////


/////////////////////////
//�����������Թ�������⡣
//����˼·�������У����߿����С�--�ҵ���
//�Ż���1�߹���·�̲��ǹ̶���ǡ�����+=2���������������Եõ�����·����λ�õĴ���

//�Ż���2ͨ���ж���һ����ֵ��ȷ��֮ǰ�Ƿ�·�������·������ôһֱ���˵��õ㡣�ų�������ͬ�ͻ������⡣


//ȱ�㣬û���ҵ����еĿ�ͨ��·����ֻ���ҵ������߷�������·�ߡ�


//���Ž���˼·��1�ݹ�ʵ�֣���ÿ��λ�ý����ĸ�����ֱ���⣬���ǿ�����������ߡ�
//2.����ʮ��·�ڣ��ڵݹ��ж�ÿ��·����⣬
//3.����·��Ϊ��̵ļ���·��

//if(����3��
//stack+=min������
//
void GetMazeMap(int *a,int row,int col)
{
	FILE *FOut;
	fopen_s(&FOut,"MazeMap.txt", "r");
	
	assert(FOut);

	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col;)
		{
			char ch = fgetc(FOut);
			if (ch == '0' || ch == '1')
			{
				a[i*col + j] = ch - '0';
				++j;
			}
		}
	}
}

void PrintMazeMap(int *a, int row, int col)
{
	cout << "MazeMap:(row,col):(" << row <<","<< col <<")."<< endl;
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			cout << a[i*col + j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}
struct Step
{
	int row;// ��
	int col;//��
	bool operator==(Step &s)
	{
		return row == s.row&&col == s.col;
	}
};
struct man
{
	man(Step s, int d)
	:_cur(s)
	, _dir(d)
	{}
	Step _cur;
	int _dir;// 0-3 ����4������

	man nextPos(int dir)
	{
		Step cur = _cur;
		dir = (dir+4) % 4;
		switch (dir)
		{
		case 0:
			cur.row--;
			break;
		case 1:
			cur.col++;
			break;
		case 2:
			cur.row++;
			break;
		case 3:
			cur.col--;
			break;
		}
		return man(cur, dir);
	}
};

stack<man> paths;


void PrintPathStep()
{
	while (!paths.empty())
	{
		man tmp = paths.top();
		cout << "[" << tmp._cur.row << "," << tmp._cur.col << "]:" << tmp._dir << "" << "-->\t";

		paths.pop();
	}
	cout << "Over!" << endl;
}

//����map����ڵ����꣬���Թ���
void GetMazePaths(int *map, int row, int col, Step& entry)
{
	//��ǰλ��.
	man m(entry, 0);
	map[m._cur.row*col + m._cur.col] = 2;
	paths.push(m);
	while (1)
	{
		man top = paths.top();
		man cur = top.nextPos(top._dir - 1);
		//man cur = top.nextPos(top._dir + 1);

		if (cur._cur.col<0 || cur._cur.row<0 || cur._cur.col>=col || cur._cur.row>=row)
		{
			cout << "Խ��" << endl;
			top._dir++;
			
			//top._dir--;
			paths.pop();
			paths.push(top);
			continue;
		}
		//�߽磬Ҳ���ǳ����
		if ((cur._cur.col == 0 || cur._cur.row == 0 || cur._cur.col == col-1 || cur._cur.row == row-1)
			&&map[cur._cur.row*col + cur._cur.col] == 0)
		{
			cout << "�����ǳ����" << endl;
			if (!(cur._cur == entry))
			{

				map[cur._cur.row*col + cur._cur.col] = 2;
				paths.push(cur);
				cout << "�õ�����" <<cur._cur.row<<" "<<cur._cur.col<< endl;
				break;
			}
		}

		//������
		//��һ��λ��Ϊ��ǰ�������һ��λ��
		if (map[cur._cur.row*col + cur._cur.col] != 1)
		{
			map[cur._cur.row*col + cur._cur.col] += 2;

			if (map[cur._cur.row*col + cur._cur.col] == 4)
			{
				Step tmp;
				tmp.row = cur._cur.row;
				tmp.col = cur._cur.col;
				//////////////////////////////////////////////////////////////////////////////
				//���˹��̡�
				while (paths.top()._cur.row != tmp.row || paths.top()._cur.col != tmp.col)
				{
					map[paths.top()._cur.row*col + paths.top()._cur.col] = 1;
					paths.pop();
				}
			}
			map[cur._cur.row*col + cur._cur.col] = 2;
			paths.push(cur);
		}
		else
		{
			top._dir++;
			
			//top._dir--;
			paths.pop();
			paths.push(top);
		}
	}
}

void GetNextAccessPath(int *map, int row, int col, man& entry)
{
	man tmp(entry);
	if (
		(
		entry._cur.row == row - 1 
		|| entry._cur.col == col - 1
		||entry._cur.row == 0 
	//	|| entry._cur.col == 0
	)
		&&	map[entry._cur.row*col + entry	._cur.col] != 1
		)
	{
		paths.push(entry);
		return;
	}
	else
	{
		paths.push(entry);
		tmp = entry.nextPos(entry._dir-1);
		while (map[tmp._cur.row*col + tmp._cur.col] == 1)
		{
			tmp = entry.nextPos(tmp._dir+1);
		}
		entry = tmp;
		GetNextAccessPath(map, row, col, entry);
	}
}

void GetMazePathsMin(int *map, int row, int col, man& entry)
{
	man tmp(entry);
	if (
		(
		entry._cur.row == row - 1
		|| entry._cur.col == col - 1
		|| entry._cur.row == 0
		//	|| entry._cur.col == 0
		)
		&& map[entry._cur.row*col + entry._cur.col] != 1
		)
	{
		paths.push(entry);
		return;
	}
	else
	{
		man m(entry);
		map[m._cur.row*col + m._cur.col] = 2;
		paths.push(m);

		man tmp1 = m.nextPos(m._dir - 1);
		man tmp2 = m.nextPos(m._dir);
		man tmp3 = m.nextPos(m._dir + 1);

		if (map[tmp1._cur.row*col + tmp1._cur.col] != 1)
		{
			GetMazePathsMin(map, row, col, tmp1);
		}
		if (map[tmp2._cur.row*col + tmp2._cur.col] != 1)
		{
			GetMazePathsMin(map, row, col, tmp2);
		}
		if (map[tmp3._cur.row*col + tmp3._cur.col] != 1)
		{
			GetMazePathsMin(map, row, col, tmp3);
		}
			//�����ߣ�������

			//if(�����ߣ�λ�ò�ͬ)

			//��֧ȷ��������Ϊ3������
			//��õ㵽ĩ�˵����·����
	}

}

void main()
{
	int a[20][20] = {};   
	::GetMazeMap((int*)a, 20, 20);
	Step ent = { 2, 0 };
	man entm = { { 2, 0 }, 1 };
	GetMazePaths((int*)a, 20, 20, ent);
	//GetMazePathsMin((int*)a, 20, 20, entm);
	//GetNextAccessPath((int*)a, 10, 10, man(ent, 0));

	::PrintMazeMap((int*)a, 20, 20);
	::PrintPathStep();
	system("pause");
}
