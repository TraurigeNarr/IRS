#pragma once

#include "IRenderable.h"

#include "Vertex.h"

class Renderer;

class VertexSet : public IRenderable
{
private:
	std::vector<Vertex> m_vertices;
	size_t m_index;

public:
	VertexSet(size_t index)
		: m_index(index)
	{}
	virtual void Draw(IRenderer* renderer) const override;
	void AddVertex(Vertex&& vertex);
	void Clear();

	virtual Vector3D GetPosition() const override
	{
		return Vector3D(0, 0, 0);
	}

	size_t GetIndex() const {
		return m_index;
	}
};