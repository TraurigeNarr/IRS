#pragma once

#include "IRenderable.h"

// C RunTime Header Files:
#include <d2d1.h>


template<class Interface>
inline void SafeRelease(
  Interface **ppInterfaceToRelease
  )
{
  if (*ppInterfaceToRelease != NULL)
  {
    (*ppInterfaceToRelease)->Release();

    (*ppInterfaceToRelease) = nullptr;
  }
}


#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b) do {if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}} while(0)
#else
#define Assert(b)
#endif //DEBUG || _DEBUG
#endif



#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

class Grid;

class Renderer
{
private:
  HWND                    m_hwnd;
  ID2D1Factory*           m_pDirect2dFactory;
  ID2D1HwndRenderTarget*  m_pRenderTarget;
  ID2D1SolidColorBrush*   m_pLightSlateGrayBrush;
  ID2D1SolidColorBrush*   m_pCornflowerBlueBrush;
  IDWriteFactory*         m_pWriteFactory;
  IDWriteTextFormat*      mp_default_text_format;
  ID2D1SolidColorBrush*   mp_default_brush;

  std::unique_ptr<Grid>   mp_grid;
private:
  // Initialize device-independent resources.
  HRESULT CreateDeviceIndependentResources();

  // Initialize device-dependent resources.
  HRESULT CreateDeviceResources();

  // Release device-dependent resource.
  void DiscardDeviceResources();

public:
  Renderer(HWND ip_hwnd);
  ~Renderer();

  // Register the window class and call methods for instantiating drawing resources
  HRESULT Initialize();

  // Draw content.
  HRESULT OnRender();

  Grid* GetGrid();

  // Resize the render target.
  void OnResize(UINT width, UINT height);

  ID2D1Factory* GetFactory();
  IDWriteFactory* GetWriteFactory();
  ID2D1HwndRenderTarget* GetRenderTarget();
};