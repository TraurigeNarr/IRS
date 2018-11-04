#include "stdafx.h"

#include "Vertex.h"

#include "MainFrm.h"
#include "IMode.h"
#include "Renderer.h"
#include "Grid.h"

#include "MainFrm.h"
#include "ModeInformation.h"

#include "Database.h"
#include "IRenderer.h"


Vertex::Vertex (const Vector3D& i_point, size_t i_index)
  : m_scale_factor(1.f)
  , Vector3D(i_point)
  , m_index(i_index)
  , m_color(Color(0,0,255,255).m_color)
  {
  }

Vertex::~Vertex()
{
}

void Vertex::SetScale(float i_scale)
{
  m_scale_factor = i_scale;
}

void Vertex::SetColor(int i_color)
{
  m_color = i_color;
}

size_t Vertex::GetIndex() const
{
  return m_index;
}

void Vertex::Draw(IRenderer* ip_renderer) const
{
	CMainFrame* pMainWnd = (CMainFrame *)AfxGetMainWnd();
	ModeInformation& mode_info = pMainWnd->GetController()->GetCurrentMode()->GetInformation();

	D2D1_POINT_2F coordinates = IRS::DatabaseInstance->GetGrid()->TransformToRendererCoords(*this);

	const double radius = static_cast<double>(mode_info.PointRadius());
	if (IsSelected())
	{
		ip_renderer->RenderCircle(Vector3D(coordinates.x, coordinates.y, 0.), radius*1.5, Color(m_color));
	}
	else
	{
		ip_renderer->RenderRectangle(Vector3D(coordinates.x, coordinates.y, 0.), radius, radius, Color(m_color));
	}

	//draw index of vertex
	if (mode_info.ShowPointNumbers())
	{
		char text[10];
		sprintf_s(text, "%d", GetIndex());
		ip_renderer->RenderText(Vector3D(coordinates.x + 3, coordinates.y - 5, 0.), text, IRS::CL_BLACK);
	}
}

Vector3D Vertex::GetPosition() const
{
  return *this;
}

void Vertex::SetPosition(double i_x, double i_y, double i_z)
{
  (*this)[0] = i_x;
  (*this)[1] = i_y;
  (*this)[2] = i_z;
}