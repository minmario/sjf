#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include "processRecord.h"

#define MAX_LINE 100
#define DELIM		": \t\r\n"
processRecord g_new, g_ready, g_running, g_waiting, g_terminated;
int g_total = 0;
void print_waiting_time() {
	processRecord* go;
	int total = 0, max = 0;
	printf("Process : Waiting Time \n");
	printf("--------------------\n");
	for (go = g_terminated.m_next; go; go = go->m_next) {
		printf("%-5s	: %3d\n", go->m_name, go->m_waiting_time);
		max++;
		total += go->m_waiting_time;
	}
	printf("--------------------\n");
	printf("Average Waiting Time = %5.2\n", (double)total / (double)max);
}
//확실
void print_queue(processRecord* head) {
	processRecord* go;
	for (go = head->m_next; go; go = go->m_next) go->print();
	printf("\n");
}
//확실
void print_queue(int time = 0) {
	printf("%2d--------------------\n", time);
	printf("NEW:\n"); print_queue(&g_new);
	printf("READY:\n"); print_queue(&g_ready);
	printf("RUNNING:\n"); print_queue(&g_running);
	printf("WAITING:\n"); print_queue(&g_waiting);
	printf("TERMINATED:\n"); print_queue(&g_terminated);
	getchar();
}

void add_new_queue(processRecord* node) {
	processRecord* go;
	for (go = &g_new; go->m_next; go = go->m_next) {
		if (node->m_arrival_time < go->m_next->m_arrival_time)break;
	}
	node->m_next = go->m_next;
	go->m_next = node;
}

void add_ready_queue(processRecord* node) {
	processRecord* go, * p;
	node->m_next = nullptr; // Ensure the next pointer of the new node is NULL

	// If there is a process currently running
	if (g_running.m_next) {
		p = g_running.m_next;

		// If the remaining burst time of the new process is less than the current running process
		if (node->m_list[node->m_i] < p->m_list[p->m_i]) {
			// Insert the new process before the current running process in the ready queue
			g_running.m_next = node;
			node->m_next = p;
			return; // Exit the function since insertion is done
		}
	}

	// If the new process is not inserted before the running process, add it to the end of the ready queue
	for (go = &g_ready; go->m_next; go = go->m_next) {
		// Find the position to insert the new process based on its remaining burst time
		if (node->m_list[node->m_i] < go->m_next->m_list[go->m_next->m_i]) {
			break;
		}
	}

	// Insert the new process into the ready queue
	node->m_next = go->m_next;
	go->m_next = node;
}
void add_terminated_queue(processRecord* node) {
	processRecord* go;
	for (go = &g_terminated; go->m_next; go = go->m_next) {
		if (node->m_no < go->m_next->m_no) break;

	}
	node->m_next = go->m_next;
	go->m_next = node;

	g_total--;
}
//확실
void add_waiting_queue(processRecord* node) {
	processRecord* go;

	for (go = &g_waiting; go->m_next; go = go->m_next) {
		if (node->m_list[node->m_i] < go->m_next->m_list[go->m_next->m_i])break;
	}
	node->m_next = go->m_next;
	go->m_next = node;
}
void read_file(char* filename) {
	FILE* fp;
	char line[MAX_LINE], * ret;
	processRecord* p;
	int count = 0;

	if (!(fp = fopen(filename, "r"))){ 
		printf("ERROR: file open\n"); exit(1); 
	}
	while (fgets(line, MAX_LINE, fp)) {
		printf("%s", line);

		p = new processRecord();

		ret = strtok(line, DELIM);
		if (ret == NULL) { delete p; break; }
		strcpy(p->m_name, ret);
		strtok(NULL, DELIM);
		p->m_arrival_time = atoi(strtok(NULL, DELIM));
		p->m_no = count++;

		while (strtok(NULL, DELIM))p->m_list[p->m_i_max++] = atoi(strtok(NULL, DELIM));

		add_new_queue(p);
		g_total++;

			
	}
	fclose(fp);
	printf("\n");


}
//확실
void move_from_ready_to_running(int time) {
	processRecord *head,*p;

	head = &g_ready;
	if (head->m_next == NULL) return;

	p = head->m_next;
	head->m_next=p->m_next;

	g_running.m_next = p;
	p->m_next = NULL;
}
//확실
void move_from_new_to_ready(int time) {
	processRecord *head, *p;
	for (head = &g_new; head->m_next;) {
		p = head->m_next;
		if (p->m_arrival_time > time)break;
		head->m_next = p->m_next;
		p->m_next = NULL;
		add_ready_queue(p);

	}
}
//확실
void move_from_waiting_to_ready(int time) {
	processRecord *head, *p;
	for (head = &g_waiting; head->m_next; ) {
		p = head->m_next;
		if (p->m_list[p->m_i])
			break;

		p->m_i++;
		head->m_next = p->m_next;
		p->m_next = NULL;
		add_ready_queue(p);

	}
}
//확실
void update_ready_queue() {
	processRecord* go;
	for (go = g_ready.m_next; go; go = go->m_next)
		go->m_waiting_time++;
}
//확실
void update_waiting_queue() {
	processRecord* go;
	for (go = g_waiting.m_next; go; go = go->m_next) {
		go->m_list[go->m_i]--;
	}
}
int main(int argc, char* argv[]) {
	int time;
	processRecord* p;

	if (argc != 2) {
		printf("ERROR: No input file! ...\n");
		exit(1);
	}
	read_file(argv[1]);

	//SJF 강제뺏기 scheduler
	for (time = 0; g_total; time++) {
		update_ready_queue(); //waiting_time++ for all process is the ready queue
		update_waiting_queue();//I/0_time-- for all process in the waiting queue

		if (g_running.m_next) {
			//running process가 있을시
			p = g_running.m_next;
			p->m_list[p->m_i]--;//cpu time 감소
			//소진시
			if (p->m_list[p->m_i] == 0) 
			{
				g_running.m_next = NULL;
				p->m_i++;
				//terminating이나 waiting으로 바꿈
				if (p->m_i == p->m_i_max) 
					add_terminated_queue(p);
				else 
					add_waiting_queue(p);
			}

		}
		move_from_new_to_ready(time);
		move_from_waiting_to_ready(time);
		//running이 없을시
		if (g_running.m_next == NULL)
			move_from_ready_to_running(time);

		print_queue(time);
	}
	//sjf 마침

	print_waiting_time();

	getchar();

	return 0;
}
//확실
//