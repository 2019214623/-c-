#include<iostream>
#include<fstream>
#include<string>
#include "scheduling.h"
#pragma comment(lib,"lib.lib")
using namespace std;
int main() {
	ofstream out;
	ifstream in;
	out.open("scheduling.txt", ios::app);
	struct sys s;
	struct task* a = new struct task;
	int num;
	cout << "请输入作业个数：";
	cin >> num;
	init_task(a, NULL, -1, -1, -1);
	init_sys(&s, num, a);
	int nuum = 4;
	struct dl* feedback = new struct dl[nuum];
	s._arr_task = new struct task[num];
	struct task* b = new struct task;
	struct task* c1 = new struct task;
	struct task* d = new struct task;
	struct task* e = new struct task;
	init_task(b, NULL, -1, -1, -1);
	init_task(c1, NULL, -1, -1, -1);
	init_task(d, NULL, -1, -1, -1);
	init_task(e, NULL, -1, -1, -1);
	for (int i = 0; i < num; ++i) {
		int arv, svc, pri;
		char* name = new char[10];
		printf("请输入第%d个作业的名称、到来时间、服务时间和优先级:>", i + 1);
		//scanf_s("%s %d %d %d", name, &arv, &svc, &pri);
		cin >> name >> arv >> svc >> pri;
		init_task(&s._arr_task[i], name, arv, svc, pri);
	}
	cout << "0.先来先服务调度\n1.短进程优先调度\n2.优先级调度\n3.高响应比优先调度\n4.时间片轮转调度\n5.多级反馈队列调度\n6.退出\n7.读运行情况\n";
	while (1) {
		cout << "请输入选项:>";
		char c;
		int n = 0;
		cin >> c;
		switch (c) {
		case '0': FCFS(&s); break;
		case '1': SJF(&s); break;
		case '2': PR(&s); break;
		case '3': HRRN(&s); break;
		case '4': cout << "请输入时间片大小:";
			cin >> n;
			RR(&s, n);
			break;
		case '5': boss(&s, b, c1, d, e, feedback); break;
		case '6': exit(0);
		case'7': in.open("scheduling.txt");
			if (!in.is_open()) cout << "文件读取失败！" << endl;
			else {
				while (!in.eof()) {
					string str;
					getline(in, str);
					cout << str << endl;
				}
			}
		default: cout << endl;
		}
		printf("┌──────┬────────┬────────┬────────┬────────┬────────┬────────────┐\n");
		printf("│进程名│到达时间│开始时间│执行时间│完成时间│周转时间│带权周转时间│\n");
		out << "│进程名│到达时间│开始时间│执行时间│完成时间│周转时间│带权周转时间│" << endl;
		for (int i = 0; i < num; ++i) {
			printf("├──────┼────────┼────────┼────────┼────────┼────────┼────────────┤\n");
			printf("│%6s│%8d│%8d│%8d│%8d│%8d│%12.2f│\n",
				s._arr_task[i]._name,
				s._arr_task[i]._arv_time,
				s._arr_task[i]._start_time,
				s._arr_task[i]._svc_time,
				s._arr_task[i]._fns_time,
				s._arr_task[i]._wle_time,
				s._arr_task[i]._wwle_time);
			out << s._arr_task[i]._name << "\t" << s._arr_task[i]._arv_time << "\t" << s._arr_task[i]._start_time
				<< "\t" << s._arr_task[i]._svc_time << "\t" << s._arr_task[i]._fns_time << "\t"
				<< s._arr_task[i]._wle_time << "\t"
				<< s._arr_task[i]._wwle_time << endl;
		}
		printf("└──────┴────────┴────────┴────────┴────────┴────────┴────────────┘\n");
		
	}
	out.close();
}