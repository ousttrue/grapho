#pragma once
#include <DirectXMath.h>
#include <memory>
#include <stdint.h>

namespace grapho {
namespace gl3 {
struct Vao;
class ShaderProgram;
}
}

struct Skybox
{
  std::shared_ptr<grapho::gl3::Vao> Cube;
  uint32_t CubeDrawCount = 0;

  std::shared_ptr<grapho::gl3::ShaderProgram> BackgroundShader;

  Skybox();
  void Draw(const DirectX::XMFLOAT4X4& projection,
            const DirectX::XMFLOAT4X4& view);
};
