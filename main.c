#include "header.h"

int main(void)
{
	FILE* fp; FILE** fptr = &fp;
	fp = fopen("process_list.txt", "r");

	if (fp == NULL)
	{
		printf("������ �� �� �����ϴ�.");
		return 1;
	}

	while (time <= 50)
	{
		printf("time %d\n", time);
		create_processes(fptr, time);	// read events.txt	

		if (tail == NULL)
			break;
		round_robin();

		time++;
		printf("\n");
	}
	printf("��� ���μ��� ����\n\n");
	printf("��� �Ͼ���� Ÿ�� %f: ", (double)tot_turnaround_time / num_process);

	fclose(fp);
	return 0;
}

