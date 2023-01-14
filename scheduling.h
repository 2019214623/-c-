#ifndef SCHEDULING_H
#define SCHEDULING_H
//��������ṹ��
extern "C" struct task {
	char* _name;		//���� 
	int _arv_time;  	//����ʱ��
	int _start_time;	//��ʼʱ��
	int _svc_time;  	//����ʱ��
	int _svc_time_bak;	//����ʱ�䱸�� 
	int _priority;  	//���ȼ� 
	float _res_ratio; 	//��Ӧ�� 
	int _fns_time;  	//���ʱ�� 
	int _wle_time;  	//��תʱ�� 
	float _wwle_time; 	//��Ȩ��תʱ�� 
	struct task* next;
};

//����ϵͳ�ṹ��
extern "C" struct sys
{
	int _task_num;      //��������
	int _cur_num;       //��ǰ�ѵ���������
	int _time;          //ϵͳʱ��
	struct task* _cur_task;    //��ǰ����ִ�е���ҵ
	struct task* _arr_task;    //ȫ����ҵ����
	struct task* _head;         //����ͷ���
};
//������з������� 
extern "C" struct dl
{
	struct task* _head;         //���������׵�ַ 
	int slice;
};

//��ʼ������
extern "C" void init_task(struct task* t, char* name, int arv, int svc, int pri);
//��ʼ��ϵͳ
extern "C" void init_sys(struct sys* s, int num, struct task* head);
//��ʼ������
extern "C" void init_sche(struct sys* s);
//��⵽���������
extern "C" void push_arv_task(struct sys* s);
//��ȡ���ȵ��������
extern "C" struct task* get_first_task(struct sys* s);
//��ȡҪ�����ʱ����̵�����
extern "C" struct task* get_min_srv_task(struct sys* s);
//��ȡ������ȼ�������
extern "C" struct task* get_max_pri_task(struct sys* s);
//������Ӧ��
extern "C" void update_res_ratio(struct sys* s);
//��ȡ�����Ӧ�ȵ�����
extern "C" struct task* get_max_res_task(struct sys* s);
//�����ȷ������
extern "C" void FCFS(struct sys* s);
//����ҵ���ȵ���
extern "C" void SJF(struct sys* s);
//���ȼ�����
extern "C" void PR(struct sys* s);
//����Ӧ�����ȵ���
extern "C" void HRRN(struct sys* s);
//ʱ��Ƭ��ת����
extern "C" void RR(struct sys* s, int n);
//�༶�������� 
extern "C" void boss(struct sys* s, struct task* bb, struct task* cc, struct task* dd, struct task* ee, struct dl* ddd);
#endif