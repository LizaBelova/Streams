#include "ThreadArray.h"

ThreadArray::ThreadArray(int s)
{
	size = s;
	arr = new std::thread[size];
}

ThreadArray::~ThreadArray()
{
	allJoin();

	delete[] arr;
}

void ThreadArray::allJoin()
{
	for (int i = 0; i < size; ++i)
	{
		if (arr[i].joinable())
			arr[i].join();
	}
}
