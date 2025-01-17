#ifndef PROC_PARSER_H
#define PROC_PARSER_H

#include <algorithm>
#include <iostream>
#include <math.h>
#include <thread>
#include <chrono>
#include <iterator>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cerrno>
#include <cstring>
#include <dirent.h>
#include <time.h>
#include <unistd.h>
#include <regex>
#include "constants.h"
#include "util.h"

using namespace std;

class ProcessParser{
private:
    std::ifstream stream_;
    public:
    static string getCmd(const string& pid);
    static vector<string> getPidList();
    static string getVmSize(const string& pid);
    static string getCpuPercent(const string& pid);
    static long int getSysUpTime();
    static string getProcUpTime(const string& pid);
    static string getProcUser(const string& pid);
    static vector<string> getSysCpuPercent(const string& coreNumber = "");
    static float getSysRamPercent();
    static string getSysKernelVersion();
    static int getNumberOfCores();
    static int getTotalThreads();
    static int getTotalNumberOfProcesses();
    static int getNumberOfRunningProcesses();
    static string getOSName();
    static std::string printCpuStats(const std::vector<std::string>& values1, const std::vector<std::string>& values2);
    static bool isPidExisting(const string& pid);
};

#endif
