#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX 500
//파일의 형식을 읽는부분
#define DELIM " \r\t'P''ARRIVAL'':''CPU''I/O'"
//각 프로세스의 Finish Time
int ProcessFT[10] = { 0,0,0,0,0,0,0,0,0,0 };
//각 프로세스의 PID
int ProcessID[10] = { 0,0,0,0,0,0,0,0,0,0 };
//각 프로세스의 arrival Time
int arrivalT[10] = { 0,0,0,0,0,0,0,0,0,0 };
//각 0~9번의 프로세스들 각각 모든 cpu burst 시간 합함
int allcpuburst[10] = { 0,0,0,0,0,0,0,0,0,0 };
int allcpuburst2[10] = { 0,0,0,0,0,0,0,0,0,0 };
//모든 프로세스의 모든 cpuburst를 더한 값
int totalcpuburst = 0;



int main(int argc, char** argv) {

	if (argc >= 3) {
		printf("파일을 하나만 입력하세요\n");
		return 0;
	}
	char str[MAX], * tok;

	FILE* fp;
	int a[MAX][MAX] = { 0 };
	int i, j;
	int minarr = 0;
	int mincpu[100];
	int timeslice = 0;
	int maxc = 0;
	int avg = 0;
	int Ar, Ac;
	//각 cpu burst들중 최소값을 mincpu로 구한다
	for (i = 0; i < 100; i++) {
		mincpu[i] = 100;
	}
	//파일을 가져오는 부분
	fp = fopen("argv[1]", "r");
	if (!(fp = fopen(argv[1], "r"))) {
		printf("ERROR: file open\n");
		exit(0);
	}
	Ar = 0;
	while (fgets(str, MAX, fp)) {
		if (Ar > 0) {//첫행에 기록

			if (Ac > maxc)// 가장 최고의 열 길이를 maxc에 저장
				maxc = Ac;
		}
		
		printf("%s", str);
		tok = strtok(str, DELIM);
		Ac = 0;
		do {
			a[Ar][Ac++] = atoi(tok);
		} while (tok = strtok(NULL, DELIM));
		Ar++;
	}
	printf("\n");
	for (i = 0; i < Ar; i++) {
		//프로세스의 id 도착시간 넣기
		ProcessID[i] = a[i][0];
		arrivalT[i] = a[i][1];
		// 프로세스의 모든 cpu burst값을 합한값 넣기
		for (j = 2; j < maxc; j = j + 2) {
			allcpuburst[i] += a[i][j];
			allcpuburst2[i] += a[i][j];
		}
	}

	

	minarr = arrivalT[0];
	//0번 프로세스의 arrival time을 minarr에 저장 
	//최소알고리즘을 이용해 minarr에 최소의 arrivalT를 지정한다
	for (i = 0; i < Ar; i++) {
		if (minarr > arrivalT[i])
			minarr = arrivalT[i];
		totalcpuburst += allcpuburst[i];
		//모든 프로세스의 cpuburst 합하기

	}
	timeslice = minarr;
	//1초간격으로 allcpuburst를 하나씩 줄여나갈 timeslice 변수에 최소 arrival time을 넣는다

	while (timeslice <= totalcpuburst) {//time slice를 두어 실질적으로 cpu가 할당된다
		for (i = 0; i < Ar; i++) {
			for (j = 2; j < maxc; j = j + 2) {
				if (a[i][1] <= timeslice) {//process가 arrive 했는지 안했는지
					if (mincpu[timeslice] > a[i][j] && a[i][j] != 0) {//최소 cpuburst를 구한다
						mincpu[timeslice] = a[i][j];
						break;
					}
				}
			}
			
		}
		for (i = 0; i < Ar; i++) {
			for (j = 2; j < maxc; j = j + 2) {
				if (mincpu[timeslice] == a[i][j] && a[i][j] != 0) {//선택되어지면
					allcpuburst[i]--;//값을 하나씩 줄인다
					a[i][j]--;//해당열의 cpuburst도 줄인다 0이 되어 나가게끔한다
					break;
				}
			}
			
		}
		// allcpuburst가 다 바닥난 프로세스가 있다면
		for (i = 0; i < Ar; i++) 
			// 값이 또 들어가는 것을 방지하고
			if (allcpuburst[i] == 0 && ProcessFT[i] == 0) {
				// PFinishTime에 +1을 한다
				ProcessFT[i] = timeslice + 1;
				
				break;
			}
		
		timeslice++;
	}
	printf("\n");
	printf("Process Waiting Time\n");
	printf("--------------------\n");
	for (i = 0; i < Ar; i++) {

	
		printf("P%d\t: %d \n", a[i][0], (ProcessFT[i] - arrivalT[i] - allcpuburst2[i] + 1));
		avg += (ProcessFT[i] - arrivalT[i] - allcpuburst2[i] + 1);
	}
	printf("--------------------\n");
	printf("Average Waiting Time = %0.2f\n", (avg / (double)Ar));

	fclose(fp);


	return 0;
}