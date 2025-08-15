#include "VertexArray.h"

VertexArray::VertexArray(
	const void* const pVerts,
	unsigned int numVerts,
	ELayout layout,
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

	unsigned int vertexSize = 8 * sizeof(float);
	if (layout == ELayout::POS_NORM_SKIN_TEX)
	{
		vertexSize += 8 * sizeof(char);
	}

	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, numVerts * vertexSize, pVerts, GL_STATIC_DRAW);

	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), pIndices, GL_STATIC_DRAW);

	if (layout == ELayout::POS_NORM_TEX)
	{
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<void*>(sizeof(float) * 3));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<void*>(sizeof(float) * 6));
	}
	else if (layout == ELayout::POS_NORM_SKIN_TEX)
	{
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<void*>(sizeof(float) * 3));

		glEnableVertexAttribArray(2);
		glVertexAttribIPointer(2, 4, GL_UNSIGNED_BYTE, vertexSize, reinterpret_cast<void*>(sizeof(float) * 6));

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE, vertexSize, reinterpret_cast<void*>(sizeof(float) * 6 + sizeof(char) * 4));

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<void*>(sizeof(float) * 6 + sizeof(char) * 8));
	}
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

unsigned int VertexArray::GetNumIndices() const
{
	return mNumIndices;
}

unsigned int VertexArray::GetNumVerts() const
{
	return mNumVerts;
}
