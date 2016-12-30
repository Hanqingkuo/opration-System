#include <iostream>
#include <vector>
#include <assert.h>
#include <stdlib.h>

using namespace std;

typedef vector<int>::iterator Iterator;
typedef vector<vector<int> >::iterator IteratorVector;
typedef struct _stat
{
	vector<int> Available;
	vector<vector<int> > Max;
	vector<vector<int> > Need;
	vector<vector<int> > Allocation;
    int processCount;
    int sourceCount;

}Stat;

void Init(Stat &s);
bool Safe(Stat &s);
bool Bank(Stat &s);
void Show(Stat &s);

void Init(Stat &s)
{
	int sourceCount;
	int processCount; 
	cout<<"请输入资源个数:\n";
	cin>>sourceCount;
    s.sourceCount = sourceCount;
	cout<<"请输入每个资源数:\n";
	for(int i = 0; i < sourceCount; ++i)
	{
		int source;
		cin>>source;
		s.Available.push_back(source);
	}
	cout<<"请输入进程个数:\n";
	cin>>processCount;
    s.processCount = processCount;
	cout<<"请输入进程对各个资源的最大需求量:\n";
	for(int i = 0; i < processCount; ++i)
	{	
		cout<<"请输入第"<<i+1<<"个进程对各个资源的最大需求量\n";
        vector<int>MaxTmp;
		for(int j = 0; j < sourceCount; ++j)
		{
			int need;
			cin>>need;
            MaxTmp.push_back(need);
		}
        s.Max.push_back(MaxTmp);
	
	}
	cout<<"输入各个进程已占资源数:\n";
	for(int i = 0; i < processCount; ++i)
	{
		cout<<"第"<<i+1<<"进程分配各个资源的数目:\n";
		vector<int>AllocationTmp;
        for(int j = 0; j < sourceCount; ++j)
		{
			int source;
			cin>>source;
			AllocationTmp.push_back(source);
		}
        s.Allocation.push_back(AllocationTmp);
	}
	IteratorVector beginMax =  s.Max.begin();
	IteratorVector beginAlloc = s.Allocation.begin();
	for(int j = 0; beginMax != s.Max.end(); ++beginMax,++beginAlloc,++j)
	{
        vector<int> NeedVector;
		for(int i = 0; i < sourceCount; ++i)
		{
			int tmp = (*beginMax)[i] - (*beginAlloc)[i];
			NeedVector.push_back(tmp);
		}
        s.Need.push_back(NeedVector);
	}
	
	cout<<"初始化完成\n";
}
bool Safe(Stat &s)
{
    int i,j;
    int l = 0;
    bool *finish = (bool *)malloc(sizeof(bool) * s.processCount);
    assert(finish != NULL);
    int *work = (int *)malloc(sizeof(int)*s.sourceCount);
    assert(work != NULL);
    int *processId = (int*)malloc(sizeof(int)*s.processCount);
    assert(processId != NULL);
    for(i = 0; i < s.processCount; ++i)
    {
        finish[i] = false;
        processId[i] = -1;
    }
    for(i = 0; i < s.sourceCount; ++i)
    {
        work[i] = s.Available[i];
    }
    bool possible = true;
    int k = 0;
    cout<<"show work\n";
    while(possible)
    {

        for(i = 0; i < s.processCount; ++i)
        {
            
            if(finish[i] == true)
            {
                ++k;
            }
            else
            {
                for(j = 0; j < s.sourceCount; ++j)
                {
                    if(s.Need[i][j] > work[j])
                    {
                        break;
                    }
                }
                if(j == s.sourceCount)
                {
                   
                   
                    finish[i] = true;
                    for(int m = 0; m < s.sourceCount; ++m)
                    {
                        work[m] += s.Allocation[i][m];
                    }
                    processId[l++] = i;
                }
                else
                {
                    ++k;
                    continue;
                } 
		for(int m = 0; m < s.sourceCount; ++m)
                {
                     cout<<work[m]<<"   ";
                }
                cout<<"\n";
                
            }

        }
        if(k == s.processCount)
        {
            possible = false;
        }
    }
    if(l == s.processCount)
    {
        cout<<"this process Safe Sequence is:\n";
        for(i = 0; i < s.processCount; ++i)
        {
            if(i == s.processCount -1)
            {
                cout<<"p"<<processId[i]<<"\n";
                break;
            }
            cout<<"P"<<processId[i]<<"---->";
        }
        return true;
    }
    else
    {
        cout<<"this process not safe\n";
    }
    return false;
}
bool Bank(Stat &s)
{
    int process;
    vector<int> need;
    int choose = 0;
    cout<<"选择1直接给出安全序列 或者  选择2输入进程ID给出安全序列:\n";
    cin>>choose;
    if(choose == 1)
    {
        if(Safe(s))
        {
            //cout<<"\n"<<"Allocation is:\n";
            /*
            for(int i = 0; i < s.processCount; ++i)
            {
                for(int j = 0; j < s.sourceCount; ++j)
                {
                    cout<<s.Allocation[i][j]<<"   ";
                }
                cout<<"\n";
            }*/
            Show(s);
            return true;
        }
	    else
            return false;
    }  
    cout<<"please enter process need id(from 0 begin):";
    cin>>process;
    if(process >= s.processCount ||  process < 0)
    {
        cout<<"no this process id";
        return false;
    }
    cout<<"please enter this process need for every source:\n";
    for(int i = 0; i < s.Available.size(); ++i)
    {
        int needTmp;
        cin>>needTmp;
        need.push_back(needTmp);
    }
    for(int i = 0; i < s.sourceCount; ++i)
    {
        if(s.Need[process][i] < need[i])
        {
            cout<<"this process need source over MaxNeed";
            return false;
        }
    }
    for(int i = 0; i < s.sourceCount; ++i)
    {
        if(s.Available[i] < need[i])
        {
            cout<<"this process need source more than Available,must wait";
            return false;
        }
    }
    for(int i = 0; i < s.sourceCount; ++i)
    {
        s.Available[i] -= need[i];
        s.Allocation[process][i] += need[i];
        s.Need[process][i] -= need[i];
    }
    if(Safe(s))
    {
        cout<<"source quest success\n";
        Show(s);
        /*
        for(int i = 0; i < s.processCount; ++i)
        {
            for(int j = 0; j < s.sourceCount; ++j)
            {
                cout<<s.Allocation[i][j]<<"   ";
            }
            cout<<"\n";
        }*/
    }
    else
    {
        cout<<"quest faile\n";
        for(int i = 0; i < s.Available.size(); ++i)
        {
            s.Available[i] += need[i];
            s.Need[process][i] += need[i];
            s.Allocation[process][i] -= need[i];
        }
    }
    
}
void Show(Stat &s)
{
    cout<<"Available:\n";
    for(Iterator it = s.Available.begin(); it != s.Available.end(); ++it)
    {
        cout<<*it<<"  ";
    }
    cout<<"\n";
    cout<<"Max need\n";
    for(IteratorVector it = s.Max.begin(); it != s.Max.end(); ++it)
    {
        for(Iterator it1 = it->begin(); it1 != it->end(); ++it1)
        {
            cout<<*it1<<"  ";
        }
        cout<<"\n";
    }
    cout<<"Allocation\n";
    for(IteratorVector it = s.Allocation.begin(); it != s.Allocation.end(); ++it)
    {
        for(Iterator it1 = it->begin(); it1 != it->end(); ++it1)
        {
            cout<<*it1<<"  ";
        }
        cout<<"\n";
    }
    cout<<"need\n";
    for(IteratorVector it = s.Need.begin(); it != s.Need.end(); ++it)
    {
        for(Iterator it1 = it->begin(); it1 != it->end(); ++it1)
        {
            cout<<*it1<<"  ";
        }
        cout<<"\n";
    }
}
int  main()
{
    Stat s;
    Init(s);
    Show(s);
    int choose;
    cout<<"enter 1 continue or enter -1 exit:\n";
    while(cin>>choose && choose != -1)
    {
        Bank(s); 
        cout<<"enter 1 continue or enter -1 exit:\n";
    }
}
