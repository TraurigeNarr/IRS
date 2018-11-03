#include "stdafx.h"

#include "MouseBrush.h"

#include "Database.h"
#include "Grid.h"
#include "IMode.h"
#include "MainFrm.h"
#include "Renderer.h"
#include "RendererFrame.h"
#include "TabController.h"
#include "Vertex.h"

#include "resource.h"

//////////////////////////////////////////////////////////////////////////

MouseBrush::MouseBrush()
	: IMouse(MOUSE_BRUSH)
{
	m_cursor = AfxGetApp()->LoadCursor(MAKEINTRESOURCE(ID_CURSOR_POINTS));
}

MouseBrush::~MouseBrush()
{

}

void MouseBrush::OnActivate()
{

}

void MouseBrush::OnDeactivate()
{

}

void MouseBrush::MouseClicked(CView* ip_view, WPARAM wParam, LPARAM lParam)
{
	CRendererFrame* render_frame = dynamic_cast<CRendererFrame*>(ip_view);

	if (nullptr == render_frame || !m_active)
		return;

	int xPos = GET_X_LPARAM(lParam);
	int yPos = GET_Y_LPARAM(lParam);

	CMainFrame* pMainWnd = (CMainFrame *)AfxGetMainWnd();
	ModeInformation& mode_info = pMainWnd->GetController()->GetCurrentMode()->GetInformation();

	Vertex* v = new Vertex(IRS::DatabaseInstance->GetGrid()->GetPoint(xPos, yPos), mode_info.GetNextPointIndex());

	IRS::DatabaseInstance->RegisterItem(v);
	render_frame->Invalidate();
}

void MouseBrush::MouseDown(CView* ip_view, int x, int y)
{
	m_pressed = true;

	TryAddPoint(ip_view, x, y);
}

void MouseBrush::MouseUp(CView* ip_view, int x, int y)
{
	m_pressed = false;
}

void MouseBrush::Move(CView* ip_view, int x, int y)
{
	if (!m_pressed)
		return;

	TryAddPoint(ip_view, x, y);
}

void MouseBrush::TryAddPoint(CView* ip_view, int x, int y)
{
	CRendererFrame* render_frame = dynamic_cast<CRendererFrame*>(ip_view);

	if (nullptr == render_frame || !m_active)
		return;

	if (std::abs(x - m_prev_x) < 2 && std::abs(y - m_prev_y) < 2)
		return;

	m_prev_x = x;
	m_prev_y = y;

	CMainFrame* pMainWnd = (CMainFrame *)AfxGetMainWnd();
	ModeInformation& mode_info = pMainWnd->GetController()->GetCurrentMode()->GetInformation();

	Vertex* v = new Vertex(IRS::DatabaseInstance->GetGrid()->GetPoint(x, y), mode_info.GetNextPointIndex());

	IRS::DatabaseInstance->RegisterItem(v);
	render_frame->Invalidate();
}