
#ifndef Q_CTASK_H_
#define Q_CTASK_H_

//�̡߳���ʱ�������
class CTask
{
public:
    CTask(void){};
    virtual ~CTask(){};

    /*��Ҫʵ�ֵĽӿ�*/
    virtual void Run(void) = 0;
};

#endif//Q_CTASK_H_
