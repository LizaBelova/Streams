#include <iostream>
#include <thread>
#include <fstream>
#include <vector>
#include <time.h>
#include "ThreadArray.h"

using namespace std;

void megaSum(vector<float> &vec1, vector<float> &vec2, int beg, int end, float  &res)
{
	float result = 0;
	for (int i = beg; i < end; ++i)
		result += vec1[i] * vec2[i];

	res = result;
}

int main()
{
	ifstream input;
	input.open("input.txt");

	int n;
	input >> n;

	vector<float> a;
	vector<float> b;

	float x;
	for (int i = 0; i < n; ++i)
	{
		input >> x;
		a.push_back(x);
	}
	for (int i = 0; i < n; ++i)
	{
		input >> x;
		b.push_back(x);
	}

	long parall = clock();

	int threadCount = thread::hardware_concurrency();
	threadCount = threadCount < 2 ? 2 : threadCount;

	ThreadArray allThread(threadCount+1);
	float *allRes = new float[threadCount+1];
	allRes[threadCount] = 0;

	int pos = 0;
	int part = n / threadCount;

	bool notFull = part*threadCount < n;

	if (part != 0)
	{
		for (int i = 0; i < threadCount; ++i)
		{
			allThread[i] = move(thread(megaSum, ref(a), ref(b), pos, pos + part, ref(allRes[i])));
			pos += part;
		}
	}

	if (notFull)
		allThread[threadCount] = move(thread(megaSum, ref(a), ref(b), pos, n, ref(allRes[threadCount])));

	allThread.allJoin();

	parall = clock() - parall;


	cout << "Paral: ";
	if (part != 0)
	{
		float res = 0;
		
		for (int i = 0; i < threadCount + 1; ++i)
			res += allRes[i];

		cout << res << endl;
	}
	else
	{
		cout << allRes[threadCount] << endl;
	}


	long normal = clock();

	float testRes = 0;
	for (int i = 0; i < n; ++i)
		testRes += a[i] * b[i];

	normal = clock() - normal;

	cout << "True: " << testRes << endl;


	cout << endl;
	cout << "Paral: " << parall << endl;
	cout << "True: " << normal << endl;

	input.close();
	cin >> n;
	return 0;
}