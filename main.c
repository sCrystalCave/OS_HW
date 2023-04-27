#include "header.h"

int main(void)
{
	int time = 0; // ����� �� ����ϴ� ��
	FILE* fp; FILE** fptr=&fp;
	fp = fopen("event.txt", "r");

	if (fp == NULL)
	{
		printf("������ �� �� �����ϴ�");
		return 1;
	}

	while (time <= END_TIME)
	{
		printf("time %d", time);
		create_processes(fptr, time);	// read events.txt	

		//�ϴ� �ð��� �� �������� ��ٷ�����.
		//���� ���μ����� ���ܼ� ������� ��
		//�ϴ� event�� �� �ð��� ������ ������ ��� ��� ������ �Ǿ���.
		round_robin();

		time++;
		printf("\n");
	}
	fclose(fp);
	return 0;
}

void create_processes(FILE** fptr,int time)
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
			add_process(pid, cpu_execution_time);

		}
		else return;
	}
}

void add_process(int pid, int cpu_execution_time)
{
	if (prio_table[pid] != 0)
		return; 
	else 
	{
		//�޸� ���� �����Ҵ�
		struct Process* new_process = (struct Process*)malloc(sizeof(struct Process));
		//�켱���� ���̺� ������Ʈ
		prio_table[pid] = new_process;
		
		//��� �� ����
		new_process->pid = pid;
		new_process->remaining_time = cpu_execution_time;
		//����� ���Ḯ��Ʈ �� �տ� ����
		new_process->next = head;
		new_process->prev = NULL;
		if (head != NULL)
			head->prev = new_process;
		head = new_process;

		//Ÿ�� ������ ���������� �����ٸ��� �Ͼ�� �ʴ´�.
		//�׷��ϱ� �����ÿ��� �����ٸ��� �� �ʿ����.

	}

	printf("���μ���%d (����ð� %d) ����\n", pid, cpu_execution_time);

}

void delete_process(struct Process* del_process)
{
	//�켱���� ���̺� ������Ʈ
	prio_table[del_process->pid] = 0;

	//����� ���Ḯ��Ʈ �� �տ� ����

	if (head == del_process) {
		head = del_process->next;
	}

	// ������ ����� ���� ���� ���� ��带 ����
	if (del_process->prev != NULL) {
		del_process->prev->next = del_process->next;
	}
	if (del_process->next != NULL) {
		del_process->next->prev = del_process->prev;
	}

	// ������ ��� ����
	printf("���μ���%d ����\n", del_process->pid);
	free(del_process);

	//�����ÿ��� �׻� �����ٸ� �ؾ���.


}

void round_robin() 
{
	static int i = 1; //����ƽ���� �����Ѵ�.
	if (i == TIME_QUANTUM) 
	{
		cpu_scheduling();
		i = 1;
	}
	else
	{
		i++;
	}
	

	//if, else�� �Ѵ� �ʿ��� �Լ� : �Ʒ� ����
	//���� ���μ����� remaining time ���ҽ�Ű��. printf("���μ���~�� �������Դϴ�.")
	//remaining time�� 0�� �Ǹ� delete_process�� ȣ���ϰ� ��� �����ٸ���.
}

void cpu_scheduling() {

}

//���Ḯ��Ʈ �� ��ҿ� �����ϴ� ����� ���� ������
//1. prio_table <- ��Ȯ�� ���?
// �ٸ� ����� �ֳ�?..?