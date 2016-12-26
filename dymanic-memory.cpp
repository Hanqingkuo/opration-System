#include <iostream>
#include <list>
#include <assert.h>
#include <stdlib.h>
using namespace std;

typedef struct _memhead{
	int begin;
	int size;
	int end;
}MemHead;

typedef list<MemHead*>::iterator Iterator;

bool compare(MemHead *a, MemHead *b)
{
	assert(a != NULL);
	assert(b != NULL);
	return a->size < b->size;
}
void init(list<MemHead*>& p)
{
	std::cout<<"please enter block number\n";
	int num;
	cin>>num;
    int i = 0;
	while(i++,num--)
	{
	     MemHead *tmp = (MemHead *)malloc(sizeof(MemHead));
		 if(tmp == NULL)
		 {
			 cout<<"the malloc error\n";
			 exit(1);
		 }
		 cout<<"please enter "<<i<<" block begin address\n";
		 cin>>tmp->begin;
		 cout<<"please enter block size\n";
		 cin>>tmp->size;
		 tmp->end = tmp->begin+tmp->size-1;
		 p.push_back(tmp);
	}
}
int find_ff(list<MemHead*> &p,list<MemHead *>&pWork,int size)
{
	Iterator begin = p.begin();
	Iterator end = p.end();
	for(; begin != end; ++begin)
	{
		if((*begin)->size >= size)
		{
			break;
		}
	}
	if(begin == end)
	{
		cout<<"block not found\n";
		return 0;
	}
	MemHead *tmp = (MemHead*)malloc(sizeof(MemHead));
	assert(tmp != NULL);
	tmp->begin = (*begin)->begin;
	tmp->end = tmp->begin+size-1;
	tmp->size = size;

	(*begin)->begin = tmp->end;
	(*begin)->size =  (*begin)->size - size;
	pWork.push_back(tmp);
	return tmp->begin;
}
int find_bf(list <MemHead*>& p,list<MemHead *>&pWork,int size)
{
	p.sort(compare);
	Iterator begin = p.begin();
	Iterator end = p.end();
	for(; begin != end; ++begin)
	{
		if((*begin)->size >= size)
		{
			break;
		}
	}
	if(begin == end)
	{
		cout<<"the block not found fit size\n";
		return 0;
	}
	MemHead* tmp = (MemHead*)malloc(sizeof(MemHead));
	assert(tmp != NULL);
	tmp->begin = (*begin)->begin;
	tmp->end = tmp->begin+size-1;
	tmp->size = size;

	(*begin)->begin = tmp->end;
	(*begin)->size =  (*begin)->size - size;
	pWork.push_back(tmp);
	return tmp->begin;

}

Iterator find_address(list<MemHead*>& p,int add)
{
	Iterator begin = p.begin();
	Iterator end = p.end();
	for(;begin != end; ++begin)
	{
		if((*begin)->begin == add)
		{
			break;
		}
	}
	if(begin == end)
	{
		cout<<"not exsist address in workspcae \n";
		return end;
	}
	return begin;
}

bool my_realloc(list<MemHead*> &p,list<MemHead *>&pWork,int address)
{
	int addres = address;
	Iterator work = find_address(pWork,addres);
	if(work == pWork.end())
	{
		return false;
	}
	Iterator begin = p.begin();
	Iterator end = p.end();
	Iterator i,j;
	int front = 0;
	int behind = 0;
	for(; begin != end; ++begin)
	{
		if((*begin)->end == (*work)->begin)
		{
			front = 1;
			i = begin;
		}
		if((*begin)->begin == (*work)->end)
		{
			behind = 1;
			j = begin;
		}
	}
	if(!front && !behind)
	{
		MemHead*tmp = (MemHead*)malloc(sizeof(MemHead));
		assert(tmp != NULL);
		tmp->begin = (*work)->begin;
		tmp->end = (*work)->end;
		tmp->size = (*work)->size;
		p.push_back(tmp);
	}
    if(front && behind == 0)
	{
		(*i)->size += (*work)->size;
		(*i)->end  = (*work)->end;	
	}
	if(front == 0 && behind)
	{
		(*j)->begin = (*work)->begin;
		(*j)->size += (*work)->size;
	}
	if(front && behind)
	{
		(*i)->size += ((*work)->size + (*j)->size);
		(*i)->end = (*j)->end;
	}
	MemHead *tmp = *work;
	free(tmp);
	pWork.erase(work);
	return true;	
}

void print_leisure(list<MemHead*>&p)
{
	cout<<"all have "<<p.size()<<" block\n";
	Iterator begin = p.begin();
	Iterator end = p.end();
	for(;begin != end; ++begin)
	{
		cout<<"begin :"<<(*begin)->begin<<"  size :"<<(*begin)->size<<"  end :"<<(*begin)->end<<"\n";
	}
}
void print_work(list<MemHead*>&work)
{
	cout<<"work have "<<work.size()<<" number \n";
	Iterator begin = work.begin();
	Iterator end = work.end();
	for(;begin != end; ++begin)
	{
		cout<<"begin :"<<(*begin)->begin<<"  size :"<<(*begin)->size<<"  end :"<<(*begin)->end<<"\n";
	}
}


int main()
{
	list<MemHead*>leisure;
	list<MemHead*>work;
	init(leisure);
	print_leisure(leisure);
    cout<<"choose 1. fist fit 2. best fit -1.exit\n";
    int choose;
    cin>>choose;
    
    if(choose == 1)
    {
        int size = 0;
        cout<<"please enter you need size \n";
        while(cin>>size,size != -1)
        {
            find_ff(leisure,work,size);
            print_work(work);
            cout<<"please enter you size or enter -1 exit\n";
        }
    }
    else if(choose == 2)
    {
        int size = 0;
        cout<<"please enter you size\n";
        while(cin>>size,size != -1)
        {
            find_bf(leisure,work,size);
            print_work(work);
            cout<<"please enter you size or enter -1 exit\n";
        }
    }
    else
    {
        cout<<"enter error \n";
    }

    int address = 0;
    cout<<"please you need realloc address\n";
    while(cin>>address, address != -1)
    {
        my_realloc(leisure,work,address);
        print_leisure(leisure);
        print_work(work);
        cout<<"please you need realloc address or enter -1 exit\n";
    }
}
