#pragma once

#include "IMouse.h"

class MouseBrush : public IMouse
{
public:
	MouseBrush();
	~MouseBrush();

	virtual void MouseClicked(CView* ip_view, WPARAM wParam, LPARAM lParam) override;
	virtual void MouseDown(CView* ip_view, int x, int y) override;
	virtual void MouseUp(CView* ip_view, int x, int y) override;
	virtual void Move(CView* ip_view, int x, int y) override;

	virtual void OnActivate() override;
	virtual void OnDeactivate() override;

private:
	void TryAddPoint(CView* ip_view, int x, int y);

private:
	bool m_pressed = false;

	int m_prev_x = 0;
	int m_prev_y = 0;
};