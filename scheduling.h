#ifndef SCHEDULING_H
#define SCHEDULING_H
//定义任务结构体
extern "C" struct task {
	char* _name;		//名字 
	int _arv_time;  	//到达时间
	int _start_time;	//开始时间
	int _svc_time;  	//服务时间
	int _svc_time_bak;	//服务时间备份 
	int _priority;  	//优先级 
	float _res_ratio; 	//响应比 
	int _fns_time;  	//完成时间 
	int _wle_time;  	//周转时间 
	float _wwle_time; 	//带权周转时间 
	struct task* next;
};

//定义系统结构体
extern "C" struct sys
{
	int _task_num;      //总任务数
	int _cur_num;       //当前已到达任务数
	int _time;          //系统时间
	struct task* _cur_task;    //当前正在执行的作业
	struct task* _arr_task;    //全部作业数组
	struct task* _head;         //链表头结点
};
//定义多列反馈队列 
extern "C" struct dl
{
	struct task* _head;         //就绪队列首地址 
	int slice;
};

//初始化任务
extern "C" void init_task(struct task* t, char* name, int arv, int svc, int pri);
//初始化系统
extern "C" void init_sys(struct sys* s, int num, struct task* head);
//初始化调度
extern "C" void init_sche(struct sys* s);
//检测到达任务并添加
extern "C" void push_arv_task(struct sys* s);
//获取最先到达的任务
extern "C" struct task* get_first_task(struct sys* s);
//获取要求服务时间最短的任务
extern "C" struct task* get_min_srv_task(struct sys* s);
//获取最高优先级的任务
extern "C" struct task* get_max_pri_task(struct sys* s);
//更新响应比
extern "C" void update_res_ratio(struct sys* s);
//获取最高响应比的任务
extern "C" struct task* get_max_res_task(struct sys* s);
//先来先服务调度
extern "C" void FCFS(struct sys* s);
//短作业优先调度
extern "C" void SJF(struct sys* s);
//优先级调度
extern "C" void PR(struct sys* s);
//高响应比优先调度
extern "C" void HRRN(struct sys* s);
//时间片轮转调度
extern "C" void RR(struct sys* s, int n);
//多级反馈队列 
extern "C" void boss(struct sys* s, struct task* bb, struct task* cc, struct task* dd, struct task* ee, struct dl* ddd);
#endif