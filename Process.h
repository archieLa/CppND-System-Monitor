#ifndef PROCESS_H_
#define PROCESS_H

#include <string>
#include "ProcessParser.h"

using namespace std;
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process 
{   
    private:
        string pid_;
        string user_;
        string cmd_;
        string cpu_;
        string mem_;
        string up_time_;

    public:
        Process(const string& pid)
        {
            pid_ = pid;
            user_ = ProcessParser::getProcUser(pid);
            cmd_ = ProcessParser::getCmd(pid);
            cpu_ = ProcessParser::getCpuPercent(pid);
            mem_ = ProcessParser::getVmSize(pid);
            up_time_ = ProcessParser::getProcUpTime(pid);
        }
    
    void setPid(int pid)
    {
        pid_ = to_string(pid);    
    }

    string getPid() const
    {
        return pid_;
    }

    string getUser() const
    {
        return user_;
    }
    
    string getCmd() const
    {
        return cmd_;
    }
    
    float getCpu() const
    {
        return stof(cpu_);
    }
    
    float getMem() const
    {
        return stof(mem_);
    }

    string getUpTime() const
    {
        return up_time_;
    }

    string getProcess()
    {
        if(!ProcessParser::isPidExisting(pid_))
        {
            return "";
        }

        mem_ = ProcessParser::getVmSize(pid_);
        up_time_ = ProcessParser::getProcUpTime(pid_);
        cpu_ = ProcessParser::getCpuPercent(pid_);
    
        return (pid_ + " "
                + user_ + " "
                + mem_.substr(0, 5) + " "
                + cpu_.substr(0, 5) + " "
                + up_time_.substr(0, 5) + " "
                + cmd_.substr(0, 30) + "...");
    }

};


#endif