//一些文件IO相关的函数
//头文件在px4_file.h中

#include <string.h>
#include <string>
#include <dirent.h>
#include <sys/types.h>
#include <sys/statvfs.h>  //文件容量
#include <deque>
#include <unistd.h>




/*找到文件夹目录下的序号最大的文件名（比如1.jpg,2.jpg....)
1. fpath为文件目录
2. ftype为文件类型，比如“IIQ”或“jpg”
*/
uint32_t find_max_seq(const char * fpath,const char * ftype)
{
	DIR *dir;
	
	struct dirent *ent;
	
	std::string path = std::string(fpath); 
	
	int max_iiq_number = 0;  //默认为0
	
	
	if ((dir = opendir (path.c_str())) != NULL) {
	  	while ((ent = readdir (dir)) != NULL) {

			std::string filename = std::string(ent->d_name);

			int dot_index = filename.find_last_of(".");
			int fn_len = filename.length();

			if (((fn_len - dot_index -1) == 3)) {   //缩小范围:后缀名长度必须为3

				std::string extension = filename.substr(filename.find_last_of(".") + 1);
				std::string filename_wo_ext = filename.erase(filename.find_last_of("."),4);
				
				if ((strcmp(extension.c_str(),ftype) ==0) && \
				(!filename_wo_ext.empty() && filename_wo_ext.find_first_not_of("0123456789") == std::string::npos))   //缩小范围:后缀名ftype
				{
					int number = std::atoi (filename_wo_ext.c_str());
					max_iiq_number = (max_iiq_number < number)? number : max_iiq_number;
				}
			}
		}
	  	closedir (dir);
	} else {
	  	/* could not open directory */
	  	printf("file_function:could not open directory \n");
	  	return 0;
	}	
	return max_iiq_number;
}


//查看文件夹剩余容量比例，比如0.97表示剩余容量为97%
float GetAvailableSpaceRatio(const char* path)
{
  struct statvfs stat;
  float ratio;
  
  if (statvfs(path, &stat) != 0) {
    // error happens, just quits here
    printf("read statvfs error\n");
    return -1;
  }

  // the available size is f_bsize * f_bavail
  float numerator = (stat.f_bavail/1000);  //stat.f_bavail >> 30
  numerator /= 1000;
  numerator /= 1000;
  float denominator = (stat.f_blocks/1000);
  denominator /= 1000;
  denominator /= 1000;
  ratio = numerator /denominator;
  return ratio;
}

//查看文件夹剩余容量,单位Byte
unsigned long long GetAvailableSpace(const char* path)
{
  struct statvfs stat;
  unsigned long long freeSpace  = 0;
  unsigned long long block_size  = 0;
  
  if (statvfs(path, &stat) != 0) {
    // error happens, just quits here
    printf("read statvfs error\n");
    return -1;
  }

  // the available size is f_bsize * f_bavail byte
  block_size = stat.f_bsize;
  freeSpace = block_size * stat.f_bavail;

  return freeSpace;
}


/* 
用途:
	获得dir目录下的文件名称（必须是数字的），并按照升序排列. 
参数:
	fpath:必须是带"/"的路径，比如 /mnt/sata/
	ftype:文件后缀名，必须是3个字符，比如 jpg
	dq:双向的deque，不带后缀名
*/
int get_sorted_filelist(const char * fpath,const char * ftype,std::deque<std::string> &dq)
{
	int ret = -1;
	struct dirent **namelist;
    int n,i;
    
    n = scandir(fpath, &namelist, 0, versionsort);  //按升序扫描dir
    if (n >0 )
    {
		std::string filename;
		int dot_index = 0;
		int fn_len = 0;
        for(i =0 ; i < n; ++i)
        {
			filename = std::string(namelist[i]->d_name);
			dot_index = filename.find_last_of(".");
			fn_len = filename.length();
			std::string fullName = std::string(fpath) + filename;  //文件路径+文件名

			if (((fn_len - dot_index -1) == 3)) {   //缩小范围:后缀名长度必须为3

				std::string extension = filename.substr(filename.find_last_of(".") + 1);
				std::string filename_wo_ext = filename.erase(filename.find_last_of("."),4); //filename经过erase之后，自身就和filename_wo_ext一样了
				
				if ((strcmp(extension.c_str(),ftype) ==0) && \
				(!filename_wo_ext.empty() && filename_wo_ext.find_first_not_of("0123456789") == std::string::npos))   //缩小范围:后缀名ftype
				{
					dq.push_back(filename);
					ret = 0;
					
				}
			}
        }
        free(namelist);
    }
	return ret;
}