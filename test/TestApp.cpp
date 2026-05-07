
#include <gmock/gmock.h>
#include <memory>

#define private public
#include <App.hpp>
#undef private

class TestApp : public testing::Test {

    protected:

    void createUut() {
        uut_ = std::make_unique<App>();
    }

    std::unique_ptr<App> uut_;

};

TEST_F(TestApp, TestApp_run_nominal) {

    createUut();
    uut_->run();

    // no expect here

} 

TEST_F(TestApp, TestApp_foo_nominal) {

    createUut();

    EXPECT_EQ(uut_->foo(), 12);

}
