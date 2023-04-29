#pragma once
#include <expected>
#include <string>
#include <vector>

namespace grapho {

enum class ValueType
{
  Float,
};

struct VertexId
{
  uint32_t AttributeLocation;
  uint32_t Slot;
  std::string SemanticName;
  uint32_t SemanticIndex;
};

struct VertexLayout
{
  VertexId Id;
  ValueType Type;
  uint32_t Count;
  uint32_t Offset;
  uint32_t Stride;
  uint32_t Divisor = 0;
};

}
