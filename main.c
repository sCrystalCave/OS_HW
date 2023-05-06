#include "header.h"

int main(void)
{
	FILE* fp; FILE** fptr = &fp;
	fp = fopen("process_list.txt", "r");

	if (fp == NULL)
	{
		printf("파일을 열 수 없습니다.");
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
	printf("모든 프로세스 종료\n\n");
	printf("평균 턴어라운드 타임 %f: ", (double)tot_turnaround_time / num_process);

	fclose(fp);
	return 0;
}

