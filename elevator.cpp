
//created by Herbert on Nov 24, 2017

//模拟实现扫描算法   
#include<stdio.h>   
#include<stdlib.h>  
#include<time.h>   
void time_random(int *t)//定义取时间随机数   
{     
    int i,number=0;  
    srand((unsigned) time(NULL)); //用时间做种，每次产生随机数不一样,但还是可能会相同   
    for (i=0; i<20; i++)//取三十个随机数，放入数组t中   
    {     
        number = rand() % 201;//对21取余   
        while(number==0)//若余数为0，则一直取到不为0为止   
            number = rand() % 21; //产生1-20的随机数  
        t[i]=number; //将随机数赋给数组   
    }   
}   
void Print(int *a)//打印数组函数   
{  
    for(int i=0;a[i]!='\0';i++)//打印每一次排序的结果   
        printf("%d ",a[i]);  
    printf("\n");//输个换行符  
}  
void Insert(int *a)//插入排序   
{     
    int j;int k;  
    int temp = 0;   
    for(int i=1;a[i]!='\0';i++)//假设第一个元素已经是有序队列   
    {  
        for(j=0;j<i&&a[j]<a[i];)  
            j++;  
        if(i!=j)  
        {     
            temp = a[i];  
            for(k = i-1;k>=j;k--)  
                a[k+1]=a[k];  
            a[j] = temp;   
        }         
    }   
}  
void change(int *a,int *b)  
{     
    int i,j = 0;  
    for(i =  1;j<15;i++)  
    {  
        if(a[i]!=a[i-1])  
        {  
            b[j]=a[i-1];  
            j++;  
        }     
    }  
    b[15]='\0';  
}  
void run(int *x)  
{     
    int loc ;  
    int avglength;  
    for(loc = 0;loc<15&&x[loc]<100;loc++)  
        ;//空语句，只为 loc++   
    avglength = x[14]*2-x[0]-100;  
    avglength /=15;  
    for(int j = loc;j<15;j++)  
    {     
        printf("\n%3d访问",x[j]);  
        x[j] = '0';  
    }  
    for(int j = loc-1;j>=0;j--)  
    {  
        printf("\n%3d访问",x[j]);  
        x[j] = '0';   
    }  
    printf("\n\n平均寻道长度:%d\n\n",avglength);   
}  
int main()  
{         
    int t[21]={0};  
    int tx[16];  
    time_random(t);  
    Insert(t);  
    printf("磁头在100号磁道上，并向磁道号增加的方向上移动\n");   
    printf("\n15个进程访问磁盘:\n");  
    change(t,tx);  
    Print(tx);   
    //开始运行   
    run(tx);      
    return 0;  
}  
