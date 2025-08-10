#pragma once

#include <GL/glew.h>

class VertexArray
{
public:
	VertexArray(
		const float* const pVerts,
		unsigned int numVerts,
		const unsigned int* const pIndices,
		const unsigned int numIndices
	);
	virtual ~VertexArray();

	void SetActive();

	unsigned int GetNumIndices() const;
	unsigned int GetNumVerts() const;

private:
	unsigned int mVertexBuffer;
	unsigned int mNumVerts;

	unsigned int mIndexBuffer;
	unsigned int mNumIndices;

	unsigned int mVertexArray;
};