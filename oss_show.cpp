#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/ioctl.h>
#include <iostream>
#include "oss_show.h"


void getTerminalSize(int &cols, int &lines)
{
#ifdef TIOCGSIZE
    struct ttysize ts;
    ioctl(STDIN_FILENO, TIOCGSIZE, &ts);
    cols = ts.ts_cols;
    lines = ts.ts_lines;
#elif defined(TIOCGWINSZ)
    struct winsize ts;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
    cols = ts.ws_col;
    lines = ts.ws_row;
#endif
}
void ossShowObject(const std::vector<std::string> &str, oss_color oc)
{
    int cols, lines;
    getTerminalSize(cols, lines);
    cols = std::max(1, cols);
    int maxLen = 1, tmp = 1;
    for(int i=0; i < str.size(); i++){
        tmp = str[i].length();
        maxLen = std::max(tmp, maxLen);
    }
    maxLen += 2;
    int num = cols / maxLen; 
    int len = cols / num;
    char format[100];
    sprintf(format, "%%-%ds", len);
    if(0 == num)
        num ++;
    for(int i=0; i < str.size(); i++){
        printf(format, str[i].c_str());
        if((i + 1) % num == 0)
            printf("\n");
    }
    printf("\n");
}
