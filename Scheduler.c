#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>

void firstComeFirstServe(int ProcessId[], int Arrival[], int cpuBurst[], int Priority[]);
void shortestJobFirst(int ProcessId[], int Arrival[], int cpuBurst[], int Priority[]);

	struct Process{
		int ProcessId;
		int Arrival;
		int cpuBurst;
		int Priority;
		bool hasRun;
		bool hasArrived;
		int completion;
		int started;
		};

	struct Stats{
		int pid;
		int arrival;
		int completion;
	};

int main()
{

	struct Process processList[7];

	// Example input until file reading is implemented
	int ProcessId[] = {100, 101, 102, 103, 104, 105, 106};
	int Arrival[] = {0, 6, 8, 12, 19, 30, 35};
	int cpuBurst[] = {10, 10, 4, 20, 15, 5, 10};
	int Priority[] = {1,1,1,1,1,1,1,};

	firstComeFirstServe(ProcessId, Arrival, cpuBurst, Priority);
	shortestJobFirst(ProcessId, Arrival, cpuBurst, Priority);
	return 0;
}

// First Come First Serve
void firstComeFirstServe(int ProcessId[], int Arrival[], int cpuBurst[], int Priority[]){
	int length = 7;
	struct Stats process = {.pid = ProcessId[0], .arrival = Arrival[0], .completion = 0};
	double turnaround[length];
	double att = 0;
	double response[length];
	double art = 0;

	printf("ProcessID\tArrival\tcpuBurst\tPriority\n");
	int i;
	for(i = 0; i < length; i++){
		printf("%d\t\t%d\t%d\t\t%d\n", ProcessId[i], Arrival[i], cpuBurst[i], Priority[i]);
	}

	// Table header
	printf("First come first serve");
	printf("\npid\tarrival\tcompletion\t\n");

	for(i = 0; i < length; i++){
		process.pid = ProcessId[i];
		process.arrival = Arrival[i];
		response[i] = process.completion - process.arrival;
		process.completion += cpuBurst[i];
		turnaround[i] = (process.completion - process.arrival);
		printf("%d\t%d\t%d\n", process.pid, process.arrival, process.completion);
	}

	// RUNSTATS
	// Calculate throughput
	double throughput = process.completion / length;
	printf("Throughput = %.2f\n", throughput);
	
	// Calculate att
	for(i = 0; i < length; i++){
		att += turnaround[i];
	}
	att = att / (double)length;
	printf("Average turnaround time = %.2f\n", att);

	// Calculate art
	for(i = 0; i < length; i++){
		art += response[i];
	}
	art = art / (double)length;
	printf("Average respones time = %.2f\n", art);

	printf("____________________________________\n");
}

void shortestJobFirst(int ProcessId[], int Arrival[], int cpuBurst[], int Priority[]){
	int length = 7;
	int i, z;
	struct Process job[7];
	int time = 0;
	int shortestJobIndex = 0;
	int shortestJob = 999;
	int burst = 0;
	bool foundOne = false;
	double turnaround[length];
	double att = 0;
	double response[length];
	double art = 0;
	double throughput;

	//header
	printf("shortest job first\n");
	printf("pid\tarrival\tcompletion\n");

	for(i = 0; i < length; i++){
		job[i].ProcessId = ProcessId[i];
		job[i].Arrival = Arrival[i];
		job[i].cpuBurst = cpuBurst[i];
		job[i].Priority = Priority[i];
		job[i].hasRun = false;
		job[i].hasArrived = false;
	}

	for(z = 0; z < length; z++){
	// update arrival
		for(i = 0; i < length; i++){
			if((job[i].Arrival <= time) && !(job[i].hasRun)){
				job[i].hasArrived = true;
			}
		}

		shortestJob = 999;
		for(i = 0; i < length; i++){
			if((job[i].hasArrived) && (!job[i].hasRun)){
				if(job[i].cpuBurst < shortestJob){
					shortestJobIndex = i;
					shortestJob = job[i].cpuBurst;
				}
			}
		}
		job[shortestJobIndex].started = time;
		time += job[shortestJobIndex].cpuBurst;
		job[shortestJobIndex].hasRun = true;
		job[shortestJobIndex].completion = time;
		printf("%d\t%d\t%d\n", job[shortestJobIndex].ProcessId, job[shortestJobIndex].Arrival, time);
	}
	//Calc throughput
	throughput = time / 7.0;

	//Calculate ATT
	for(i = 0; i < length; i++){
		att += job[i].completion - job[i].Arrival;
	}
	att = att / 7.0;
	printf("Turnaround time: %.2f\n", att);

	// Calculate art
	for(i = 0; i < length; i++){
		art += job[i].completion - job[i].started;
	}
	art = art / (double)length;
	printf("Average respones time = %.2f\n", art);
	printf("Throughput: %.2f\n", throughput);
}


/*	
struct Process{
		int ProcessId;
		int Arrival;
		int cpuBurst;
		int Priority;
		bool hasRun;
		bool hasArrived;
		};
*/
