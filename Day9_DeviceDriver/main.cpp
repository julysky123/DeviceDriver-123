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

class DeviceDriverFixture : public Test {
public:
	MockMemoryDevice MOCKED_MEMORY_DEVICE;
	long READ_ADDRESS = 0xFF;
	DeviceDriver driver{ &MOCKED_MEMORY_DEVICE };

};

TEST_F(DeviceDriverFixture, ReadFromHW) {
	const int WRITTEN_VALUE = 0;
	EXPECT_CALL(MOCKED_MEMORY_DEVICE, read(READ_ADDRESS))
		.Times(5)
		.WillRepeatedly(Return(WRITTEN_VALUE));
	int expected = WRITTEN_VALUE;
	int actual = driver.read(READ_ADDRESS);
	EXPECT_EQ(expected, actual);
}

TEST_F(DeviceDriverFixture, ReadFromHWButHWReturnsInconsistentValues) {
	EXPECT_CALL(MOCKED_MEMORY_DEVICE, read(READ_ADDRESS))
		.Times(AtMost(5))
		.WillOnce(Return(1))
		.WillOnce(Return(2))
		.WillOnce(Return(1))
		.WillOnce(Return(2))
		.WillOnce(Return(1));

	EXPECT_THROW(driver.read(READ_ADDRESS), ReadFailException);
}

int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}