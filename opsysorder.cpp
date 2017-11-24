//created by Herbert on Nov 24,2017
/*
C语言 磁盘调度模拟
    什么是磁盘调度？
            磁盘调度指待的是访问的磁道时，当前磁头访问的方式（也指待是算法）。
        磁盘的访问也是一种I/O设备的访问，在数据访问中需要知道如何去访问从内存以及I/O传输过来的数据。
        怎么样去存储或者是读取在磁盘上面的数据。磁盘的调度的算法有很多种，例如：
            FIFO:先进先出的调度策略，这个策略具有公平的优点，因为每个请求都会得到处理，并且是按照接收到的顺序进行处理。
            SSTF：选择使磁头从当前位置开始移动最少的磁盘I/O请求，所以SSTF总是选择导致最小寻道时间的请求。
            SCAN:SCAN要求磁头仅仅沿一个方向移动，并在途中满足所有未完成的请求，知道它到达这个方向上的最后一个磁道，或者在这个方向上没有其他请求为止。
            C-SCAN:把扫描限定在一个方向，当访问到某个方向的最后一个磁道时，磁道返回磁盘相反方向磁道的末端，并再次开始扫描。
模拟内容
    编程序模拟磁盘移臂调度,假定磁盘有200个柱面,编号为0~199,当前存取臂的位置在143号柱面上，并刚刚完成了128号柱面的服务请求,
如果请求的先后顺序为：86,145,93,179,95,150,103,176,132。试分别采用：
(1) 先来先服务寻道算法FCFS。(2) 最短寻道时间优先算法SSTF。(3) 扫描算法SCAN。(4) 循环扫描算法CSCAN。输出存取臂的移动顺序，并分别计算出存取臂的移动总量。
*/
//算法实现：


#include <stdio.h>
#include <math.h>

void fcfs(int f[],int );
void sstf(int m[],int );
void scan(int d[],int);
void cscan(int e[],int);
void sort(int a[],int);

int main()
{
     printf("模拟磁盘移臂调度程序\n");
     printf("***********************************************\n");
     printf("***********************************************\n");
     int now=143;
     printf("start: %d\n",now);

     

     int a[]={86,145,93,179,95,150,103,176,132};
     fcfs(a,now);
     int aa[]={86,145,93,179,95,150,103,176,132};
     sstf(aa,now);
     int aaa[]={86,145,93,179,95,150,103,176,132};
     scan(aaa,now);
     int aaaa[]={86,145,93,179,95,150,103,176,132};
     cscan(aaaa,now);

     return 0;
}

void fcfs(int f[],int fnow){//先来先服务
     int s=0,i;
     printf("先来先服务寻道顺序为：");
     for(i=0;i<9;i++)
          {
               s=s+fabs(f[i] - fnow);
               printf("%d ",f[i]);
          }
     printf("\nFIFO和为:%d\n",s);
}

void sstf(int m[],int mnow){//最短寻道时间优先
     int n,k,tmp,x,sum=0;
     printf("最短寻道时间优先服务顺序：");
     for(n=0;n<9;n++){
          tmp = fabs(m[0]-mnow);
          x = 0;
          for(k=1;k<9-n;k++){
               if(fabs(m[k]-mnow)<tmp){
                    x = k;
                    tmp = fabs(m[k]-mnow);
               }
          }
          printf("%d ",m[x]);
               sum =sum +tmp;
               mnow = m[x];
               for(k=0;k<9-x-n;k++){
                    m[x+k]=m[x+k+1];
               }
     }
     printf("\nSSTF道数和为：%d\n",sum);
}

void scan(int d[],int dnow){//扫描算法
     int x,y,sum=0;
     printf("扫描服务顺序：");
     sort(d,9);
     for(x = 0; x < 9; x++){
          if(d[x] > dnow){
               y = x;
               break;
          }
     }
     for(x = y; x < 9; x++){
          printf("%d ",d[x]);
          sum = sum + fabs(dnow-d[x]);
     }
     for(x = y-1;x>=0;x--){
          printf("%d ",d[x]);
          sum = sum + fabs(dnow-d[x]);
     }
     printf("\nSCAN道数和为：%d\n",sum);
}

void cscan(int e[],int enow){//循环扫描
     int x,y,sum =0;
     sort(e,9);
     printf("循环扫描顺序为：");
     for(x = 0; x < 9; x++){
          if(e[x] > enow){
               y = x;
               break;
          }
     }
     for(x = y; x < 9; x++ ){
          printf("%d ",e[x]);
          sum = sum + fabs(enow-e[x]);
     }
     for(x = 0;x < y; x++){
          printf("%d ",e[x]);
          sum = sum + fabs(enow-e[x]);
     }
     printf("\n电梯算法道数和为：%d\n",sum);

}

void sort(int a[],int n){
    int i,j,tmp;
    for(i=0;i<n-1;i++){
        for(j=0;j<n-i-1;j++){
            if(a[j]>a[j+1]){
                tmp=a[j+1];
                a[j+1]=a[j];
                a[j]=tmp;                //做数据交换位置处理
         }
     }
   }
} 