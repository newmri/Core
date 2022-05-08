#include "Include.h"
#include "json/json.h"

IMPLEMENT_SINGLETON(WebManager)

void WebManager::Init(void)
{
    curl_global_init(CURL_GLOBAL_ALL);
}

void WebManager::Release(void)
{
    curl_global_cleanup();
	GetInstance().~WebManager();
}

void WebManager::Clear(CURL* curl, curl_slist* headers)
{
    if (curl)
        curl_easy_cleanup(curl);

	if (headers)
		curl_slist_free_all(headers);
}

size_t WebManager::curl_callback(void* ptr, size_t size, size_t nmemb, std::string* data)
{
    data->append((char*)ptr, size * nmemb);
    return size * nmemb;
}

void WebManager::GetWorldList(void)
{
	auto response = Get("http://localhost:5000/worldlist");
	if (response.empty())
	{
		CORE_LOG.Log(LogType::LOG_ERROR, "GetWorldList curl Failed");
		abort();
	}

	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(response, root))
	{
		CORE_LOG.Log(LogType::LOG_ERROR, "GetWorldList Parse Failed");
		abort();
	}

	CORE_LOG.Log(LogType::LOG_DEBUG, "[WorldList]");
	auto member = root.getMemberNames();
	for (auto& d : member)
	{
		CORE_LOG.Log(LogType::LOG_DEBUG, d + ": " + root[d].asString());
	}
}

bool WebManager::Signup(std::string_view ID, std::string_view Password)
{
	std::string response;
	{
		Json::Value root;
		root["ID"] = ID.data();
		root["Password"] = Password.data();
		Json::FastWriter fastWriter;
		if (response = Post(fastWriter.write(root), "http://localhost:5000/worldlist/signup"); response.empty())
		{
			CORE_LOG.Log(LogType::LOG_ERROR, "SignUp curl Failed");
			abort();
		}
	}

	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(response, root))
	{
		CORE_LOG.Log(LogType::LOG_ERROR, "SignUp Parse Failed");
		abort();
	}

	return root["IsSuccess"].asBool();
}

std::string WebManager::Get(const char* path)
{
	CURL* curl = curl_easy_init();
	std::string response;
	struct curl_slist* headers = nullptr;
	if (!curl)
	{
		Clear(curl, headers);
		return response;
	}

	headers = curl_slist_append(headers, "Content-Type: application/json");

	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
	curl_easy_setopt(curl, CURLOPT_URL, path);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WebManager::curl_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

	CURLcode status = curl_easy_perform(curl);
	if (status != 0)
	{
		Clear(curl, headers);
		return response;
	}

	Clear(curl, headers);

	response.erase(0, 1);
	response.erase(response.size() - 1, 1);
	return response;
}

std::string WebManager::Post(std::string_view data, const char* path)
{
	CURL* curl = curl_easy_init();
	std::string response;
	struct curl_slist* headers = nullptr;
	if (!curl)
	{
		Clear(curl, headers);
		return response;
	}

	headers = curl_slist_append(headers, "Content-Type: application/json");

	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
	curl_easy_setopt(curl, CURLOPT_URL, path);

	curl_easy_setopt(curl, CURLOPT_POST, 1L);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.data());

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WebManager::curl_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

	CURLcode status = curl_easy_perform(curl);
	if (status != CURLE_OK)
	{
		Clear(curl, headers);
		return response;
	}

	Clear(curl, headers);
	return response;
}
