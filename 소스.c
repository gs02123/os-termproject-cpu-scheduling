#include <stdio.h>
#include <stdlib.h>
#include <time.h>


float awtcompare[6];
float attcompare[6];
int ghanttchart[1000];

typedef struct Process {
	int pid;
	int priority;
	int arrivalTime;
	int burst;
	int ioburst;
	int remaining;
	int executed;
	int waitingTime;
	int turnaroundTime;
	int interrupt;
}Process;

typedef struct ProcessQueue
{
	int cnt;
	int maxsize;
	Process* processes;
} Queue;

Process* createoneprocess(int pid, int priority, int arrivaltime, int burst, int ioburst, int interrupt)
{

	Process* newprocess = NULL;
	newprocess = (Process*)malloc(sizeof(Process));

	newprocess->pid = pid;
	newprocess->priority = priority;
	newprocess->arrivalTime = arrivaltime;
	newprocess->burst = burst;
	newprocess->ioburst = ioburst;
	newprocess->remaining = burst;
	newprocess->executed = 0;
	newprocess->waitingTime = 0;
	newprocess->turnaroundTime = 0;
	newprocess->interrupt = interrupt;
	return newprocess;
}

Queue* createQueue(int size)
{
	Queue* newQueue = NULL;
	newQueue = (Queue*)malloc(sizeof(Queue));
	newQueue->cnt = 0;
	newQueue->maxsize = size;
	newQueue->processes = (Process*)malloc(sizeof(Process)*newQueue->maxsize);
	return newQueue;
}

Queue* Copyqueue(Queue * originalqueue)
{
	int i = 0;
	Queue * copyqueue = NULL;
	copyqueue = createQueue(originalqueue->maxsize);
	for (i = 0; i<originalqueue->cnt; i++)
	{
		copyqueue->processes[i].arrivalTime = originalqueue->processes[i].arrivalTime;
		copyqueue->processes[i].pid = originalqueue->processes[i].pid;
		copyqueue->processes[i].priority = originalqueue->processes[i].priority;
		copyqueue->processes[i].burst = originalqueue->processes[i].burst;
		copyqueue->processes[i].ioburst = originalqueue->processes[i].ioburst;
		copyqueue->processes[i].executed = originalqueue->processes[i].executed;
		copyqueue->processes[i].remaining = originalqueue->processes[i].remaining;
		copyqueue->processes[i].waitingTime = originalqueue->processes[i].waitingTime;
		copyqueue->processes[i].turnaroundTime = originalqueue->processes[i].turnaroundTime;
		copyqueue->processes[i].interrupt = originalqueue->processes[i].interrupt;

		copyqueue->cnt++;
	}
	return copyqueue;
}

Process* CopyProcess(Process* process)
{
	Process* temp = NULL;
	temp = (Process*)malloc(sizeof(Process));
	temp->arrivalTime = process->arrivalTime;
	temp->pid = process->pid;
	temp->priority = process->priority;
	temp->burst = process->burst;
	temp->ioburst = process->ioburst;
	temp->executed = process->executed;
	temp->remaining = process->remaining;
	temp->waitingTime = process->waitingTime;
	temp->turnaroundTime = process->turnaroundTime;
	temp->interrupt = process->interrupt;

	return temp;
}

Queue* SortByArrival(Queue* queue)
{
	int i = 0;
	int j = 0;
	Queue* sorted = NULL;
	sorted = Copyqueue(queue);
	Process temp;
	for (i = 0; i < sorted->cnt; i++)
	{
		for (j = 0; j < sorted->cnt - 1; j++)
		{
			if (sorted->processes[j].arrivalTime > sorted->processes[j + 1].arrivalTime || (sorted->processes[j].arrivalTime == sorted->processes[j + 1].arrivalTime &&sorted->processes[j].pid > sorted->processes[j + 1].pid))
			{
				temp = sorted->processes[j];
				sorted->processes[j] = sorted->processes[j + 1];
				sorted->processes[j + 1] = temp;

			}
		}
	}
	return sorted;
}
Queue* SortByArrivalPid(Queue* queue)
{
	int i = 0;
	int j = 0;
	Queue* sorted = NULL;
	sorted = Copyqueue(queue);
	Process temp;
	for (i = 0; i < sorted->cnt; i++)
	{
		for (j = 0; j < sorted->cnt - 1; j++)
		{
			if ((sorted->processes[j].arrivalTime > sorted->processes[j + 1].arrivalTime) ||
				((sorted->processes[j].arrivalTime == sorted->processes[j + 1].arrivalTime) && (sorted->processes[j].priority > sorted->processes[j + 1].priority)) ||
				((sorted->processes[j].arrivalTime == sorted->processes[j + 1].arrivalTime) && (sorted->processes[j].priority == sorted->processes[j + 1].priority) && (sorted->processes[i].pid>sorted->processes[i + 1].pid)))
			{
				temp = sorted->processes[j];
				sorted->processes[j] = sorted->processes[j + 1];
				sorted->processes[j + 1] = temp;

			}
		}
	}
	return sorted;
}

Queue* SortRoundRobin(Queue* ReadyQueue)
{
	int o = 0;
	int t = 0;
	Queue* sorted = NULL;
	sorted = Copyqueue(ReadyQueue);
	Process temp;
	for (o = 0; o < sorted->cnt; o++)
	{
		for (t = 0; t < sorted->cnt - 1; t++)
		{
			if ((sorted->processes[t].remaining < sorted->processes[t].burst) && (sorted->processes[t + 1].remaining == sorted->processes[t + 1].burst))
			{
				temp = sorted->processes[t];
				sorted->processes[t] = sorted->processes[t + 1];
				sorted->processes[t + 1] = temp;


			}

		}
	}
	return sorted;
}

Queue* SortByArrivalsjf(Queue* queue)
{
	int i = 0;
	int j = 0;
	Queue* sorted = NULL;
	sorted = Copyqueue(queue);
	Process temp;
	for (i = 0; i < sorted->cnt; i++)
	{
		for (j = 0; j < sorted->cnt - 1; j++)
		{
			if ((sorted->processes[j].arrivalTime > sorted->processes[j + 1].arrivalTime) ||
				((sorted->processes[j].arrivalTime == sorted->processes[j + 1].arrivalTime) && (sorted->processes[j].burst > sorted->processes[j + 1].burst)) ||
				((sorted->processes[j].arrivalTime == sorted->processes[j + 1].arrivalTime) && (sorted->processes[j].burst == sorted->processes[j + 1].burst) && (sorted->processes[i].pid>sorted->processes[i + 1].pid)))
			{
				temp = sorted->processes[j];
				sorted->processes[j] = sorted->processes[j + 1];
				sorted->processes[j + 1] = temp;

			}
		}
	}
	return sorted;
}
Queue* SortByRemaining(Queue* queue)
{
	int i = 0;
	int j = 0;
	Queue* sorted = NULL;
	sorted = Copyqueue(queue);
	Process temp;
	for (i = 0; i < sorted->cnt; i++)
	{
		for (j = 0; j < sorted->cnt - 1; j++)
		{
			if (sorted->processes[j].remaining > sorted->processes[j + 1].remaining || ((sorted->processes[j].remaining == sorted->processes[j + 1].remaining) && (sorted->processes[j].arrivalTime > sorted->processes[j + 1].arrivalTime)))
			{
				temp = sorted->processes[j];
				sorted->processes[j] = sorted->processes[j + 1];
				sorted->processes[j + 1] = temp;

			}
		}
	}
	return sorted;
}

Queue* SortByPriority(Queue* queue)
{
	int i = 0;
	int j = 0;
	Queue* sorted = NULL;
	sorted = Copyqueue(queue);
	Process temp;
	for (i = 0; i < sorted->cnt; i++)
	{
		for (j = 0; j < sorted->cnt - 1; j++)
		{
			if (sorted->processes[j].priority > sorted->processes[j + 1].priority || ((sorted->processes[i].priority == sorted->processes[i + 1].priority) && (sorted->processes[i].arrivalTime> sorted->processes[i + 1].arrivalTime)) || ((sorted->processes[i].priority == sorted->processes[i + 1].priority) && (sorted->processes[i].arrivalTime == sorted->processes[i + 1].arrivalTime) && (sorted->processes[i].pid>sorted->processes[i + 1].pid)))
			{
				temp = sorted->processes[j];
				sorted->processes[j] = sorted->processes[j + 1];
				sorted->processes[j + 1] = temp;

			}
		}
	}
	return sorted;
}

Queue* createJobQueue()
{


	//ioburst time은 1~3
	// burst time 은 13~21
	// arrival time은 1~7
	// number of process 3부터 8사이
	int processnumber = 0;
	printf("생성할 프로세스의 수를 입력하세요(3부터 8사이)\n");
	scanf("%d", &processnumber);
	while (processnumber<3 || processnumber>8)
	{
		printf("생성할 프로세스의 수를 다시 입력하세요 (3부터 8사이)\n");
		scanf("%d", &processnumber);
	}
	Queue* JobQueue = NULL;
	JobQueue = createQueue(processnumber);
	int select = 0;
	printf("직접 만드려면 1번을, 자동 생성하려면 2번을 입력해주세요\n");
	scanf("%d", &select);
	while (select<1 || select>2)
	{
		printf("직접 만드려면 1번을, 자동 생성하려면 2번을 입력해주세요\n");
		scanf("%d", &select);
	}
	srand((unsigned)time(NULL));
	if (select == 1)
	{
		int k = 0;
		int arrival = 0;
		int priority = 0;
		int burst = 0;
		int ioburst = 0;
		for (k = 0; k < JobQueue->maxsize; k++)
		{
			JobQueue->processes[k].pid = k + 1;
			printf("%d번째 프로세스 설정하기\n", k + 1);
			printf("도착시간(1-7):");
			scanf("%d", &arrival);
			//while (arrival > 7 || arrival < 1)
			//{
			//	printf("도착시간(1-7):");
			//	scanf("%d", &arrival);
			//}
			JobQueue->processes[k].arrivalTime = arrival;
			printf("우선순위(낮은것이 더 우선):");
			scanf("%d", &priority);
			JobQueue->processes[k].priority = priority;
			printf("CPU burst(13-21):");
			scanf("%d", &burst);
			//while (burst> 21 || burst < 13)
			//{
			//	printf("CPU burst(13-21):");
			//	scanf("%d", &burst);
			//}
			JobQueue->processes[k].burst = burst;
			printf("io burst(1-3):");
			scanf("%d", &ioburst);
			//while (ioburst> 3 || ioburst < 1)
			//{
			//	printf("io burst(1-3):");
			//	scanf("%d", &ioburst);
			//}
			JobQueue->processes[k].ioburst = ioburst;
			JobQueue->processes[k].remaining = burst;
			JobQueue->processes[k].executed = 0;
			JobQueue->processes[k].waitingTime = 0;
			JobQueue->processes[k].turnaroundTime = 0;
			JobQueue->processes[k].interrupt = rand() % 3 + 3;
			JobQueue->cnt++;
		}
	}
	if (select == 2)
	{
		int i = 0;


		for (i = 0; i < JobQueue->maxsize; i++)
		{
			JobQueue->processes[i] = *(createoneprocess(i + 1, rand() % JobQueue->maxsize + 1, rand() % 6 + 1, rand() % 8 + 13, rand() % 3 + 1, rand() % 3 + 3));
			JobQueue->cnt++;
		}
	}
	return JobQueue;
}

void printqueue(Queue *newQueue)
{
	int i = 0;
	int sum = 0;

	for (i = 0; i<newQueue->cnt; i++)
	{
		printf("pid: %d arrivalTime: %d cpuburst: %d ioburst:%d interrupt:%d priority:%d \n", newQueue->processes[i].pid, newQueue->processes[i].arrivalTime, newQueue->processes[i].burst, newQueue->processes[i].ioburst, newQueue->processes[i].interrupt, newQueue->processes[i].priority);
		sum += newQueue->processes[i].remaining;
	}

	printf("%d\n", sum);
}

void insertQueue(Queue* queue, Process process)
{
	queue->processes[queue->cnt] = process;
	queue->cnt++;
}

Process pullQueue(Queue* queue)
{

	Process temp;
	temp = queue->processes[0];
	int i = 0;
	for (i = 0; i < queue->cnt - 1; i++)
	{
		queue->processes[i] = queue->processes[i + 1];
	}

	queue->cnt--;
	return temp;
}
void CPUinit(Process* CPU)
{
	CPU->arrivalTime = 0;
	CPU->burst = 0;
	CPU->ioburst = 0;
	CPU->executed = 0;
	CPU->pid = 0;
	CPU->priority = 0;
	CPU->remaining = 0;
	CPU->turnaroundTime = 0;
	CPU->waitingTime = 0;
	CPU->interrupt = 0;
}

void chartdraw(int time)
{
	int k = 0;
	int l = 0;
	int g = 0;
	for (k = 0; k < time; k++)
	{
		if (ghanttchart[k] != ghanttchart[k + 1])
			printf("======");
	}
	printf("=");
	printf("\n");
	printf("|");
	for (k = 0; k < time; k++)
	{
		if (ghanttchart[k] != ghanttchart[k + 1])
			printf("%3d  |", ghanttchart[k]);
	}
	printf("\n");
	printf("0");
	for (l = 0; l < time; l++)
	{
		if (ghanttchart[l] != ghanttchart[l + 1])
			printf("   %3d", l + 1);
	}
	printf("\n");
	for (k = 0; k < time; k++)
	{
		if (ghanttchart[k] != ghanttchart[k + 1])
			printf("======");
	}
	printf("=");
}

void fcfs(Queue* queue)
{

	Queue* sorted = NULL;
	Queue* WaitingQueue = createQueue(queue->maxsize);
	Queue* ReadyQueue = createQueue(queue->maxsize);
	Queue* Terminated = createQueue(queue->maxsize);
	Process CPU;
	CPUinit(&CPU);
	int i = 0;
	int r = 0;
	int time = 0;
	int j = 0;
	sorted = Copyqueue(queue);
	sorted = SortByArrival(sorted);
	time = sorted->processes[0].arrivalTime;
	insertQueue(ReadyQueue, pullQueue(sorted));
	for (r = 0; r < time; r++)
	{
		ghanttchart[r] = CPU.pid;
	}
	CPU = pullQueue(ReadyQueue);
	while (Terminated->cnt != Terminated->maxsize)
	{


		while (sorted->cnt > 0)
		{

			if (sorted->processes[0].arrivalTime != time)
				break;
			else
			{
				insertQueue(ReadyQueue, pullQueue(sorted));
			}
		}



		for (i = 0; i < ReadyQueue->cnt; i++)
		{
			ReadyQueue->processes[i].waitingTime++;
		}

		if (WaitingQueue->cnt >0)
		{
			if (WaitingQueue->processes[0].ioburst != 0)
			{
				WaitingQueue->processes[0].ioburst--;

			}
			if (WaitingQueue->processes[0].ioburst == 0)
			{

				insertQueue(ReadyQueue, pullQueue(WaitingQueue));
			}
		}
		CPU.executed++;
		CPU.remaining--;
		ghanttchart[time] = CPU.pid;

		if (CPU.remaining == CPU.interrupt)
		{


			insertQueue(WaitingQueue, CPU);
			if (ReadyQueue->cnt > 0)
				CPU = pullQueue(ReadyQueue);
			else
			{
				while (CPU.ioburst > 0)
				{
					time++;
					CPU.pid = 0;
					ghanttchart[time] = CPU.pid;
					CPU.ioburst--;
				}
				insertQueue(ReadyQueue, pullQueue(WaitingQueue));
				CPU = pullQueue(ReadyQueue);

			}

		}
		if (CPU.remaining == 0)
		{
			CPU.turnaroundTime = time+1 - CPU.arrivalTime;
			insertQueue(Terminated, CPU);
			CPU = pullQueue(ReadyQueue);
		}


		time++;


	}
	chartdraw(time);
	printf("\n");

	int totwaiting = 0;
	float avgwaiting = 0;
	int totturnaround = 0;
	float avgturnaround = 0;

	int z = 0;
	for (z = 0; z < Terminated->cnt; z++)
	{
		totwaiting += Terminated->processes[z].waitingTime;
		totturnaround += (Terminated->processes[z].turnaroundTime - Terminated->processes[z].arrivalTime);
	}

	avgturnaround = (float)totturnaround / Terminated->cnt;
	avgwaiting = (float)totwaiting / Terminated->cnt;

	attcompare[0] = avgturnaround;
	awtcompare[0] = avgwaiting;

}

void unsortnewfcfs(Queue* queue)
{

	Queue* sorted = NULL;
	Queue* WaitingQueue = createQueue(queue->maxsize);
	Queue* ReadyQueue = createQueue(queue->maxsize);
	Queue* Terminated = createQueue(queue->maxsize);
	Process CPU;
	CPUinit(&CPU);
	int i = 0;
	int r = 0;
	int time = 0;
	int j = 0;
	sorted = Copyqueue(queue);
	sorted = SortByArrival(sorted);
	time = sorted->processes[0].arrivalTime;
	insertQueue(ReadyQueue, pullQueue(sorted));
	for (r = 0; r < time; r++)
	{
		ghanttchart[r] = CPU.pid;
	}
	CPU = pullQueue(ReadyQueue);
	while (Terminated->cnt != Terminated->maxsize)
	{


		while (sorted->cnt > 0)
		{

			if (sorted->processes[0].arrivalTime != time)
				break;
			else
			{
				insertQueue(ReadyQueue, pullQueue(sorted));
			}
		}

		if (CPU.pid == 0 && ReadyQueue->cnt > 0)
		{
			CPU = pullQueue(ReadyQueue);
		}



		for (i = 0; i < ReadyQueue->cnt; i++)
		{
			ReadyQueue->processes[i].waitingTime++;
		}

		if (WaitingQueue->cnt >0)
		{
			if (WaitingQueue->processes[0].ioburst != 0)
			{
				WaitingQueue->processes[0].ioburst--;

			}
			if (WaitingQueue->processes[0].ioburst == 0)
			{

				insertQueue(ReadyQueue, pullQueue(WaitingQueue));
			}
		}
		CPU.executed++;
		CPU.remaining--;
		ghanttchart[time] = CPU.pid;

		if (CPU.remaining == CPU.interrupt)
		{


			insertQueue(WaitingQueue, CPU);
			if (ReadyQueue->cnt > 0)
				CPU = pullQueue(ReadyQueue);
			else
			{
				CPUinit(&CPU);

			}

		}
		if (CPU.remaining == 0)
		{
			CPU.turnaroundTime = time+1 - CPU.arrivalTime;
			if (CPU.pid != 0)
				insertQueue(Terminated, CPU);
			if (ReadyQueue->cnt > 0)
				CPU = pullQueue(ReadyQueue);
			else
				CPUinit(&CPU);
		}

		time++;


	}
	chartdraw(time);
	printf("\n");

	printf("%d", time);

	int totwaiting = 0;
	float avgwaiting = 0;
	int totturnaround = 0;
	float avgturnaround = 0;

	int z = 0;
	for (z = 0; z < Terminated->cnt; z++)
	{
		printf("%d의 waiting time %d and turnaroundtime %d\n", Terminated->processes[z].pid, Terminated->processes[z].waitingTime, Terminated->processes[z].turnaroundTime);
		totwaiting += Terminated->processes[z].waitingTime;
		totturnaround += (Terminated->processes[z].turnaroundTime - Terminated->processes[z].arrivalTime);
	}

	avgturnaround = (float)totturnaround / Terminated->cnt;
	avgwaiting = (float)totwaiting / Terminated->cnt;

	attcompare[0] = avgturnaround;
	awtcompare[0] = avgwaiting;

}

void newfcfs(Queue* queue)
{

	Queue* sorted = NULL;
	Queue* WaitingQueue = createQueue(queue->maxsize);
	Queue* ReadyQueue = createQueue(queue->maxsize);
	Queue* Terminated = createQueue(queue->maxsize);
	Process CPU;
	CPUinit(&CPU);
	int i = 0;
	int r = 0;
	int time = 0;
	int j = 0;
	sorted = Copyqueue(queue);
	sorted = SortByArrival(sorted);
	time = sorted->processes[0].arrivalTime;
	insertQueue(ReadyQueue, pullQueue(sorted));
	for (r = 0; r < time; r++)
	{
		ghanttchart[r] = CPU.pid;
	}
	CPU = pullQueue(ReadyQueue);
	while (Terminated->cnt != Terminated->maxsize)
	{


		while (sorted->cnt > 0)
		{

			if (sorted->processes[0].arrivalTime != time)
				break;
			else
			{
				insertQueue(ReadyQueue, pullQueue(sorted));
				ReadyQueue = SortByArrival(ReadyQueue);
			}
		}

		if (CPU.pid == 0 && ReadyQueue->cnt > 0)
		{
			ReadyQueue = SortByArrival(ReadyQueue);
			CPU = pullQueue(ReadyQueue);
		}

	

		for (i = 0; i < ReadyQueue->cnt; i++)
		{
			ReadyQueue->processes[i].waitingTime++;
		}

		if (WaitingQueue->cnt >0)
		{
			if (WaitingQueue->processes[0].ioburst != 0)
			{
				WaitingQueue->processes[0].ioburst--;

			}
			if (WaitingQueue->processes[0].ioburst == 0)
			{

				insertQueue(ReadyQueue, pullQueue(WaitingQueue));
			}
		}
		CPU.executed++;
		CPU.remaining--;
		ghanttchart[time] = CPU.pid;

		if (CPU.remaining == CPU.interrupt)
		{


			insertQueue(WaitingQueue, CPU);
			if (ReadyQueue->cnt > 0)

			{
				ReadyQueue = SortByArrival(ReadyQueue);
				CPU = pullQueue(ReadyQueue);
			}
			else
			{
				CPUinit(&CPU);

			}

		}
		if (CPU.remaining == 0)
		{
			CPU.turnaroundTime = time+1- CPU.arrivalTime;
			if (CPU.pid!=0)
				insertQueue(Terminated, CPU);
			if (ReadyQueue->cnt > 0)
			{
				ReadyQueue = SortByArrival(ReadyQueue);
				CPU = pullQueue(ReadyQueue);
			}
			else
				CPUinit(&CPU);
		}


		ReadyQueue = SortByArrival(ReadyQueue);
		time++;


	}
	chartdraw(time);
	printf("\n");

	int totwaiting = 0;
	float avgwaiting = 0;
	int totturnaround = 0;
	float avgturnaround = 0;

	int z = 0;
	for (z = 0; z < Terminated->cnt; z++)
	{
		printf("프로세스 %d의 waiting time %d and turnaroundtime %d\n", Terminated->processes[z].pid, Terminated->processes[z].waitingTime, Terminated->processes[z].turnaroundTime);
		totwaiting += Terminated->processes[z].waitingTime;
		totturnaround += (Terminated->processes[z].turnaroundTime - Terminated->processes[z].arrivalTime);
	}

	avgturnaround = (float)totturnaround / Terminated->cnt;
	avgwaiting = (float)totwaiting / Terminated->cnt;

	attcompare[0] = avgturnaround;
	awtcompare[0] = avgwaiting;

}


void sjf(Queue* queue)
{
	Queue* sorted = NULL;
	Queue* WaitingQueue = createQueue(queue->maxsize);
	Queue* ReadyQueue = createQueue(queue->maxsize);
	Queue* Terminated = createQueue(queue->maxsize);
	Process CPU;
	CPUinit(&CPU);
	int i = 0;
	int r = 0;
	int time = 0;
	int j = 0;
	sorted = Copyqueue(queue);
	sorted = SortByArrivalsjf(sorted);
	time = sorted->processes[0].arrivalTime;
	insertQueue(ReadyQueue, pullQueue(sorted));

	for (r = 0; r < time; r++)
	{
		ghanttchart[r] = CPU.pid;
	}
	CPU = pullQueue(ReadyQueue);
	while (Terminated->cnt != Terminated->maxsize)
	{
		while (sorted->cnt > 0)
		{

			if (sorted->processes[0].arrivalTime != time)
				break;
			else
			{
				insertQueue(ReadyQueue, pullQueue(sorted));
				ReadyQueue = SortByRemaining(ReadyQueue);
			}
		}

		CPU.executed++;
		CPU.remaining--;
		ghanttchart[time] = CPU.pid;
		ReadyQueue = SortByRemaining(ReadyQueue);


		for (i = 0; i < ReadyQueue->cnt; i++)
		{
			ReadyQueue->processes[i].waitingTime++;
		}

		if (WaitingQueue->cnt >0)
		{
			if (WaitingQueue->processes[0].ioburst != 0)
			{
				WaitingQueue->processes[0].ioburst--;

			}
			if (WaitingQueue->processes[0].ioburst == 0)
			{

				insertQueue(ReadyQueue, pullQueue(WaitingQueue));
				ReadyQueue = SortByRemaining(ReadyQueue);
			}
		}

		if (CPU.remaining == CPU.interrupt)
		{

			insertQueue(WaitingQueue, CPU);
			if (ReadyQueue->cnt > 0)
				CPU = pullQueue(ReadyQueue);
			else
			{
				while (CPU.ioburst > 0)
				{
					time++;
					CPU.pid = 0;
					ghanttchart[time] = CPU.pid;
					CPU.ioburst--;
				}
				insertQueue(ReadyQueue, pullQueue(WaitingQueue));
				CPU = pullQueue(ReadyQueue);

			}

		}

		if (CPU.remaining == 0)
		{
			CPU.turnaroundTime = time+1- CPU.arrivalTime;
			insertQueue(Terminated, CPU);
			CPU = pullQueue(ReadyQueue);
		}
		time++;


	}

	chartdraw(time);
	printf("\n");


	int totwaiting = 0;
	float avgwaiting = 0;
	int totturnaround = 0;
	float avgturnaround = 0;

	int z = 0;
	for (z = 0; z < Terminated->cnt; z++)
	{
		totwaiting += Terminated->processes[z].waitingTime;
		totturnaround += (Terminated->processes[z].turnaroundTime - Terminated->processes[z].arrivalTime);
	}

	avgturnaround = (float)totturnaround / Terminated->cnt;
	avgwaiting = (float)totwaiting / Terminated->cnt;

	attcompare[3] = avgturnaround;
	awtcompare[3] = avgwaiting;

}

void newsjf(Queue* queue)
{
	Queue* sorted = NULL;
	Queue* WaitingQueue = createQueue(queue->maxsize);
	Queue* ReadyQueue = createQueue(queue->maxsize);
	Queue* Terminated = createQueue(queue->maxsize);
	Process CPU;
	CPUinit(&CPU);
	int i = 0;
	int r = 0;
	int time = 0;
	int j = 0;
	sorted = Copyqueue(queue);
	sorted = SortByArrivalsjf(sorted);
	time = sorted->processes[0].arrivalTime;
	insertQueue(ReadyQueue, pullQueue(sorted));

	for (r = 0; r < time; r++)
	{
		ghanttchart[r] = CPU.pid;
	}
	CPU = pullQueue(ReadyQueue);
	while (Terminated->cnt != Terminated->maxsize)
	{
		while (sorted->cnt > 0)
		{

			if (sorted->processes[0].arrivalTime != time)
				break;
			else
			{
				insertQueue(ReadyQueue, pullQueue(sorted));
				ReadyQueue = SortByRemaining(ReadyQueue);
			}
		}




		if (CPU.pid == 0 && ReadyQueue->cnt > 0)
		{
			ReadyQueue = SortByRemaining(ReadyQueue);
			CPU = pullQueue(ReadyQueue);
		}


		CPU.executed++;
		CPU.remaining--;
		ghanttchart[time] = CPU.pid;
		ReadyQueue = SortByRemaining(ReadyQueue);


		for (i = 0; i < ReadyQueue->cnt; i++)
		{
			ReadyQueue->processes[i].waitingTime++;
		}

		if (WaitingQueue->cnt >0)
		{
			if (WaitingQueue->processes[0].ioburst != 0)
			{
				WaitingQueue->processes[0].ioburst--;

			}
			if (WaitingQueue->processes[0].ioburst == 0)
			{

				insertQueue(ReadyQueue, pullQueue(WaitingQueue));
				ReadyQueue = SortByRemaining(ReadyQueue);
			}
		}

		if (CPU.remaining == CPU.interrupt)
		{

			insertQueue(WaitingQueue, CPU);
			if (ReadyQueue->cnt > 0)
			{
				ReadyQueue = SortByRemaining(ReadyQueue);
				CPU = pullQueue(ReadyQueue);
			}
			else
			{
				CPUinit(&CPU);

			}

		}

		if (CPU.remaining == 0)
		{
			CPU.turnaroundTime = time+1 - CPU.arrivalTime;;
			if (CPU.pid != 0)
				insertQueue(Terminated, CPU);
			if (ReadyQueue->cnt > 0)
			{
				ReadyQueue = SortByRemaining(ReadyQueue);
				CPU = pullQueue(ReadyQueue);
			}
			else
				CPUinit(&CPU);
		}
		time++;


	}

	chartdraw(time);
	printf("\n");


	int totwaiting = 0;
	float avgwaiting = 0;
	int totturnaround = 0;
	float avgturnaround = 0;

	int z = 0;
	for (z = 0; z < Terminated->cnt; z++)
	{

		printf("프로세스 %d의 waiting time %d and turnaroundtime %d\n", Terminated->processes[z].pid, Terminated->processes[z].waitingTime, Terminated->processes[z].turnaroundTime);
		totwaiting += Terminated->processes[z].waitingTime;
		totturnaround += (Terminated->processes[z].turnaroundTime - Terminated->processes[z].arrivalTime);
	}

	avgturnaround = (float)totturnaround / Terminated->cnt;
	avgwaiting = (float)totwaiting / Terminated->cnt;

	attcompare[3] = avgturnaround;
	awtcompare[3] = avgwaiting;

}



void newpriority(Queue* queue)
{
	Queue* sorted = NULL;
	Queue* WaitingQueue = createQueue(queue->maxsize);
	Queue* ReadyQueue = createQueue(queue->maxsize);
	Queue* Terminated = createQueue(queue->maxsize);
	Process CPU;
	CPUinit(&CPU);
	int i = 0;
	int r = 0;
	int time = 0;
	int j = 0;
	sorted = Copyqueue(queue);
	sorted = SortByArrivalPid(sorted);
	time = sorted->processes[0].arrivalTime;
	insertQueue(ReadyQueue, pullQueue(sorted));

	for (r = 0; r < time; r++)
	{
		ghanttchart[r] = CPU.pid;
	}


	CPU = pullQueue(ReadyQueue);

	while (Terminated->cnt != Terminated->maxsize)
	{

		while (sorted->cnt > 0)
		{
			if (sorted->processes[0].arrivalTime != time)
				break;
			else
			{
				insertQueue(ReadyQueue, pullQueue(sorted));
				ReadyQueue = SortByPriority(ReadyQueue);
			}

		}


		for (i = 0; i < ReadyQueue->cnt; i++)
		{
			ReadyQueue->processes[i].waitingTime++;
		}

		if (WaitingQueue->cnt >0)
		{
			if (WaitingQueue->processes[0].ioburst != 0)
			{
				WaitingQueue->processes[0].ioburst--;

			}
			if (WaitingQueue->processes[0].ioburst == 0)
			{

				insertQueue(ReadyQueue, pullQueue(WaitingQueue));
			}
		}
		CPU.executed++;
		CPU.remaining--;
		ghanttchart[time] = CPU.pid;

		if (CPU.remaining == CPU.interrupt)
		{


			insertQueue(WaitingQueue, CPU);
			if (ReadyQueue->cnt > 0)
				CPU = pullQueue(ReadyQueue);
			else
			{
				while (CPU.ioburst > 0)
				{
					time++;
					CPU.pid = 0;
					ghanttchart[time] = CPU.pid;
					CPU.ioburst--;
				}
				insertQueue(ReadyQueue, pullQueue(WaitingQueue));
				CPU = pullQueue(ReadyQueue);

			}

		}
		if (CPU.remaining == 0)
		{
			CPU.turnaroundTime = time+1 - CPU.arrivalTime;;
			insertQueue(Terminated, CPU);
			CPU = pullQueue(ReadyQueue);
		}


		ReadyQueue = SortByPriority(ReadyQueue);
		time++;


	}
	chartdraw(time);
	printf("\n");


	int totwaiting = 0;
	float avgwaiting = 0;
	int totturnaround = 0;
	float avgturnaround = 0;

	int z = 0;
	for (z = 0; z < Terminated->cnt; z++)
	{
		totwaiting += Terminated->processes[z].waitingTime;
		totturnaround += (Terminated->processes[z].turnaroundTime - Terminated->processes[z].arrivalTime);
	}

	avgturnaround = (float)totturnaround / Terminated->cnt;
	avgwaiting = (float)totwaiting / Terminated->cnt;

	attcompare[4] = avgturnaround;
	awtcompare[4] = avgwaiting;

}
void priority(Queue* queue)
{
	Queue* sorted = NULL;
	Queue* WaitingQueue = createQueue(queue->maxsize);
	Queue* ReadyQueue = createQueue(queue->maxsize);
	Queue* Terminated = createQueue(queue->maxsize);
	Process CPU;
	CPUinit(&CPU);
	int i = 0;
	int r = 0;
	int time = 0;
	int j = 0;
	sorted = Copyqueue(queue);
	sorted = SortByArrivalPid(sorted);
	time = sorted->processes[0].arrivalTime;
	insertQueue(ReadyQueue, pullQueue(sorted));

	for (r = 0; r < time; r++)
	{
		ghanttchart[r] = CPU.pid;
	}


	CPU = pullQueue(ReadyQueue);

	while (Terminated->cnt != Terminated->maxsize)
	{

		while (sorted->cnt > 0)
		{
			if (sorted->processes[0].arrivalTime != time)
				break;
			else
			{
				insertQueue(ReadyQueue, pullQueue(sorted));
				ReadyQueue = SortByPriority(ReadyQueue);
			}

		}



		if (CPU.pid == 0 && ReadyQueue->cnt > 0)
		{
			ReadyQueue = SortByPriority(ReadyQueue);
			CPU = pullQueue(ReadyQueue);
		}

		for (i = 0; i < ReadyQueue->cnt; i++)
		{
			ReadyQueue->processes[i].waitingTime++;
		}

		if (WaitingQueue->cnt >0)
		{
			if (WaitingQueue->processes[0].ioburst != 0)
			{
				WaitingQueue->processes[0].ioburst--;

			}
			if (WaitingQueue->processes[0].ioburst == 0)
			{

				insertQueue(ReadyQueue, pullQueue(WaitingQueue));
			}
		}
		CPU.executed++;
		CPU.remaining--;
		ghanttchart[time] = CPU.pid;

		if (CPU.remaining == CPU.interrupt)
		{


			insertQueue(WaitingQueue, CPU);
			if (ReadyQueue->cnt > 0)
			{
				ReadyQueue = SortByPriority(ReadyQueue);
				CPU = pullQueue(ReadyQueue);
			}
			else
			{
				CPUinit(&CPU);

			}

		}
		if (CPU.remaining == 0)
		{
			CPU.turnaroundTime = time+1 - CPU.arrivalTime;
			if (CPU.pid != 0)
				insertQueue(Terminated, CPU);
			if (ReadyQueue->cnt > 0)
			{
				ReadyQueue = SortByPriority(ReadyQueue);
				CPU = pullQueue(ReadyQueue);
			}
			else
				CPUinit(&CPU);
		}


		ReadyQueue = SortByPriority(ReadyQueue);
		time++;


	}
	chartdraw(time);
	printf("\n");


	int totwaiting = 0;
	float avgwaiting = 0;
	int totturnaround = 0;
	float avgturnaround = 0;

	int z = 0;
	for (z = 0; z < Terminated->cnt; z++)
	{

		printf("프로세스 %d의 waiting time %d and turnaroundtime %d\n", Terminated->processes[z].pid, Terminated->processes[z].waitingTime, Terminated->processes[z].turnaroundTime);
		totwaiting += Terminated->processes[z].waitingTime;
		totturnaround += (Terminated->processes[z].turnaroundTime - Terminated->processes[z].arrivalTime);
	}

	avgturnaround = (float)totturnaround / Terminated->cnt;
	avgwaiting = (float)totwaiting / Terminated->cnt;

	attcompare[4] = avgturnaround;
	awtcompare[4] = avgwaiting;

}

void presjf(Queue* queue)
{
	Queue* sorted = NULL;
	Queue* WaitingQueue = createQueue(queue->maxsize);
	Queue* ReadyQueue = createQueue(queue->maxsize);
	Queue* Terminated = createQueue(queue->maxsize);
	Process CPU;
	CPUinit(&CPU);
	int i = 0;
	int r = 0;
	int time = 0;
	int j = 0;
	sorted = Copyqueue(queue);
	sorted = SortByArrivalsjf(sorted);
	time = sorted->processes[0].arrivalTime;
	insertQueue(ReadyQueue, pullQueue(sorted));

	for (r = 0; r < time; r++)
	{
		ghanttchart[r] = CPU.pid;
	}


	CPU = pullQueue(ReadyQueue);
	while (Terminated->cnt != Terminated->maxsize)
	{


		while (sorted->cnt > 0)
		{
			if (sorted->processes[0].arrivalTime != time)
				break;
			else
			{
				insertQueue(ReadyQueue, pullQueue(sorted));
				ReadyQueue = SortByRemaining(ReadyQueue);
				if (ReadyQueue->processes[0].remaining < CPU.remaining)
				{
					insertQueue(ReadyQueue, CPU);
					CPU = pullQueue(ReadyQueue);
					ReadyQueue = SortByRemaining(ReadyQueue);
				}
			}
		}


		CPU.executed++;
		CPU.remaining--;
		ghanttchart[time] = CPU.pid;
		ReadyQueue = SortByRemaining(ReadyQueue);


		for (i = 0; i < ReadyQueue->cnt; i++)
		{
			ReadyQueue->processes[i].waitingTime++;
		}

		if (WaitingQueue->cnt >0)
		{
			if (WaitingQueue->processes[0].ioburst != 0)
			{
				WaitingQueue->processes[0].ioburst--;

			}
			if (WaitingQueue->processes[0].ioburst == 0)
			{

				insertQueue(ReadyQueue, pullQueue(WaitingQueue));
				ReadyQueue = SortByRemaining(ReadyQueue);
			}
		}



		if (CPU.remaining == CPU.interrupt)
		{

			insertQueue(WaitingQueue, CPU);
			if (ReadyQueue->cnt > 0)
				CPU = pullQueue(ReadyQueue);
			else
			{
				while (CPU.ioburst > 0)
				{
					time++;
					CPU.pid = 0;
					ghanttchart[time] = CPU.pid;
					CPU.ioburst--;
				}
				insertQueue(ReadyQueue, pullQueue(WaitingQueue));
				CPU = pullQueue(ReadyQueue);

			}

		}

		if (CPU.remaining > ReadyQueue->processes[0].remaining)
		{
			insertQueue(ReadyQueue, CPU);
			CPU = pullQueue(ReadyQueue);
		}

		if (CPU.remaining == 0)
		{
			CPU.turnaroundTime = time+1 - CPU.arrivalTime;
			insertQueue(Terminated, CPU);
			CPU = pullQueue(ReadyQueue);
		}
		time++;


	}

	chartdraw(time);
	printf("\n");


	int totwaiting = 0;
	float avgwaiting = 0;
	int totturnaround = 0;
	float avgturnaround = 0;

	int z = 0;
	for (z = 0; z < Terminated->cnt; z++)
	{
		totwaiting += Terminated->processes[z].waitingTime;
		totturnaround += (Terminated->processes[z].turnaroundTime - Terminated->processes[z].arrivalTime);
	}

	avgturnaround = (float)totturnaround / Terminated->cnt;
	avgwaiting = (float)totwaiting / Terminated->cnt;

	attcompare[2] = avgturnaround;
	awtcompare[2] = avgwaiting;


}
void newpresjf(Queue* queue)
{
	Queue* sorted = NULL;
	Queue* WaitingQueue = createQueue(queue->maxsize);
	Queue* ReadyQueue = createQueue(queue->maxsize);
	Queue* Terminated = createQueue(queue->maxsize);
	Process CPU;
	CPUinit(&CPU);
	int i = 0;
	int r = 0;
	int time = 0;
	int j = 0;
	sorted = Copyqueue(queue);
	sorted = SortByArrivalsjf(sorted);
	time = sorted->processes[0].arrivalTime;
	insertQueue(ReadyQueue, pullQueue(sorted));

	for (r = 0; r < time; r++)
	{
		ghanttchart[r] = CPU.pid;
	}


	CPU = pullQueue(ReadyQueue);
	while (Terminated->cnt != Terminated->maxsize)
	{


		while (sorted->cnt > 0)
		{
			if (sorted->processes[0].arrivalTime != time)
				break;
			else
			{
				insertQueue(ReadyQueue, pullQueue(sorted));
				ReadyQueue = SortByRemaining(ReadyQueue);
				if (ReadyQueue->processes[0].remaining < CPU.remaining)
				{
					insertQueue(ReadyQueue, CPU);
					CPU = pullQueue(ReadyQueue);
					ReadyQueue = SortByRemaining(ReadyQueue);
				}
			}
		}

		if (CPU.pid == 0 && ReadyQueue->cnt > 0)
		{
			CPU = pullQueue(ReadyQueue);
		}


		CPU.executed++;
		CPU.remaining--;
		ghanttchart[time] = CPU.pid;
		ReadyQueue = SortByRemaining(ReadyQueue);


		for (i = 0; i < ReadyQueue->cnt; i++)
		{
			ReadyQueue->processes[i].waitingTime++;
		}

		if (WaitingQueue->cnt >0)
		{
			if (WaitingQueue->processes[0].ioburst != 0)
			{
				WaitingQueue->processes[0].ioburst--;

			}
			if (WaitingQueue->processes[0].ioburst == 0)
			{

				insertQueue(ReadyQueue, pullQueue(WaitingQueue));
				ReadyQueue = SortByRemaining(ReadyQueue);
			}
		}



		if (CPU.remaining == CPU.interrupt)
		{

			insertQueue(WaitingQueue, CPU);
			if (ReadyQueue->cnt > 0)
				CPU = pullQueue(ReadyQueue);
			else
			{
				CPUinit(&CPU);
			}

		}

		if (CPU.remaining > ReadyQueue->processes[0].remaining)
		{
			insertQueue(ReadyQueue, CPU);
			CPU = pullQueue(ReadyQueue);
		}

		if (CPU.remaining == 0)
		{
			CPU.turnaroundTime = time+1 - CPU.arrivalTime;
			if (CPU.pid != 0)
				insertQueue(Terminated, CPU);
			if (ReadyQueue->cnt > 0)
				CPU = pullQueue(ReadyQueue);
			else
				CPUinit(&CPU);
		}
		time++;


	}

	chartdraw(time);
	printf("\n");


	int totwaiting = 0;
	float avgwaiting = 0;
	int totturnaround = 0;
	float avgturnaround = 0;

	int z = 0;
	for (z = 0; z < Terminated->cnt; z++)
	{

		printf("프로세스 %d의 waiting time %d and turnaroundtime %d\n", Terminated->processes[z].pid, Terminated->processes[z].waitingTime, Terminated->processes[z].turnaroundTime);
		totwaiting += Terminated->processes[z].waitingTime;
		totturnaround += (Terminated->processes[z].turnaroundTime - Terminated->processes[z].arrivalTime);
	}

	avgturnaround = (float)totturnaround / Terminated->cnt;
	avgwaiting = (float)totwaiting / Terminated->cnt;

	attcompare[2] = avgturnaround;
	awtcompare[2] = avgwaiting;


}


void prepriority(Queue* queue)
{
	Queue* sorted = NULL;
	Queue* WaitingQueue = createQueue(queue->maxsize);
	Queue* ReadyQueue = createQueue(queue->maxsize);
	Queue* Terminated = createQueue(queue->maxsize);
	Process CPU;
	CPUinit(&CPU);
	int i = 0;
	int r = 0;
	int time = 0;
	int j = 0;
	sorted = Copyqueue(queue);
	sorted = SortByArrivalPid(sorted);
	time = sorted->processes[0].arrivalTime;
	insertQueue(ReadyQueue, pullQueue(sorted));
	for (r = 0; r < time; r++)
	{
		ghanttchart[r] = CPU.pid;
	}


	CPU = pullQueue(ReadyQueue);
	while (Terminated->cnt != Terminated->maxsize)
	{


		while (sorted->cnt > 0)
		{
			if (sorted->processes[0].arrivalTime != time)
				break;
			else
			{
				insertQueue(ReadyQueue, pullQueue(sorted));
				ReadyQueue = SortByPriority(ReadyQueue);
				if (ReadyQueue->processes[0].priority < CPU.priority)
				{
					insertQueue(ReadyQueue, CPU);
					CPU = pullQueue(ReadyQueue);
					ReadyQueue = SortByPriority(ReadyQueue);
				}
			}
		}


		CPU.executed++;
		CPU.remaining--;
		ghanttchart[time] = CPU.pid;
		ReadyQueue = SortByPriority(ReadyQueue);


		for (i = 0; i < ReadyQueue->cnt; i++)
		{
			ReadyQueue->processes[i].waitingTime++;
		}

		if (WaitingQueue->cnt >0)
		{
			if (WaitingQueue->processes[0].ioburst != 0)
			{
				WaitingQueue->processes[0].ioburst--;
			}
			if (WaitingQueue->processes[0].ioburst == 0)
			{

				insertQueue(ReadyQueue, pullQueue(WaitingQueue));
				ReadyQueue = SortByPriority(ReadyQueue);
			}
		}



		if (CPU.remaining == CPU.interrupt)
		{
			insertQueue(WaitingQueue, CPU);
			if (ReadyQueue->cnt > 0)
				CPU = pullQueue(ReadyQueue);
			else
			{
				while (CPU.ioburst > 0)
				{
					time++;
					CPU.pid = 0;
					ghanttchart[time] = CPU.pid;
					CPU.ioburst--;
				}
				insertQueue(ReadyQueue, pullQueue(WaitingQueue));
				CPU = pullQueue(ReadyQueue);

			}

		}

		if (CPU.priority > ReadyQueue->processes[0].priority)
		{
			insertQueue(ReadyQueue, CPU);
			CPU = pullQueue(ReadyQueue);
		}

		if (CPU.remaining == 0)
		{
			CPU.turnaroundTime = time+1 - CPU.arrivalTime;
			insertQueue(Terminated, CPU);
			CPU = pullQueue(ReadyQueue);
		}
		time++;


	}
	int l = 0;
	for (l = 0; l < Terminated->cnt; l++)
		printf("%d  ", Terminated->processes[l].pid);
	printf("\n");
	int k = 0;

	chartdraw(time);
	printf("\n");

	int totwaiting = 0;
	float avgwaiting = 0;
	int totturnaround = 0;
	float avgturnaround = 0;

	int z = 0;
	for (z = 0; z < Terminated->cnt; z++)
	{
		totwaiting += Terminated->processes[z].waitingTime;
		totturnaround += (Terminated->processes[z].turnaroundTime - Terminated->processes[z].arrivalTime);
	}

	avgturnaround = (float)totturnaround / Terminated->cnt;
	avgwaiting = (float)totwaiting / Terminated->cnt;

	attcompare[5] = avgturnaround;
	awtcompare[5] = avgwaiting;
}
void newprepriority(Queue* queue)
{
	Queue* sorted = NULL;
	Queue* WaitingQueue = createQueue(queue->maxsize);
	Queue* ReadyQueue = createQueue(queue->maxsize);
	Queue* Terminated = createQueue(queue->maxsize);
	Process CPU;
	CPUinit(&CPU);
	int i = 0;
	int r = 0;
	int time = 0;
	int j = 0;
	sorted = Copyqueue(queue);
	sorted = SortByArrivalPid(sorted);
	time = sorted->processes[0].arrivalTime;
	insertQueue(ReadyQueue, pullQueue(sorted));
	for (r = 0; r < time; r++)
	{
		ghanttchart[r] = CPU.pid;
	}


	CPU = pullQueue(ReadyQueue);
	while (Terminated->cnt != Terminated->maxsize)
	{


		while (sorted->cnt > 0)
		{
			if (sorted->processes[0].arrivalTime != time)
				break;
			else
			{
				insertQueue(ReadyQueue, pullQueue(sorted));
				ReadyQueue = SortByPriority(ReadyQueue);
				if (ReadyQueue->processes[0].priority < CPU.priority)
				{
					insertQueue(ReadyQueue, CPU);
					CPU = pullQueue(ReadyQueue);
					ReadyQueue = SortByPriority(ReadyQueue);
				}
			}
		}

		if (CPU.pid == 0 && ReadyQueue->cnt > 0)
		{
			CPU = pullQueue(ReadyQueue);
		}

		CPU.executed++;
		CPU.remaining--;
		ghanttchart[time] = CPU.pid;
		ReadyQueue = SortByPriority(ReadyQueue);


		for (i = 0; i < ReadyQueue->cnt; i++)
		{
			ReadyQueue->processes[i].waitingTime++;
		}

		if (WaitingQueue->cnt >0)
		{
			if (WaitingQueue->processes[0].ioburst != 0)
			{
				WaitingQueue->processes[0].ioburst--;
			}
			if (WaitingQueue->processes[0].ioburst == 0)
			{

				insertQueue(ReadyQueue, pullQueue(WaitingQueue));
				ReadyQueue = SortByPriority(ReadyQueue);
			}
		}



		if (CPU.remaining == CPU.interrupt)
		{
			insertQueue(WaitingQueue, CPU);
			if (ReadyQueue->cnt > 0)
				CPU = pullQueue(ReadyQueue);
			else
			{
				CPUinit(&CPU);

			}

		}

		if (CPU.priority > ReadyQueue->processes[0].priority)
		{
			insertQueue(ReadyQueue, CPU);
			CPU = pullQueue(ReadyQueue);
		}

		if (CPU.remaining == 0)
		{
			CPU.turnaroundTime = time+1 - CPU.arrivalTime;
			if (CPU.pid != 0)
				insertQueue(Terminated, CPU);
			if (ReadyQueue->cnt > 0)
				CPU = pullQueue(ReadyQueue);
			else
				CPUinit(&CPU);
		}
		time++;


	}
	int l = 0;
	for (l = 0; l < Terminated->cnt; l++)
		printf("%d  ", Terminated->processes[l].pid);
	printf("\n");
	int k = 0;

	chartdraw(time);
	printf("\n");

	int totwaiting = 0;
	float avgwaiting = 0;
	int totturnaround = 0;
	float avgturnaround = 0;

	int z = 0;
	for (z = 0; z < Terminated->cnt; z++)
	{

		printf("프로세스 %d의 waiting time %d and turnaroundtime %d\n", Terminated->processes[z].pid, Terminated->processes[z].waitingTime, Terminated->processes[z].turnaroundTime);
		totwaiting += Terminated->processes[z].waitingTime;
		totturnaround += (Terminated->processes[z].turnaroundTime - Terminated->processes[z].arrivalTime);
	}

	avgturnaround = (float)totturnaround / Terminated->cnt;
	avgwaiting = (float)totwaiting / Terminated->cnt;

	attcompare[5] = avgturnaround;
	awtcompare[5] = avgwaiting;
}


void roundrobin(Queue* queue)
{
	int timequantum = 0;

	printf("time quantum 설정하세요 (2~8): ");
	scanf("%d", &timequantum);
	while (timequantum<2 || timequantum>8)
	{
		printf("time quantum 다시 설정하세요 (2~8)\n");
		scanf("%d", &timequantum);
	}


	Queue* sorted = NULL;
	Queue* WaitingQueue = createQueue(queue->maxsize);
	Queue* ReadyQueue = createQueue(queue->maxsize);
	Queue* Terminated = createQueue(queue->maxsize);
	Process CPU;
	CPUinit(&CPU);
	int i = 0;
	int r = 0;
	int time = 0;
	int j = 0;
	int timer = 0;
	timer = timequantum;
	sorted = Copyqueue(queue);
	sorted = SortByArrival(sorted);
	time = sorted->processes[0].arrivalTime;
	insertQueue(ReadyQueue, pullQueue(sorted));
	for (r = 0; r < time; r++)
	{
		ghanttchart[r] = CPU.pid;
	}
	CPU = pullQueue(ReadyQueue);
	while (Terminated->cnt != Terminated->maxsize)
	{

		while (sorted->cnt > 0)
		{
			if (sorted->processes[0].arrivalTime != time)
				break;
			else
			{
				insertQueue(ReadyQueue, pullQueue(sorted));
			}
		}

		if (CPU.pid == 0 && ReadyQueue->cnt > 0)
		{
			CPU = pullQueue(ReadyQueue);
			timer = timequantum;
		}

		CPU.executed++;
		CPU.remaining--;
		timer--;
		ghanttchart[time] = CPU.pid;



		for (i = 0; i < ReadyQueue->cnt; i++)
		{
			ReadyQueue->processes[i].waitingTime++;
		}

		if (CPU.remaining == CPU.interrupt)
		{

			insertQueue(WaitingQueue, CPU);

			if (ReadyQueue->cnt > 0)
			{
				CPU = pullQueue(ReadyQueue);
				timer = timequantum;
			}
			else
			{
				CPU.pid = 0;
			}


		}
		if (CPU.remaining == 0)
		{
			CPU.turnaroundTime = time+1 - CPU.arrivalTime;
			insertQueue(Terminated, CPU);
			CPU = pullQueue(ReadyQueue);
			timer = timequantum;

		}

		if (WaitingQueue->cnt > 0)
		{
			if (WaitingQueue->processes[0].ioburst == 0)
			{

				insertQueue(ReadyQueue, pullQueue(WaitingQueue));
				if (Terminated->cnt == (Terminated->maxsize) - 1)
				{
					CPU = pullQueue(ReadyQueue);
					timer = timequantum;
				}
			}
			if (WaitingQueue->processes[0].ioburst != 0)
			{
				WaitingQueue->processes[0].ioburst--;
			}

		}


		if (timer == 0)
		{

			if (CPU.pid != 0 && ReadyQueue->cnt > 0)
			{
				insertQueue(ReadyQueue, CPU);
				if (sorted->cnt > 0 && ((time + 1) == sorted->processes[0].arrivalTime))
				{
					insertQueue(ReadyQueue, pullQueue(sorted));
				}
				CPU = pullQueue(ReadyQueue);
				timer = timequantum;
			}
			else if (CPU.pid != 0 && ReadyQueue->cnt == 0)
			{

				if (sorted->cnt > 0 && ((time + 1) == sorted->processes[0].arrivalTime))
				{

					insertQueue(ReadyQueue, pullQueue(sorted));
					insertQueue(ReadyQueue, CPU);
					CPU = pullQueue(ReadyQueue);
					timer = timequantum;
				}
				timer = timequantum;
			}

			else if (CPU.pid == 0 && ReadyQueue->cnt > 0)
			{
				CPU = pullQueue(ReadyQueue);
				timer = timequantum;
			}
			else
			{
				CPU.pid = 0;
				timer = timequantum;
				if (sorted->cnt > 0 && ((time + 1) == sorted->processes[0].arrivalTime))
				{
					insertQueue(ReadyQueue, pullQueue(sorted));
					CPU = pullQueue(ReadyQueue);

				}
			}
		}

		time++;

	}
	chartdraw(time);
	printf("\n");



	int totwaiting = 0;
	float avgwaiting = 0;
	int totturnaround = 0;
	float avgturnaround = 0;

	int z = 0;
	for (z = 0; z < Terminated->cnt; z++)
	{
		totwaiting += Terminated->processes[z].waitingTime;
		totturnaround += (Terminated->processes[z].turnaroundTime - Terminated->processes[z].arrivalTime);
	}

	avgturnaround = (float)totturnaround / Terminated->cnt;
	avgwaiting = (float)totwaiting / Terminated->cnt;

	attcompare[1] = avgturnaround;
	awtcompare[1] = avgwaiting;
}
void sortnewroundrobin(Queue* queue)
{
	int timequantum = 0;

	printf("time quantum 설정하세요 (2~8): ");
	scanf("%d", &timequantum);
	while (timequantum<2 || timequantum>8)
	{
		printf("time quantum 다시 설정하세요 (2~8)\n");
		scanf("%d", &timequantum);
	}


	Queue* sorted = NULL;
	Queue* WaitingQueue = createQueue(queue->maxsize);
	Queue* ReadyQueue = createQueue(queue->maxsize);
	Queue* Terminated = createQueue(queue->maxsize);
	Process CPU;
	CPUinit(&CPU);
	int i = 0;
	int r = 0;
	int time = 0;
	int j = 0;
	int timer = 0;
	timer = timequantum;
	sorted = Copyqueue(queue);
	sorted = SortByArrival(sorted);
	time = sorted->processes[0].arrivalTime;
	insertQueue(ReadyQueue, pullQueue(sorted));
	for (r = 0; r < time; r++)
	{
		ghanttchart[r] = CPU.pid;
	}
	CPU = pullQueue(ReadyQueue);
	while (Terminated->cnt != Terminated->maxsize)
	{

		while (sorted->cnt > 0)
		{
			if (sorted->processes[0].arrivalTime != time)
				break;
			else
			{
				insertQueue(ReadyQueue, pullQueue(sorted));
				ReadyQueue = SortByArrival(ReadyQueue);
			}
		}
		if (CPU.pid == 0 && ReadyQueue->cnt > 0)
		{
			CPU = pullQueue(ReadyQueue);
			timer = timequantum;
		}




		for (i = 0; i < ReadyQueue->cnt; i++)
		{
			ReadyQueue->processes[i].waitingTime++;
		}

		CPU.executed++;
		CPU.remaining--;
		timer--;
		ghanttchart[time] = CPU.pid;

		if (CPU.remaining == CPU.interrupt)
		{

			insertQueue(WaitingQueue, CPU);

			if (ReadyQueue->cnt > 0)
			{
				CPU = pullQueue(ReadyQueue);
				timer = timequantum;
			}
			else
			{
				CPUinit(&CPU);
				timer = timequantum;
			}


		}

		if (CPU.remaining == 0)
		{
			CPU.turnaroundTime = time+1 - CPU.arrivalTime;
			if (CPU.pid != 0)
				insertQueue(Terminated, CPU);
			if (ReadyQueue->cnt == 0)
			{
				CPUinit(&CPU);
				timer = timequantum;
			}
			else
			{
				CPU = pullQueue(ReadyQueue);
				timer = timequantum;
			}

		}

		if (WaitingQueue->cnt > 0)
		{
			if (WaitingQueue->processes[0].ioburst == 0)
			{

				insertQueue(ReadyQueue, pullQueue(WaitingQueue));
				if (Terminated->cnt == (Terminated->maxsize) - 1)
				{
					CPU = pullQueue(ReadyQueue);
					timer = timequantum;
				}
			}
			if (WaitingQueue->processes[0].ioburst != 0)
			{
				WaitingQueue->processes[0].ioburst--;
			}

		}


		if (timer == 0)
		{

			if (CPU.pid != 0 && ReadyQueue->cnt > 0)
			{
				insertQueue(ReadyQueue, CPU);
				CPU = pullQueue(ReadyQueue);
				timer = timequantum;
			}
			else if (CPU.pid != 0 && ReadyQueue->cnt == 0)
			{
				timer = timequantum;
			}

			else if (CPU.pid == 0 && ReadyQueue->cnt > 0)
			{
				CPU = pullQueue(ReadyQueue);
				timer = timequantum;
			}
			else
			{
				CPUinit(&CPU);
				timer = timequantum;
			}
		}

		ReadyQueue = SortByArrival(ReadyQueue);
		time++;
	}

	chartdraw(time);
	printf("\n");



	int totwaiting = 0;
	float avgwaiting = 0;
	int totturnaround = 0;
	float avgturnaround = 0;

	int z = 0;
	for (z = 0; z < Terminated->cnt; z++)
	{

		printf("%d의 waiting time %d and turnaroundtime %d\n", Terminated->processes[z].pid, Terminated->processes[z].waitingTime, Terminated->processes[z].turnaroundTime);
		totwaiting += Terminated->processes[z].waitingTime;
		totturnaround += (Terminated->processes[z].turnaroundTime - Terminated->processes[z].arrivalTime);
	}

	avgturnaround = (float)totturnaround / Terminated->cnt;
	avgwaiting = (float)totwaiting / Terminated->cnt;

	attcompare[1] = avgturnaround;
	awtcompare[1] = avgwaiting;
}
void newroundrobin(Queue* queue)
{
	int timequantum = 0;

	printf("time quantum 설정하세요 (2~8): ");
	scanf("%d", &timequantum);
	while (timequantum<2 || timequantum>8)
	{
		printf("time quantum 다시 설정하세요 (2~8)\n");
		scanf("%d", &timequantum);
	}


	Queue* sorted = NULL;
	Queue* WaitingQueue = createQueue(queue->maxsize);
	Queue* ReadyQueue = createQueue(queue->maxsize);
	Queue* Terminated = createQueue(queue->maxsize);
	Process CPU;
	CPUinit(&CPU);
	int i = 0;
	int r = 0;
	int time = 0;
	int j = 0;
	int timer = 0;
	timer = timequantum;
	sorted = Copyqueue(queue);
	sorted = SortByArrival(sorted);
	time = sorted->processes[0].arrivalTime;
	insertQueue(ReadyQueue, pullQueue(sorted));
	for (r = 0; r < time; r++)
	{
		ghanttchart[r] = CPU.pid;
	}
	CPU = pullQueue(ReadyQueue);
	while (Terminated->cnt != Terminated->maxsize)
	{

		while (sorted->cnt > 0)
		{
			if (sorted->processes[0].arrivalTime != time)
				break;
			else
			{
				insertQueue(ReadyQueue, pullQueue(sorted));
			}
		}
		if (CPU.pid == 0 && ReadyQueue->cnt > 0)
		{
			CPU = pullQueue(ReadyQueue);
			timer = timequantum;
		}




		for (i = 0; i < ReadyQueue->cnt; i++)
		{
			ReadyQueue->processes[i].waitingTime++;
		}

		CPU.executed++;
		CPU.remaining--;
		timer--;
		ghanttchart[time] = CPU.pid;

		if (CPU.remaining == CPU.interrupt)
		{

			insertQueue(WaitingQueue, CPU);

			if (ReadyQueue->cnt > 0)
			{
				CPU = pullQueue(ReadyQueue);
				timer = timequantum;
			}
			else
			{
				CPUinit(&CPU);
				timer = timequantum;
			}


		}

		if (CPU.remaining == 0)
		{
			CPU.turnaroundTime = time+1-CPU.arrivalTime;
			if (CPU.pid != 0)
				insertQueue(Terminated, CPU);
			if (ReadyQueue->cnt == 0)
			{
				CPUinit(&CPU);
				timer = timequantum;
			}
			else
			{
				CPU = pullQueue(ReadyQueue);
				timer = timequantum;
			}

		}

		if (WaitingQueue->cnt > 0)
		{
			if (WaitingQueue->processes[0].ioburst == 0)
			{

				insertQueue(ReadyQueue, pullQueue(WaitingQueue));
				if (Terminated->cnt == (Terminated->maxsize) - 1)
				{
					CPU = pullQueue(ReadyQueue);
					timer = timequantum;
				}
			}
			if (WaitingQueue->processes[0].ioburst != 0)
			{
				WaitingQueue->processes[0].ioburst--;
			}

		}


		if (timer == 0)
		{

			if (CPU.pid != 0 && ReadyQueue->cnt > 0)
			{
				insertQueue(ReadyQueue, CPU);
				CPU = pullQueue(ReadyQueue);
				timer = timequantum;
			}
			else if (CPU.pid != 0 && ReadyQueue->cnt == 0)
			{
				timer = timequantum;
			}

			else if (CPU.pid == 0 && ReadyQueue->cnt > 0)
			{
				CPU = pullQueue(ReadyQueue);
				timer = timequantum;
			}
			else
			{
				CPUinit(&CPU);
				timer = timequantum;
			}
		}

		time++;
	}

	chartdraw(time);
	printf("\n");



	int totwaiting = 0;
	float avgwaiting = 0;
	int totturnaround = 0;
	float avgturnaround = 0;

	int z = 0;
	for (z = 0; z < Terminated->cnt; z++)
	{

		printf("프로세스 %d의 waiting time %d and turnaroundtime %d\n", Terminated->processes[z].pid, Terminated->processes[z].waitingTime, Terminated->processes[z].turnaroundTime);
		totwaiting += Terminated->processes[z].waitingTime;
		totturnaround += (Terminated->processes[z].turnaroundTime - Terminated->processes[z].arrivalTime);
	}

	avgturnaround = (float)totturnaround / Terminated->cnt;
	avgwaiting = (float)totwaiting / Terminated->cnt;

	attcompare[1] = avgturnaround;
	awtcompare[1] = avgwaiting;
}
void comparison(Queue* newQueue)
{

	printf("FCFS 알고리즘\n");
	newfcfs(newQueue);
	printf("PRIORITY 알고리즘\n");
	priority(newQueue);
	printf("SJF 알고리즘\n");
	newsjf(newQueue);
	printf("선점형 SJF 알고리즘\n");
	newpresjf(newQueue);
	printf("선점형 PRIORITY 알고리즘\n");
	newprepriority(newQueue);
	printf("ROUND ROBIN 알고리즘\n");
	newroundrobin(newQueue);



	int a = 0;
	printf("----#----대기시간----실행완료까지--\n");
	for (a = 0; a < 6; a++)
	{
		printf("|   %d |      %.2f |         %.2f|\n", a + 1, awtcompare[a], attcompare[a]);

	}
	printf("-----------------------------------\n");

	int which = 0;
	int which1 = 0;
	float awtmin = awtcompare[0];
	float attmin = attcompare[0];
	for (a = 0; a < 6; a++)
	{
		if (awtcompare[a] < awtmin)
		{
			awtmin = awtcompare[a];
			which = a + 1;
		}
		if (attcompare[a] < attmin)
		{
			attmin = attcompare[a];
			which1 = a + 1;
		}
	}

	printf("%d번 알고리즘이 %f초로 가장 적은 평균 waitingtime을 보임\n", which, awtmin);
	printf("%d번 알고리즘이 %f초로 가장 적은 평균 aturnaround을 보임\n", which1, attmin);

}

void select(Queue* newQueue)
{

	int algselect = 0;

	while (1)
	{
		do
		{
			printf("알고리즘을 선택하세요\n");
			printf(" 1: fcfs\n 2: rr\n 3: pre-sjf\n 4: sjf\n 5: priority\n 6: pre-priority\n 7: evaluation\n 8: exit");
			scanf("%d", &algselect);
		} while (algselect > 8 || algselect < 1);

		if (algselect == 1)
			newfcfs(newQueue);
		printf("\n");
		if (algselect == 2)
			newroundrobin(newQueue);
		printf("\n");
		if (algselect == 3)
			newpresjf(newQueue);
		printf("\n");
		if (algselect == 4)
			newsjf(newQueue);
		printf("\n");
		if (algselect == 5)
			priority(newQueue);
		printf("\n");
		if (algselect == 6)
			newprepriority(newQueue);
		printf("\n");
		if (algselect == 7)
			comparison(newQueue);
		printf("\n");
		if (algselect == 8)
			return;
	}
}

int main(void)
{

	Queue* newQueue = NULL;
	newQueue = createJobQueue();
	printqueue(newQueue);
	select(newQueue);

	return 0;

}