#pragma once
#include <functional>
#include <optional>

namespace grapho {
namespace gl3 {
inline std::optional<const char*>
TryGetError()
{
  auto err = glGetError();
  switch (err) {
    case GL_NO_ERROR:
      // No error has been recorded. The value of this symbolic constant is
      // guaranteed to be 0.
      return std::nullopt;

    case GL_INVALID_ENUM:
      // An unacceptable value is specified for an enumerated argument. The
      // offending command is ignored and has no other side effect than to set
      // the error flag.
      return "GL_INVALID_ENUM";

    case GL_INVALID_VALUE:
      // A numeric argument is out of range. The offending command is ignored
      // and has no other side effect than to set the error flag.
      return "GL_INVALID_VALUE";

    case GL_INVALID_OPERATION:
      // The specified operation is not allowed in the current state. The
      // offending command is ignored and has no other side effect than to set
      // the error flag.
      return "GL_INVALID_OPERATION";

    case GL_INVALID_FRAMEBUFFER_OPERATION:
      // The framebuffer object is not complete. The offending command is
      // ignored and has no other side effect than to set the error flag.
      return "GL_INVALID_FRAMEBUFFER_OPERATION";

    case GL_OUT_OF_MEMORY:
      // There is not enough memory left to execute the command. The state of
      // the GL is undefined, except for the state of the error flags, after
      // this error is recorded.
      return "GL_OUT_OF_MEMORY";

    case GL_STACK_UNDERFLOW:
      // An attempt has been made to perform an operation that would cause an
      // internal stack to underflow.
      return "GL_STACK_UNDERFLOW";

    case GL_STACK_OVERFLOW:
      // An attempt has been
      return "GL_STACK_OVERFLOW";

    default:
      return "UNKNOWN";
  }
}

inline void
CheckAndPrintError(const std::function<void(const char*)>& print)
{
  while (auto error = TryGetError()) {
    print(*error);
  }
}

} // namespace
} // namespace
