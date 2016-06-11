
#include <iostream>
#include <fstream>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>

using namespace std;

const int buffer_max = 10; 
const int buffer_min = 0; 

const int sem_init = 1234;
const int empty = 0;
const int full = 1;
const int mutex = 2;
const int sem_count = 3;

int readBuffer()
{
	int buffer;
	fstream f("./buffer", ios::in);
	f >> buffer;
	f.close();
	return buffer;
}

void writeBuffer(int buffer)
{
	fstream f("./buffer", ios::out);
	f << buffer;
	f.close();
}

void produceBuffer()
{
	int buffer = readBuffer();
	if (buffer < buffer_max)
	{
		cout << buffer << "->" << buffer + 1 << endl;
		buffer++;
	}
	else
	{
		cout << "waiting" << endl;
	}
	writeBuffer(buffer);
}

void consumeBuffer()
{
	int buffer = readBuffer();
	if (buffer > buffer_min)
	{
		cout << buffer << "->" << buffer - 1 << endl;
		buffer--;
	}
	else
	{
		cout << "waiting" << endl;
	}
	writeBuffer(buffer);
}

// P²Ù×÷
void P(int sem_id, int sem_num)
{
	struct sembuf xx;
	xx.sem_num = sem_num;
	xx.sem_op = -1;
	xx.sem_flg = SEM_UNDO;
	if (semop(sem_id, &xx, 1) != 0)
		cout << "error" << errno << endl;
}

// V²Ù×÷
void V(int sem_id, int sem_num)
{
	struct sembuf xx;
	xx.sem_num = sem_num;
	xx.sem_op = 1;
	xx.sem_flg = SEM_UNDO;
	if (semop(sem_id, &xx, 1) != 0)
		cout << "error" << errno << endl;
}

int main(int argc, char *argv[])
{
	
	writeBuffer(0);
	int sem_id = semget(sem_init, 0, IPC_CREAT | 0660);
	semctl(sem_id, IPC_RMID, 0);

	int p, c;
	cout << "please input p and c: ";
	cin >> p >> c;
	sem_id = semget(sem_init, sem_count, IPC_CREAT | 0660);
	if (sem_id < 0)
	{
		cout << "error when semget, errno: " << errno << endl;
		return -1;
	}
	semctl(sem_id, empty, SETVAL, buffer_max);
	semctl(sem_id, full, SETVAL, 0);
	semctl(sem_id, mutex, SETVAL, 1);

	for (int pCount = 0; pCount < p; ++pCount)
	{
		int pid = fork();
		if (pid < 0)
		{
			cout << "Error on fork." << endl;;
			exit(1);
		}
		if (pid == 0)
		{
			int id = pCount;
			while (true)
			{
				cout << "Producer " << id << ": ";
				P(sem_id, empty);
				P(sem_id, mutex);
				produceBuffer();
				V(sem_id, mutex);
				V(sem_id, full);
				sleep(1);
			}
			exit(0);
		}
	}


	for (int cCount = 0; cCount < c; ++cCount)
	{
		int pid = fork();
		if (pid < 0)
		{
			cout << "Error on fork." << endl;;
			exit(1);
		}
		if (pid == 0)
		{
			int id = cCount;
			while (true)
			{
				cout << "Consumer " << id << ": ";
				P(sem_id, full);
				P(sem_id, mutex);
				consumeBuffer();
				V(sem_id, mutex);
				V(sem_id, empty);
				sleep(1);
			}
			exit(0);
		}
	}

	while (wait(0) != -1);
	semctl(sem_id, IPC_RMID, 0);
	cout << "Main process ends." << endl;
	return 0;
}

