
#ifndef Q_CTASK_H_
#define Q_CTASK_H_

//�̡߳���ʱ�������
class CTask
{
public:
    CTask(void) : m_bDel(true)
    {

    };
    virtual ~CTask()
    {

    };

    void setDel(const bool bDel)
    {
        m_bDel = bDel;
    };
    bool getDel(void)
    {
        return m_bDel;
    };


    /*��Ҫʵ�ֵĽӿ�*/
    virtual void Run(void) = 0;

private:
    bool m_bDel;
};

#endif//Q_CTASK_H_
