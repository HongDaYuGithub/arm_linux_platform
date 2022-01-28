#include "ftplib.h"
#include <string>
#include <cstdint>

struct Ftp{
    std::string get_filename;
    std::string login_name;
    std::string login_password;
    std::string ftp_addr; // ip+port IF NOT USE DEFAULT IS 21 PORT
    std::string put_filename;
    Ftp(){}
    Ftp(std::string f,std::string ln,std::string lp,std::string fa,std::string pf):get_filename(f), \
    login_name(ln),login_password(lp),ftp_addr(fa),put_filename(pf){}
    int32_t ftp_get(); //ftp 下载功能 直接获取源文件
    int32_t ftp_put(char* buf,uint32_t size); //ftp 上传采取的数据
};
