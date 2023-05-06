#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define END_TIME 1000  // 1000ms �� 1�ʿ� �����ؼ� ���
#define TIME_QUANTUM 2

#include <stdio.h>
#include <windows.h>

int num_process = 0; ////�� ������ ���μ��� ��
int time = 0; // ����� �� ����ϴ� ��
int cur_tslot = 0;		////���� �������� time slot �������� ����
int tot_turnaround_time = 0;


struct Process* head = NULL;
struct Process* tail = NULL;	////����Ʈ�� �ǵ� pcb�� ����Ű�� ����ü ������ ����(��ȯ�� ����Ʈ�� �ƴϱ� ����)
struct Process* current_process = NULL;

struct Process
{
	struct Process* prev; //���� ���μ��� ID ��¿�
	struct Process* next; //���� ���μ��� ID ��¿�
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
	//�޸� ���� �����Ҵ�
	struct Process* new_process = (struct Process*)malloc(sizeof(struct Process));

	//��� �� ����
	new_process->start_time = start_time;
	new_process->pid = pid;
	new_process->remaining_time = cpu_execution_time;


	////�Ǿ� ����� prev�� null�� ��(head�� �� ��, new_process->prev�� �ڱ� �ڽ��� ����Ű�� ��)
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
	printf("���μ���%d (����ð� %d) ����\n", pid, cpu_execution_time);

}

void delete_process(struct Process* del_process)
{

	del_process->exe_time = ((time + 1) - del_process->start_time);
	tot_turnaround_time += del_process->exe_time;

	///������ ����� ��ġ�� ������� ���� ���Ḯ��Ʈ �籸��
	if (del_process->prev == NULL && del_process->next == NULL)	//����� �ϳ��� ������ ���
	{
		head = NULL;
		tail = NULL;
		current_process = NULL;
	}
	if (del_process->prev != NULL && del_process->next != NULL)	//next,prev�� �� ����(�߰� ���)
	{
		del_process->next->prev = del_process->prev;
		del_process->prev->next = del_process->next;
	}
	if (del_process->prev == NULL && del_process->next != NULL)	//next�� ���� prev�� ����(�� ��)
	{
		del_process->next->prev = NULL;
		head = del_process->next;
	}
	if (del_process->next == NULL && del_process->prev != NULL)	//prev�� ���� next�� ����(�� ��)
	{
		del_process->prev->next = NULL;
		tail = del_process->prev;
	}


	//�����ÿ��� �׻� �����ٸ� �ؾ���., �����ٸ� ���� ���� �� �޸� ���� -> �����ٸ� �� �� �����Ǵ� ���μ����� ����Ʈ ��ġ���� �ʿ��ϱ� ����
	change_current_process();

	// ������ ��� ����
	printf("time %d�� ���μ���%d ����\n", time + 1, del_process->pid);
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
	if (current_process->prev == NULL)	//���� �����ٸ� �ƴ� ����� ����Ʈ �Ǿ��̸� �ٽ� �� �� ��� �����ٸ�
		current_process = tail;
	else
		current_process = current_process->prev;
	cur_tslot = 0;
}

void process_execute(struct Process* current_process) {

	printf("���� �������� ���μ��� %d (���� ����ð� %d)\n", current_process->pid, current_process->remaining_time);
	//Sleep(1000);
	current_process->remaining_time--;
	cur_tslot++;
}
