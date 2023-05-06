#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define END_TIME 1000  // 1000ms 즉 1초와 관련해서 사용
#define TIME_QUANTUM 2

#include <stdio.h>
#include <windows.h>

int num_process = 0; ////총 생성된 프로세스 수
int time = 0; // 출력할 때 사용하는 초
int cur_tslot = 0;		////현재 실행중인 time slot 전역변수 선언
int tot_turnaround_time = 0;


struct Process* head = NULL;
struct Process* tail = NULL;	////리스트의 맨뒤 pcb를 가리키는 구조체 포인터 선언(순환식 리스트가 아니기 때문)
struct Process* current_process = NULL;

struct Process
{
	struct Process* prev; //이전 프로세스 ID 출력용
	struct Process* next; //다음 프로세스 ID 출력용
	int start_time;
	int pid;
	int remaining_time;
	int exe_time;
};

void create_processes(FILE**, int);
void add_process(int start_time, int pid, int cpu_execution_time);
void delete_process(struct Process* del_process);
void round_robin();
void change_current_process();
void process_execute(struct Process* current_process);


void create_processes(FILE** fptr, int time)
{
	int start_time, pid;
	while (1) {
		if ((fscanf(*fptr, "%d", &start_time)) != EOF)
		{
			if (start_time != time) {
				fseek(*fptr, -1, SEEK_CUR);
				return;
			}
			int cpu_execution_time = 0;
			fscanf(*fptr, "%d %d", &pid, &cpu_execution_time);
			add_process(start_time, pid, cpu_execution_time);

		}
		else return;
	}
}

void add_process(int start_time, int pid, int cpu_execution_time)
{
	//메모리 공간 동적할당
	struct Process* new_process = (struct Process*)malloc(sizeof(struct Process));

	//멤버 값 대입
	new_process->start_time = start_time;
	new_process->pid = pid;
	new_process->remaining_time = cpu_execution_time;


	////맨앞 블록의 prev를 null로 함(head로 할 시, new_process->prev도 자기 자신을 가리키게 됨)
	if (head == NULL)
	{
		head = new_process;
		new_process->prev = NULL;
		new_process->next = NULL;
		tail = new_process;

		current_process = new_process;
	}
	else
	{
		new_process->next = head;
		new_process->prev = NULL;
		head->prev = new_process;
		head = new_process;
	}

	num_process++;
	printf("프로세스%d (실행시간 %d) 생성\n", pid, cpu_execution_time);

}

void delete_process(struct Process* del_process)
{

	del_process->exe_time = ((time + 1) - del_process->start_time);
	tot_turnaround_time += del_process->exe_time;

	///끊어질 블록의 위치가 어딘지에 따라 연결리스트 재구성
	if (del_process->prev == NULL && del_process->next == NULL)	//블록이 하나만 남았을 경우
	{
		head = NULL;
		tail = NULL;
		current_process = NULL;
	}
	if (del_process->prev != NULL && del_process->next != NULL)	//next,prev가 다 존재(중간 블록)
	{
		del_process->next->prev = del_process->prev;
		del_process->prev->next = del_process->next;
	}
	if (del_process->prev == NULL && del_process->next != NULL)	//next는 없고 prev만 존재(맨 끝)
	{
		del_process->next->prev = NULL;
		head = del_process->next;
	}
	if (del_process->next == NULL && del_process->prev != NULL)	//prev는 없고 next만 존재(맨 앞)
	{
		del_process->prev->next = NULL;
		tail = del_process->prev;
	}


	//해제시에는 항상 스케줄링 해야함., 스케줄링 먼저 진행 후 메모리 해제 -> 스케줄링 할 때 해제되는 프로세스의 리스트 위치정보 필요하기 때문
	change_current_process();

	// 삭제할 노드 해제
	printf("time %d에 프로세스%d 해제\n", time + 1, del_process->pid);
	free(del_process);
}

void round_robin()
{
	if (current_process == NULL)
		return;
	else
	{
		process_execute(current_process);
		if (cur_tslot == TIME_QUANTUM || current_process->remaining_time <= 0)
		{
			if (current_process->remaining_time <= 0)
				delete_process(current_process);
			else
				change_current_process();
		}

	}
}

void change_current_process() {
	if (current_process == NULL)
		return;
	if (current_process->prev == NULL)	//현재 스케줄링 됐던 블록이 리스트 맨앞이면 다시 맨 뒤 블록 스케줄링
		current_process = tail;
	else
		current_process = current_process->prev;
	cur_tslot = 0;
}

void process_execute(struct Process* current_process) {

	printf("현재 실행중인 프로세스 %d (남은 실행시간 %d)\n", current_process->pid, current_process->remaining_time);
	//Sleep(1000);
	current_process->remaining_time--;
	cur_tslot++;
}
