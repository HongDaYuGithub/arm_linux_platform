#ifndef __SQLITE3__USER
#define __SQLITE3__USER

#include <sqlite3.h>

/**
 * @brief sqlite3 的基本使用方法
 * 
 */
class Sqlite3{
public:
    Sqlite3(char* path,sqlite3* db):db_path(path),database(db){}
    virtual void init();
    virtual void sql(char* _sql);
    ~Sqlite3();
private:
sqlite3* database;
char* db_path;
};

#endif