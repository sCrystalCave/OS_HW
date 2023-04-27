#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define END_TIME 1000  // 1000ms �� 1�ʿ� �����ؼ� ���
#define TIME_QUANTUM 2

#include <stdio.h>
//#include <windows.h>

int prio_table[100] = { 0 }; //��������� 1
struct Process* head = NULL;

struct Process
{
	struct Process* prev; //���� ���μ��� ID ��¿�
	struct Process* next; //���� ���μ��� ID ��¿�
	int pid;
	int remaining_time;
};

void create_processes(FILE**, int);
void add_process(int pid, int cpu_execution_time);
void round_robin();
void cpu_scheduling();