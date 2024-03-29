#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>

void firstComeFirstServe(int ProcessId[], int Arrival[], int cpuBurst[], int Priority[]);
void shortestJobFirst(int ProcessId[], int Arrival[], int cpuBurst[], int Priority[]);
void roundRobin(int ProcessId[], int Arrival[], int cpuBurst[], int Priority[]);

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

		//redefine process
	struct Job{
		int ProcessId;
		int Arrival;
		int cpuBurst;
		int Priority;
		bool completed;
		bool hasArrived;
		int startTime;
		int completionTime;
		int started;
		int timeleft;
		int count;
	};

int main()
{
	struct Process processList[7];
	int nJobs = 7;

	// Example input until file reading is implemented
	int ProcessId[nJobs];
	int Arrival[nJobs];
	int cpuBurst[nJobs];
	int Priority[nJobs];
	
	//File Reading
	FILE *jobs;
	char string[80];
	int fpId, farrival, fcpuBurst, fpriority;
	int j;
	
	/* Open file of jobs to be put in the ready que. */
	jobs = fopen("joblist.txt", "r");
	while(fscanf(jobs, "%d %d %d %d", &fpId, &farrival, &fcpuBurst, &fpriority) != EOF) {
		ProcessId[j]= fpId;
		Arrival[j] = farrival;
		cpuBurst[j] = fcpuBurst;
		Priority[j] = fpriority;
		j = j+1;
	}
	
	fclose(jobs);

	firstComeFirstServe(ProcessId, Arrival, cpuBurst, Priority);
	shortestJobFirst(ProcessId, Arrival, cpuBurst, Priority);
    roundRobin(ProcessId, Arrival, cpuBurst, Priority);
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
	printf("Average response time = %.2f\n", art);
	printf("Throughput: %.2f\n", throughput);
}

void roundRobin(int ProcessId[], int Arrival[], int cpuBurst[], int Priority[]){
	//change quanta for fun
	int quanta = 15;
	double throughput, att, art;
	int runtime, counter, shortestRuntimeIndex;
	int currentTime = 0;
	int i = 0;
	int length = 7;
	int currentJobIndex = 0;
	bool allProcessesRun = false;
    struct Job job[7];

	//  Assign initial structure properties
	for(i = 0; i < length; i++){
		job[i].ProcessId = ProcessId[i];
		job[i].Arrival = Arrival[i];
		job[i].cpuBurst = cpuBurst[i];
		job[i].Priority = Priority[i];
		job[i].completed = false;
		job[i].hasArrived = false;
		job[i].timeleft = cpuBurst[i];
		job[i].count  = 0;
		job[i].startTime = -1;
	}

	// Print Header
	printf("ROUND ROBIN\n");
	printf("pid\tarrival\tcompletion\n");

	currentJobIndex = 0;
	while((!allProcessesRun)){
		//  Find job to run
		for(i = 0; i < length; i++){
			if(job[i].count  < job[currentJobIndex].count) {
				if((!job[i].completed) && (job[i].hasArrived))
				currentJobIndex = i;
			}
		}

		//set start time if not run before
		if(job[currentJobIndex].startTime == -1){
			job[currentJobIndex].startTime = currentTime;
		}


		//  Set runtime to quanta OR .timeleft (whichever is lower)
		if(job[currentJobIndex].timeleft < quanta){
			runtime = job[currentJobIndex].timeleft;
		}
		else runtime = quanta;

		//  Reduce job.timeleft
		//  If timeleft == 0 
		//  	1. it marks it as completed
		//		2. set completionTime
		//      3. print detail line
		job[currentJobIndex].timeleft -= runtime;
		job[currentJobIndex].count += 1;
		if(job[currentJobIndex].timeleft == 0){
			job[currentJobIndex].completed = true;
			job[currentJobIndex].completionTime = currentTime + runtime;
			job[currentJobIndex].count = 9999999;
			printf("%d\t%d\t%d\n", job[currentJobIndex].ProcessId, job[currentJobIndex].Arrival, job[currentJobIndex].completionTime);
		}

		//  Update time
		currentTime += runtime;

		//  Update hasArrived
		for(i = 0; i < length; i++){
			if(job[i].Arrival <= currentTime){
				job[i].hasArrived = true;
			}
		}

		//check if all jobs completed
		// if all jobs not completed, it sets allProcessesRun to false
		allProcessesRun = true;
		for(i = 0; i < length; i++){
			if(!job[i].completed)
			allProcessesRun = false;
		}
	}

	// print stats
		//Calc throughput
	throughput = currentTime / 7.0;

	//Calculate ATT
	for(i = 0; i < length; i++){
		att += job[i].completionTime - job[i].Arrival;
	}
	att = att / 7.0;
	printf("Turnaround time: %.2f\n", att);

	// Calculate art
	for(i = 0; i < length; i++){
		art += job[i].completionTime - job[i].startTime;
	}
	art = art / (double)length;
	printf("Average response time = %.2f\n", art);
	printf("Throughput: %.2f\n\n", throughput);
}

