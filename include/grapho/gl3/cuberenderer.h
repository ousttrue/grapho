#pragma once
#include "../mesh.h"
#include "error_check.h"
#include "fbo.h"
#include "vao.h"
#include <assert.h>
#include <functional>

namespace grapho {
namespace gl3 {

class CubeRenderer
{
  std::shared_ptr<grapho::gl3::Vao> m_cube;
  uint32_t m_cubeDrawCount = 0;
  uint32_t m_mode = 0;

  // pbr: set up projection and view matrices for capturing data onto the 6
  // cubemap face directions
  // ---------------------------------------------------------------------------
  DirectX::XMFLOAT4X4 m_captureProjection;
  DirectX::XMFLOAT4X4 m_captureViews[6];

public:
  CubeRenderer()
  {
    auto cube = grapho::mesh::Cube();
    m_cube = grapho::gl3::Vao::Create(cube);
    m_cubeDrawCount = cube->DrawCount();
    m_mode = *grapho::gl3::GLMode(cube->Mode);

    DirectX::XMStoreFloat4x4(
      &m_captureProjection,
      DirectX::XMMatrixPerspectiveFovRH(
        DirectX::XMConvertToRadians(90.0f), 1.0f, 0.1f, 10.0f));

    DirectX::XMStoreFloat4x4(
      &m_captureViews[0],
      DirectX::XMMatrixLookAtRH(DirectX::XMVectorZero(),
                                DirectX::XMVectorSet(1.0f, 0, 0, 0),
                                DirectX::XMVectorSet(0, -1.0f, 0, 0)));
    DirectX::XMStoreFloat4x4(
      &m_captureViews[1],
      DirectX::XMMatrixLookAtRH(DirectX::XMVectorZero(),
                                DirectX::XMVectorSet(-1.0f, 0, 0, 0),
                                DirectX::XMVectorSet(0, -1.0f, 0, 0)));
    DirectX::XMStoreFloat4x4(
      &m_captureViews[2],
      DirectX::XMMatrixLookAtRH(DirectX::XMVectorZero(),
                                DirectX::XMVectorSet(0, 1.0f, 0, 0),
                                DirectX::XMVectorSet(0, 0, 1.0f, 0)));
    DirectX::XMStoreFloat4x4(
      &m_captureViews[3],
      DirectX::XMMatrixLookAtRH(DirectX::XMVectorZero(),
                                DirectX::XMVectorSet(0, -1.0f, 0, 0),
                                DirectX::XMVectorSet(0, 0, -1.0f, 0)));
    DirectX::XMStoreFloat4x4(
      &m_captureViews[4],
      DirectX::XMMatrixLookAtRH(DirectX::XMVectorZero(),
                                DirectX::XMVectorSet(0, 0, 1.0f, 0),
                                DirectX::XMVectorSet(0, -1.0f, 0, 0)));
    DirectX::XMStoreFloat4x4(
      &m_captureViews[5],
      DirectX::XMMatrixLookAtRH(DirectX::XMVectorZero(),
                                DirectX::XMVectorSet(0, 0, -1.0f, 0),
                                DirectX::XMVectorSet(0, -1.0f, 0, 0)));
  }
  ~CubeRenderer() {}

  using CallbackFunc = std::function<void(const DirectX::XMFLOAT4X4& projection,
                                          const DirectX::XMFLOAT4X4& view)>;
  void Render(int size,
              uint32_t dst,
              const CallbackFunc& callback,
              int mipLevel = 0) const
  {
    auto fbo = std::make_shared<grapho::gl3::Fbo>();
    grapho::camera::Viewport fboViewport{
      .Width = size, .Height = size,
      // .Color = { 0, 0, 0, 0 },
    };
    for (unsigned int i = 0; i < 6; ++i) {
      callback(m_captureProjection, m_captureViews[i]);
      fbo->AttachCubeMap(i, dst, mipLevel);
      assert(!TryGetError());
      grapho::gl3::ClearViewport(fboViewport, { .Depth = false });
      assert(!TryGetError());
      m_cube->Draw(m_mode, m_cubeDrawCount);
      assert(!TryGetError());
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
};

}
}
