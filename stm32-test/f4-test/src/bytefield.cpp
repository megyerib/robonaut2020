#include "bytefield.h"
#include <cstring>

ByteField::ByteField(size_t* fieldSizes, size_t fieldCnt)
{
	this->fieldSizes = fieldSizes;
	this->fieldCnt = fieldCnt;

	fieldOffsets = new size_t(fieldCnt);

	size_t offset;

	for (size_t i = 0; i < fieldCnt; i++)
	{
		fieldOffsets[i] = offset;
		offset += fieldSizes[i];
	}

	size_t fullDataSize = offset;

	data = new uint8_t(fullDataSize);
}

ByteField::~ByteField()
{
	delete fieldOffsets;
}

void ByteField::WriteField(uint32_t field, void* data)
{
	memcpy(
		&this->data[fieldOffsets[field]],
		data,
		fieldSizes[field]
	);
}

void ByteField::ReadField(uint32_t field, void* data)
{
	memcpy(
		data,
		&this->data[fieldOffsets[field]],
		fieldSizes[field]
	);
}
