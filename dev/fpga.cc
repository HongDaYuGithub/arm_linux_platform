#include "../../platform/include/platform.hpp"

extern xilinx xilinx_platform;

class zu6cg:public device{
public:
    virtual void init_class();
    virtual void config(json& fp);
    virtual size_t read(int reg,void* buf);
    virtual bool write(int reg,void* buf);
    virtual void info();
private:
    std::string version;
};

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

size_t zu6cg::read(int reg,void *buf){
    return 0;
}

bool zu6cg::write(int reg,void* buf){
    return true;
}

void register_device(platform* plat,json& fp){
    std::string ver;
    fp.find("fpga","version",ver);
    if(!ver.find("zu6cg")){
        zu6cg* fpga = new zu6cg;
        fpga->config(fp);
        plat->platform_register(fpga);
    }
}