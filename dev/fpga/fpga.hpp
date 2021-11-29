#ifndef __FPGA_DEV
#define __FPGA_DEV

class zu6cg:public device{
public:
    virtual void init_class();
    virtual void config(json& fp);
    virtual size_t read(int reg,void* buf);
    virtual bool write(int reg,void* buf);
    virtual std::string get_dev_type();
    virtual void info();
private:
    std::string version;
};

#endif