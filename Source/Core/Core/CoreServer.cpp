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

	unsigned int threadNum = (std::thread::hardware_concurrency() * 2) + 1;
	for(unsigned int i = 0; i < threadNum; ++i)
		this->asyncThread.create_thread(boost::bind(&boost::asio::io_context::run, &this->ioContext));

	CORE_LOG.Log(LogType::LOG_DEBUG, "Server is Running [Thread Num]: " +
		         TO_STR(threadNum) + " [Port]: " +
		         TO_STR(this->acceptor.local_endpoint().port()));

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
				size_t uid = this->uid.fetch_add(1);
				std::string ip = socket.remote_endpoint().address().to_string();
				CORE_LOG.Log(LogType::LOG_CONNECT, "[uid]: " + TO_STR(uid) + " [ip]: " + ip);

				auto session = std::make_shared<CoreClientSession>(std::move(socket), uid, this);
				session->Start();

				WRITE_LOCK(this->mutex);
				this->sessionList[uid] = session;
			}

			Accept();
		});
}

void CoreServer::Close(std::shared_ptr<CoreClientSession> session)
{
	if (session->IsConnected())
	{
		boost::asio::ip::tcp::socket& socket = session->GetSocket();
		size_t uid = session->GetUID();
		std::string ip = socket.remote_endpoint().address().to_string();
		CORE_LOG.Log(LogType::LOG_DISCONNECT, "[uid]: " + TO_STR(uid) + " [ip]: " + ip);

		socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
		socket.close();

		WRITE_LOCK(this->mutex);
		this->sessionList.erase(this->sessionList.find(uid));
	}
}