
#include <iostream>
#define MAXNAME 10000
#define MAXLIST 10000
class processRecord {
public:
	char m_name[MAXNAME];
	int m_no;
	int m_arrival_time;
	int m_i, m_i_max, m_list[MAXLIST];
	int m_waiting_time;
	//È®½Ç
	//queue ¿ë
	
	processRecord* m_next;

	processRecord(void) { memset(this, 0, sizeof(processRecord)); }
	processRecord(processRecord* r) { memcpy(this, r, sizeof(processRecord)); }
	~processRecord(void) {}
	void reset() { memset(this, 0, sizeof(processRecord)); }
	void set(processRecord* r) { memcpy(this, r, sizeof(processRecord)); }

	void print();
protected:
};
