#pragma once

#include "MicroEngine.h"

class TriangleMaterial
{
public:
	TriangleMaterial() = default;
	TriangleMaterial(TriangleMaterial&&) = default;
	TriangleMaterial(const TriangleMaterial&) = default;
	TriangleMaterial(const Triangle& inTri) : v{ inTri[0], inTri[1], inTri[2] } {}
	TriangleMaterial(const Vector3& v1, const Vector3& v2, const Vector3& v3) : v{ v1, v2, v3 } {}

	TriangleMaterial& operator=(TriangleMaterial&&) = default;
	TriangleMaterial& operator=(const TriangleMaterial&) = default;

	constexpr Vector3& operator[](size_t i) noexcept { return v[i]; }
	constexpr const Vector3& operator[](size_t i) const noexcept { return v[i]; }

	Vector3 GetCentroid() const { return (v[0] + v[1] + v[2]) * (1.0f / 3.0f); }

	// Vertices
	Vector3 v[3];
	uint32_t materialIndex = 0;
};

using TriangleList = std::vector<TriangleMaterial>;
using VertexList = std::vector<Vector3>;

// Triangle with 32-bit indices and material index
class IndexedTriangle
{
public:
	IndexedTriangle() = default;
	IndexedTriangle(uint32_t inI1, uint32_t inI2, uint32_t inI3) { idx[0] = inI1; idx[1] = inI2; idx[2] = inI3; }
	IndexedTriangle(uint32_t inI1, uint32_t inI2, uint32_t inI3, uint32_t inMaterialIndex) { idx[0] = inI1; idx[1] = inI2; idx[2] = inI3; materialIndex = inMaterialIndex; }

	// Check if two triangles are identical
	bool operator==(const IndexedTriangle& inRHS) const
	{
		return idx[0] == inRHS.idx[0] && idx[1] == inRHS.idx[1] && idx[2] == inRHS.idx[2] && materialIndex == inRHS.materialIndex;
	}

	// Check if two triangles are equivalent (using the same vertices)
	bool IsEquivalent(const IndexedTriangle& inRHS) const
	{
		return (idx[0] == inRHS.idx[0] && idx[1] == inRHS.idx[1] && idx[2] == inRHS.idx[2])
			|| (idx[0] == inRHS.idx[1] && idx[1] == inRHS.idx[2] && idx[2] == inRHS.idx[0])
			|| (idx[0] == inRHS.idx[2] && idx[1] == inRHS.idx[0] && idx[2] == inRHS.idx[1]);
	}

	// Check if two triangles are opposite (using the same vertices but in opposing order)
	bool IsOpposite(const IndexedTriangle& inRHS) const
	{
		return (idx[0] == inRHS.idx[0] && idx[1] == inRHS.idx[2] && idx[2] == inRHS.idx[1])
			|| (idx[0] == inRHS.idx[1] && idx[1] == inRHS.idx[0] && idx[2] == inRHS.idx[2])
			|| (idx[0] == inRHS.idx[2] && idx[1] == inRHS.idx[1] && idx[2] == inRHS.idx[0]);
	}

	// Check if triangle is degenerate
	bool IsDegenerate() const
	{
		return idx[0] == idx[1] || idx[1] == idx[2] || idx[2] == idx[0];
	}

	// Rotate the vertices so that the second vertex becomes first etc. This does not change the represented triangle.
	void Rotate()
	{
		uint32_t tmp = idx[0];
		idx[0] = idx[1];
		idx[1] = idx[2];
		idx[2] = tmp;
	}

	// Get center of triangle
	Vector3 GetCentroid(const VertexList& inVertices) const
	{
		return (inVertices[idx[0]] + inVertices[idx[1]] + inVertices[idx[2]]) / 3.0f;
	}

	// Rotate the vertices so that the lowest vertex becomes the first. This does not change the represented triangle.
	IndexedTriangle	GetLowestIndexFirst() const
	{
		if( idx[0] < idx[1] )
		{
			if( idx[0] < idx[2] )
				return IndexedTriangle(idx[0], idx[1], idx[2], materialIndex); // 0 is smallest
			else
				return IndexedTriangle(idx[2], idx[0], idx[1], materialIndex); // 2 is smallest
		}
		else
		{
			if( idx[1] < idx[2] )
				return IndexedTriangle(idx[1], idx[2], idx[0], materialIndex); // 1 is smallest
			else
				return IndexedTriangle(idx[2], idx[0], idx[1], materialIndex); // 2 is smallest
		}
	}

	uint32_t idx[3];
	uint32_t materialIndex = 0;
};

using IndexedTriangleList = std::vector<IndexedTriangle>;