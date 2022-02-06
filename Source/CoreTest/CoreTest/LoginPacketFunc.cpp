#include "Include.h"

void LoginPacketFunc::SC_LOGIN_RES(const void* data)
{
	auto raw = static_cast<const Login::SC_LOGIN_RES*>(data);
	CORE_LOG.Log(TO_STR(raw->result()));
}