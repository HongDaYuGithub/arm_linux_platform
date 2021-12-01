#include "net.hpp"

void app_layer::app_init(){
    
    std::string path,db,config;

    this->fp.find("sqlite3","path",path);
    this->fp.find("sqlite3","db",db);
    this->fp.find("sqlite3","config",config);

    std::cout << "app init start ... ...\n";
    std::cout << path << std::endl;
    std::cout << db << std::endl;
    std::cout << config << std::endl;
    sqlite(path.c_str(),db.c_str());
    sqlite.init(); //开始初始化数据库,获取数据库的句柄
    if(!sqlite.check_tab(db)) //如果数据表不存在,则创建数据表
        sqlite.config(config);
}