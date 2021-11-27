//
// Created by hongyu on 2021/11/21.
//

#include "../include/json.hpp"

json::json(const char* n): file_attr(n){
    fp = fopen(name,"r");
    buffer = new char(this->size());
    fread(buffer,this->size(),1,fp);
    root = cJSON_Parse(buffer);
}

json::~json() {
   cJSON_Delete(root); //退出之后,销毁所有的分配的节点的信息
}

void json::print() {
    std::cout << buffer << std::endl;
}

void json::find(const char *node, std::string & value) {
    cJSON* ptr = cJSON_GetObjectItem(root,node);
    if(ptr != nullptr){
       value = std::string(ptr->valuestring);
    }
}

void json::find(const char *node, int & value) {
    cJSON* ptr = cJSON_GetObjectItem(root,node);
    if(ptr != nullptr){
        value = ptr->valueint;
    }
}

void json::find(const char *node, const char *subnode, std::string &value) {
    cJSON* ptr = cJSON_GetObjectItem(root,node);
    if(ptr != nullptr) {
        cJSON *sub_ptr = cJSON_GetObjectItem(ptr, subnode);
        if(sub_ptr != nullptr)
            value = std::string(sub_ptr->valuestring);
    }else{
        std::cout << "Can't find " << node << "->" << subnode << std::endl; //常用的debug 信息
    }
}

void json::find(const char *node, const char *subnode, int &value) {
    cJSON* ptr = cJSON_GetObjectItem(root,node);
    if(ptr != nullptr) {
        cJSON *sub_ptr = cJSON_GetObjectItem(ptr, subnode);
        if(sub_ptr != nullptr)
            value = sub_ptr->valueint;
    }
}