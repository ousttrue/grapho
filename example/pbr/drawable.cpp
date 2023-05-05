#include <GL/glew.h>

#include "drawable.h"
#include "pbrmaterial.h"
#include <grapho/gl3/vao.h>
#include <grapho/mesh.h>

Drawable::Drawable()
{
  auto sphere = grapho::mesh::Sphere();
  Mesh = grapho::gl3::Vao::Create(sphere);
  MeshDrawCount = sphere->DrawCount();
  MeshDrawMode = *grapho::gl3::GLMode(sphere->Mode);
}

void
Drawable::Draw(const DirectX::XMFLOAT4X4& projection,
               const DirectX::XMFLOAT4X4& view,
               const DirectX::XMFLOAT3& cameraPos,
               uint32_t UBO_LIGHTS_BINDING)
{
  Material->Activate(projection, view, Position, cameraPos, UBO_LIGHTS_BINDING);
  Mesh->Draw(MeshDrawMode, MeshDrawCount);
}
