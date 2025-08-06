#pragma once

#include <GL/glew.h>

class VertexArray final
{
public:
	VertexArray(
		const float* const pVerts,
		const unsigned int numVerts,
		const unsigned int* const pIndices,
		const unsigned int numIndices
	);

	virtual ~VertexArray();

	void SetActive();

	inline unsigned int GetNumIndices() const;
	inline unsigned int GetNumVerts() const;

private:
	unsigned int mVertexBuffer;
	unsigned int mNumVerts;

	unsigned int mIndexBuffer;
	unsigned int mNumIndices;

	unsigned int mVertexArray;
};

inline unsigned int VertexArray::GetNumIndices() const
{
	return mNumIndices;
}

inline unsigned int VertexArray::GetNumVerts() const
{
	return mNumVerts;
}