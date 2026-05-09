
#include <gmock/gmock.h>
#include <memory>

#include "App.hpp"

class TestApp : public testing::Test {

    protected:

    void createUut() {
        uut_ = std::make_unique<App>();
    }

    std::unique_ptr<App> uut_;

};

TEST_F(TestApp, TestApp_run_nominal) {

    createUut();

    //EXPECT_EQ(uut_->run(), 0l);

} 
