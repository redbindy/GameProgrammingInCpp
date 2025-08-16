#pragma once

#include <GL/glew.h>

class VertexArray final
{
public:
	enum class ELayout
	{
		POS_NORM_TEX,
		POS_NORM_SKIN_TEX
	};

public:
	VertexArray(
		const void* const pVerts,
		unsigned int numVerts,
		const ELayout,
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