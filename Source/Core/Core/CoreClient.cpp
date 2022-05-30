#include "CoreInclude.h"

CoreClient::CoreClient(const int64_t oid) : resolver(ioContext)
{
	this->session = std::make_shared<CoreServerSession>(oid, ioContext);
}

CoreClient::~CoreClient()
{
	if (!this->ioContext.stopped())
		this->ioContext.stop();

	this->asyncThread.join_all();
}

bool CoreClient::Connect(std::string_view ip, std::string_view port)
{
	if (this->session->Connect(resolver.resolve(ip, port)))
	{
		this->asyncThread.create_thread(boost::bind(&boost::asio::io_service::run, &GetContext()));
		return true;
	}
	
	return false;
}

bool CoreClient::IsConnected(void)
{
	return this->session->IsConnected();
}

void CoreClient::Write(const CorePacket& packet)
{
	this->session->Write(packet);
}

boost::asio::io_context& CoreClient::GetContext(void)
{
	return this->ioContext;
}