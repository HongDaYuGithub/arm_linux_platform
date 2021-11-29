#include "../../platform/include/platform.hpp"
#include "fpga.hpp"

extern xilinx xilinx_platform;

void zu6cg::config(json& fp){
    fp.find("fpga","version",version);
}

void zu6cg::init_class(){
    std::cout << "zu6cg init start ... ...\n";
}

void zu6cg::info(){
    std::cout << "fpga type: zu6cg" << std::endl;
    std::cout << "fpga version: " << version << std::endl;
    bool st = this->check_status();
    std::cout << "device status: " <<  device_status_check(st) << std::endl;
}

size_t zu6cg::read(int reg,void *buf){ //可以构建任何的数据类型进行使用,写入和使用相关的buffer
    std::cout << "addr: " << reg << std::endl;
    return 0;
}

bool zu6cg::write(int reg,void* buf){
    return true;
}

std::string zu6cg::get_dev_type(){
    return this->version;
}