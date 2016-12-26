#include <iostream>
#include <vector>
#include <stdlib.h>

using namespace std;

typedef enum {
	READ ,
	RUNING,
	BLOCK,
}ProcessType;



typedef struct {
	char name;
	int data;
	ProcessType type;
}ProcessHead;

typedef struct _PCD
{
	std::vector<ProcessHead *> head;
	int splice;
	int num;
}PCD;

typedef vector<ProcessHead*>::iterator Iterator;

void init(PCD *p)
{
	std::cout<<"please enter process number\n";
	int num;
	std::cin>>num;
	for(int i = 0; i < num; ++i)
	{
		ProcessHead *tmp = (ProcessHead*)malloc(sizeof(ProcessHead));
		if(tmp == NULL)
		{
			num = i;
			std::cout<<"only create "<<i<<"number\n";
			return;
		}
		p->head.insert(p->head.end(),tmp);
	}
	p->num = num;
	int splice = 0;
	std::cout<<"please enter splice time:";
	std::cin>>splice;
	p->splice = splice;
}


void input_process_information(PCD *p)
{
	if(p == NULL)
	{
		std::cout<<"please use init function\n";
		return ;
	}
	int size = p->head.size();
	for(int i = 0; i < size; ++i)
	{
		ProcessHead *tmp = p->head[i];
		cout<<"please enter process name:\n";
		cin>>tmp->name;
		cout<<"please enter process run time\n";
		cin>>tmp->data;
		tmp->type = READ;	
	}
}

void print(PCD *p)
{
    cout<<"**********************************\n";
	if(p == NULL)
	{
		return ;
	}
	Iterator begin = p->head.begin();
	Iterator end = p->head.end();
	for(; begin != end; ++begin)
	{
		cout<<"name	"<<"time "<<" state"<<"\n";
		cout<<(*begin)->name<<"	"<<(*begin)->data<<"     ";
        if((*begin)->type == 0)
        {
            cout<<"READ\n";
        }
        else
        {
            cout<<"RUNING\n";
        }
	}
    cout<<"**********************************\n";
}
void run(PCD *p)
{
	print(p);
    cout<<"the programe run\n";

	int splice = p->splice;
	int size = p->head.size();
	Iterator begin = p->head.begin();
	Iterator end = p->head.end();
	for(; begin < end; )
	{
		if((*begin)->data <= splice)
		{
			(*begin)->data = 0;
			(*begin)->type = RUNING;
			print(p);
			begin = p->head.erase(begin);
		}
		else
		{
			(*begin)->data -= splice;
			(*begin)->type = RUNING;
			print(p);
            (*begin)->type = READ;
			++begin;
		}
        if(begin == end)
        {
            begin = p->head.begin();
            end = p->head.end();
            continue;
        }
        end = p->head.end();
	}
}


int main()
{
    PCD p;
    init(&p);
    input_process_information(&p);
    run(&p);
}
