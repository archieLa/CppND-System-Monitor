#include "ProcessParser.h"

string ProcessParser::getCmd(const string& pid)
{
    std::ifstream stream;
    string cmd = "N/A";
    string temp_path = Path::basePath() + pid + Path::cmdPath(); 
    
    Util::getStream(temp_path, stream);
    getline(stream, cmd);
    
    return cmd;
}

vector<string> ProcessParser::getPidList()
{
    vector<string> pids;
    DIR* pids_dir = opendir(Path::basePath().c_str());
    if (!pids_dir)
    {
        throw runtime_error(std::strerror(errno));
    }

    dirent *pids_dir_entry = nullptr;
    while(pids_dir_entry = readdir(pids_dir))
    {
        // If entry is not a dir then it is not a PID
        if (pids_dir_entry->d_type != DT_DIR)
        {
            continue;
        }
        else
        {
            string temp(pids_dir_entry->d_name);
            if (all_of(temp.begin(), temp.end(), [](char c){return isdigit(c);}))
            {
                // PIDS names are all numbers
                pids.push_back(temp);
            }
        }
        
    }
    
    if (closedir(pids_dir) < 0)
    {
        throw runtime_error(std::strerror(errno));   
    }

    return pids;

}

string ProcessParser::getVmSize(string pid)
{
    // TODO
}

string ProcessParser::getCpuPercent(string pid)
{
    //TODO
}

long int ProcessParser::getSysUpTime()
{
    ifstream up_time_stream; 
    Util::getStream((Path::basePath() + Path::upTimePath()), up_time_stream);

    stringstream up_time_sstream;
    up_time_sstream << up_time_stream.rdbuf();
    long int up_time = -1;
    up_time_sstream >> up_time;
    return up_time;
}

string ProcessParser::getProcUpTime(string pid)
{
    //TODO
}

string ProcessParser::getProcUser(string pid)
{
    //TODO
}

vector<string> ProcessParser::getSysCpuPercent(string coreNumber)
{
    ifstream cpu_percent_stream; 
    Util::getStream((Path::basePath() + Path::statPath()), cpu_percent_stream);
    string line_from_file;
    
    while (getline(cpu_percent_stream, line_from_file))
    {
        string cpu_num = "cpu" + coreNumber;
        const int kEqual = 0;
        if (line_from_file.compare(0, cpu_num.length(), cpu_num) == kEqual)
        {
            istringstream helper_iss(line_from_file);
            return vector<string>(istream_iterator<string>(helper_iss), istream_iterator<string>());
        }
    }    
    return vector<string>();
}

float ProcessParser::getSysRamPercent()
{
    //TODO
}

string ProcessParser::getSysKernelVersion()
{
    ifstream kernel_version_stream; 
    Util::getStream((Path::basePath() + Path::versionPath()), kernel_version_stream);
    string line_from_file;
    smatch regex_match;
    regex regex_to_search(R"(Linux version ?(\d+.\d+.\d-\d+-\w+)\s)");

    while(getline(kernel_version_stream, line_from_file))
    {
        if (regex_search(line_from_file, regex_match, regex_to_search))
        {
            return regex_match[1];
        }        
    }
       
    return "";
    
}

int ProcessParser::getNumberOfCores()
{
    ifstream num_cores_stream; 
    Util::getStream((Path::basePath() + "cpuinfo"), num_cores_stream);
    string line_from_file;
    smatch regex_match;
    regex regex_to_search(R"(processor\s*:\s*\d+)");
    int num_of_cores = 0;

    while(getline(num_cores_stream, line_from_file))
    {
        if (regex_search(line_from_file, regex_match, regex_to_search))
        {
            num_of_cores++;
        }        
    }

    return num_of_cores;       
}

int ProcessParser::getTotalThreads()
{
    //TODO
}

int ProcessParser::getTotalNumberOfProcesses()
{
   
}

int ProcessParser::getNumberOfRunningProcesses()
{

}

string ProcessParser::getOSName()
{
    ifstream os_name_stream; 
    Util::getStream("/etc/os-release", os_name_stream);
    string line_from_file;
    smatch regex_match;
    regex regex_to_search(R"(NAME\s*?\=\s*\"(\w+)\")");

    while(getline(os_name_stream, line_from_file))
    {
        if (regex_search(line_from_file, regex_match, regex_to_search))
        {
            return regex_match[1];
        }        
    }
       
    return "";
}

std::string ProcessParser::PrintCpuStats(std::vector<std::string> values1, std::vector<std::string>values2)
{

}

bool ProcessParser::isPidExisting(const string& pid)
{
    vector<string> pids = ProcessParser::getPidList();
    auto pid_exist = find(pids.begin(), pids.end(), pid);
    if (pid_exist != pids.end())
    {
        return true;
    }
    else
    {
        return false;
    }
    
}