
// IRSBatchView.h : interface of the CIRSBatchView class
//

#pragma once


class CIRSBatchView : public CView
{
private:
	bool m_mouse_over;

private:
  CRect GetControlPosition(int nID);
 
protected: // create from serialization only
	CIRSBatchView();
	DECLARE_DYNCREATE(CIRSBatchView)

// Attributes
public:
	CIRSBatchDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
  virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CIRSBatchView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
  afx_msg int  OnCreate(LPCREATESTRUCT lpsc);
  
  LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in IRSBatchView.cpp
inline CIRSBatchDoc* CIRSBatchView::GetDocument() const
   { return reinterpret_cast<CIRSBatchDoc*>(m_pDocument); }
#endif

