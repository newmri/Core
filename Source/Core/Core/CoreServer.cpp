#include "CoreInclude.h"

CoreServer::CoreServer(const unsigned short port) : acceptor(ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{

}

CoreServer::~CoreServer()
{

}

void CoreServer::Run(void)
{
	this->asyncWork.reset(new boost::asio::io_context::work(this->ioContext));
	this->asyncThread.create_thread(boost::bind(&boost::asio::io_context::run, &this->ioContext));

	CORE_LOG.Log("Server is Running Port: " + TO_STR(this->acceptor.local_endpoint().port()));

	Accept();
}

void CoreServer::Stop(void)
{
	this->asyncWork.reset();
	this->ioContext.stop();
	this->asyncThread.join_all();

	CORE_LOG.Log("Server Stoped");
}

void CoreServer::Accept(void)
{
	this->acceptor.async_accept(
		[this](boost::system::error_code error, boost::asio::ip::tcp::socket socket)
		{
			if (error)
			{
				CORE_LOG.Log(LogType::LOG_ERROR, "accept error " + error.value() + error.message());
			}
			else
			{
				CORE_LOG.Log("Client Connected");

				auto session = std::make_shared<CoreClientSession>(std::move(socket), this);

				this->sessionList.push_back(session);

				session->Start();
			}

			Accept();
		});
}

void CoreServer::Close(std::shared_ptr<CoreClientSession> session)
{
	if (session->IsConnected())
	{
		CORE_LOG.Log("Client Disconnected");
		boost::asio::ip::tcp::socket& socket = session->GetSocket();
		socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
		socket.close();
		this->sessionList.remove(session);
	}
}