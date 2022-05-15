#include "CoreInclude.h"

CorePacket::CorePacket(const uint8_t* body, const uint32_t bodySize) : bodySize(bodySize)
{
	if (MAX_BODY_SIZE < bodySize)
	{
		CORE_LOG.Log(LogType::LOG_ERROR, "body size is " + TO_STR(bodySize));
		return;
	}

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

uint32_t CorePacket::GetSize() const
{
	return HEADER_SIZE + this->bodySize;
}

uint32_t CorePacket::GetBodySize(void) const
{
	return this->bodySize;
}

void CorePacket::SetBodySize(const uint32_t newSize)
{
	this->bodySize = newSize;
}

bool CorePacket::DecodeHeader(void)
{
	memcpy_s(&this->bodySize, sizeof(this->bodySize), &this->data, HEADER_SIZE);

	if (this->bodySize > MAX_BODY_SIZE || this->bodySize <= 0)
	{
		this->bodySize = 0;
		return false;
	}

	return true;
}

void CorePacket::EncodeHeader(void)
{
	memcpy_s(this->data, sizeof(this->data), &this->bodySize, HEADER_SIZE);
}
