
// IRSBatchView.cpp : implementation of the CIRSBatchView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "IRSBatch.h"
#endif

#include "IRSBatchDoc.h"
#include "IRSBatchView.h"

#include "RendererFrame.h"
#include "TabController.h"

#include "CommandHandler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIRSBatchView

IMPLEMENT_DYNCREATE(CIRSBatchView, CView)

BEGIN_MESSAGE_MAP(CIRSBatchView, CView)
  ON_WM_CREATE()
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()


// CIRSBatchView construction/destruction

CIRSBatchView::CIRSBatchView()
  : m_mouse_over(false)
{
}

CIRSBatchView::~CIRSBatchView()
{
  
}

BOOL CIRSBatchView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

int CIRSBatchView::OnCreate(LPCREATESTRUCT lpsc)
{
  if (0 != CView::OnCreate(lpsc))
    return -1;

  return  0;
}

LRESULT CIRSBatchView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
  if (message == WM_MOUSEMOVE)
  {
    if(!m_mouse_over)
    {
      CommandHandlerInstance->TrackMouseLeave(GetSafeHwnd(), wParam, lParam);
      m_mouse_over = true;
    }
  }
  else if (message == WM_MOUSELEAVE)
  {
    m_mouse_over = false;
  }

  return CView::WindowProc(message, wParam, lParam);
}

// CIRSBatchView drawing

void CIRSBatchView::OnDraw(CDC* /*pDC*/)
{
	CIRSBatchDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

// CIRSBatchView printing

BOOL CIRSBatchView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CIRSBatchView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CIRSBatchView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

CRect CIRSBatchView::GetControlPosition(int nID)
{
  CRect rect = CRect();
  GetDlgItem(nID)->GetWindowRect(&rect);
  ScreenToClient(&rect);
  return rect;
}

void CIRSBatchView::OnInitialUpdate()
{
  CView::OnInitialUpdate();
}


// CIRSBatchView diagnostics

#ifdef _DEBUG
void CIRSBatchView::AssertValid() const
{
	CView::AssertValid();
}

void CIRSBatchView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIRSBatchDoc* CIRSBatchView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIRSBatchDoc)));
	return (CIRSBatchDoc*)m_pDocument;
}
#endif //_DEBUG


// CIRSBatchView message handlers
