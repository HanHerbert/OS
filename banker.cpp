#include <iostream>
using namespace std;
#define MAXPROCESS 50                          /*最大进程数*/
#define MAXRESOURCE 100                          /*最大资源数*/
int AVAILABLE[MAXRESOURCE];                      /*可用资源数组*/
int MAX[MAXPROCESS][MAXRESOURCE];              /*最大需求矩阵*/
int ALLOCATION[MAXPROCESS][MAXRESOURCE];      /*分配矩阵*/
int NEED[MAXPROCESS][MAXRESOURCE];              /*需求矩阵*/
int REQUEST[MAXPROCESS][MAXRESOURCE];          /*进程需要资源数*/
bool FINISH[MAXPROCESS];                          /*系统是否有足够的资源分配*/
int p[MAXPROCESS];                               /*记录序列*/
int m,n;                                      /*m个进程,n个资源*/
void Init();
bool Safe();
void Bank();
int main()
{
      Init();
      Safe();
      Bank();
}
void Init()                  /*初始化算法*/
{
      int i,j;
      cout<<"\t---------------------------------------------------"<<endl;
      cout<<"\t||                                               ||"<<endl;
      cout<<"\t||                   银行家算法                  ||"<<endl;
      cout<<"\t||                                               ||"<<endl;
      cout<<"\t||                 XXXXXXXXXXXXXXXXXXXX          ||"<<endl;
      cout<<"\t||                                               ||"<<endl;
      cout<<"\t||                       2004024031 2004024054   ||"<<endl;
      cout<<"\t---------------------------------------------------"<<endl;
      cout<<"请输入进程的数目:";
      cin>>m;
      cout<<"请输入资源的种类:";
      cin>>n;3
      cout<<"请输入每个进程最多所需的各资源数,按照"<<m<<"x"<<n<<"矩阵输入"<<endl;
      for(i=0;i<m;i++)
      for(j=0;j<n;j++)
      cin>>MAX[i][j];
      cout<<"请输入每个进程已分配的各资源数,也按照"<<m<<"x"<<n<<"矩阵输入"<<endl;
      for(i=0;i<m;i++)
      {
          for(j=0;j<n;j++)
          {
              cin>>ALLOCATION[i][j];
              NEED[i][j]=MAX[i][j]-ALLOCATION[i][j];
              if(NEED[i][j]<0)
              {
                  cout<<"您输入的第"<<i+1<<"个进程所拥有的第"<<j+1<<"个资源数错误,请重新输入:"<<endl;
                  j--;
                  continue;
              }
          }
      }
      cout<<"请输入各个资源现有的数目:"<<endl;
      for(i=0;i<n;i++)
      {
          cin>>AVAILABLE[i];
      }
}
void Bank()                  /*银行家算法*/
{
      int i,cusneed;
      char again;
      while(1)
      {
          cout<<"请输入要申请资源的进程号(注:第1个进程号为0,依次类推)"<<endl;
          cin>>cusneed;
          cout<<"请输入进程所请求的各资源的数量"<<endl;
          for(i=0;i<n;i++)
          {
              cin>>REQUEST[cusneed][i];
          }
          for(i=0;i<n;i++)
          {
              if(REQUEST[cusneed][i]>NEED[cusneed][i])
              {
                  cout<<"您输入的请求数超过进程的需求量!请重新输入!"<<endl;
                  continue;
              }
              if(REQUEST[cusneed][i]>AVAILABLE[i])
              {
                  cout<<"您输入的请求数超过系统有的资源数!请重新输入!"<<endl;

    continue;
              }
          }
          for(i=0;i<n;i++)
          {
              AVAILABLE[i]-=REQUEST[cusneed][i];
              ALLOCATION[cusneed][i]+=REQUEST[cusneed][i];
              NEED[cusneed][i]-=REQUEST[cusneed][i];
          }
          if(Safe())
          {
              cout<<"同意分配请求!"<<endl;
          }
          else
          {
              cout<<"您的请求被拒绝!"<<endl;
              for(i=0;i<n;i++)
              {
                  AVAILABLE[i]+=REQUEST[cusneed][i];
                  ALLOCATION[cusneed][i]-=REQUEST[cusneed][i];
                  NEED[cusneed][i]+=REQUEST[cusneed][i];
              }
          }
          for(i=0;i<m;i++)
          {
              FINISH[i]=false;
          }
          cout<<"您还想再次请求分配吗?是请按y/Y,否请按其它键"<<endl;
          cin>>again;
          if(again=='y'||again=='Y')
          {
              continue;
          }
          break;
          }
}
bool Safe()                                      /*安全性算法*/
{
      int i,j,k,l=0;
      int Work[MAXRESOURCE];                      /*工作数组*/
      for(i=0;i<n;i++)
      Work[i]=AVAILABLE[i];
      for(i=0;i<m;i++)
      {
          FINISH[i]=false;
      }
      for(i=0;i<m;i++)
      {    
          if(FINISH[i]==true)
          {
              continue;
          }
          else
          {
              for(j=0;j<n;j++)
              {
                  if(NEED[i][j]>Work[j])
                  {
                      break;
                  }
              }
              if(j==n)
              { 
                  FINISH[i]=true;
                  for(k=0;k<n;k++)
                  {
                      Work[k]+=ALLOCATION[i][k];
                  }
                  p[l++]=i;
                  i=-1;
              }
              else
              {                  continue; 
              }
          }
          if(l==m)
          {
              cout<<"系统是安全的"<<endl;
              cout<<"安全序列:"<<endl;
              for(i=0;i<l;i++)
              {
                  cout<<p[i];
                  if(i!=l-1)
                  {
                      cout<<"-->";
                  }
              }
              cout<<""<<endl;
              return true;
          }
      }
      cout<<"系统是不安全的"<<endl;
      return false;
}