

#include <KrampusReturns/Shaders/Primary.hpp>

#include <iostream>
#include <sstream>
#include <stdexcept>


namespace KrampusReturns
{
namespace Shaders
{


Primary::Primary()
   : AllegroFlare::Shaders::Base("KrampusReturns/Shaders/Primary", obtain_vertex_source(), obtain_fragment_source())
   , initialized(false)
   , tint(ALLEGRO_COLOR{1, 1, 1, 1})
   , tint_intensity(1.0f)
{
}


Primary::~Primary()
{
}


void Primary::set_tint(ALLEGRO_COLOR tint)
{
   this->tint = tint;
}


void Primary::set_tint_intensity(float tint_intensity)
{
   this->tint_intensity = tint_intensity;
}


ALLEGRO_COLOR Primary::get_tint() const
{
   return tint;
}


float Primary::get_tint_intensity() const
{
   return tint_intensity;
}


void Primary::initialize()
{
   if (!((!initialized)))
   {
      std::stringstream error_message;
      error_message << "[Primary::initialize]: error: guard \"(!initialized)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Primary::initialize: error: guard \"(!initialized)\" not met");
   }
   AllegroFlare::Shaders::Base::initialize(); // NOTE: Unsure if this is correct usage
   initialized = true;
   return;
}

void Primary::activate()
{
   if (!initialized)
   {
      throw std::runtime_error("[KrampusReturns::Shaders::Primary] Attempting to activate() "
                               "shader before it has been initialized");
   }
   AllegroFlare::Shaders::Base::activate();
   set_values_to_activated_shader();
   return;
}

void Primary::set_values_to_activated_shader()
{
   set_vec3("tint", tint.r, tint.g, tint.b);
   set_float("tint_intensity", tint_intensity);
   return;
}

std::string Primary::obtain_vertex_source()
{
   static const std::string source = R"DELIM(
     attribute vec4 al_pos;
     attribute vec4 al_color;
     attribute vec2 al_texcoord;
     uniform mat4 al_projview_matrix;
     varying vec4 varying_color;
     varying vec2 varying_texcoord;

     void main()
     {
        varying_color = al_color;
        varying_texcoord = al_texcoord;
        gl_Position = al_projview_matrix * al_pos;
     }
   )DELIM";
   return source;
}

std::string Primary::obtain_fragment_source()
{
   static const std::string source = R"DELIM(
     uniform sampler2D al_tex;
     uniform float tint_intensity;
     uniform vec3 tint;
     varying vec4 varying_color;
     varying vec2 varying_texcoord;

     void main()
     {
        vec4 tmp = texture2D(al_tex, varying_texcoord);
        float inverse_tint_intensity = 1.0 - tint_intensity;
        tmp.r = (tmp.r * inverse_tint_intensity + tint.r * tint_intensity) * tmp.a;
        tmp.g = (tmp.g * inverse_tint_intensity + tint.g * tint_intensity) * tmp.a;
        tmp.b = (tmp.b * inverse_tint_intensity + tint.b * tint_intensity) * tmp.a;
        tmp.a = tmp.a;

        //gl_FragColor = tmp * tint;
        //tmp.r = (tmp.r * (0.4 + 0.6 * tmp.r) * tint.r);
        //tmp.g = (tmp.g * (0.4 + 0.6 * tmp.g) * tint.g); //inverse_tint_intensity + tint.g * tint_intensity) * tmp.a;
        //tmp.b = (tmp.b * (0.5 + 0.5 * tmp.b) * tint.b); //inverse_tint_intensity + tint.b * tint_intensity) * tmp.a;
        //tmp.a = tmp.a;

        gl_FragColor = tmp;
     }
   )DELIM";
   return source;
}


} // namespace Shaders
} // namespace KrampusReturns


