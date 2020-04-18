#ifndef GET_CONF_H
#define GET_CONF_H
#include <stdio.h>
#include <fstream>
typedef struct global_data_tag
{
    int is_color_mode;      //0 for non-color mode, 1 for color-mode
    int top_records[3];     //records top 3 score
}global_data;
int getIntConf(std::ifstream& input ,const std::string &name, int& val);
#endif