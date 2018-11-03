#pragma once

#include "IRenderable.h"

#include "Vertex.h"

class Renderer;

class VertexSet : public IRenderable
{
private:
	std::vector<Vertex> m_vertices;

public:
	virtual void Draw(IRenderer* renderer) const override;
	void AddVertex(Vertex&& vertex);
	void Clear();
};