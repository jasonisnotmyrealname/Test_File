#pragma once
#include <deque>

uint32_t find_max_seq(const char * fpath,const char * ftype);
float GetAvailableSpaceRatio(const char* path);
int get_sorted_filelist(const char * fpath,const char * ftype, std::deque<std::string> &dq);
unsigned long long GetAvailableSpace(const char* path);
