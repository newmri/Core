#pragma once

class CorePacket
{
public:
	enum { HEADER_SIZE = 4 };
	enum { MAX_BODY_SIZE = 512 };

public:
	DEFAULT_CONSTRUCTOR(CorePacket)
	CorePacket(const uint8_t* body, const size_t bodySize);

public:
	const uint8_t* GetData(void) const;
	uint8_t* GetData(void);

public:
	const uint8_t* GetBody(void) const;
	uint8_t* GetBody(void);

public:
	size_t GetSize() const;
	size_t GetBodySize(void) const;
	void SetBodySize(const size_t newSize);

public:
	bool DecodeHeader(void);
	void EncodeHeader(void);

private:
	uint8_t data[HEADER_SIZE + MAX_BODY_SIZE] = { 0, };
	size_t bodySize = 0;
};