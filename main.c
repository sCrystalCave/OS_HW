#include "header.h"

int main(void)
{
	int time = 0; // 출력할 때 사용하는 초
	FILE* fp; FILE** fptr=&fp;
	fp = fopen("event.txt", "r");

	if (fp == NULL)
	{
		printf("파일을 열 수 없습니다");
		return 1;
	}

	while (time <= END_TIME)
	{
		printf("time %d", time);
		create_processes(fptr, time);	// read events.txt	

		//일단 시간이 다 갈때까지 기다려야함.
		//언제 프로세스가 생겨서 실행될지 모름
		//일단 event에 앞 시간만 적었긴 했지만 어디에 적어도 수행이 되야함.
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
		//메모리 공간 동적할당
		struct Process* new_process = (struct Process*)malloc(sizeof(struct Process));
		//우선순위 테이블 업데이트
		prio_table[pid] = new_process;
		
		//멤버 값 대입
		new_process->pid = pid;
		new_process->remaining_time = cpu_execution_time;
		//양방향 연결리스트 맨 앞에 연결
		new_process->next = head;
		new_process->prev = NULL;
		if (head != NULL)
			head->prev = new_process;
		head = new_process;

		//타임 퀀텀이 끝날때까지 스케줄링이 일어나지 않는다.
		//그러니까 생성시에는 스케줄링을 할 필요없다.

	}

	printf("프로세스%d (실행시간 %d) 생성\n", pid, cpu_execution_time);

}

void delete_process(struct Process* del_process)
{
	//우선순위 테이블 업데이트
	prio_table[del_process->pid] = 0;

	//양방향 연결리스트 맨 앞에 연결

	if (head == del_process) {
		head = del_process->next;
	}

	// 삭제할 노드의 이전 노드와 다음 노드를 연결
	if (del_process->prev != NULL) {
		del_process->prev->next = del_process->next;
	}
	if (del_process->next != NULL) {
		del_process->next->prev = del_process->prev;
	}

	// 삭제할 노드 해제
	printf("프로세스%d 해제\n", del_process->pid);
	free(del_process);

	//해제시에는 항상 스케줄링 해야함.


}

void round_robin() 
{
	static int i = 1; //스태틱으로 정의한다.
	if (i == TIME_QUANTUM) 
	{
		cpu_scheduling();
		i = 1;
	}
	else
	{
		i++;
	}
	

	//if, else에 둘다 필요한 함수 : 아래 내용
	//실행 프로세스의 remaining time 감소시키기. printf("프로세스~가 실행중입니다.")
	//remaining time이 0이 되면 delete_process를 호출하고 즉시 스케줄링함.
}

void cpu_scheduling() {

}

//연결리스트 각 요소에 접근하는 방법이 뭐가 있을까
//1. prio_table <- 정확히 어떻게?
// 다른 방법이 있나?..?