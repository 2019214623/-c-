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
	cout << "��������ҵ������";
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
		printf("�������%d����ҵ�����ơ�����ʱ�䡢����ʱ������ȼ�:>", i + 1);
		//scanf_s("%s %d %d %d", name, &arv, &svc, &pri);
		cin >> name >> arv >> svc >> pri;
		init_task(&s._arr_task[i], name, arv, svc, pri);
	}
	cout << "0.�����ȷ������\n1.�̽������ȵ���\n2.���ȼ�����\n3.����Ӧ�����ȵ���\n4.ʱ��Ƭ��ת����\n5.�༶�������е���\n6.�˳�\n7.���������\n";
	while (1) {
		cout << "������ѡ��:>";
		char c;
		int n = 0;
		cin >> c;
		switch (c) {
		case '0': FCFS(&s); break;
		case '1': SJF(&s); break;
		case '2': PR(&s); break;
		case '3': HRRN(&s); break;
		case '4': cout << "������ʱ��Ƭ��С:";
			cin >> n;
			RR(&s, n);
			break;
		case '5': boss(&s, b, c1, d, e, feedback); break;
		case '6': exit(0);
		case'7': in.open("scheduling.txt");
			if (!in.is_open()) cout << "�ļ���ȡʧ�ܣ�" << endl;
			else {
				while (!in.eof()) {
					string str;
					getline(in, str);
					cout << str << endl;
				}
			}
		default: cout << endl;
		}
		printf("���������������Щ����������������Щ����������������Щ����������������Щ����������������Щ����������������Щ�������������������������\n");
		printf("��������������ʱ�䩦��ʼʱ�䩦ִ��ʱ�䩦���ʱ�䩦��תʱ�䩦��Ȩ��תʱ�䩦\n");
		out << "��������������ʱ�䩦��ʼʱ�䩦ִ��ʱ�䩦���ʱ�䩦��תʱ�䩦��Ȩ��תʱ�䩦" << endl;
		for (int i = 0; i < num; ++i) {
			printf("���������������੤���������������੤���������������੤���������������੤���������������੤���������������੤������������������������\n");
			printf("��%6s��%8d��%8d��%8d��%8d��%8d��%12.2f��\n",
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
		printf("���������������ة����������������ة����������������ة����������������ة����������������ة����������������ة�������������������������\n");
		
	}
	out.close();
}