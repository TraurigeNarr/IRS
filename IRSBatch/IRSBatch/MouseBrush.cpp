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
#include "VertexSet.h"

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
}

void MouseBrush::MouseDown(CView* ip_view, int x, int y)
{
	m_pressed = true;

	CMainFrame* pMainWnd = (CMainFrame *)AfxGetMainWnd();
	ModeInformation& mode_info = pMainWnd->GetController()->GetCurrentMode()->GetInformation();
	mp_current_set = new VertexSet(mode_info.GetNextVSetIndex());
	IRS::DatabaseInstance->RegisterItem(mp_current_set);

	TryAddPoint(ip_view, x, y);
}

void MouseBrush::MouseUp(CView* ip_view, int x, int y)
{
	m_pressed = false;
	mp_current_set = nullptr;
}

void MouseBrush::Move(CView* ip_view, int x, int y)
{
	if (!m_pressed)
		return;

	TryAddPoint(ip_view, x, y);
}

void MouseBrush::TryAddPoint(CView* ip_view, int x, int y)
{
	Assert(mp_current_set != nullptr);
	if (mp_current_set == nullptr)
		return;

	CRendererFrame* render_frame = dynamic_cast<CRendererFrame*>(ip_view);

	if (nullptr == render_frame || !m_active)
		return;

	int change = std::abs(x - m_prev_x) + std::abs(y - m_prev_y);
	if (change < 1)
		return;

	m_prev_x = x;
	m_prev_y = y;

	CMainFrame* pMainWnd = (CMainFrame *)AfxGetMainWnd();
	ModeInformation& mode_info = pMainWnd->GetController()->GetCurrentMode()->GetInformation();
	
	const int brush_size = 3;
	for (int i = -brush_size; i <= brush_size; ++i)
	{
		for (int j = -brush_size; j <= brush_size; ++j)
		{
			mp_current_set->AddVertex(Vertex(IRS::DatabaseInstance->GetGrid()->GetPoint(x + i, y + j), mode_info.GetNextPointIndex()));
		}
	}

	render_frame->Invalidate();
}