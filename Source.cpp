#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<fstream>
#include<iostream>

using namespace std;

#define MAX_SIZE 1000

char inputString[MAX_SIZE];
int process_seq[200];
int seq = 0;

void printResult(int pnum, int *processID, int *arrivalTime, int *cpuBurst, int *finish, int *turnaroundTime, int *waitingTime) {
	int i, sum_tt = 0, sum_wt = 0;
	int temp;
	int process[100];
	
	for (i = 0; i < pnum; i++) {
		// ��� ���� ���
		finish[i] = turnaroundTime[i] + arrivalTime[i];
		process[i] = processID[i];  // ������ ���̵� ����

		sum_tt += turnaroundTime[i];
		sum_wt += waitingTime[i];
	}
	for (int i = 0; i < pnum; i++){
		for (int j = i + 1; j < pnum; j++){
			if (processID[i] > processID[j]){  // ���μ��� ���̵� ���� ���Ͽ� ����
				temp = processID[i];
				processID[i] = processID[j];
				processID[j] = temp;
				temp = arrivalTime[i];
				arrivalTime[i] = arrivalTime[j];
				arrivalTime[j] = temp;
				temp = cpuBurst[i];
				cpuBurst[i] = cpuBurst[j];
				cpuBurst[j] = temp;
				temp = finish[i];
				finish[i] = finish[j];
				finish[j] = temp;
				temp = turnaroundTime[i];
				turnaroundTime[i] = turnaroundTime[j];
				turnaroundTime[j] = temp;
				temp = waitingTime[i];
				waitingTime[i] = waitingTime[j];
				waitingTime[j] = temp;
			}
		}
		printf("%d %d %d %d %d %d\n", processID[i], arrivalTime[i], cpuBurst[i], finish[i], turnaroundTime[i], waitingTime[i]);
	}

	cout << (float)sum_tt / pnum << endl;
	cout << (float)sum_wt / pnum << endl;  // ��հ� ���

	if (seq != 0)
	{
		for (i = 0; i < seq; i++)
		{
			cout << process_seq[i] << " ";
		}
	}
	else {
		for (i = 0; i < pnum; i++) {
			cout << process[i] << " ";
		}
	}
	cout << endl;
}

void FCFS(int pnum, int *processID, int *arrivalTime, int *cpuBurst, int *turnaroundTime, int *waitingTime) {
	// FCFS ó��
	//int i;
	int time = 0;

	cout << "FCFS Scheduling Algorithm" << endl;

	for (int i = 0; i < pnum; time = time + cpuBurst[i], i++) {
		if (i == 0) {
			turnaroundTime[i] = cpuBurst[i];
			continue;
		}
		if (arrivalTime[i] < time) {
			waitingTime[i] = time - arrivalTime[i];
			turnaroundTime[i] = cpuBurst[i] + waitingTime[i];
		}
		else if (arrivalTime[i] > time) {
			time += arrivalTime[i] - time;
			turnaroundTime[i] = cpuBurst[i];
			waitingTime[i] = 0;
		}
	}
}

void Information(int info[], int from, int to, int process)
{
	int i;
	for (i = from; i <= to; i++)
		info[i] = process;
}
int Rearrange_process_queue(int pq[], int n, int running_processes)
{
	int i;
	int temp = pq[0];
	for (i = 0; i<running_processes; i++)
	{
		pq[i] = pq[i + 1];
	}
	pq[running_processes - 1] = temp;

	return pq[running_processes - 1];
}

void RoundRobin(int pnum, int *processID, int *arrivalTime, int *cpuBurst, int *turnaroundTime, int *waitingTime){

	int count, i, j, flag, current = 0;
	int running_processes = 0;
	int time_quantum; 
	int time = 0; 
	int checkTime = 0; // ó���� �ð�
	int info[200], remainingTime[100], pq[100], pflag[100];

	time_quantum = 2;  // Time Quantum�� 2�� ����

	cout << "Round-Robin Scheduling Algorithm" << endl;
	
	if (pnum <= 0 || time_quantum <= 0)
	{
		printf("ó���� �����Ͱ� �����ϴ�.");
		return;
	}
	
	for (j = 0; j<pnum; j++)
	{
		// process queue �ʱ�ȭ
		pq[j] = 0;
		pflag[j] = 0;
	}
	
	for (count = 0; count<pnum; count++)
	{
		remainingTime[count] = cpuBurst[count];
		checkTime += cpuBurst[count];
		
		if (arrivalTime[count] < 0 || cpuBurst[count] <= 0)
		{
			printf("ó���� �����Ͱ� �����ϴ�!");
			return;
		}
	}
	
	for (i = 0; i < 200; i++)
	{
		info[i] = 0;  // �ʱ�ȭ
		process_seq[i] = -1; // �ʱ�ȭ
	}
	
	pq[0] = 1;
	running_processes = 1;
	pflag[0] = 1;
	flag = 0;
	
	while (running_processes != 0)
	{
		flag = 0;				  
		if (remainingTime[pq[0] - 1]>time_quantum)
		{
			remainingTime[pq[0] - 1] = remainingTime[pq[0] - 1] - time_quantum;
			time = time + time_quantum;
			Information(info, current, time, pq[0]);
			current = time;
		}
		else
		{
			time = time + remainingTime[pq[0] - 1];
			remainingTime[pq[0] - 1] = 0;
			flag = 1;
			Information(info, current, time, pq[0]);
			current = time;
			
			turnaroundTime[pq[0] - 1] = time - arrivalTime[pq[0] - 1];  // TurnAround Time ó��
			waitingTime[pq[0] - 1] = turnaroundTime[pq[0] - 1] - cpuBurst[pq[0] - 1];  // Waiting Time ó��
		}
		for (i = 0; i<pnum; i++)
		{
			if (arrivalTime[i] <= time && pflag[i] == 0)
			{
				pq[running_processes] = i + 1;
				running_processes = running_processes + 1;
				pflag[i] = 1;
			}
		}
		
		process_seq[seq] = Rearrange_process_queue(pq, pnum, running_processes);  // �ϳ��� ���μ��� ���� ��� �迭�� ����
		seq++;
		// �������� process�� �������� Ȯ��
		if (flag == 1)
			running_processes = running_processes - 1;
	}
}

void PriorityNonPreemption(int pnum, int *processID, int *arrivalTime, int *cpuBurst, int *turnAroundTime, int *waitingTime, int *priority){

	int i, j, limit, sum = 0, position, temp;
	limit = pnum;

	cout << "Priority Non-Preemptive Scheduling Algorithm" << endl;
	for (i = 0; i < limit; i++){
		position = i;
		for (j = i + 1; j < limit; j++){
			if (priority[j] < priority[position]){
				position = j;
			}
		}
		temp = priority[i];
		priority[i] = priority[position];
		priority[position] = temp;

		temp = cpuBurst[i];
		cpuBurst[i] = cpuBurst[position];
		cpuBurst[position] = temp;

		temp = processID[i];
		processID[i] = processID[position];
		processID[position] = temp;

		temp = arrivalTime[i];  // arrival time�� ����
		arrivalTime[i] = arrivalTime[position];
		arrivalTime[position] = temp;
	}
	waitingTime[0] = 0;
	for (i = 1; i < limit; i++){
		waitingTime[i] = 0;
		for (j = 0; j < i; j++){
			waitingTime[i] = waitingTime[i] + cpuBurst[j]; // waiting Time ����
		}
		waitingTime[i] = waitingTime[i] - arrivalTime[i];
	}
	for (i = 0; i < limit; i++){
		turnAroundTime[i] = cpuBurst[i] + waitingTime[i];  // turnAround Time ����
	}
}

void PriorityPreemption(int pnum, int *processID, int *arrivalTime, int *cpuBurst, int *turnAroundTime, int *waitingTime, int *priority){
	
	int processNum, i,j,temp;
	processNum = pnum;

	cout << "Priority Preemptive Scheduling Alogorithm" << endl;
	for (i = 0; i < processNum; i++){
		for (j = i + 1; j < processNum; j++){
			if(priority[i] > priority[j]){
				temp = priority[i];
				priority[i] = priority[j];
				priority[j] = temp;
				temp = cpuBurst[i];
				cpuBurst[i] = cpuBurst[j];
				cpuBurst[j] = temp;
				temp = processID[i];
				processID[i] = processID[j];
				processID[j] = temp;
				temp = arrivalTime[i];  // arrival time ����
				arrivalTime[i] = arrivalTime[j];
				arrivalTime[j] = temp;
			}
		}
		waitingTime[i] = 0;
	}
	for (i = 0; i < processNum; i++){
		for (j = 0; j < i; j++){
			waitingTime[i] += cpuBurst[j];
		}
		waitingTime[i] = waitingTime[i] - arrivalTime[i];
		turnAroundTime[i] = waitingTime[i] + cpuBurst[i];
	}
}

int main(int argc, char* argv[] ) {   // argv[i]���� string 

	int pnum = 0;
	int i;
	int *processID, *arrivalTime, *priority, *cpuBurst, *turnAroundTime, *waitingTime, *finish;
	int type = 9;

	if (argc > 2){

		//���� ��� (�б�)
		ifstream inFile(argv[1]);
		//4��° ���ڿ��� �޾Ƽ� ������ ��ȯ
		type = atoi(argv[2]);

		while (!inFile.eof()) {  // ���̻� ������ ������ ����

			inFile.getline(inputString, 100);

			pnum++;// ���μ��� ���� 4
		}
		//cout << "Process Num: " << pnum << endl;
		inFile.close();

		FILE *fp = fopen(argv[1], "r");

		processID = (int*)calloc(pnum, sizeof(int));
		arrivalTime = (int*)calloc(pnum, sizeof(int));
		cpuBurst = (int*)calloc(pnum, sizeof(int));
		priority = (int*)calloc(pnum, sizeof(int));
		turnAroundTime = (int*)calloc(pnum, sizeof(int));
		waitingTime = (int*)calloc(pnum, sizeof(int));
		finish = (int*)calloc(pnum, sizeof(int));

		if (fp == NULL){
			printf("File Open ERROR");
		}
		else{

			for (i = 0; i < pnum; ++i) {  // ���Ͽ� �ִ� ���� ��� ����
				fscanf(fp, "%d %d %d %d", &processID[i], &arrivalTime[i], &cpuBurst[i], &priority[i]);
				//printf("%d %d %d %d\n", processID[i], arrivalTime[i], cpuBurst[i], priority[i]);
				turnAroundTime[i] = 0;
				waitingTime[i] = 0; // �ʱ�ȭ
			}
		}
		fclose(fp);


		if (type == 0) { // FCFS�� ���
		    FCFS(pnum, processID, arrivalTime, cpuBurst, turnAroundTime, waitingTime);
		}
		else if (type == 1) { // Round-Robin�� ���(Time quantum size:2)
		    RoundRobin(pnum, processID, arrivalTime, cpuBurst, turnAroundTime, waitingTime);
		}
		else if (type == 2) { // Priority Non-Preemptive�� ���
			PriorityNonPreemption(pnum, processID, arrivalTime, cpuBurst, turnAroundTime, waitingTime, priority);
		}
		else if (type == 3) {  // Priority Preemptive �� ���
			PriorityPreemption(pnum, processID, arrivalTime, cpuBurst, turnAroundTime, waitingTime, priority);
		}
		else {
		cout << "�˰����� ������ �ּ���!" << endl;
		}
		
		printResult(pnum, processID, arrivalTime, cpuBurst, finish, turnAroundTime, waitingTime);
		memset(turnAroundTime, 0, sizeof(int)*pnum);
		memset(waitingTime, 0, sizeof(int)*pnum);

	}
	else{   // �Է� ���� ���
		cout << "�߸� �Է� �ϼ̽��ϴ�." << endl;
	}

	system("pause");

	return 0;

}