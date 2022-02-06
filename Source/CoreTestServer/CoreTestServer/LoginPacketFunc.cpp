#include "Include.h"

void LoginPacketFunc::CS_LOGIN_REQ(const void* data)
{
	auto raw = static_cast<const Login::CS_LOGIN_REQ*>(data);
	CORE_LOG.Log(raw->name()->c_str());
}