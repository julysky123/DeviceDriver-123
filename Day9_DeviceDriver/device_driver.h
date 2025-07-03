#pragma once
#include "flash_memory_device.h"

class DeviceDriver
{
public:
    DeviceDriver(FlashMemoryDevice* hardware);
    virtual int read(long address);
    void write(long address, int data);

private:
    void checkReadPostCondition(int firstReadValue, long address);
    void checkWritePreCondition(long address);
protected:
    FlashMemoryDevice* m_hardware;
};