/*
	created by herbert on 10 Nov
 */

#include <iostream>
#include <queue>
#include <algorithm>
#include <cstdio>
#include <iomanip>
using namespace std;

//进程有三种状态，这里增加一种，表示虽然输入，但还没有到达进入系统时刻
typedef enum ProcessState{Executing, Ready, Finish, Unarrive} STATE;

//进程控制块PCB的定义
typedef struct process_pcb{
    int ID;//进程标识
    int priority;//进程优先数，值越大，优先级越高
    int arrive_time;//进程到达时间，以时间片为单位
    int service_time;//进程需要总的服务时间，以时间片为单位
    int start_time;//进程开始执行时间，以时间片为单位
    int end_time;//进程结束时间，以时间按片为单位
    int all_time;//进程仍然需要的运行时间，以时间片为单位
    int cpu_time;//进程已占用CPU时间，以时间片为单位
    STATE state;//进程状态
}PCB;

const char* StateString[] = {"Executing","Ready", "Finish", "--"};//用于打印三种状态
PCB * running_process = NULL;//当前运行任务
vector<PCB>input_queue;//输入队列，存放到达时刻大于当前时刻的进程
vector<PCB>ready_queue;//就绪队列
vector<PCB>finish_queue;//完成队列

bool cmp_arrive_time(const PCB a, const PCB b);//输入队列按进程到达时间排序函数
bool cmp_priority(const PCB a, const PCB b);//就绪队列按进程优先数排序函数
void input_process();//输入进程信息
int select_policy();//选择进程调度策略
void print_all(int current);//打印所有进程信息

void FCFS();//先来先服务算法
void round_robin();//时间片轮转算法
void dynamic_prio();//动态优先级算法

int main(){
    cout<<"========================================================\n";
    cout<<"                  模拟实现进程调度功能\n";
    cout<<"========================================================\n";

    input_process();//输入进程信息

    print_all(-1);//-1表示打印所有进程的初始状态

    int policy = select_policy();
    switch(policy){
        case 1:
            FCFS();
            break;
        case 2:
            round_robin();
            break;
        case 3:
            dynamic_prio();
            break;
        default:
            FCFS();
            break;
    }
    
    return 0;
}

void input_process(){//输入进程信息
    int num;
    cout<<"请输入进程数量： ";
    cin>>num;
    PCB pro;//定义进程的进程控制块
    for(int i=0; i<num;i++){
        cout<<"请输入第"<<i<<"个进程的到达时间、服务时间及优先级（以空格隔开）： ";
        cin>>pro.arrive_time>>pro.service_time>>pro.priority;
        pro.ID = i;
        pro.all_time = pro.service_time;
        pro.cpu_time = 0;
        pro.start_time = -1;//开始及几万数时间默认wier-1,表示进程尚未被调度过
        pro.end_time = -1;
        pro.state = Unarrive;//初始化为尚未进入系统
        input_queue.push_back(pro);
    }
    sort(input_queue.begin(),input_queue.end(),cmp_arrive_time);//按到达时间升序排队
}

int select_policy(){//选择进程调度策略
    cout<<"请选择调度算法(输入数字1、2、3选择)：  \n";
    cout<<"1.先来先服务算法(FCFS)              \n";
    cout<<"2.时间片轮转法(Round-robin)         \n";
    cout<<"3.动态优先级调度算法(DynamicPriority)\n";

    int n;

    cout<<"请选择调度算法序号： ";
    while(scanf("%d", &n) == 1){
        if(n>3||n<1)
            cout<<"对不起，输入有误，请重新输入！\n";
        else
            break;
    }

    return n;
}

void FCFS(){//先来先服务调度算法
    int chip=0;//初始的时间片为0
    bool need_schedule = true;//需要调度的标志，默认为true

    while(1){//如果当前没有正在运行进程，并且输入队列和就绪队列都为空，则所有进程完成
        if(!running_process && input_queue.empty() && ready_queue.empty())
            break;

        while(!input_queue.empty()){//将到达时间小于等于当前时间片的进程从输入队列移到就绪队列中
            PCB pro = input_queue[0];
            if(pro.arrive_time <= chip){
                pro.state = Ready;
                ready_queue.push_back(pro);//放入就绪列队尾
                input_queue.erase(input_queue.begin()+0);//从输入队列中删除
            }
            else{
                break;
            }
        }
        if(need_schedule && !ready_queue.empty()){//判断是否需要调度，如果需要就从就绪队列取出队首进程进行调度
            running_process = new PCB;
            *running_process = ready_queue[0];//取出就绪队首进程
            ready_queue.erase(ready_queue.begin()+0);//从就绪队列中删除之
    
            running_process->start_time = chip;//调度进程开始运行
            running_process->state = Executing;
            need_schedule = false;
        }

        print_all(chip);//打印当前时刻所有进程的信息
        
        if(running_process){//当前运行任务完成一个时间片，更改其信息
            running_process->cpu_time += 1;
            running_process->all_time -= 1;
            if(running_process->all_time == 0){//任务运行结束
                running_process->end_time = chip+1;
                running_process->state = Finish;
                finish_queue.push_back(*running_process);//将其放入完成队列
                delete running_process;
                running_process = NULL;
                need_schedule = true;
            }
            else{
                need_schedule = false;
            }
        }
        chip+=1;
    }
    print_all(chip);  
}

bool cmp_arrive_time(const PCB a, const PCB b){//按进程到达时间升序排列
    return a.arrive_time < b.arrive_time;
}

bool cmp_priority(const PCB a, const PCB b){
    if(a.priority != b.priority){
        return a.priority > b.priority;
    }
    else{
        return a.arrive_time < b.arrive_time;
    }
}

void print_process(PCB * pro){//打印单个进程信息
    if(pro == NULL){
        return;
    }
    printf("%4d%10d%10d%6d%15s",pro->ID,pro->arrive_time,pro->service_time,pro->priority,StateString[pro->state]);
    //如果进程尚未开始，则开始时间、结束时间以及剩余时间以”--“来表示
    //如果进程已经开始，但尚未结束，则其时间以”--“表示
    if(pro->start_time == -1){
        cout<<setw(10)<<"--"<<setw(10)<<"--"<<setw(10)<<"--";
    }
    else{
        if(pro->end_time == -1){
            cout<<setw(10)<<pro->start_time<<setw(10)<<"--"<<setw(10)<<pro->all_time;
        }
        else{
            cout<<setw(10)<<pro->start_time<<setw(10)<<pro->end_time<<setw(10)<<pro->all_time;
        }
    }

    if(pro->state == Finish){//进程结束，统计其周转时间及加权周转时间
        cout<<setw(10)<<pro->end_time - pro->start_time<<setw(10)<<(float)(pro->end_time - pro->arrive_time)/(float)pro->service_time<<endl;
    }
    else{
        cout<<setw(10)<<"--"<<setw(10)<<"--"<<endl;
    }
}

void print_all(int current){//打印所有进程的信息，-1为打印进程初始输入状态
    if(current == -1){
        cout<<"\n进程初始状态： \n";
    }
    else{
        cout<<"\n当前时刻为： "<<current<<endl;
    }
    printf(" 进程号  到达时间 服务时间  优先级     状态       开始时间   结束时间   剩余时间   周转时间 带权周转时间\t\n");
    //打印正在运行的进程
    if(running_process != NULL){
        print_process(running_process);
    }
    vector<PCB>::iterator it;
    
    for(it = ready_queue.begin();it != ready_queue.end();it++){//打印就绪队列中的进程
        print_process(&(*it));
    }

    for(it = finish_queue.begin();it != finish_queue.end();it++){//打印完成队列中的进程
        print_process(&(*it));
    }

    for(it = input_queue.begin();it != input_queue.end();it++){//打印仍然在输入队列中的进程
        print_process(&(*it));
    }
    cout<<"\n";
}

void round_robin(){//时间片轮转法
    int chip=0;
    bool need_schedule = true;//初始时间片为0
    while(1){//如当前无运行进程，同时输入队列和就绪队列都为空，则所有进程完成
        if(!running_process && input_queue.empty() && ready_queue.empty()){
            break;
        }
        while(!input_queue.empty()){//将到达时间小于等于当前时间片的进程从输入队列移到就绪队列中
            PCB pro = input_queue[0];
            if(pro.arrive_time <= chip){
                pro.state = Ready;
                ready_queue.push_back(pro);
                input_queue.erase(input_queue.begin()+0);
            }
            else
                break;
        }

        if(need_schedule && !ready_queue.empty()){//判断是否需要调度，如需要则取出就绪队列队首进程进行调度
            running_process = new PCB;
            *running_process = ready_queue[0];//从就绪队列中取出
            ready_queue.erase(ready_queue.begin()+0);//从就绪队列中删除
            if(running_process->start_time == -1){//首次运行
                running_process->start_time = chip;
            }
            running_process->state = Executing;
            need_schedule = false;
        }
        print_all(chip);//打印当前时刻所有进程的信息
        
        if(running_process){//当前运行任务完成一个时间片，更改其信息
            running_process->cpu_time += 1;
            running_process->all_time -= 1;
            if(running_process->all_time == 0){//任务运行结束
                running_process->end_time = chip+1;
                running_process->state = Finish;
                finish_queue.push_back(*running_process);//将其放入完成队列
                delete running_process;
                running_process = NULL;
                need_schedule = true;
            }
            else{
                if(!ready_queue.empty()){
                    running_process->state = Ready;
                    ready_queue.push_back(*running_process);//将其放回就绪队列中
                    delete running_process;
                    running_process=NULL;
                    need_schedule = true;
                }
                else{
                    need_schedule = false;
                }
            }
        }
        chip+=1;
    }
    print_all(chip);  
}

void dynamic_prio(){
    int chip=0;//初始的时间片为0
    bool need_schedule = true;//需要调度的标志，默认为true

    while(1){//如果当前没有正在运行进程，并且输入队列和就绪队列都为空，则所有进程完成
        if(!running_process && input_queue.empty() && ready_queue.empty())
            break;

        while(!input_queue.empty()){//将到达时间小于等于当前时间片的进程从输入队列移到就绪队列中
            PCB pro = input_queue[0];
            if(pro.arrive_time <= chip){
                pro.state = Ready;
                ready_queue.push_back(pro);//放入就绪列队尾
                input_queue.erase(input_queue.begin()+0);//从输入队列中删除
            }
            else{
                break;
            }
        }
        if(!ready_queue.empty()){
            sort(ready_queue.begin(),ready_queue.end(),cmp_priority);//按优先级降序排列
        }
        if(need_schedule && !ready_queue.empty()){//判断是否需要调度，如果需要就从就绪队列取出队首进程进行调度
            running_process = new PCB;
            *running_process = ready_queue[0];//取出就绪队首进程
            ready_queue.erase(ready_queue.begin()+0);//从就绪队列中删除之
    
            running_process->start_time = chip;//调度进程开始运行
            running_process->state = Executing;
            need_schedule = false;
        }

        print_all(chip);//打印当前时刻所有进程的信息
        
        if(running_process){//当前运行任务完成一个时间片，判断任务是否完成，更改其信息
            running_process->cpu_time += 1;
            running_process->all_time -= 1;
            if(running_process->all_time == 0){//任务运行结束
                running_process->end_time = chip+1;
                running_process->state = Finish;
                finish_queue.push_back(*running_process);//将其放入完成队列
                delete running_process;
                running_process = NULL;
                need_schedule = true;
            }
            else{//任务未完成，如果就绪队列仍有任务，且优先级大于本任务优先级，则轮转调度，否则不调度
                if(running_process->priority > 1){
                    running_process -> priority -= 1;
                }
                if(ready_queue.empty() && ready_queue[0].priority > running_process->priority){
                    running_process->state = Ready;
                    ready_queue.push_back(*running_process);//将其放回就绪队列
                    delete running_process;
                    running_process = NULL;
                    need_schedule = true;
                }
                else{
                    need_schedule = false;
                }
            }
        }
        chip+=1;
    }
    print_all(chip);  
}