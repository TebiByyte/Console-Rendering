#pragma once
#include "Color.h"
#include "VertexAttribute.h"
#include "Triangle.h"
#include <Windows.h>
#include <list>

class Mesh {
public:
	Triangle* m_data;
	VertexAttribute* m_verts;

	Mesh(std::string);
private:

};