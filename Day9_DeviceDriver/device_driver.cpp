#include "device_driver.h"
#include "device_driver_exception.h"

DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware)
{
}

int DeviceDriver::read(long address)
{
    bool isConsistent = true;
    int firstRead = m_hardware->read(address);
    for (int i = 2; i <= 5; i++) {
        int additionalRead = m_hardware->read(address);
        if (firstRead != additionalRead) {
            isConsistent = false;
        }
    }
    if (isConsistent == false) {
        throw ReadFailException("The hardware returns an inconsistent value.\n");
    }
    return firstRead;
}

void DeviceDriver::write(long address, int data)
{
    // TODO: implement this method
    m_hardware->write(address, (unsigned char)data);
}