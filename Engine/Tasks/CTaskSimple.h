class CTaskSimple : protected CTask
{
public:
    virtual CTask *GetSubTask() const;
    virtual bool IsSimpleTask() const;
    virtual bool SetPedPosition();
protected:
private:

};