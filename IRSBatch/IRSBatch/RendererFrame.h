#pragma once

class Renderer;
class IRenderer;

class CRendererFrame : public CView
{
  typedef std::unique_ptr<Renderer> TRenderer;
  typedef std::unique_ptr<IRenderer> TRendererImpl;
private:
 // TRenderer mp_renderer;

  TRendererImpl mp_renderer_impl;
  bool m_mouse_over;
protected:
  DECLARE_DYNCREATE(CRendererFrame)

  // Attributes
public:
  CRendererFrame();
  ~CRendererFrame();

public:
  virtual void OnDraw(CDC* pDC) override;      // overridden to draw this view
  void Invalidate();

  IRenderer* GetRenderer();

#ifdef _DEBUG
  virtual void AssertValid() const;
#ifndef _WIN32_WCE
  virtual void Dump(CDumpContext& dc) const;
#endif
#endif
private:
  void MouseClicked(WPARAM wParam, LPARAM lParam);

protected:
  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg int OnCreate(LPCREATESTRUCT lpsc);
  afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
 
  LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};