#ifndef __SQLITE3__USER
#define __SQLITE3__USER

#include <sqlite3.h>
#include <vector> // 使用动态数组记录查询结果
#include <string> // 查询记录都是使用字符串进行保存

/**
 * @brief sqlite3 的基本使用方法
 * @author HongDaYu
 * @brief  接受无参数构造
 */

int sqlite_common_callback(void*,int,char**,char**);

class Sqlite3{
public:
    Sqlite3(){}
    Sqlite3(const char* path,const char* db_name):db_path(path),db_table(db_name){
        this->callback = sqlite_common_callback;
    }
    void operator()(const char*,const char*);
    void init();
    void sql(const char* _sql); //只能用作常规的增删改三种接口
    void find(const char* _sql);
    bool check_tab(std::string tab);
    void config(std::string file);
    void update_callback(sqlite3_callback call);
    void update_sqlite(); //更新数据库
    ~Sqlite3();
private:
    typedef int (*sqlite3_callback)(void* data,int argc,char** argv,char** azColName);
    //在回调查询的时候进行更新数据库,需要维护的内部指针变量
    sqlite3_callback callback;     
    sqlite3* database;
   const  char* db_table;
   const  char* db_path;
};

#endif