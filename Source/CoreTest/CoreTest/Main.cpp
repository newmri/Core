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


	while (true)
	{
		Sleep(100);
		if (client.IsConnected())
		{
			flatbuffers::FlatBufferBuilder builder;

			auto name = builder.CreateString("Hello World!");
			auto data = Login::CreateCS_LOGIN_REQ(builder, name);
			auto root = Login::CreateRoot(builder, Login::Packet_CS_LOGIN_REQ, data.Union());

			builder.Finish(root);

			client.Write(CorePacket(builder.GetBufferPointer(), builder.GetSize()));
			break;
		}
	}
	t.join();

	return 0;
}