//
// Created by hongyu on 2021/11/21.
//

#ifndef PLATFORM_JSON_H
#define PLATFORM_JSON_H

#include "cJSON.hpp"
#include "cJSON_Utils.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <fcntl.h>
#include <sys/stat.h>
#include <typeinfo>

class file_attr{
public:
    file_attr(){}
    const char* name;
    file_attr(const char*n):name(n){}
    int32_t size(){
        struct stat fs;
        stat(name,&fs);
        return fs.st_size;
    }
};

class json: private file_attr{
public:
    json(){}
    json(const char* n);
    void print();
    void find(const char* node,std::string& value);
    void find(const char* node,int& value);
    void find(const char* node,const char* subnode,std::string& value);
    void find(const char* node,const char* subnode,int& value);
    ~json();
private:
    cJSON* root;
    FILE* fp;
    char* buffer;
};


#endif//PLATFORM_JSON_H
