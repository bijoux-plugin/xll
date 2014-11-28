
//#include "gtest/gtest.h"
#include <string>
#include <Windows.h>
#include "xlCall.h"
#include "TcpClient.h"

#include "gmock/gmock.h"

using namespace std;
using ::testing::Return;

class Foo {
public:
	//static void decode ( XStream& xs, LPXLOPER xl );
  //virtual ~Foo();
  virtual int GetSize() const = 0;
  virtual string Describe(const char* name) = 0;
  virtual string Describe(int type) = 0;
};

class MockFoo : public Foo {
public:
  MOCK_CONST_METHOD0(GetSize, int());
  MOCK_METHOD1(Describe, string(const char* name));
  MOCK_METHOD1(Describe, string(int type));
};



int Factorial ( int a ) {
	return -5;
}

TEST(FactorialTest, Negative) {
  EXPECT_EQ(-5, Factorial(-5));
}

TEST(TestCreatingMock, TestOne) {
	MockFoo niceCodec;

	ON_CALL(niceCodec, GetSize())
		.WillByDefault(Return(1));

	EXPECT_CALL(niceCodec, GetSize())
		.Times(1)
		.WillOnce(Return(1));

	EXPECT_EQ(1, niceCodec.GetSize());
	
	TcpClient tcpClient;
	XLOPER xlInt;
	xlInt.xltype = xltypeInt;
	xlInt.val.w = 100;

}

/*
 Test 1 - Establish connection
 Test 2 - Send login information
 Test 3 - Receive Boolean Ok
 Test 4 - Call a function with 0 parameters and get result
 Test 5 - Call a function with 1 parameter and get result
 Test 6 - Call a function with 2 parameters and get result
 Test 7 - Call a function with 2 parameters and get result
*/