#include "Include.h"

int main(void)
{
#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG

	LoginClient client;

	client.Connect("127.0.0.1", "7777");
	boost::asio::io_context& ioContext = client.GetContext();
	std::thread t([&ioContext]() { ioContext.run(); });

	t.join();

	return 0;
}