#include "VertexArray.h"

constexpr int COUNT = 5;

VertexArray::VertexArray(
	const float* const pVerts,
	const unsigned int numVerts,
	const unsigned int* const pIndices,
	const unsigned int numIndices
)
	: mVertexBuffer(0)
	, mNumVerts(numVerts)
	, mIndexBuffer(0)
	, mNumIndices(numIndices)
	, mVertexArray(0)
{
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);

	glBufferData(
		GL_ARRAY_BUFFER,
		numVerts * COUNT * sizeof(float),
		pVerts,
		GL_STATIC_DRAW
	);

	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		numIndices * sizeof(unsigned int),
		pIndices,
		GL_STATIC_DRAW
	);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(float) * COUNT,
		0
	);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(float) * COUNT,
		reinterpret_cast<void*>(sizeof(float) * 3)
	);
}

VertexArray::~VertexArray()
{
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteBuffers(1, &mIndexBuffer);
	glDeleteVertexArrays(1, &mVertexArray);
}

void VertexArray::SetActive()
{
	glBindVertexArray(mVertexArray);
}
