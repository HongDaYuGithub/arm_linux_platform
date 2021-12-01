#include "sqlite3.hpp"
#include "../platform/include/platform.hpp"

int sqlite_common_callback(void* data,int argc,char** argv,char** Col){ //简单打印查询信息
    for(int i = 0 ; i < argc ; i++){
        std::cout << argv[i] << std::endl;
    }
}

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

void Sqlite3::find(char* _sql){
    try{
        char* errMsg;
        int ret = sqlite3_exec(database,_sql,callback,NULL,&errMsg);
        if(ret < 0)
            throw errMsg;
    }catch(char* e){
        std::cout << e << std::endl;
    }
}

void Sqlite3::update_callback(sqlite3_callback call){
    this->callback = call; //重新更新callback的调用操作
}