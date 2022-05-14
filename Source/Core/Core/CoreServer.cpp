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

	int32_t threadNum = (std::thread::hardware_concurrency() * 2) + 1;
	for(int32_t i = 0; i < threadNum; ++i)
		this->asyncThread.create_thread(boost::bind(&boost::asio::io_context::run, &this->ioContext));

	CORE_LOG.Log(LogType::LOG_DEBUG, "Server is Running...");
	CORE_LOG.Log(LogType::LOG_DEBUG, "[Thread Num]: " + TO_STR(threadNum));
	CORE_LOG.Log(LogType::LOG_DEBUG, "[Port]: " + TO_STR(this->acceptor.local_endpoint().port()));

	Accept();
}

void CoreServer::Stop(void)
{
	this->asyncWork.reset();
	this->ioContext.stop();
	this->asyncThread.join_all();

	CORE_TIME_DELEGATE_MANAGER.Stop();

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
				int64_t oid = this->oid.fetch_add(1);
				std::string ip = socket.remote_endpoint().address().to_string();
				CORE_LOG.Log(LogType::LOG_CONNECT, oid, "[ip]: " + ip);

				auto session = std::make_shared<CoreClientSession>(oid, std::move(socket), this);
				session->Start();

				WRITE_LOCK(this->mutex);
				this->sessionList[oid] = session;
			}

			Accept();
		});
}

bool CoreServer::IsValidSession(const int64_t oid)
{
	READ_LOCK(this->mutex);

	auto iter = this->sessionList.find(oid);
	if (IS_NOT_SAME(iter, this->sessionList.end()))
		return true;

	return false;
}

void CoreServer::Close(std::shared_ptr<CoreClientSession> session, bool needSocketClose)
{
	if (session->IsConnected())
	{
		boost::asio::ip::tcp::socket& socket = session->GetSocket();
		int64_t oid = session->GetOID();
		if (!IsValidSession(oid))
			return;

		CORE_LOG.Log(LogType::LOG_DISCONNECT, oid, "");

		if (needSocketClose)
		{
			socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
			socket.close();
		}

		CORE_ACCOUNT_MANAGER.SetLogout(session->GetAccountUID());

		WRITE_LOCK(this->mutex);
		this->sessionList.erase(oid);
	}
}