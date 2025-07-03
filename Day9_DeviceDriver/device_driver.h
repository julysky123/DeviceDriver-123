#pragma once
#include "flash_memory_device.h"

class DeviceDriver
{
public:
    DeviceDriver(FlashMemoryDevice* hardware);
    virtual int read(long address);
    void write(long address, int data);

private:
    static constexpr int MEMORY_READ_COUNT = 5;
    bool isSame(char first, char second);

protected:
    FlashMemoryDevice* m_hardware;
};