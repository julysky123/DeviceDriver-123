#include "gmock/gmock.h"
#include "device_driver.h"
#include "device_driver_exception.h"
#include <stdexcept>
using namespace testing;
class MockMemoryDevice : public FlashMemoryDevice {
public:
	MOCK_METHOD(unsigned char, read, (long), (override));
	MOCK_METHOD(void, write, (long, unsigned char), (override));
};

class MockDeviceDriver : public DeviceDriver {
public:
	MockDeviceDriver(FlashMemoryDevice* hardware) :DeviceDriver{ hardware } {}
	MOCK_METHOD(int, read, (long), (override));
};

class DeviceDriverFixture : public Test {
protected:
	void SetUp() override {
		driver = new DeviceDriver(&MOCKED_MEMORY_DEVICE);
	}
public:
	static constexpr long MEMORY_ADDRESS = 0xFF;
	static constexpr int EMPTY_DATA = 0xFF;
	static constexpr int WRITING_DATA = 0x3;

	MockMemoryDevice MOCKED_MEMORY_DEVICE;
	MockDeviceDriver MOCKED_DEVICE_DRIVER{ &MOCKED_MEMORY_DEVICE };

	DeviceDriver* driver;
};

TEST_F(DeviceDriverFixture, ReadFromHW) {
	static constexpr int WRITTEN_VALUE = 0;
	EXPECT_CALL(MOCKED_MEMORY_DEVICE, read(MEMORY_ADDRESS))
		.Times(5)
		.WillRepeatedly(Return(WRITTEN_VALUE));
	int expected = WRITTEN_VALUE;
	int actual = driver->read(MEMORY_ADDRESS);
	EXPECT_EQ(expected, actual);
}

TEST_F(DeviceDriverFixture, ReadFromHWButHWReturnsInconsistentValues) {
	EXPECT_CALL(MOCKED_MEMORY_DEVICE, read(MEMORY_ADDRESS))
		.Times(AtMost(5))
		.WillOnce(Return(1))
		.WillOnce(Return(2))
		.WillOnce(Return(1))
		.WillOnce(Return(2))
		.WillOnce(Return(1));

	EXPECT_THROW(driver->read(MEMORY_ADDRESS), ReadFailException);
}

TEST_F(DeviceDriverFixture, WriteToHW) {
	driver = &MOCKED_DEVICE_DRIVER;

	EXPECT_CALL(MOCKED_MEMORY_DEVICE, write(MEMORY_ADDRESS, WRITING_DATA))
		.Times(1);
	EXPECT_CALL(MOCKED_DEVICE_DRIVER, read(MEMORY_ADDRESS))
		.Times(1)
		.WillRepeatedly(Return(EMPTY_DATA));

	driver->write(MEMORY_ADDRESS, WRITING_DATA);
}

TEST_F(DeviceDriverFixture, WriteToHWButAlreadyDataWritten) {
	driver = &MOCKED_DEVICE_DRIVER;

	static constexpr char ALREADY_WRITTEN_VALUE = 0x7;
	EXPECT_CALL(MOCKED_DEVICE_DRIVER, read(MEMORY_ADDRESS))
		.WillRepeatedly(Return(ALREADY_WRITTEN_VALUE));

	EXPECT_THROW(driver->write(MEMORY_ADDRESS, WRITING_DATA), WriteFailException);
}

int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}