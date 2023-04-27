#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define END_TIME 1000  // 1000ms 즉 1초와 관련해서 사용
#define TIME_QUANTUM 2

#include <stdio.h>
//#include <windows.h>

int prio_table[100] = { 0 }; //들어있으면 1
struct Process* head = NULL;

struct Process
{
	struct Process* prev; //이전 프로세스 ID 출력용
	struct Process* next; //다음 프로세스 ID 출력용
	int pid;
	int remaining_time;
};

void create_processes(FILE**, int);
void add_process(int pid, int cpu_execution_time);
void round_robin();
void cpu_scheduling();