#include <assert.h>
#include <iostream>
#include <Windows.h>
#include <time.h>
using namespace std;
#define _DEBUG 
#undef _DEBUG
#define _DEBUG_CONDITION
#undef	_DEBUG_CONDITION
class Date
{
public:
	friend ostream&operator <<(ostream&, const Date&);
	friend istream& operator >>(istream&is, Date& d);

	//static void 
	static Date NowTime()
	{
		time_t t;
		time(&t);
		tm *pt = localtime(&t);
		return Date(pt->tm_year + 1900, pt->tm_mon + 1, pt->tm_mday);
	}// ���ڶ�Ӧ����
	static int TransitionYTW(Date&dd)
	{
		int y = dd._year;
		int m = dd._month;
		int d = dd._day;
		if (m == 1 || m == 2)
		{
			m += 12;
			y--;
		}
		int iWeek = 0;
		iWeek = (d + 2 * m + 3 * (m + 1) / 5 + y + y / 4 - y / 100 + y / 400) % 7;
		return iWeek + 1;
	}
	static void ShowMonth(Date &dd)
	{
		Date tmp(dd);
		int TodayIs = tmp._day;
		int lo = 1;
		tmp._day = 1;
		int F = TransitionYTW(tmp);
		printf(" ��  һ  ��  ��  ��  ��  ��  \n");
		for (int j = 0; j < F; ++j)
			printf("    ");
		int monthDay = Date::GetMonDay(tmp._year,tmp._month);
		for (int k = 1; k <= monthDay; ++k)
		{
			if (F++ % 7 == 0)
			{
				lo++;
				printf("\n");
			}
			if (k == TodayIs)
			{
				printf("[%2d]", k);
			}
			else
				printf(" %2d ", k);
		}
		cout << endl;
	}
	static void Menu()
	{
		Date t1;
		Date t2(2018,1,1);
		
		int select = 1;
		int N = 100;
		while (select)
		{
			system("cls");
			cout << "*********************" << endl;
			cout << "**[1]����ʱ�䵽��ǰ**" << endl;
			cout << "**[2]����������  **" << endl;
			cout << "**[3]N��ǰ/��������**" << endl;
			cout << "**[4]��������鿴  **" << endl;
			cout << "**[0]�˳�����      **" << endl;
			cout << "*********************" << endl;
			cout << "**������ѡ�����   **" << endl;

			Date Now = NowTime();
			cin >> select;
			switch (select)
			{
			case 1:
				cout <<"��ǰʱ���ǣ�"<< Now << endl;
				break;
			case 2:
#ifndef _DEBUG_CONDITION
				cout << "����������ʱ����в�ѯeg�� 2015 3 3  2012 3 3 " << endl;
				cin >> t1 >> t2;
#endif
				cout << t1 << "   -    " << t2 << "   =  " << t1 - t2 << endl;
				system("pause");
				break;
			case 3:
#ifndef _DEBUG
				cout << "������������в�ѯ" << endl;
				cin >> N;
#endif
				cout << Now << "  +  " << N << "   =	" << Now+N << endl;
				system("pause");
				break;
			case 4:
				ShowMonth(Now);
				system("pause");
				break;
			case 0:
				select = 0;
				break;
			default:;
			}
		}
	}
	static int GetMonDay(const int &year,const int &month)
	{
		assert(year >= 1990);
		assert(month>0&&month<=12);
		static int monthArr[13] = {29,31,28,31,30,31,30,31,31,30,31,30,31};
		//����2��
		if (month ==2 &&(year%4==0&& ((year % 100)) || year % 400==0))
		{
			return monthArr[0];
		}
		return monthArr[month];
	}
	static int GetYearDay(const int &year)
	{
		return (year % 4 == 0 && ((year % 100)) || year % 400 == 0) ? 366 : 365;
	}
	static int TodayInYear(Date& d)
	{
		int ret = 0;
		for (int i = 1; i < d._month;i++)
		{
			ret += (d.GetMonDay(d._year, i));
		}
		return ret+d._day;
	}
public:
	//����
	Date(int year = 2000, int month = 1, int day = 1):
		_year(year),
		_month(month),
		_day(day){}
	//��������
	Date(const Date& d) :
		_year(d._year),
		_month(d._month),
		_day(d._day){}
	//����
	~Date(){}
	//��ֵ
	Date& operator = (const Date& d)
	{
		if (this != &d)
		{
			_year = d._year;
			_month = d._month;
			_day = d._day;
		}
		return *this;
	}
	bool Late(const Date& d)const
	{
		return (_year > d._year || (_year == d._year&&_month > d._month) ||
			(_year == d._year&&_month == d._month&&_day > d._day));
	}

	Date operator +(int  days)
	{
		Date tmp(*this);
		if (days < 0)
		{
			return operator-(-days);
		}
		tmp._day += days;
		//�Ż�1
		while (tmp._day>GetYearDay(tmp._year)-TodayInYear(tmp)+tmp._day)
		{
			tmp._day -= GetYearDay(tmp._year) - TodayInYear(tmp) + tmp._day;
			tmp._year++; //��ݶ���
			tmp._month = 1;
		}
		///year==
		while (tmp._day > GetMonDay(tmp._year, tmp._month))
		{
			tmp._day -= GetMonDay(tmp._year, tmp._month);
			tmp._month++;
			
		}
		return tmp;
	}
	//���������ࡣeg;2015-11-11   -    2016-4-6
	int operator-(const Date& d1)
	{
		Date tmp(*this);
		int flag = 1;
		Date d11(d1);
		//swap���� ��
		if (tmp > d11)
		{
			d11 = tmp;
			tmp = d1;
			flag = -1;
		}
		//tmp  ��
		int ret = 0;
		while (tmp < d11)
		{
			if (tmp._year != d11._year)
			{
				int yearDay = GetYearDay(tmp._year);
				int todayInYear = tmp.TodayInYear(tmp);
				ret += yearDay - todayInYear;		///�������1,1
				tmp._year++;
				tmp._month = 1;
				tmp._day = 1;
			}
			else
			{
				if (tmp._month != d11._month)
				{
					int monthDay = GetMonDay(tmp._year, tmp._month) - tmp._day+1; //�¶�����1,1
					tmp += monthDay;
					ret += monthDay;
				}
				else
				{
					ret += d11._day - tmp._day;
					break;
				}
			}
		}
		return ret*flag;
	}

	Date operator-(int days)
	{
		Date tmp(*this);
		if (days < 0)
		{
			return operator+(-days);
		}
		tmp._day -= days;

		////��������==0  ����<= 0
		while (tmp._day <= 0)
		{
			
			if (tmp._month == 1)
			{
				tmp._year--;
				tmp._month = 12;
			}
			else
			{
				tmp._month--;
			}
			tmp._day += GetMonDay(tmp._year, tmp._month);
		}
		return tmp;
	}

	Date& operator ++()
	{
		*this += 1;
		return *this;
	}
	Date operator++(int)
	{
		Date tmp(*this);
		*this +=1;
		return tmp;
	}
	Date& operator --()
	{
		*this -= 1;
		return *this;
	}
	Date operator--(int)
	{
		Date tmp(*this);
		*this -= 1;
		return tmp;
	}

	Date& operator+=(int days)
	{
		*this = *this + days;
		return *this;
	}
	Date& operator-=(int days)
	{
		*this = *this - days;
		return *this;
	}
	bool operator == (const Date&d1)const
	{
		return (_day == d1._day
			&&_month == d1._month
			&&_year == d1._year);
	}
	bool operator !=(const Date &d2)const
	{
		return !(*this == d2);
	}
	bool operator>(const Date& d)const
	{
		return Late(d);
	}
	bool operator<=(const Date& d)const
	{
		return !(Late(d));
	}
	bool operator>=(const Date& d)const
	{
		return ((*this) > d || *this == d);
	}
	bool operator <(const Date& d)const
	{
		return !(Late(d) || *this == d);
	}
	void Display()const
	{
		cout << _year << "��" << _month << "��" << _day << "��" << endl;
	}
private:
	int _year;
	int _month;
	int _day;
};

ostream& operator<<(ostream&os, const Date& d)
{
	os << d._year << "-" << d._month << "-" << d._day;
	return os;
}
istream& operator >>(istream&is, Date& d)
{
	is >> d._year >> d._month >> d._day;
	return is;
}


int Test()
{
	Date d1(2015,11,30);
	Date d2;
	d2 = d1 - 500;
	d2.Display();
	//cin >> d2;
	cout << d2 << endl;
	cout << ++d2 << endl;
	cout << d2++ << endl;
	d1 == d2;
	cout <<endl<< "Test for operarot-(Date &d) " << endl;
	Date d3(2019, 1, 4);
	Date d4(2017, 12, 22);
	Date d44(2016, 12, 29);
	Date d5(2016, 1, 4);
	cout << d1 << " - " << d3 <<" = ";
	cout << (d1 - d3) << endl;
	cout << d44 << " - " << d4 << " = ";
	cout << (d44 - d4) << endl;
	cout << d1 << " - " << d5 << " = ";
	cout << (d1 - d5) << endl;
	cout << d3  << " - " << d1 << " = ";
	cout << (d3 - d1) << endl;/**/
	return 0;
}
void main()
{
//	Test();
//  Date::Menu();
}