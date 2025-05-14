#include "processRecord.h"
void processRecord::print() {
	int i;
	printf("%3s	WAITING:%2d	ARRIVAL:%d	", m_name, m_waiting_time, m_arrival_time);
	for (i = 0; ; ) {
		printf("CPU:%-2d	",m_list[i++]);
		if (i >= m_i_max)
			break;
		printf("I/O:%-2d	", m_list[i++]);

	}
	printf("\n");
}