#include "ProcessParser.h"

// 2 static helper functions
static float get_sys_active_cpu_time(const vector<string>& values)
{
    return (stof(values[S_USER]) + stof(values[S_NICE]) + stof(values[S_SYSTEM]) +
            stof(values[S_IRQ]) + stof(values[S_SOFTIRQ]) + stof(values[S_STEAL]) +
            stof(values[S_GUEST]) + stof(values[S_GUEST_NICE]));
}

static float get_sys_idle_cpu_time(const vector<string>& values)
{
    return (stof(values[S_IDLE]) + stof(values[S_IOWAIT]));
}

static vector<string> seperate_strings_by_space(const string& in_string_to_seperate)
{
    istringstream buff(in_string_to_seperate);
    return vector<string>(istream_iterator<string>(buff), istream_iterator<string>());
}


string ProcessParser::getCmd(const string& pid)
{
    std::ifstream stream;
    Util::getStream((Path::basePath() + pid + Path::cmdPath()), stream);
    
    string cmd = "N/A"; 
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

string ProcessParser::getVmSize(const string& pid)
{
    ifstream file_input;
    Util::getStream((Path::basePath() + pid + Path::statusPath()), file_input);
    string line_from_file;
    smatch regex_match;
    regex regex_to_search(R"(VmData:\s*(\d+))");
    while (getline(file_input, line_from_file))
    {
        if(regex_search(line_from_file, regex_match, regex_to_search))
        {
            return regex_match[1];
        }
    }
    return "";

}

string ProcessParser::getCpuPercent(const string& pid)
{
    ifstream in_file;
    Util::getStream((Path::basePath() + pid + "/" + Path::statPath()), in_file);

    string in_line;
    getline(in_file, in_line);

    vector<string> stats = seperate_strings_by_space(in_line);

    const uint8_t kStime = 14;
    const uint8_t kCutime = 15;
    const uint8_t kCstime = 16;
    const uint8_t kStartTime = 21;

    // Retrieve stats based on position and calculate values
    float utime  = stof(getProcUpTime(pid));
    float stime = stof(stats[kStime]);
    float cutime = stof(stats[kCutime]);
    float cstime = stof(stats[kCstime]);
    float start_time = stof(stats[kStartTime]);
    float uptime = getSysUpTime();
    float freq = sysconf(_SC_CLK_TCK);
    float total_time = utime + stime + cutime + cstime;
    float seconds_process_running = uptime - (start_time/freq);
    float cpu_percent = 100 * ((total_time/freq) / seconds_process_running);
    return to_string(cpu_percent);

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

string ProcessParser::getProcUpTime(const string& pid)
{
    ifstream in_file;
    Util::getStream((Path::basePath() + pid + "/" + Path::statPath()), in_file);
    
    string in_line;    
    getline(in_file, in_line);
        
    vector<string> proc_stats = seperate_strings_by_space(in_line);
   
    const uint8_t kUpTimePos = 13;
    return to_string((stof(proc_stats[kUpTimePos])) / sysconf(_SC_CLK_TCK));
      
}

string ProcessParser::getProcUser(const string& pid)
{
    ifstream file_stream; 
    Util::getStream((Path::basePath() + pid + Path::statusPath()), file_stream);
    string line_from_file = "";
    smatch regex_match;
    regex regex_to_search(R"(Uid:\s*(\d*)\s+)");
    string uid = "-1";
    string user = "n/a";

    while(getline(file_stream, line_from_file))
    {
        if (regex_search(line_from_file, regex_match, regex_to_search))
        {
            uid = regex_match[1];
            break;
        }        
    }    
    file_stream.close();
    
    // If we found uid of the process, find the name by search in /etc/passw
    if (uid != "-1")
    {
        Util::getStream("/etc/passwd", file_stream);
        while (getline(file_stream, line_from_file))
        {
            size_t pos = 0;
            if ((pos = line_from_file.find((":x:" + uid))) != string::npos)
            {
                user = line_from_file.substr(0, pos);
            }
        }
    }

    return user;

}

vector<string> ProcessParser::getSysCpuPercent(const string& coreNumber)
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
    const string kMemAvailable = "MemAvailable";
    const string kMemFree = "MemFree";
    const string kBuffers = "Buffers";
    
    const uint8_t kValuePos = 1;

    float mem_available = 0;
    float mem_free = 0;
    float buffers = 0;
    float sys_ram_percent = 0;

    const uint8_t kMemStatsToFind = 3;
    vector<bool> found_mem_stats;

    ifstream in_file;
    Util::getStream((Path::basePath() + Path::memInfoPath()), in_file);

    string in_line;
    while (getline(in_file, in_line))
    {
        vector<string> values;
        
        if (in_line.compare(0, kMemAvailable.size(), kMemAvailable) == 0)
        {
            values = seperate_strings_by_space(in_line);
            mem_available = stof(values[kValuePos]);
            found_mem_stats.push_back(true);
        }

        if (in_line.compare(0, kMemFree.size(), kMemFree) == 0)
        {
            values = seperate_strings_by_space(in_line);
            mem_free = stof(values[kValuePos]);
            found_mem_stats.push_back(true);
        }

        if (in_line.compare(0, kBuffers.size(), kBuffers) == 0)
        {
            values = seperate_strings_by_space(in_line);
            buffers = stof(values[kValuePos]);
            found_mem_stats.push_back(true);
        }

        // No need to check all the lines of the file if req mem stats are found
        if (found_mem_stats.size() == kMemStatsToFind)
        {
            sys_ram_percent =  (100 * (1 - (mem_free / (mem_available - buffers))));               
            break;
        }
    }

    return sys_ram_percent;

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
    
    smatch regex_match;
    regex regex_to_search(R"(processor\s*:\s*\d+)");
    
    int num_of_cores = 0;

    ifstream num_cores_stream; 
    Util::getStream((Path::basePath() + "cpuinfo"), num_cores_stream);
    string line_from_file;
    
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
    
    regex regex_to_search(R"(Threads:\s*(\d+))");
    
    int total_threads = 0;
        
    vector<string> pids = getPidList();
    for (string& pid : pids)
    {
        ifstream in_file;
        Util::getStream((Path::basePath() + pid + Path::statusPath()), in_file);
        string in_line;
        while (getline(in_file, in_line))
        {
            smatch regex_match;
            if (regex_search(in_line, regex_match, regex_to_search))
            {
                total_threads += stoi(regex_match[1]);
                // No need to keep checking entire file
                // Continue with the next pid
                break;
            }       
        }
        in_file.close();        
    }

    return total_threads;
}

int ProcessParser::getTotalNumberOfProcesses()
{
    return getPidList().size();
}

int ProcessParser::getNumberOfRunningProcesses()
{
    regex regex_to_search(R"(procs_running\s+(\d+))");
    int running_process_count = 0;
    
    ifstream in_file;
    Util::getStream((Path::basePath() + "/" + Path::statPath()), in_file);
    
    string in_line;
    while (getline(in_file, in_line))
    {
        smatch regex_match;
        if (regex_search(in_line, regex_match, regex_to_search))
        {
            running_process_count = stoi(regex_match[1]);
            break;
        }
    }
    return running_process_count;
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

std::string ProcessParser::printCpuStats(const std::vector<std::string>& values1, const std::vector<std::string>& values2)
{
    float active_time = get_sys_active_cpu_time(values2) - get_sys_active_cpu_time(values1);
    float idle_time = get_sys_idle_cpu_time(values2) - get_sys_idle_cpu_time(values1);
    float total_time = active_time + idle_time;
    float cpu_active_percent = 100 * (active_time / total_time);
    return to_string(cpu_active_percent);
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