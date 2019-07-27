#include <string>

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
        string upTime_;

    public:
        Process(const string& pid)
        {
            pid_ = pid;
            user_ = ProcessParser::getProcUser(pid);
            cmd_ = ProcessParser::getCmd(pid);
            cpu_ = ProcessParser::getCpuPercent(pid);
            mem_ = ProcessParser::getVmSize(pid);
            upTime_ = ProcessParser::getProcUpTime(pid);
        }
    

    string getPid() const;
    string getUser() const;
    string getCmd() const;
    
    int getCpu() const;
    int getMem() const;
    string getUpTime() const;
    string getProcess() ;
};


string Process::getPid() const 
{
    return this->pid;
}

string Process::getUser() const
{
    return user_;
}


string Process::getProcess(){
    if(!ProcessParser::isPidExisting(this->pid))
    {
        return "";
    }

    this->mem = ProcessParser::getVmSize(this->pid);
    this->upTime = ProcessParser::getProcUpTime(this->pid);
    this->cpu = ProcessParser::getCpuPercent(this->pid);

    return (this->pid + "   " + //TODO: finish the string! this->user + "   "+ mem...cpu...upTime...;
}
