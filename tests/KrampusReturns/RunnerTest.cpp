
#include <gtest/gtest.h>

#include <KrampusReturns/Runner.hpp>


TEST(KrampusReturns_RunnerTest, can_be_created_without_blowing_up)
{
   KrampusReturns::Runner runner;
}


TEST(KrampusReturns_RunnerTest, run__returns_the_expected_response)
{
   KrampusReturns::Runner runner;
   std::string expected_string = "Hello World!";
   EXPECT_EQ(expected_string, runner.run());
}


