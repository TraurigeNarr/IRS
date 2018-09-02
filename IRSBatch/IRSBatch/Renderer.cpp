#include "stdafx.h"

#include "Database.h"
#include "Renderer.h"
#include "Types.h"
#include "Grid.h"

#include <algorithm>

Renderer::Renderer(HWND p_wnd) 
  : m_hwnd(p_wnd)
  , m_pDirect2dFactory(NULL)
  , m_pRenderTarget(NULL)
  , m_pLightSlateGrayBrush(NULL)
  , m_pCornflowerBlueBrush(NULL)
{
  mp_grid.reset(new Grid());
}

Renderer::~Renderer()
{
  IRS::DatabaseInstance->SetRenderer(nullptr);
  mp_grid.reset(nullptr);
  SafeRelease(&m_pDirect2dFactory);
  SafeRelease(&m_pRenderTarget);
  SafeRelease(&m_pLightSlateGrayBrush);
  SafeRelease(&m_pCornflowerBlueBrush);
}

Grid* Renderer::GetGrid()
{
  return mp_grid.get();
}

HRESULT Renderer::Initialize()
{
  HRESULT hr;
  // Initialize device-independent resources, such
  // as the Direct2D factory.
  hr = CreateDeviceIndependentResources();
  
  if (SUCCEEDED(hr))
  {
    // Because the CreateWindow function takes its size in pixels,
    // obtain the system DPI and use it to scale the window size.
    FLOAT dpiX, dpiY;

    // The factory returns the current system DPI. This is also the value it will use
    // to create its own windows.
    m_pDirect2dFactory->GetDesktopDpi(&dpiX, &dpiY);

    //HWND hwndRenderTarget = (HWND)m_OgreRenderTargetControl;
    //ASSERT(::IsWindow(hwndRenderTarget));
    // Create the window.
   
    hr = m_hwnd ? S_OK : E_FAIL;
  }

  return hr;
}

HRESULT Renderer::CreateDeviceIndependentResources()
{
  HRESULT hr = S_OK;
  return hr;
  // Create a Direct2D factory.
  //hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);
  
  RECT rc;
  GetClientRect(m_hwnd, &rc);

  D2D1_SIZE_U size = D2D1::SizeU(
    rc.right - rc.left,
    rc.bottom - rc.top
    );

  // Create a Direct2D render target.
  hr = m_pDirect2dFactory->CreateHwndRenderTarget(
    D2D1::RenderTargetProperties(),
    D2D1::HwndRenderTargetProperties(m_hwnd, size),
    &m_pRenderTarget
    );

  if (SUCCEEDED(hr))
  {
    // Create a gray brush.
    hr = m_pRenderTarget->CreateSolidColorBrush(
      D2D1::ColorF(D2D1::ColorF::LightSlateGray),
      &m_pLightSlateGrayBrush
      );
  }

  if (SUCCEEDED(hr))
  {
    // Create a blue brush.
    hr = m_pRenderTarget->CreateSolidColorBrush(
      D2D1::ColorF(D2D1::ColorF::CornflowerBlue),
      &m_pCornflowerBlueBrush
      );
  }

  if (SUCCEEDED(hr))
  {
    // Create a blue brush.
    hr = m_pRenderTarget->CreateSolidColorBrush(
      D2D1::ColorF(D2D1::ColorF::CornflowerBlue),
      &mp_default_brush
      );
  }

  if (SUCCEEDED(hr))
  {
    //create write factory
    /*hr = DWriteCreateFactory(
      DWRITE_FACTORY_TYPE_SHARED,
      __uuidof(IDWriteFactory),
      reinterpret_cast<IUnknown**>(&m_pWriteFactory)
      );*/
  }

  if (SUCCEEDED(hr))
  {
    //create text format
    hr = m_pWriteFactory->CreateTextFormat(
      L"Arial",                // Font family name.
      NULL,                       // Font collection (NULL sets it to use the system font collection).
      DWRITE_FONT_WEIGHT_REGULAR,
      DWRITE_FONT_STYLE_NORMAL,
      DWRITE_FONT_STRETCH_NORMAL,
      10.0f,
      L"en-us",
      &mp_default_text_format
      );
  }

//  mp_grid->CreateDeviceIndependentResources(this);

  return hr;
}

HRESULT Renderer::CreateDeviceResources()
{
  HRESULT hr = S_OK;

  if (!m_pRenderTarget)
  {
    RECT rc;
    GetClientRect(m_hwnd, &rc);
    

    D2D1_SIZE_U size = D2D1::SizeU(
      rc.right - rc.left,
      rc.bottom - rc.top
      );

    // Create a Direct2D render target.
    hr = m_pDirect2dFactory->CreateHwndRenderTarget(
      D2D1::RenderTargetProperties(),
      D2D1::HwndRenderTargetProperties(m_hwnd, size),
      &m_pRenderTarget
      );


    if (SUCCEEDED(hr))
    {
      // Create a gray brush.
      hr = m_pRenderTarget->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF::Black),
        &m_pLightSlateGrayBrush
        );
    }
    if (SUCCEEDED(hr))
    {
      // Create a blue brush.
      hr = m_pRenderTarget->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF::CornflowerBlue),
        &m_pCornflowerBlueBrush
        );
    }
//    mp_grid->CreateDeviceResources(this);
  }

  return hr;
}

void Renderer::DiscardDeviceResources()
{
  SafeRelease(&m_pRenderTarget);
  SafeRelease(&m_pLightSlateGrayBrush);
  SafeRelease(&m_pCornflowerBlueBrush);
}

HRESULT Renderer::OnRender()
{
HRESULT hr = S_OK;

hr = CreateDeviceResources();  

if (SUCCEEDED(hr))
  {
  m_pRenderTarget->BeginDraw();

  m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

  m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

//  mp_grid->Draw(m_pRenderTarget);

 /* IRS::TRenderableItems& items = IRS::DatabaseInstance->GetItems<IRenderable>();
  std::for_each(items.begin(), items.end(), [this](IRenderable* ip_item)
    {
    ip_item->Draw(m_pRenderTarget);
    });*/

  m_pRenderTarget->EndDraw();
  }

  return hr;
}

void Renderer::OnResize(UINT width, UINT height)
{
  if (m_pRenderTarget)
  {
    // Note: This method can fail, but it's okay to ignore the
    // error here, because the error will be returned again
    // the next time EndDraw is called.
    m_pRenderTarget->Resize(D2D1::SizeU(width, height));
    mp_grid->Reset();
  }
}

ID2D1HwndRenderTarget* Renderer::GetRenderTarget()
{
  return m_pRenderTarget;
}

ID2D1Factory* Renderer::GetFactory()
{
  return m_pDirect2dFactory;
}

IDWriteFactory* Renderer::GetWriteFactory()
{
  return m_pWriteFactory;
}