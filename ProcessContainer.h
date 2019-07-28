#ifndef PROCESS_CONTAINER_H_
#define PROCESS_CONTAINER_H

#include "Process.h"
#include <vector>

class ProcessContainer
{

private:
    std::vector<Process> list_;

public:
    ProcessContainer()
    {
        refreshList();
    }
    void refreshList();
    std::string printList();
    std::vector<std::vector<std::string>> getList();
};

void ProcessContainer::refreshList()
{
    std::vector<std::string> pidList = ProcessParser::getPidList();   
    list_.clear();
    for(int i=0; i < pidList.size(); i++){
        Process proc(pidList[i]);
        list_.push_back(proc);
    }
}

std::string ProcessContainer::printList()
{    
    std::string result="";
    for(int i=0; i < list_.size(); i++)
    {
        result += list_[i].getProcess();
    }
    return result;
}

std::vector<std::vector<std::string> > ProcessContainer::getList()
{
    std::vector<std::vector<std::string>> values;
    std::vector<std::string> stringifiedList;
    
    for(int i=0; i < list_.size(); i++)
    {
        stringifiedList.push_back(list_[i].getProcess());
    }
    
    int lastIndex = 0;
    for (int i=0; i < stringifiedList.size(); i++)
    {
        if (i % 10 == 0 && i > 0)
        {
          std::vector<std::string>  sub(&stringifiedList[i-10], &stringifiedList[i]);
          values.push_back(sub);
          lastIndex = i;
        }
        
        if(i == (list_.size() - 1) && (i - lastIndex) < 10)
        {
            std::vector<std::string> sub(&stringifiedList[lastIndex], &stringifiedList[i+1]);
            values.push_back(sub);
        }
    }
    
    return values;

}

#endif