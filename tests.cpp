#include "ProcessParser.h"
#include <vector>
#include <iostream>
#include <cassert>
#include "Process.h"

int main(int argc, char *argv[] )
{
    
    
    
    std::vector<std::string> pids = ProcessParser::getPidList();
    assert(pids.size() != 0);
    for (std::string& pid : pids)
    {
        std::cout << pid << "\n";
    }

    std::cout << ProcessParser::getCmd(pids[3]) << "\n";



    assert(ProcessParser::isPidExisting(pids[3]) == true);
    
    std::cout << ProcessParser::getSysUpTime() << "\n";

        
    for (int i = -1; i < 4; i++)
    {
        vector<string> cpu_stats;
        if (i == -1)
        {
            cpu_stats = ProcessParser::getSysCpuPercent();
        }
        else
        {
            cpu_stats = ProcessParser::getSysCpuPercent(to_string(i));
        }
        
        for (auto& stat : cpu_stats)
        {
            cout << stat << " " ;
        }
        cout << "\n";        
    }

    cout << ProcessParser::getSysKernelVersion() << "\n";
    cout <<ProcessParser::getOSName() << "\n";
    cout << ProcessParser::getNumberOfCores() << "\n";
    cout << ProcessParser::getProcUpTime(pids[3]) << "\n";
    cout << ProcessParser::getProcUser(pids[3]) << "\n";
    cout << ProcessParser::getVmSize(pids[3]) << "\n";
    cout << ProcessParser::getTotalThreads() << "\n";
    cout << ProcessParser::getTotalNumberOfProcesses() << "\n";
    cout << ProcessParser::getNumberOfRunningProcesses() << "\n";

}
