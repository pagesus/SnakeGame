#include "get_conf.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <sstream>

int getIntConf(std::ifstream& input ,const std::string &name, int& val)
{
    int retVal = -1;
    std::string line;
    val = 0;
    while(std::getline(input, line))
    {
        if(std::string::npos != line.find(name))
        {
            std::istringstream is(line);
            std::string tmp;
            is >> tmp >> val;
            retVal = 0;
            break;
        }
    }

    return retVal;
}



