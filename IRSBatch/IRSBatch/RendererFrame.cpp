// PaintWnd.cpp : implementation file

#include "stdafx.h"

#include "RendererFrame.h"

#include "OpenGL_Renderer.h"

#include "Grid.h"
#include "Vertex.h"
#include "Renderer.h"
#include "Database.h"

#include "CommandHandler.h"
#include "MouseController.h"


IMPLEMENT_DYNCREATE(CRendererFrame, CView)

CRendererFrame::CRendererFrame()
  : mp_renderer_impl(nullptr)
  , m_mouse_over(false)
  {  }

CRendererFrame::~CRendererFrame()
  {
  if (mp_renderer_impl)
    {
    mp_renderer_impl->Release();
    mp_renderer_impl.reset(nullptr);
    }
  }

BEGIN_MESSAGE_MAP(CRendererFrame, CView)
  ON_WM_SIZE()
  ON_WM_CREATE()
  ON_WM_SETCURSOR()
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////

int CRendererFrame::OnCreate(LPCREATESTRUCT lpsc)
{
  if (0 != CView::OnCreate(lpsc))
    return -1;

  ASSERT(::IsWindow(m_hWnd));

  CRect rc;
  GetClientRect(&rc);

  mp_renderer_impl.reset(new OpenGL_Renderer(rc, this));
  mp_renderer_impl->Initialize();
  IRS::Database* instance = IRS::DatabaseInstance;
  instance->SetRenderer(mp_renderer_impl.get());
  return 0;
}

void CRendererFrame::OnSize(UINT nType, int cx, int cy)
{
  mp_renderer_impl->Reshape();
  IRS::DatabaseInstance->GetGrid()->OnSize(cx, cy);
  IRS::DatabaseInstance->Update();
}

void CRendererFrame::OnDraw(CDC* pDC)
  {
  std::function<void()> draw = std::bind(&IRS::Database::DrawItems, IRS::DatabaseInstance);
  mp_renderer_impl->SetDrawSceneFunction(draw);
  mp_renderer_impl->SetClearColor(IRS::CL_GRAY);
  mp_renderer_impl->RenderScene();
  mp_renderer_impl->SetDrawSceneFunction(std::function<void()>());
  }

void CRendererFrame::Invalidate()
  {
  std::function<void()> draw = std::bind(&IRS::Database::DrawItems, IRS::DatabaseInstance);
  mp_renderer_impl->SetDrawSceneFunction(draw);
  mp_renderer_impl->SetClearColor(IRS::CL_GRAY);
  mp_renderer_impl->RenderScene();
  mp_renderer_impl->SetDrawSceneFunction(std::function<void()>());
  }

IRenderer* CRendererFrame::GetRenderer()
{
  return mp_renderer_impl.get();
}

// CPaintWnd diagnostics

#ifdef _DEBUG
void CRendererFrame::AssertValid() const
{
  CView::AssertValid();
}

#ifndef _WIN32_WCE
void CRendererFrame::Dump(CDumpContext& dc) const
{
  CView::Dump(dc);
}
#endif
#endif //_DEBUG

// CPaintWnd message handlers

LRESULT CRendererFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message)
  {
  case WM_MOUSEMOVE:
    {
      if (!m_mouse_over)
      {
        CommandHandlerInstance->TrackMouseLeave(GetSafeHwnd(), wParam, lParam);
        m_mouse_over = true;
      }
    }
    break;
  case WM_LBUTTONUP:
    {
      MouseClicked(wParam, lParam);
    }
    break;
  case WM_MOUSELEAVE:
    {
      m_mouse_over = false;
    }
    break;
  case WM_NCDESTROY:
    mp_renderer_impl->Release();
    mp_renderer_impl.reset(nullptr);
    break;
  default:
    break;
  }
  return CView::WindowProc(message, wParam, lParam);
}

BOOL CRendererFrame::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
  if (MouseControllerInstance->SetCursor(pWnd, nHitTest, message))
    return TRUE;

  return CView::OnSetCursor(pWnd, nHitTest, message);
}


//////////////////////////////////////////////////////////////////////////

void CRendererFrame::MouseClicked(WPARAM wParam, LPARAM lParam)
{
  MouseControllerInstance->MouseClicked(this, wParam, lParam);
}