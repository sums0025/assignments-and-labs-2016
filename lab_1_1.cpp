// 场景1，使用了pthread库的方法
#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

// 这里让消费者和生产者一直运行，直到外部终止进程

const int MAX_BUFFER = 10; // 缓冲区最大数值
const int MIN_BUFFER = 0; // 缓冲区最小数值

pthread_mutex_t gMutex;
pthread_cond_t condc, condp;
int buffer = 0; // 生产者、消费者使用的缓冲区

void* producer(void* param)
{
	long long id = (long long)param;
	while (true)
	{
		pthread_mutex_lock(&gMutex);

		while (buffer == MAX_BUFFER)
		{
			cout << "Producer " << id << ": waiting" << endl;
			pthread_cond_wait(&condp, &gMutex);
			cout << "Producer " << id << ": resume" << endl;
		}
		cout << "Producer " << id << ": " << buffer << "->" << buffer + 1 << endl;
		buffer++;
		pthread_cond_signal(&condc);

		pthread_mutex_unlock(&gMutex);
		sleep(1); // 防止刷屏，睡1秒
	}

	pthread_exit(0);
}

void* consumer(void* param)
{
	long long id = (long long)param;
	while (true)
	{
		pthread_mutex_lock(&gMutex);

		while (buffer == MIN_BUFFER)
		{
			cout << "Consumer " << id << ": waiting" << endl;
			pthread_cond_wait(&condc, &gMutex);
			cout << "Consumer " << id << ": resume" << endl;
		}
		cout << "Consumer " << id << ": " << buffer << "->" << buffer - 1 << endl;
		buffer--;
		pthread_cond_signal(&condp);

		pthread_mutex_unlock(&gMutex);
		sleep(1);
	}
	pthread_exit(0);

}

int main(int argc, char *argv[])
{
	pthread_mutex_init(&gMutex, 0);
	pthread_cond_init(&condc, 0);
	pthread_cond_init(&condp, 0);

	int p, c;
	cout << "please input p and c: ";
	cin >> p >> c;

	pthread_t producers[p];
	pthread_t consumers[c];

	for (long long i = 0; i < p; ++i)
		pthread_create(&producers[i], 0, producer, (void*)i);
	for (long long i = 0; i < c; ++i)
		pthread_create(&consumers[i], 0, consumer, (void*)i);

	for (long long i = 0; i < p; ++i)
		pthread_join(producers[i], 0);
	for (long long i = 0; i < c; ++i)
		pthread_join(consumers[i], 0);

	pthread_cond_destroy(&condc);
	pthread_cond_destroy(&condp);
	pthread_mutex_destroy(&gMutex);

	return 0;
}

