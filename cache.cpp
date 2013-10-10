#include<iostream>
#include<fstream>
#include<cmath>
#include<cstdlib>
#include<ctime>
typedef struct node
{
        int p[32];
        struct node *next;
}Node;
using namespace std;
int main()
{
    int M,S,L,A,K,column,row,cycle=-1,index_num,i,n,j,h,amove,correct,ii,comp,num,timeall,s9=0;
    double columnt;
    char test2[20];
    Node *temp,*sat,*sa;
    ifstream bfin("cacheA.org");
    ofstream fcout("index.rpt");
    ofstream fout("ii.rpt");
    bfin>>test2;
    while(test2[0]!='M')
        bfin>>test2;
    bfin>>M;
    while(s9!=6)
        {
        bfin>>test2;
        s9++;
        }
    bfin>>S;
    bfin>>test2;
    while(test2[0]!='L')
        bfin>>test2;
    bfin>>L;
    bfin>>test2;
    while(test2[0]!='A')
        bfin>>test2;
    bfin>>A;
    K=S/L/A;
    K=(int)log2(K);
    row=M*A+A;
    h=M-K;                                           //常琌CACHEΑ 
    column=(int)pow(2,(double)K);
    char test1[M];
    ifstream fin("DataReference_real_up.lst");
    fin>>test1;
    while(test1[0]!='0'&&test1[0]!='1')
        {
        if(test1[1]=='b')
            {
            fout<<test1<<" ";
            fin>>test1;
            fout<<test1<<endl;
        }
        fin>>test1;
        }
    sa=(Node *)malloc(sizeof(Node));
    num=0;
    while(test1[1]!='e')
    {
        for(i=0;i<M;i++)
            {
            sa->p[i]=test1[i]-'0';
            }
        temp=(Node *)malloc(sizeof(Node));
        temp->next=sa;
        sa=temp;
        fin>>test1;
        num+=1;
    }
    sat=sa->next;                                    //琌块INSTRUCTION纗(LINK LIST) 
    float D_Q[M],D_dif,D_cor;
    for(i=0;i<M;i++)
    {
        D_cor=0;
        sa=sat;
        temp=sat;
        for(j=0;j<num;j++)
        {
            if(sa->p[i]==1)
                D_cor+=1;
            temp=temp->next;
            sa=temp;
        }
        D_dif=num-D_cor;
        D_Q[i]=min(D_cor,D_dif)/max(D_cor,D_dif);
    }                                                //ミ程ㄎてQ_TABLE 
    int m;
    float D_C[M][M],D2_cor,D2_dif;
     for(i=0;i<M;i++)
    {    
        for(j=0;j<M;j++)
        {
            D_C[i][j]=0;
        }
    }
    sa=sat;
    temp=sat;
    for(i=0;i<M;i++)
    {    
        for(j=i+1;j<M;j++)
        {
            D2_cor=0;
            sa=sat;
            temp=sat;
            for(m=0;m<num;m++)
            {
                if(sa->p[i]==sa->p[j])
                D2_cor+=1;
                temp=temp->next;
                sa=temp;
            }
            D2_dif=num-D2_cor;
            D_C[i][j]=min(D2_cor,D2_dif)/max(D2_cor,D2_dif);
            D_C[j][i]=D_C[i][j];
        }
    }                                              //ミ程ㄎてC_TABLE 
    int rank[M],ranknum;
    float pt;
    ranknum=0;
    pt=D_Q[0];
    for(i=0;i<M;i++)
    {
        if(pt<=D_Q[i])
        {    
            pt=D_Q[i];
            ranknum=i;
        }
    }
    rank[0]=ranknum;
    D_Q[ranknum]=-1;
    for(i=1;i<M;i++)
    {
        for(j=0;j<M;j++)
            D_Q[j]=(D_Q[j])*(D_C[rank[i-1]][j]);  
        ranknum=0;
        pt=D_Q[0];
        for(m=0;m<M;m++)
        {
            if(pt<=D_Q[m])
            {    
                pt=D_Q[m];
                ranknum=m;
            }
        }
        rank[i]=ranknum;
        D_Q[ranknum]=-1;
    }                                                //ミ程ㄎてRANK_TABLE 
    ifstream afin("DataReference_real_up.lst");
    int save[column][row],a[M],mm,misscount=0;
    char test[M];
    fcout<<"# Student ID"<<endl;
    fcout<<"I 9721103"<<endl;
    fcout<<"# Cache Organization"<<endl;
    fcout<<"M "<<M<<endl;
    fcout<<"S "<<S<<endl;
    fcout<<"L "<<L<<endl;
    fcout<<"A "<<A<<endl;
    fcout<<"# Indexing bits count"<<endl;
    fcout<<"K "<<K<<endl;
    fcout<<"# Indexing bits, from 0 to M-1,"<<endl;
    fcout<<"# msb...lsb"<<endl;
    fcout<<"D ";
    for(i=0;i<K;i++)
        fcout<<M-1-rank[i]<<" ";
    fcout<<endl;
    for(i=0;i<column;i++)
    {
        for(j=0;j<row;j++)
            save[i][j]=2;
    }
    for(i=0;i<row;i++)
    {
        for(j=0;j<column;j++)
            save[j][i]=-1;
        i+=M;
    }                                                //块惠―块家Α 
    afin>>test;
    while(test[0]!='0'&&test[0]!='1')
        afin>>test;
    cycle+=1;
    while(test[1]!='e')
    {
        for(i=0;i<M;i++)
            a[i]=test[i]-'0';
        n=0;
        index_num=0;
        for(i=0;i<K;i++)
        {
            index_num+=a[rank[i]]*((int)pow(2,(double)n));
            n+=1;
        }
        for(i=0;i<row;i++)
        {
            amove=0;
            correct=1;
            for(ii=i+1;ii<=i+M;ii++)
            {
                if(save[index_num][ii]!=a[amove])
                    correct=0;
                amove++;
            }
            if(correct==1)
            {
                for(mm=0;mm<M;mm++)
                    fout<<a[mm];
                    fout<<"  ";
                fout<<"hit"<<endl;
                save[index_num][i]=cycle;
                break;
            }
            i+=M;
        }
        if(correct==0)
        {
            comp=0;
            for(i=0;i<row;i++)
            {   
                if(save[index_num][i]<save[index_num][comp])
                    comp=i;
                i+=M;
            }
            save[index_num][comp]=cycle;
            j=0;
            for(i=comp+1;i<=comp+(M);i++)
                {
                save[index_num][i]=a[j];
                j++;
                }
        for(mm=0;mm<M;mm++)
            fout<<a[mm];
        fout<<"  ";
        fout<<"miss"<<endl;
        misscount+=1;
        }                                                   //碞琌耞MISS  OR  HIT 
    afin>>test;
    cycle+=1;
    }
    fout<<".end";
    fcout<<"# total cache miss"<<endl;
    fcout<<"F "<<misscount<<endl;
    timeall=clock();
    fcout<<"#CPU time(ms)"<<endl;
    fcout<<"T "<<timeall<<endl;
    char repri[40];
    ifstream dfin("ii.rpt");
    dfin>>repri;
    fcout<<repri<<" ";
    fout.close();
    while(repri[1]!='e')
    {
        dfin>>repri;
        
        fcout<<repri<<endl;
        dfin>>repri;
        fcout<<repri<<" ";
    }
                            
    system("pause");
}
    
