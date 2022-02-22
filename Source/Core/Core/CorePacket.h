#pragma once

class CorePacket
{
public:
	enum { HEADER_SIZE = 4 };
	enum { MAX_BODY_SIZE = 512 };

public:
	DEFAULT_CONSTRUCTOR(CorePacket)
	CorePacket(const uint8_t* body, const uint32_t bodySize);

public:
	const uint8_t* GetData(void) const;
	uint8_t* GetData(void);

public:
	const uint8_t* GetBody(void) const;
	uint8_t* GetBody(void);

public:
	uint32_t GetSize() const;
	uint32_t GetBodySize(void) const;
	void SetBodySize(const uint32_t newSize);

public:
	bool DecodeHeader(void);
	void EncodeHeader(void);

private:
	uint8_t data[HEADER_SIZE + MAX_BODY_SIZE] = { 0, };
	uint32_t bodySize = 0;
};