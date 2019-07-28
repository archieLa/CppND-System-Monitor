#ifndef SYS_INFO_H_
#define SYS_INFO_H_

#include <string>
#include <iostream>
#include <vector>
#include "ProcessParser.h"

class SysInfo 
{

private:
    std::vector<std::string> last_cpu_stats_;
    std::vector<std::string> current_cpu_stats_;
    std::vector<std::string> core_stats_;
    std::vector<std::vector<std::string>> last_cpu_cores_stats_;
    std::vector<std::vector<std::string>> current_cpu_cores_stats_;
    std::string cpu_percent_;
    float mem_percent_;
    std::string os_name_;
    std::string kernel_ver_;
    long up_time_;
    int total_proc_;
    int running_proc_;
    int threads_;

public:

    SysInfo()
    {
        // Getting initial info about system
        // Initial data for individual cores is set
        // System data is set
        getOtherCores(ProcessParser::getNumberOfCores());
        setLastCpuMeasures();
        setAttributes();
        os_name_ = ProcessParser::getOSName();
        kernel_ver_ = ProcessParser::getSysKernelVersion();
    }

    void setAttributes();
    void setLastCpuMeasures();
    std::string getMemPercent() const;
    long getUpTime() const;
    std::string getThreads() const;
    std::string getTotalProc() const;
    std::string getRunningProc() const;
    std::string getKernelVersion() const;
    std::string getOSName() const;
    std::string getCpuPercent() const;
    void getOtherCores(int _size);
    void setCpuCoresStats();
    std::vector<std::string> getCoresStats() const;
};


void SysInfo::getOtherCores(int _size)
{
    //when number of cores is detected, vectors are modified to fit incoming data
    core_stats_ = std::vector<std::string>();
    core_stats_.resize(_size);
    last_cpu_cores_stats_ = std::vector<std::vector<std::string>>();
    last_cpu_cores_stats_.resize(_size);
    current_cpu_cores_stats_ = std::vector<std::vector<std::string>>();
    current_cpu_cores_stats_.resize(_size);
    
    for(int i=0;i<_size;i++)
    {
        last_cpu_cores_stats_[i] = ProcessParser::getSysCpuPercent(to_string(i));
    }
}

void SysInfo::setLastCpuMeasures()
{
    last_cpu_stats_ = ProcessParser::getSysCpuPercent();
}

void SysInfo::setCpuCoresStats()
{
    // Getting data from files (previous data is required)
    for(int i=0; i < current_cpu_cores_stats_.size(); i++)
    {
        current_cpu_cores_stats_[i] = ProcessParser::getSysCpuPercent(to_string(i));
    }

    for(int i=0; i < current_cpu_cores_stats_.size(); i++)
    {
        // after acquirement of data we are calculating every core percentage of usage
        core_stats_[i] = ProcessParser::printCpuStats(last_cpu_cores_stats_[i], current_cpu_cores_stats_[i]);
    }
    last_cpu_cores_stats_ = current_cpu_cores_stats_;
}

void SysInfo::setAttributes()
{
    // getting parsed data
    mem_percent_ = ProcessParser::getSysRamPercent();
    up_time_ = ProcessParser::getSysUpTime();
    total_proc_ = ProcessParser::getTotalNumberOfProcesses();
    running_proc_ = ProcessParser::getNumberOfRunningProcesses();
    threads_ = ProcessParser::getTotalThreads();
    current_cpu_stats_ = ProcessParser::getSysCpuPercent();
    cpu_percent_ = ProcessParser::printCpuStats(last_cpu_stats_, current_cpu_stats_);
    last_cpu_stats_ = current_cpu_stats_;
    setCpuCoresStats();
}

// Constructing string for every core data display
std::vector<std::string> SysInfo::getCoresStats() const
{
    std::vector<std::string> result;
    for(int i=0; i < core_stats_.size(); i++)
    {
        std::string temp =("cpu" + to_string(i) +": ");
        float check;
        if(!core_stats_[i].empty())
        {
            check = stof(core_stats_[i]);
        }
        if(!check || core_stats_[i] == "nan")
        {
            return std::vector<std::string>();
        }
        temp += Util::getProgressBar(core_stats_[i]);
        result.push_back(temp);
    }
    return result;
}

std::string SysInfo::getCpuPercent() const 
{
    return cpu_percent_;
}

std::string SysInfo::getMemPercent() const 
{
    return to_string(mem_percent_);
}

long SysInfo::getUpTime() const 
{
    return up_time_;
}
std::string SysInfo::getKernelVersion() const 
{
    return kernel_ver_;
}

std::string SysInfo::getTotalProc() const
{
    return to_string(total_proc_);
}

std::string SysInfo::getRunningProc() const 
{
    return to_string(running_proc_);
}

std::string SysInfo::getThreads() const 
{
    return to_string(threads_);
}

std::string SysInfo::getOSName() const
{
    return os_name_;
}

#endif
