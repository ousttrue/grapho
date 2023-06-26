#pragma once
#include "../euclidean_transform.h"
#include "viewport.h"
#include <DirectXMath.h>
#include <array>
#include <cmath>

namespace grapho {
namespace camera {

struct Projection
{
  Viewport Viewport;
  float FovY = DirectX::XMConvertToRadians(30.0f);
  float NearZ = 0.01f;
  float FarZ = 1000.0f;

  void Update(DirectX::XMFLOAT4X4* projection)
  {
    auto aspectRatio = Viewport.AspectRatio();
    DirectX::XMStoreFloat4x4(
      projection,
      DirectX::XMMatrixPerspectiveFovRH(FovY, aspectRatio, NearZ, FarZ));
  }

  void SetSize(int w, int h)
  {
    if (w == Viewport.Width && h == Viewport.Height) {
      return;
    }
    Viewport.Width = w;
    Viewport.Height = h;
  }
};

struct Camera
{
  Projection Projection;
  EuclideanTransform Transform;

  DirectX::XMFLOAT4X4 ViewMatrix;
  DirectX::XMFLOAT4X4 ProjectionMatrix;

  float OrbitDistance = 5;

  void YawPitch(int dx, int dy)
  {
    // Yaw += DirectX::XMConvertToRadians(static_cast<float>(dx));
    // Pitch += DirectX::XMConvertToRadians(static_cast<float>(dy));
  }

  void Shift(int dx, int dy)
  {
    // auto factor = std::tan(FovY * 0.5f) * 2.0f * shift_[2] / Viewport.Height;
    // shift_[0] -= dx * factor;
    // shift_[1] += dy * factor;
  }

  void Dolly(int d)
  {
    // if (d > 0) {
    //   shift_[2] *= 0.9f;
    // } else if (d < 0) {
    //   shift_[2] *= 1.1f;
    // }
  }

  void Update()
  {
    Projection.Update(&ProjectionMatrix);

    // auto yaw = DirectX::XMMatrixRotationY(Yaw);
    // auto pitch = DirectX::XMMatrixRotationX(Pitch);
    // auto shift = DirectX::XMMatrixTranslation(shift_[0], shift_[1],
    // shift_[2]); auto v = yaw * pitch * shift;
    // DirectX::XMStoreFloat4x4((DirectX::XMFLOAT4X4*)view, v);
    DirectX::XMStoreFloat4x4(&ViewMatrix, Transform.InversedMatrix());

    // // Pos
    // DirectX::XMVECTOR det;
    // auto inv = DirectX::XMMatrixInverse(&det, v);
    // DirectX::XMStoreFloat3(
    //   &Position, DirectX::XMVector3Transform(DirectX::XMVectorZero(), inv));
  }

  void Fit(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max)
  {
    // Yaw = {};
    // Pitch = {};
    Transform.Rotation = { 0, 0, 0, 1 };
    auto height = max.y - min.y;
    if (fabs(height) < 1e-4) {
      return;
    }
    auto distance = height * 0.5f / std::atan(Projection.FovY * 0.5f);
    Transform.Translation.x = 0;
    Transform.Translation.y = (max.y + min.y) * 0.5f;
    Transform.Translation.z = distance * 1.2f;
    OrbitDistance = Transform.Translation.z;
    auto r =
      DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(
        DirectX::XMLoadFloat3(&min), DirectX::XMLoadFloat3(&max))));
    Projection.NearZ = r * 0.01f;
    Projection.FarZ = r * 100.0f;
  }
};

} // namespace
} // namespace
