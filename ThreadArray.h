#pragma once
#include <thread>
class ThreadArray
{
	int size;
	std::thread *arr;

	ThreadArray(ThreadArray & obj) = delete;
	ThreadArray& operator=(ThreadArray &obj) = delete;
public:
	ThreadArray(int s);
	~ThreadArray();

	std::thread& operator[] (int index)
	{
		return arr[index];
	}

	void allJoin();
};

