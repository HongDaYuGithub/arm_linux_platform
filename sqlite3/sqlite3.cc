#include "sqlite3.hpp"
#include "../platform/include/platform.hpp"

void Sqlite3::init(){
    try{
        int ret = sqlite3_open(db_path,&database);
        if(ret < 0)
            throw sqlite3_errstr(ret);
        
    }catch(const char* error){
        std::cout << "db name: " << db_path <<" error: " << error << std::endl;
    }
}

void Sqlite3::sql(char* _sql){
    try{
        char* errmsg;
        int ret = sqlite3_exec(database,_sql,NULL,NULL,&errmsg);
        if(ret < 0)
            throw errmsg;
    }catch(char* e){
        std::cout << e << std::endl;
    }
}

Sqlite3::~Sqlite3(){
    sqlite3_close(database); //关闭数据库
}