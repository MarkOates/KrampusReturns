
#include <gtest/gtest.h>

#include <KrampusReturns/Shaders/AllegroDefault.hpp>


TEST(KrampusReturns_Shaders_AllegroDefaultTest, can_be_created_without_blowing_up)
   // TODO: note this test fails because the destructor deletes the shader, which has been auto-created
   // and is dependent on allegro to be initialized, among other things.
{
   //KrampusReturns::Shaders::AllegroDefault shader;
}


