#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <time.h>
#include <sys/time.h>
#include "drv_hrt.h"
#include "px4_file.h"

#define PX4_STORAGEDIR "."

static constexpr const char	*LOG_ROOT = PX4_STORAGEDIR "/log";



int main(int argc, char * argv[])
{
	
	//建立文件夹
	char _log_dir[10] {};
	int n = snprintf(_log_dir, sizeof(_log_dir), "%s", LOG_ROOT);

 /* 	if (n >= (int)sizeof(_log_dir)) {
		printf("log path too long (%i) \n", n);
		printf("_log_dir is %s \n", _log_dir);
		//return -1;
	}	
	
	int mkdir_ret = mkdir(_log_dir, S_IRWXU | S_IRWXG | S_IRWXO);
	if(mkdir_ret != 0){
		printf("Can't mkdir %s \n", _log_dir);
		return -1;
	}
	
	//建立文件
	std::string filename;
	int _fd;
	for(int i = 1;i < 1000; i++){

		filename = std::string(_log_dir) + "/" + std::to_string(i) + ".jpg";
		_fd = ::open(filename.c_str(), O_CREAT | O_WRONLY, 0666);
		if (_fd < 0) {
			printf("Can't open log file %s \n", filename.c_str());
			return -1;
		}
		close(_fd);

		filename = std::string(_log_dir) + "/" + std::to_string(i) + ".IIQ";
		_fd = ::open(filename.c_str(), O_CREAT | O_WRONLY, 0666);
		if (_fd < 0) {
			printf("Can't open log file %s \n", filename.c_str());
			return -1;
		}
		close(_fd);
		//::write(_fd, read_ptr, available);
	}  */

	//遍历文件
	printf("find_max_seq begin, time: %lu us\n",hrt_absolute_time());
	uint32_t trigger_seq = find_max_seq(_log_dir,"jpg");
	printf("max iiq seq: %u \n",trigger_seq);
	printf("find_max_seq end, time: %lu us\n",hrt_absolute_time());
	
	return 0;
}
