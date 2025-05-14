#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX 500
//������ ������ �дºκ�
#define DELIM " \r\t'P''ARRIVAL'':''CPU''I/O'"
//�� ���μ����� Finish Time
int ProcessFT[10] = { 0,0,0,0,0,0,0,0,0,0 };
//�� ���μ����� PID
int ProcessID[10] = { 0,0,0,0,0,0,0,0,0,0 };
//�� ���μ����� arrival Time
int arrivalT[10] = { 0,0,0,0,0,0,0,0,0,0 };
//�� 0~9���� ���μ����� ���� ��� cpu burst �ð� ����
int allcpuburst[10] = { 0,0,0,0,0,0,0,0,0,0 };
int allcpuburst2[10] = { 0,0,0,0,0,0,0,0,0,0 };
//��� ���μ����� ��� cpuburst�� ���� ��
int totalcpuburst = 0;



int main(int argc, char** argv) {

	if (argc >= 3) {
		printf("������ �ϳ��� �Է��ϼ���\n");
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
	//�� cpu burst���� �ּҰ��� mincpu�� ���Ѵ�
	for (i = 0; i < 100; i++) {
		mincpu[i] = 100;
	}
	//������ �������� �κ�
	fp = fopen("argv[1]", "r");
	if (!(fp = fopen(argv[1], "r"))) {
		printf("ERROR: file open\n");
		exit(0);
	}
	Ar = 0;
	while (fgets(str, MAX, fp)) {
		if (Ar > 0) {//ù�࿡ ���

			if (Ac > maxc)// ���� �ְ��� �� ���̸� maxc�� ����
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
		//���μ����� id �����ð� �ֱ�
		ProcessID[i] = a[i][0];
		arrivalT[i] = a[i][1];
		// ���μ����� ��� cpu burst���� ���Ѱ� �ֱ�
		for (j = 2; j < maxc; j = j + 2) {
			allcpuburst[i] += a[i][j];
			allcpuburst2[i] += a[i][j];
		}
	}

	

	minarr = arrivalT[0];
	//0�� ���μ����� arrival time�� minarr�� ���� 
	//�ּҾ˰����� �̿��� minarr�� �ּ��� arrivalT�� �����Ѵ�
	for (i = 0; i < Ar; i++) {
		if (minarr > arrivalT[i])
			minarr = arrivalT[i];
		totalcpuburst += allcpuburst[i];
		//��� ���μ����� cpuburst ���ϱ�

	}
	timeslice = minarr;
	//1�ʰ������� allcpuburst�� �ϳ��� �ٿ����� timeslice ������ �ּ� arrival time�� �ִ´�

	while (timeslice <= totalcpuburst) {//time slice�� �ξ� ���������� cpu�� �Ҵ�ȴ�
		for (i = 0; i < Ar; i++) {
			for (j = 2; j < maxc; j = j + 2) {
				if (a[i][1] <= timeslice) {//process�� arrive �ߴ��� ���ߴ���
					if (mincpu[timeslice] > a[i][j] && a[i][j] != 0) {//�ּ� cpuburst�� ���Ѵ�
						mincpu[timeslice] = a[i][j];
						break;
					}
				}
			}
			
		}
		for (i = 0; i < Ar; i++) {
			for (j = 2; j < maxc; j = j + 2) {
				if (mincpu[timeslice] == a[i][j] && a[i][j] != 0) {//���õǾ�����
					allcpuburst[i]--;//���� �ϳ��� ���δ�
					a[i][j]--;//�ش翭�� cpuburst�� ���δ� 0�� �Ǿ� �����Բ��Ѵ�
					break;
				}
			}
			
		}
		// allcpuburst�� �� �ٴڳ� ���μ����� �ִٸ�
		for (i = 0; i < Ar; i++) 
			// ���� �� ���� ���� �����ϰ�
			if (allcpuburst[i] == 0 && ProcessFT[i] == 0) {
				// PFinishTime�� +1�� �Ѵ�
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