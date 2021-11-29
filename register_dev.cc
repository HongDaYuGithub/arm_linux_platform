#include "platform/include/platform.hpp"
#include "dev/fpga/fpga.hpp"

/*
   这一块儿就是驱动的业务层代码
   第一步将你需要添加的设备声明到 DEVICE 的空间之内
   第二步完成的配置任务,根据json文本中提到的配置项
   第三步完成的完成你自己特定的注册流程;
*/

namespace DEVICE{ //设备示例空间
    zu6cg fpga;
}

void config_fpga_device(json& fp,device* dev){
    std::string ver;
    fp.find("fpga","version",ver);
      if(!ver.find("zu6cg")){
        DEVICE::fpga.config(fp);
        
    }
}

void register_fpga(json& fp,platform* plat,device* dev)
{
    config_fpga_device(fp,dev);
    plat->platform_register(dev);
}

void register_device(platform* plat,json& fp){
    register_fpga(fp,plat,&DEVICE::fpga); //完整的FPGA设备注册流程 第一个设备流程
}