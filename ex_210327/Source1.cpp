#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

//num[]�� �ڸ����� �ø��� ó���Ѵ�.
void normalize(vector<int>& num)
{
	num.push_back(0);

	//�ڸ��� �ø��� ó���Ѵ�.
	int size = num.size();
	for (int i = 0; i < size - 1; i++)
	{
		if (num[i] < 0)
		{
			int borrow = (abs(num[i]) + 9) / 10;
			num[i + 1] -= borrow;
			num[i] += borrow * 10;
		}
		else
		{
			num[i + 1] += num[i] / 10;
			num[i] %= 10;
		}
	}
	//�տ� ���� 0�� �����Ѵ�.
	while (num.size() > 1 && num.back() == 0)
		num.pop_back();
}

//�ʵ��б��� ���� ����
vector<int> multiply(const vector<int>& a, const vector<int>& b)
{
	vector<int> c(a.size() + b.size() + 1, 0);

	int aSize = a.size();
	int bSize = b.size();

	for (int i = 0; i < aSize; i++)
		for (int j = 0; j < bSize; j++)
			c[i + j] += a[i] * b[j];
	normalize(c);
	return c;
}

//a += b * (10^k)
void addTo(vector<int>& a, const vector<int>& b, int k)
{
	int originalASize = a.size();
	if (a.size() < b.size() + k)
		a.resize(b.size() + k);
	a.push_back(0);

	int aSize = a.size();
	for (int i = originalASize; i < aSize; i++)
		a[i] = 0;

	int bSize = b.size();

	for (int i = 0; i < bSize; i++)
		a[i + k] += b[i];

	normalize(a);
}

// a -= b
// a>= b�� ��쿡�� ��� �����ϴ�.
void subFrom(vector<int>& a, const vector<int>& b)
{
	int bSize = b.size();

	for (int i = 0; i < bSize; i++)
		a[i] -= b[i];
	normalize(a);
}

vector<int> karatsuba(const vector<int>& a, const vector<int>& b)
{
	int an = a.size(), bn = b.size();

	//a�� b���� ª�� ��� ���� �ٲ۴�.
	if (an < bn)
		return karatsuba(b, a);
	//���� ��� : a�� b�� ����ִ� ���
	if (an == 0 || bn == 0)
		return vector<int>();
	//���� ��� : a�� ���� ª�� ���, O(n^2) �������� �����Ѵ�.(������ ����)
	if (an <= 50)
		return multiply(a, b);
	int half = an / 2;

	vector<int> a0(a.begin(), a.begin() + half);
	vector<int> a1(a.begin() + half, a.end());
	vector<int> b0(b.begin(), b.begin() + min<int>(b.size(), half));
	vector<int> b1(b.begin() + min<int>(b.size(), half), b.end());

	//z2 = a1 * b1
	vector<int> z2 = karatsuba(a1, b1);

	//z0 = a0 * b0
	vector<int> z0 = karatsuba(a0, b0);

	//z1 = ((a0 + a1) * (b0 + b1)) - z0 - z2
	addTo(a0, a1, 0);
	addTo(b0, b1, 0);
	vector<int> z1 = karatsuba(a0, b0);
	subFrom(z1, z0);
	subFrom(z1, z2);

	//���� ����
	//ret = z0+z1*10^half + z2 * 10(half*2)
	vector<int> ret(z2.size() + half * 2, 0);
	addTo(ret, z0, 0);
	addTo(ret, z1, half);
	addTo(ret, z2, half * 2);
	return ret;
}

int main()
{
	vector<int> a;
	vector<int> b;
	for (int i = 0; i < 100; i++)
		a.push_back(i % 10);

	for (int i = 0; i < 73; i++)
		b.push_back(i % 10);
	vector<int> c = karatsuba(b, a);

	int cSize = c.size();
	for (int i = 0; i < cSize; i++)
		cout << c[i];

	return 0;
}