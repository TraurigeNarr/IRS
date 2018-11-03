#include "stdafx.h"

#include "VertexSet.h"

#include "Renderer.h"


void VertexSet::Draw(IRenderer* ip_renderer) const
{
	for (auto& vertex : m_vertices)
	{
		vertex.Draw(ip_renderer);
	}
}

void VertexSet::AddVertex(Vertex&& vertex)
{
	m_vertices.emplace_back(std::move(vertex));
}

void VertexSet::Clear()
{
	m_vertices.clear();
}