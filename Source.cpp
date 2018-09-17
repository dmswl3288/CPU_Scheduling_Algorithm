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
		// 결과 정보 출력
		finish[i] = turnaroundTime[i] + arrivalTime[i];
		process[i] = processID[i];  // 정렬전 아이디 저장

		sum_tt += turnaroundTime[i];
		sum_wt += waitingTime[i];
	}
	for (int i = 0; i < pnum; i++){
		for (int j = i + 1; j < pnum; j++){
			if (processID[i] > processID[j]){  // 프로세스 아이디만 순서 비교하여 정렬
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
	cout << (float)sum_wt / pnum << endl;  // 평균값 출력

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
	// FCFS 처리
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
	int checkTime = 0; // 처리된 시간
	int info[200], remainingTime[100], pq[100], pflag[100];

	time_quantum = 2;  // Time Quantum을 2로 설정

	cout << "Round-Robin Scheduling Algorithm" << endl;
	
	if (pnum <= 0 || time_quantum <= 0)
	{
		printf("처리될 데이터가 없습니다.");
		return;
	}
	
	for (j = 0; j<pnum; j++)
	{
		// process queue 초기화
		pq[j] = 0;
		pflag[j] = 0;
	}
	
	for (count = 0; count<pnum; count++)
	{
		remainingTime[count] = cpuBurst[count];
		checkTime += cpuBurst[count];
		
		if (arrivalTime[count] < 0 || cpuBurst[count] <= 0)
		{
			printf("처리될 데이터가 없습니다!");
			return;
		}
	}
	
	for (i = 0; i < 200; i++)
	{
		info[i] = 0;  // 초기화
		process_seq[i] = -1; // 초기화
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
			
			turnaroundTime[pq[0] - 1] = time - arrivalTime[pq[0] - 1];  // TurnAround Time 처리
			waitingTime[pq[0] - 1] = turnaroundTime[pq[0] - 1] - cpuBurst[pq[0] - 1];  // Waiting Time 처리
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
		
		process_seq[seq] = Rearrange_process_queue(pq, pnum, running_processes);  // 하나씩 프로세스 순서 결과 배열에 저장
		seq++;
		// 실행중인 process가 끝났는지 확인
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

		temp = arrivalTime[i];  // arrival time도 변경
		arrivalTime[i] = arrivalTime[position];
		arrivalTime[position] = temp;
	}
	waitingTime[0] = 0;
	for (i = 1; i < limit; i++){
		waitingTime[i] = 0;
		for (j = 0; j < i; j++){
			waitingTime[i] = waitingTime[i] + cpuBurst[j]; // waiting Time 저장
		}
		waitingTime[i] = waitingTime[i] - arrivalTime[i];
	}
	for (i = 0; i < limit; i++){
		turnAroundTime[i] = cpuBurst[i] + waitingTime[i];  // turnAround Time 저장
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
				temp = arrivalTime[i];  // arrival time 변경
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

int main(int argc, char* argv[] ) {   // argv[i]값은 string 

	int pnum = 0;
	int i;
	int *processID, *arrivalTime, *priority, *cpuBurst, *turnAroundTime, *waitingTime, *finish;
	int type = 9;

	if (argc > 2){

		//파일 출력 (읽기)
		ifstream inFile(argv[1]);
		//4번째 문자열을 받아서 정수로 변환
		type = atoi(argv[2]);

		while (!inFile.eof()) {  // 더이상 읽을게 없으면 종료

			inFile.getline(inputString, 100);

			pnum++;// 프로세스 개수 4
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

			for (i = 0; i < pnum; ++i) {  // 파일에 있는 내용 모두 저장
				fscanf(fp, "%d %d %d %d", &processID[i], &arrivalTime[i], &cpuBurst[i], &priority[i]);
				//printf("%d %d %d %d\n", processID[i], arrivalTime[i], cpuBurst[i], priority[i]);
				turnAroundTime[i] = 0;
				waitingTime[i] = 0; // 초기화
			}
		}
		fclose(fp);


		if (type == 0) { // FCFS인 경우
		    FCFS(pnum, processID, arrivalTime, cpuBurst, turnAroundTime, waitingTime);
		}
		else if (type == 1) { // Round-Robin인 경우(Time quantum size:2)
		    RoundRobin(pnum, processID, arrivalTime, cpuBurst, turnAroundTime, waitingTime);
		}
		else if (type == 2) { // Priority Non-Preemptive인 경우
			PriorityNonPreemption(pnum, processID, arrivalTime, cpuBurst, turnAroundTime, waitingTime, priority);
		}
		else if (type == 3) {  // Priority Preemptive 인 경우
			PriorityPreemption(pnum, processID, arrivalTime, cpuBurst, turnAroundTime, waitingTime, priority);
		}
		else {
		cout << "알고리즘을 선택해 주세요!" << endl;
		}
		
		printResult(pnum, processID, arrivalTime, cpuBurst, finish, turnAroundTime, waitingTime);
		memset(turnAroundTime, 0, sizeof(int)*pnum);
		memset(waitingTime, 0, sizeof(int)*pnum);

	}
	else{   // 입력 안한 경우
		cout << "잘못 입력 하셨습니다." << endl;
	}

	system("pause");

	return 0;

}