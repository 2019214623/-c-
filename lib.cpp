#define NULL 0
#include"scheduling.h"

void init_task(struct task* t, char* name, int arv, int svc, int pri) {
	t->_name = name;
	t->_arv_time = arv;
	t->_svc_time = svc;
	t->_svc_time_bak = svc;
	t->_priority = pri;
	t->_start_time = 0;
	t->_fns_time = 0;
	t->_wle_time = 0;
	t->_wwle_time = 0;
	t->_res_ratio = 0;
	t->next = NULL;
}


void init_sys(struct sys* s, int num, struct task* head) {
	s->_task_num = num;
	s->_time = 0;
	s->_cur_num = 0;
	s->_arr_task = NULL;
	s->_cur_task = NULL;
	s->_head = head;
}

void init_sche(struct sys* s) {
	s->_time = 0;
	s->_cur_num = 0;
	s->_cur_task = NULL;
	for (int i = 0; i < s->_task_num; i++) {
		init_task(&s->_arr_task[i], s->_arr_task[i]._name, s->_arr_task[i]._arv_time, s->_arr_task[i]._svc_time, s->_arr_task[i]._priority);
	}
}

void push_arv_task(struct sys* s) {
	struct task* cur = s->_head;
	for (int i = 0; i < s->_task_num; i++) {
		if (s->_time == s->_arr_task[i]._arv_time) {
			while (cur->next) {
				cur = cur->next;
			}
			cur->next = &s->_arr_task[i];
			s->_cur_num++;
		}
	}
}

struct task* get_first_task(struct sys* s) {
	if (s->_head->next == NULL)	return NULL;
	struct task* cur = s->_head;
	struct task* first = s->_head->next;
	cur->next = cur->next->next;
	return first;
}

struct task* get_min_srv_task(struct sys* s) {
	if (s->_head->next == NULL)	return NULL;
	struct task* c = s->_head;
	struct task* min = s->_head->next;
	struct task* cur = s->_head->next->next;
	while (cur) {
		if (min->_svc_time > cur->_svc_time) {
			min = cur;
		}
		cur = cur->next;
	}
	while (c->next != min) {
		c = c->next;
	}
	c->next = c->next->next;
	return min;
}

struct task* get_max_pri_task(struct sys* s) {
	if (s->_head->next == NULL)	return NULL;
	struct task* c = s->_head;
	struct task* max = s->_head->next;
	struct task* cur = s->_head->next->next;
	while (cur) {
		if (max->_priority < cur->_priority) {
			max = cur;
		}
		cur = cur->next;
	}
	while (c->next != max) {
		c = c->next;
	}
	c->next = c->next->next;
	return max;
}

void update_res_ratio(struct sys* s) {
	if (s->_head->next == NULL)	return;
	struct task* cur = s->_head->next;
	while (cur) {
		if (cur->_svc_time == 0)	return;
		cur->_res_ratio = 1 + (s->_time - cur->_arv_time) / ((float)cur->_svc_time);
		cur = cur->next;
	}
}

struct task* get_max_res_task(struct sys* s) {
	if (s->_head->next == NULL)	return NULL;
	struct task* c = s->_head;
	struct task* max = s->_head->next;
	struct task* cur = s->_head->next->next;
	while (cur) {
		if (max->_res_ratio < cur->_res_ratio) {
			max = cur;
		}
		cur = cur->next;
	}
	while (c->next != max) {
		c = c->next;
	}
	c->next = c->next->next;
	return max;
}

void FCFS(struct sys* s) {
	init_sche(s);
	int cur_svc_time = 0;
	while (1) {
		push_arv_task(s);
		if (s->_cur_task == NULL || cur_svc_time == 0) {
			s->_cur_task = get_first_task(s);
			if (s->_cur_task != NULL) {
				cur_svc_time = s->_cur_task->_svc_time;
				s->_cur_task->_start_time = s->_time;
			}
		}
		if (s->_cur_task == NULL && s->_cur_num == s->_task_num)	break;
		if (cur_svc_time > 0)	cur_svc_time--;
		s->_time++;
		if (cur_svc_time == 0 && s->_cur_task != NULL) {
			s->_cur_task->_fns_time = s->_time;
			s->_cur_task->_wle_time = s->_cur_task->_fns_time - s->_cur_task->_arv_time;
			if (s->_cur_task->_svc_time == 0)	continue;
			s->_cur_task->_wwle_time = (float)s->_cur_task->_wle_time / s->_cur_task->_svc_time;
		}
	}
}

void SJF(struct sys* s) {
	init_sche(s);
	int cur_svc_time = 0;
	while (1) {
		push_arv_task(s);
		if (s->_cur_task == NULL || cur_svc_time == 0) {
			s->_cur_task = get_min_srv_task(s);
			if (s->_cur_task != NULL) {
				cur_svc_time = s->_cur_task->_svc_time;
				s->_cur_task->_start_time = s->_time;
			}
		}
		if (s->_cur_task == NULL && s->_cur_num == s->_task_num)	break;
		if (cur_svc_time > 0)	cur_svc_time--;
		s->_time++;
		if (cur_svc_time == 0 && s->_cur_task != NULL) {
			s->_cur_task->_fns_time = s->_time;
			s->_cur_task->_wle_time = s->_cur_task->_fns_time - s->_cur_task->_arv_time;
			if (s->_cur_task->_svc_time == 0)	continue;
			s->_cur_task->_wwle_time = (float)s->_cur_task->_wle_time / s->_cur_task->_svc_time;
		}
	}
}

void PR(struct sys* s) {
	init_sche(s);
	int cur_svc_time = 0;
	while (1) {
		push_arv_task(s);
		if (s->_cur_task == NULL || cur_svc_time == 0) {
			s->_cur_task = get_max_pri_task(s);
			if (s->_cur_task != NULL) {
				cur_svc_time = s->_cur_task->_svc_time;
				s->_cur_task->_start_time = s->_time;
			}
		}
		if (s->_cur_task == NULL && s->_cur_num == s->_task_num)	break;
		if (cur_svc_time > 0)	cur_svc_time--;
		s->_time++;
		if (cur_svc_time == 0 && s->_cur_task != NULL) {
			s->_cur_task->_fns_time = s->_time;
			s->_cur_task->_wle_time = s->_cur_task->_fns_time - s->_cur_task->_arv_time;
			if (s->_cur_task->_svc_time == 0)	continue;
			s->_cur_task->_wwle_time = (float)s->_cur_task->_wle_time / s->_cur_task->_svc_time;
		}
	}
}

void HRRN(struct sys* s) {
	init_sche(s);
	int cur_svc_time = 0;
	while (1) {
		push_arv_task(s);
		update_res_ratio(s);
		if (s->_cur_task == NULL || cur_svc_time == 0) {
			s->_cur_task = get_max_res_task(s);
			if (s->_cur_task != NULL) {
				cur_svc_time = s->_cur_task->_svc_time;
				s->_cur_task->_start_time = s->_time;
			}
		}
		if (s->_cur_task == NULL && s->_cur_num == s->_task_num)	break;
		if (cur_svc_time > 0)	cur_svc_time--;
		s->_time++;
		if (cur_svc_time == 0 && s->_cur_task != NULL) {
			s->_cur_task->_fns_time = s->_time;
			s->_cur_task->_wle_time = s->_cur_task->_fns_time - s->_cur_task->_arv_time;
			if (s->_cur_task->_svc_time == 0)	continue;
			s->_cur_task->_wwle_time = (float)s->_cur_task->_wle_time / s->_cur_task->_svc_time;
		}
	}
}

void RR(struct sys* s, int n) {
	init_sche(s);
	const int slice = n;
	int cur_slice = 0;
	int cur_srv_time = 0;
	while (1) {
		for (int i = 0; i < s->_task_num; i++) {// 如果进程时间到了就把该进程加到就绪队列head后面 
			if (s->_time == s->_arr_task[i]._arv_time) {
				struct task* cur = s->_head;
				while (cur->next) {
					cur = cur->next;
				}
				cur->next = &s->_arr_task[i];
				s->_cur_num++;
			}
		}
		if (s->_cur_task && cur_slice == 0) {
			if (s->_head->next != NULL) {//把就绪队列里面第一个进程拿出来进行时间片轮转 
				struct task* temp = s->_head->next;
				s->_head->next = s->_head->next->next;
				temp->next = NULL;
			}
			s->_cur_task->_svc_time_bak = cur_srv_time;
			if (cur_srv_time != 0) {//分给进程的时间片用完了，就把这个进程放到就绪队列的末尾 
				struct task* cur = s->_head;
				while (cur->next) {
					cur = cur->next;
				}
				cur->next = s->_cur_task;
			}
			else {//计算进程的周转时间等 
				s->_cur_task->_fns_time = s->_time;
				s->_cur_task->_wle_time = s->_cur_task->_fns_time - s->_cur_task->_arv_time;
				if (s->_cur_task->_svc_time == 0)	continue;
				s->_cur_task->_wwle_time = (float)s->_cur_task->_wle_time / s->_cur_task->_svc_time;
			}
		}
		if (s->_cur_num == s->_task_num && s->_head->next == NULL)	break;
		if (s->_cur_task == NULL || cur_slice == 0) {
			if (s->_head->next == NULL) {//就绪队列无进程 
				s->_cur_task = NULL;
			}
			else {
				s->_cur_task = s->_head->next;
				cur_srv_time = s->_cur_task->_svc_time_bak;
				if (s->_cur_task->_svc_time == s->_cur_task->_svc_time_bak) {//判断是否为该进程的第一次执行 
					s->_cur_task->_start_time = s->_time;
				}
				if (cur_srv_time > slice)	cur_slice = slice;//选取剩余服务时间和时间片中较小的那个 
				else	cur_slice = cur_srv_time;
			}
		}
		if (cur_slice > 0)	cur_slice--;
		if (cur_srv_time > 0)	cur_srv_time--;
		s->_time++;
	}
}
void boss(struct sys* s, struct task* bb, struct task* cc, struct task* dd, struct task* ee, struct dl* ddd) {
	init_sche(s);
	int cur_slice = 0;
	int cur_srv_time = 0;
	int ji = 0;
	struct dl* feedbackdl = ddd;
	//初始化队列
	for (int i = 0; i < 4; i++) {
		feedbackdl[i].slice = i + 1;
	}
	feedbackdl[0]._head = bb;
	feedbackdl[1]._head = cc;
	feedbackdl[2]._head = dd;
	feedbackdl[3]._head = ee;
	int dl2_cur_num = 0;
	int dl3_cur_num = 0;
	int dl4_cur_num = 0;
	while (1) {
		if (ji % 4 == 0)//第一级队列
		{
			for (int i = 0; i < s->_task_num; i++) {// 如果进程时间到了就把该进程加到就绪队列head后面 
				if (s->_time == s->_arr_task[i]._arv_time) {
					struct task* cur = s->_head;
					while (cur->next) {
						cur = cur->next;
					}

					cur->next = &s->_arr_task[i];
					s->_cur_num++;
				}
			}
			feedbackdl[0]._head = s->_head;
			if (s->_cur_task && cur_slice == 0) {
				if (s->_head->next != NULL) {//把正在执行的这个进程从就绪队列中删除 
					struct task* temp = s->_head->next;
					s->_head->next = s->_head->next->next;
					temp->next = NULL;
				}
				s->_cur_task->_svc_time_bak = cur_srv_time;//更新剩余服务时间 
				if (cur_srv_time != 0) {//分给进程的时间片用完了并且进程并未执行完毕，就把这个进程放到下一级队列的末尾 
					/*struct task* cur = new struct task;
					if (feedbackdl[1]._head == NULL)
					{
						feedbackdl[1]._head = &s->_arr_task[0];
						feedbackdl[1]._head->next = s->_cur_task;
						cur = feedbackdl[1]._head->next;
					}

					else {
						while (cur->next) {
							cur = cur->next;
						}
						cur->next = s->_cur_task;
					}*/
					struct task* cur = feedbackdl[1]._head;
					while (cur->next) {
						cur = cur->next;
					}
					cur->next = s->_cur_task;

				}
				else {//计算进程的周转时间等 
					s->_cur_task->_fns_time = s->_time;
					s->_cur_task->_wle_time = s->_cur_task->_fns_time - s->_cur_task->_arv_time;
					if (s->_cur_task->_svc_time == 0)	continue;
					s->_cur_task->_wwle_time = (float)s->_cur_task->_wle_time / s->_cur_task->_svc_time;
				}
			}
			if (s->_head->next == NULL) {
				ji++;
				s->_cur_task = NULL;
				continue;

			}
			if (s->_cur_task == NULL || cur_slice == 0) {
				if (s->_head->next == NULL) {//就绪队列无进程 
					s->_cur_task = NULL;
				}

				else {
					s->_cur_task = s->_head->next;
					cur_srv_time = s->_cur_task->_svc_time_bak;
					if (s->_cur_task->_svc_time == s->_cur_task->_svc_time_bak) {//判断是否为该进程的第一次执行 
						s->_cur_task->_start_time = s->_time;
					}
					if (cur_srv_time > feedbackdl[0].slice)	cur_slice = feedbackdl[0].slice;//选取剩余服务时间和时间片中较小的那个 
					else	cur_slice = cur_srv_time;
				}
			}
			if (cur_slice > 0)	cur_slice--;
			if (cur_srv_time > 0)	cur_srv_time--;
			s->_time++;
		}
		if (ji % 4 == 1)//第二级队列
		{
			bool flag = false;
			int now_count = 0;
			for (int i = 0; i < s->_task_num; i++) {// 如果进程时间到了就把该进程加到就绪队列head后面 
				if (s->_time == s->_arr_task[i]._arv_time) {
					ji = 0;
					flag = true;
					if (s->_cur_task)
					{
						s->_cur_task->_svc_time_bak = cur_srv_time;//更新剩余服务时间 
					}
					s->_cur_task = NULL;
					cur_slice = 0;
					break;
					struct task* cur = s->_head;
					while (cur->next) {
						cur = cur->next;
					}

					cur->next = &s->_arr_task[i];
					s->_cur_num++;


				}
			}
			if (flag)
			{
				continue;
			}
			if (s->_cur_task && cur_slice == 0) {
				if (feedbackdl[1]._head->next != NULL) {//把正在执行的这个进程从就绪队列中删除 
					struct task* now = feedbackdl[1]._head;

					while (now->next) {
						now_count++;
						now = now->next;
					}
					struct task* temp = feedbackdl[1]._head->next;
					if (feedbackdl[1]._head->next->next) {
						feedbackdl[1]._head->next = feedbackdl[1]._head->next->next;
						temp->next = NULL;
					}
					else
					{
						feedbackdl[1]._head->next = NULL;
					}


				}
				if (s->_cur_task)
				{
					s->_cur_task->_svc_time_bak = cur_srv_time;//更新剩余服务时间 
				}

				if (cur_srv_time != 0) {//分给进程的时间片用完了并且进程并未执行完毕，就把这个进程放到下一级队列的末尾 
					//struct task* cur = new struct task;
					////dl2_cur_num++;
					//if (feedbackdl[2]._head == NULL)
					//{
					//	feedbackdl[2]._head = new struct task;
					//	feedbackdl[2]._head->next = s->_cur_task;
					//	cur = feedbackdl[2]._head;
					//}

					//else {
					//	while (cur->next) {
					//		cur = cur->next;
					//	}
					//	cur->next = s->_cur_task;
					//}
					struct task* cur = feedbackdl[2]._head;
					while (cur->next) {
						cur = cur->next;
					}
					cur->next = s->_cur_task;
				}
				else {//计算进程的周转时间等 
					s->_cur_task->_fns_time = s->_time;
					s->_cur_task->_wle_time = s->_cur_task->_fns_time - s->_cur_task->_arv_time;
					if (s->_cur_task->_svc_time == 0)	continue;
					s->_cur_task->_wwle_time = (float)s->_cur_task->_wle_time / s->_cur_task->_svc_time;
				}
			}
			//

			if (feedbackdl[1]._head->next == NULL) {
				ji++;
				s->_cur_task = NULL;
				continue;
			}
			if (s->_cur_task == NULL || cur_slice == 0) {
				if (feedbackdl[1]._head->next == NULL) {//就绪队列无进程 
					break;
				}

				else {
					dl2_cur_num++;
					s->_cur_task = feedbackdl[1]._head->next;
					cur_srv_time = s->_cur_task->_svc_time_bak;
					if (cur_srv_time > feedbackdl[1].slice)	cur_slice = feedbackdl[1].slice;//选取剩余服务时间和时间片中较小的那个 
					else	cur_slice = cur_srv_time;
				}
			}
			if (cur_slice > 0)	cur_slice--;
			if (cur_srv_time > 0)	cur_srv_time--;
			s->_time++;
		}
		if (ji % 4 == 2)//第三级队列
		{
			bool flag = false;
			int now_count = 0;
			for (int i = 0; i < s->_task_num; i++) {// 如果进程时间到了就把该进程加到就绪队列head后面 
				if (s->_time == s->_arr_task[i]._arv_time) {
					ji = 0;
					flag = true;
					if (s->_cur_task)
					{
						s->_cur_task->_svc_time_bak = cur_srv_time;//更新剩余服务时间 
					}
					s->_cur_task = NULL;
					cur_slice = 0;
					break;
					struct task* cur = s->_head;
					while (cur->next) {
						cur = cur->next;
					}

					cur->next = &s->_arr_task[i];
					s->_cur_num++;


				}
			}
			if (flag)
			{
				continue;
			}
			if (s->_cur_task && cur_slice == 0) {
				if (feedbackdl[2]._head->next != NULL) {//把正在执行的这个进程从就绪队列中删除 
					struct task* now = feedbackdl[2]._head;

					while (now->next) {
						now_count++;
						now = now->next;
					}
					struct task* temp = feedbackdl[2]._head->next;
					if (feedbackdl[2]._head->next->next) {
						feedbackdl[2]._head->next = feedbackdl[2]._head->next->next;
						temp->next = NULL;
					}
					else
					{
						feedbackdl[2]._head->next = NULL;
					}

				}
				if (s->_cur_task)
				{
					s->_cur_task->_svc_time_bak = cur_srv_time;//更新剩余服务时间 
				}
				if (cur_srv_time != 0) {//分给进程的时间片用完了并且进程并未执行完毕，就把这个进程放到下一级队列的末尾 
					//dl3_cur_num++;
				/*struct task* cur = new struct task;
				if (feedbackdl[3]._head == NULL)
				{
					feedbackdl[3]._head = new struct task;
					feedbackdl[3]._head->next = s->_cur_task;
					cur = feedbackdl[3]._head;
				}

				else {
					while (cur->next) {
						cur = cur->next;
					}
					cur->next = s->_cur_task;
				}*/
					struct task* cur = feedbackdl[3]._head;
					while (cur->next) {
						cur = cur->next;
					}
					cur->next = s->_cur_task;
				}
				else {//计算进程的周转时间等 
					s->_cur_task->_fns_time = s->_time;
					s->_cur_task->_wle_time = s->_cur_task->_fns_time - s->_cur_task->_arv_time;
					if (s->_cur_task->_svc_time == 0)	continue;
					s->_cur_task->_wwle_time = (float)s->_cur_task->_wle_time / s->_cur_task->_svc_time;
				}
			}
			//计算当前队列总共有多少进程


			if (feedbackdl[2]._head->next == NULL) {
				ji++;
				s->_cur_task = NULL;
				continue;
			}
			if (s->_cur_task == NULL || cur_slice == 0) {
				if (feedbackdl[2]._head->next == NULL) {//就绪队列无进程 
					break;
				}
				else {
					dl3_cur_num++;
					s->_cur_task = feedbackdl[2]._head->next;
					cur_srv_time = s->_cur_task->_svc_time_bak;
					if (cur_srv_time > feedbackdl[2].slice)	cur_slice = feedbackdl[2].slice;//选取剩余服务时间和时间片中较小的那个 
					else	cur_slice = cur_srv_time;
				}
			}
			if (cur_slice > 0)	cur_slice--;
			if (cur_srv_time > 0)	cur_srv_time--;
			s->_time++;
		}
		if (ji % 4 == 3)//第四级队列
		{
			bool flag = false;
			int now_count = 0;
			for (int i = 0; i < s->_task_num; i++) {// 如果进程时间到了就把该进程加到就绪队列head后面 
				if (s->_time == s->_arr_task[i]._arv_time) {
					ji = 0;
					flag = true;
					if (s->_cur_task)
					{
						s->_cur_task->_svc_time_bak = cur_srv_time;//更新剩余服务时间 
					}
					s->_cur_task = NULL;
					cur_slice = 0;
					break;
					struct task* cur = s->_head;
					while (cur->next) {
						cur = cur->next;
					}

					cur->next = &s->_arr_task[i];
					s->_cur_num++;


				}
			}
			if (flag)
			{
				continue;
			}
			if (s->_cur_task && cur_slice == 0) {
				if (feedbackdl[3]._head->next != NULL) {//把正在执行的这个进程从就绪队列中删除 
					struct task* now = feedbackdl[3]._head;

					while (now->next) {
						now_count++;
						now = now->next;
					}
					struct task* temp = feedbackdl[3]._head->next;
					if (feedbackdl[3]._head->next->next) {
						feedbackdl[3]._head->next = feedbackdl[3]._head->next->next;
						temp->next = NULL;
					}
					else
					{
						feedbackdl[3]._head->next = NULL;
					}

				}
				if (s->_cur_task)
				{
					s->_cur_task->_svc_time_bak = cur_srv_time;//更新剩余服务时间 
				}
				if (cur_srv_time != 0) {//分给进程的时间片用完了并且进程并未执行完毕，就把这个进程放到下一级队列的末尾 

					//dl4_cur_num++;
					/*if (feedbackdl[3]._head == NULL)
					{
						feedbackdl[3]._head = new struct task;
						feedbackdl[3]._head->next = s->_cur_task;
						cur = feedbackdl[3]._head;
					}

					else {
						while (cur->next) {
							cur = cur->next;
						}
						cur->next = s->_cur_task;
					}*/
					struct task* cur = feedbackdl[3]._head;
					while (cur->next) {
						cur = cur->next;
					}
					cur->next = s->_cur_task;
				}

				else {//计算进程的周转时间等 
					s->_cur_task->_fns_time = s->_time;
					s->_cur_task->_wle_time = s->_cur_task->_fns_time - s->_cur_task->_arv_time;
					if (s->_cur_task->_svc_time == 0)	continue;
					s->_cur_task->_wwle_time = (float)s->_cur_task->_wle_time / s->_cur_task->_svc_time;
				}
			}
			//计算当前队列总共有多少进程

			if (feedbackdl[3]._head->next == NULL)	break;
			if (s->_cur_task == NULL || cur_slice == 0) {
				if (feedbackdl[3]._head->next == NULL) {//就绪队列无进程 
					break;
				}
				else {
					dl4_cur_num++;
					s->_cur_task = feedbackdl[3]._head->next;
					cur_srv_time = s->_cur_task->_svc_time_bak;
					if (cur_srv_time > feedbackdl[3].slice)	cur_slice = feedbackdl[3].slice;//选取剩余服务时间和时间片中较小的那个 
					else	cur_slice = cur_srv_time;
				}
			}
			if (cur_slice > 0)	cur_slice--;
			if (cur_srv_time > 0)	cur_srv_time--;
			s->_time++;
		}
	}
}