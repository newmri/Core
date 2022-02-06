#include "CoreInclude.h"

CorePacket::CorePacket(const uint8_t* body, const size_t bodySize) : bodySize(bodySize)
{
	std::copy(body, body + bodySize, GetBody());
	EncodeHeader();
}

const uint8_t* CorePacket::GetData(void) const
{
	return this->data;
}

uint8_t* CorePacket::GetData(void)
{
	return this->data;
}

const uint8_t* CorePacket::GetBody(void) const
{
	return this->data + HEADER_SIZE;
}

uint8_t* CorePacket::GetBody(void)
{
	return this->data + HEADER_SIZE;
}

size_t CorePacket::GetSize() const
{
	return HEADER_SIZE + this->bodySize;
}

size_t CorePacket::GetBodySize(void) const
{
	return  this->bodySize;
}

void CorePacket::SetBodySize(const size_t newSize)
{
	this->bodySize = newSize;
}

bool CorePacket::DecodeHeader(void)
{
	char header[HEADER_SIZE + 1] = { 0, };
	strncat_s(header, (char*)this->data, HEADER_SIZE);

	this->bodySize = std::atoi(header);
	if (this->bodySize > MAX_BODY_SIZE)
	{
		this->bodySize = 0;
		return false;
	}

	return true;
}

void CorePacket::EncodeHeader(void)
{
	char header[HEADER_SIZE + 1] = { 0, };
	sprintf_s(header, "%4d", static_cast<int>(this->bodySize));
	memcpy(this->data, header, HEADER_SIZE);
}
