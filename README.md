# Streams1

Дано(input.txt):
n - длинна массива(n>10^6)
Два массива a,b типа float
Вычислить:
∑▒〖(a[i]*b[i]),i=1,n〗
Указания к работе:
1) Разбить на оптимальное количество потоков.
2) Защитить потоки использовать механизм thread_guard. Внимательно следить за областью видимости переменных и когда они уничтожаются.

#include <iostream>
#include <thread>
#include <list>
using namespace std;

class thread_guard
{
	std::thread t;
	int id;
public:
//после двоеточия - это инициализация через список
	explicit thread_guard(std::thread t_, int _id) : t(std::move(t_)), id(std::move(_id))
	{
		cout << "\tThread " << id << " open" << endl;
	}
	~thread_guard()
	{
		if (t.joinable())
		{
			t.join();
		}
		cout << "\tThread " << id << " closed" << endl;
	}//конструктор копирования
	thread_guard(thread_guard const&) = delete;
	//перегрузка оператора =
	thread_guard& operator=(thread_guard const&) = delete;
};
//суммирование
void my_sum(float& s, float* a, float* b, int left, int right, int id)
{
	for (int i = left; i < right; i++)
	{
		s += a[i] * b[i];
		//cout << "sum " << id << endl;
	}
	return;
}

int main()
{
	int sum = 0, n = 1000000;
	float* a = new float[n];//создаем два массива и сначала иницилизируем его единицами
	float* b = new float[n];
	for (int i = 0; i < n; i++){
		a[i] = 1;
		b[i] = 1;
	}

	int core_number = std::thread::hardware_concurrency();//получаем количество потоков
	if (core_number<2)core_number = 2;
	cout << "Core number: " << core_number << endl;

	int h = n / core_number;//определяем шаг

	//создаем массив защитников потоков
	thread_guard* g[4];

	float *s = new float[core_number];
	for (int i = 0; i < core_number; i++)
	{
		s[i] = 0;
		g[i] = new thread_guard(std::thread(my_sum, std::ref(s[i]), a, b, i*h, (i+1)*h, i), i);
	}

	for (int i = 0; i < core_number; i++)
		delete g[i];//закрываем потоки и выводим ответ в s[i]
	int ans=0;
	for (int i = 0; i < core_number; i++)
	{
		cout << s[i] << endl;
		ans+=s[i];//складываем все 4 ответа от 4 потоков
	}
cout<< "Sum all threads: " <<ans;
	return 0;
}
