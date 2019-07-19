#include "ProcessParser.h"

string ProcessParser::getCmd(const string& pid)
{
    std::ifstream stream;
    string cmd = "N/A";
    string temp_path = Path::basePath() + pid + Path::cmdPath(); 
    try
    {
        Util::getStream(temp_path, stream);
        getline(stream, cmd);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return cmd;
}