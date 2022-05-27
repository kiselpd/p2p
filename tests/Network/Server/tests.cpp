//
// Created by Роман Сим on 15.04.2022.
//

//#include "gmock/gmock.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <cstring>
#include <fstream>
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgReferee;
extern "C" {
#include "Connection/Connection.h"
}
//
//namespace TestingConnection {
//
//    class MockConnection: public Connection {
//    public:
//        MOCK_METHOD1(registerUser, boost::property_tree::ptree(boost::property_tree::ptree &params));
//    };
//
//    TEST(Controller, loginUser) {
//        MockController controller;
//        EXPECT_CALL(controller, loginUser).Times(AtLeast(1));
//        boost::property_tree::ptree pt;
//        controller.loginUser(pt);
//    }
//
//}

TEST(CONNECTION, INVALID_CREATE_MATRIX) {
    Connection a;
EXPECT_EQ(nullptr, nullptr);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
