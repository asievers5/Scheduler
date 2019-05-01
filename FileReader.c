#include <stdio.h>

int loadJobs(char *filename);

struct JobStruct {
    int pId, arrival, cpuBurst, priority, status;
};

int loadJobs(char *filename)
{
	FILE *jobs;
	char string[80];
	int pId, arrival, cpuBurst, priority;
	int j;
	int nJobs;

    struct JobStruct newQ[99];

	/* Open file of jobs to be put in the ready que. */
	jobs = fopen(filename, "r");

	/* Load the ready que from the file. */
	//fgets(string, 80, jobs);
	//printf("%s \n", string);
	j= 0;
	while(fscanf(jobs, "%d %d %d %d", &pId, &arrival, &cpuBurst, &priority) != EOF) {
		newQ[j].pId = pId;
		newQ[j].arrival = arrival;
		newQ[j].cpuBurst = cpuBurst;
		newQ[j].priority = priority;
		//newQ[j].status = NEW;
		printf("%d		%d	%d		%d \n", newQ[j].pId, newQ[j].arrival, newQ[j].cpuBurst, 
			                                newQ[j].priority);
		j = j+1;
	}

    
	nJobs = j;
	printf("\n");
	printf("number of jobs in newQ = %d \n", nJobs);
	fclose(jobs);

	return nJobs;
}
