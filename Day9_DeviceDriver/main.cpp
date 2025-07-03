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

TEST(DeviceDriver, ReadFromHW) {
	MockMemoryDevice mockedMemoryDevice;
	EXPECT_CALL(mockedMemoryDevice, read(0xFF))
		.Times(5)
		.WillRepeatedly(Return(0));
		
	DeviceDriver driver{ &mockedMemoryDevice };
	int data = driver.read(0xFF);
	EXPECT_EQ(0, data);
}

TEST(DeviceDriver, ReadFromHWButHWReturnsInconsistentValues) {
	MockMemoryDevice mockedMemoryDevice;
	EXPECT_CALL(mockedMemoryDevice, read(0xFF))
		.Times(5)
		.WillOnce(Return(1))
		.WillOnce(Return(2))
		.WillOnce(Return(1))
		.WillOnce(Return(2))
		.WillOnce(Return(1));

	DeviceDriver driver{ &mockedMemoryDevice };
	EXPECT_THROW(driver.read(0xFF), ReadFailException);
}

int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}