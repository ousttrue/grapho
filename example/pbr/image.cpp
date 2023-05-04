#include <GL/glew.h>

#include "image.h"
#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <stb_image.h>

Image::~Image()
{
  stbi_image_free(Data);
}

bool
Image::Load(std::string_view path)
{
  std::string p(path.begin(), path.end());
  Data = stbi_load(p.c_str(), &Width, &Height, &nrComponents, 0);
  if (!Data) {
    std::cout << "Texture failed to load at path: " << path << std::endl;
    return false;
  }

  switch (nrComponents) {
    case 1:
      Format = GL_RED;
      break;
    case 3:
      Format = GL_RGB;
      break;
    case 4:
      Format = GL_RGBA;
      break;

    default:
      return false;
  }

  return true;
}

bool
Image::LoadHdr(std::string_view path)
{
  std::string p(path.begin(), path.end());
  stbi_set_flip_vertically_on_load(true);
  // int width, height, nrComponents;
  Data = (uint8_t*)stbi_loadf(p.c_str(), &Width, &Height, &nrComponents, 0);
  if (!Data) {
    std::cout << "Texture failed to load at path: " << path << std::endl;
    return false;
  }

  assert(nrComponents == 3);
  Format = GL_RGB16F;
  return true;
}
