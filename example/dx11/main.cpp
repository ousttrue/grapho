#include <Windows.h>
#include <grapho/dx11/buffer.h>
#include <grapho/dx11/device.h>
#include <grapho/dx11/drawable.h>
#include <grapho/dx11/shader.h>
#include <grapho/dx11/texture.h>
#include <iostream>
#include <stdint.h>

const auto CLASS_NAME = "dx11class";
const auto WINDOW_NAME = "dx11window";
const auto WIDTH = 640;
const auto HEIGHT = 480;

struct rgba
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
};

struct float2
{
  float x, y;
};
struct float3
{
  float x, y, z;
};
struct Vertex
{
  float2 positon;
  float2 uv;
};
auto s = 0.5f;
/// CCW
/// 3   2
/// +---+
/// |   |
/// +---+
/// 0   1
static const struct Vertex vertices[] = {
  { { -s, -s }, { 0.f, 1.f } },
  { { s, -s }, { 1.f, 1.f } },
  { { s, s }, { 1.f, 0.f } },
  { { -s, s }, { 0.f, 0.f } },
};
static const uint32_t indices[] = {
  0, 1, 2, //
  2, 3, 0, //
};
static rgba pixels[4] = {
  { 255, 0, 0, 255 },
  { 0, 255, 0, 255 },
  { 0, 0, 255, 255 },
  { 255, 255, 255, 255 },
};
struct MatrixData
{
  float mvp[16];
};

static const char* shader_text = R"(
#pragma pack_matrix(row_major)
cbuffer c0
{
    float4x4 mvp;
};
struct vs_in {
    float2 pos: POSITION;
    float2 uv: TEXCOORD;
};
struct vs_out {
    float4 position_clip: SV_POSITION;
    float2 uv: TEXCOORD;
};

vs_out vs_main(vs_in IN) {
  vs_out OUT = (vs_out)0; // zero the memory first
  OUT.position_clip = mul(mvp, float4(IN.pos.xy, 0, 1));
  OUT.uv = IN.uv;
  return OUT;
}

Texture2D colorTexture;
SamplerState colorSampler;

float4 ps_main(vs_out IN) : SV_TARGET {
  float4 texel = colorTexture.Sample(colorSampler, IN.uv);
  return texel;
}
)";

static LRESULT CALLBACK
WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message) {
    case WM_CREATE:
      return 0;

    case WM_DESTROY: {
      PostQuitMessage(0);
      return 0;
    }

    case WM_ERASEBKGND:
      return 0;

    case WM_SIZE:
      // _state.width = LOWORD(lParam);
      // _state.height = HIWORD(lParam);
      // _is_maximized = wParam == SIZE_MAXIMIZED;
      return 0;

    case WM_PAINT: {
      PAINTSTRUCT ps;
      /*HDC hdc =*/BeginPaint(hWnd, &ps);
      EndPaint(hWnd, &ps);
      return 0;
    }
  }

  return DefWindowProcA(hWnd, message, wParam, lParam);
}

int WINAPI
WinMain(HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR lpCmdLine,
        int nCmdShow)
{
  WNDCLASSEXA windowClass = {
    .cbSize = (UINT)sizeof(WNDCLASSEXW),
    .style = CS_HREDRAW | CS_VREDRAW,
    .lpfnWndProc = WndProc,
    .hInstance = hInstance,
    .hCursor = LoadCursor(NULL, IDC_ARROW),
    .lpszClassName = CLASS_NAME,
  };
  if (!RegisterClassExA(&windowClass)) {
    return 1;
  }

  auto hWnd = CreateWindowA(CLASS_NAME,
                            WINDOW_NAME,
                            WS_OVERLAPPEDWINDOW,
                            CW_USEDEFAULT,
                            CW_USEDEFAULT,
                            WIDTH,
                            HEIGHT,
                            nullptr, // We have no parent window.
                            nullptr, // We aren't using menus.
                            hInstance,
                            nullptr);
  if (!hWnd) {
    return 2;
  }
  ShowWindow(hWnd, SW_SHOW);

  auto device = grapho::dx11::CreateDevice();
  if (!device) {
    return 3;
  }

  auto swapchain = grapho::dx11::CreateSwapchain(device, hWnd);
  if (!swapchain) {
    return 4;
  }

  auto [shader, vs_compiled, error] = grapho::dx11::Shader::Create(
    device, shader_text, "vs_main", "vs_5_0", shader_text, "ps_main", "ps_5_0");
  if (!shader) {
    std::cout << error << std::endl;
    return 5;
  }

  auto constant_buffer =
    grapho::dx11::CreateConstantBuffer(device, sizeof(MatrixData), nullptr);
  if (!constant_buffer) {
    return 6;
  }

  auto index_buffer =
    grapho::dx11::CreateIndexBuffer(device, sizeof(indices), indices);
  if (!index_buffer) {
    return 7;
  }

  auto vertex_buffer =
    grapho::dx11::CreateVertexBuffer(device, sizeof(vertices), vertices);
  if (!vertex_buffer) {
    return 8;
  }

  grapho::VertexLayout layouts[] = {
    {
      .Id = { 
        .Slot = 0, 
        .SemanticName = "POSITION", 
        .SemanticIndex = 0, 
      },
      .Type = grapho::ValueType::Float,
      .Count = 2,
      .Offset = offsetof(Vertex, positon),
      .Stride = sizeof(Vertex),
    },
    {
      .Id = { 
        .Slot=0,
        .SemanticName="TEXCOORD", 
        .SemanticIndex=0, 
      },
      .Type = grapho::ValueType::Float,
      .Count = 2,
      .Offset = offsetof(Vertex, uv),
      .Stride = sizeof(Vertex),
    },
  };
  grapho::dx11::VertexSlot slots[] = {
    {
      .VertexBuffer = vertex_buffer,
      .Stride = sizeof(Vertex),
    },
  };
  auto drawable = grapho::dx11::Drawable::Create(
    device, vs_compiled, layouts, slots, index_buffer);

  winrt::com_ptr<ID3D11RasterizerState> rs;
  {
    D3D11_RASTERIZER_DESC rs_desc = {
      .FillMode = D3D11_FILL_SOLID,
      .CullMode = D3D11_CULL_BACK,
      .FrontCounterClockwise = true,
      .ScissorEnable = false,
      .MultisampleEnable = false,
    };
    auto hr = device->CreateRasterizerState(&rs_desc, rs.put());
    if (FAILED(hr)) {
      return 7;
    }
  }

  auto texture = grapho::dx11::Texture::Create(device, 2, 2, &pixels[0].r);

  auto processMessage = []() {
    MSG msg = {};
    while (true) {
      if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
          return false;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      } else {
        break;
      }
    }
    return true;
  };

  MatrixData data
  {
    .mvp = {
      1, 0, 0, 0, //
      0, 1, 0, 0, //
      0, 0, 1, 0, //
      0, 0, 0, 1, //
    }, };

  winrt::com_ptr<ID3D11Texture2D> pBackBuffer;
  swapchain->GetBuffer(0, IID_PPV_ARGS(pBackBuffer.put()));

  winrt::com_ptr<ID3D11DeviceContext> context;
  device->GetImmediateContext(context.put());
  float clear_color[4]{ 0.2f, 0.2f, 0.2f, 0 };
  while (processMessage()) {

    // update
    context->UpdateSubresource(constant_buffer.get(), 0, NULL, &data, 0, 0);
    // rtv
    winrt::com_ptr<ID3D11RenderTargetView> rtv;
    auto hr =
      device->CreateRenderTargetView(pBackBuffer.get(), NULL, rtv.put());
    assert(SUCCEEDED(hr));
    context->ClearRenderTargetView(rtv.get(), clear_color);

    // pipeline
    ID3D11RenderTargetView* rtv_list[] = {
      rtv.get(),
    };
    context->OMSetRenderTargets(1, rtv_list, nullptr);
    D3D11_VIEWPORT viewport = { 0.0f, 0.0f, WIDTH, HEIGHT, 0.0f, 1.0f };
    context->RSSetViewports(1, &viewport);

    shader->Bind(context);
    ID3D11Buffer* constants[]{
      constant_buffer.get(),
    };
    context->VSSetConstantBuffers(0, std::size(constants), constants);
    context->RSSetState(rs.get());
    texture->Bind(context, 0);
    drawable->Draw(context, std::size(indices));

    // flush
    swapchain->Present(1, 0);
  }

  return 0;
}
