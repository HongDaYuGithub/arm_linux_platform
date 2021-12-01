#ifndef __NET_APP__
#define __NET_APP__

#include "../sqlite3/sqlite3.hpp"
#include "../platform/include/platform.hpp"

class app_layer{
public:
    app_layer(){}
    app_layer(json& f,platform_interface* ptr):fp(f),plat_ptr(ptr){}
    void operator()(json& f,platform_interface*p){
        this->fp = f;
        this->plat_ptr = p;
    }
    void app_init();
private:
    json fp;
    Sqlite3 sqlite;
    platform_interface* plat_ptr;
};

#endif