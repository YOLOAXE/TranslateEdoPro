#include "translate.h"

static size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

std::string translate::translateTO(std::string value)
{
	std::string result;
	result = "{ \"input\": \"" + value + "\", \"lang\": \"fr\"}";
	
	const char * c = result.c_str();
	
	CURL *hnd = curl_easy_init();

	curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
	curl_easy_setopt(hnd, CURLOPT_URL, "https://kiara-translate.p.rapidapi.com/get_translated/");

	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "x-rapidapi-host: kiara-translate.p.rapidapi.com");
	headers = curl_slist_append(headers, "x-rapidapi-key: aedd6ef48bmsh17583e8a3de6e9ap127fcajsn63cc5ae3404c");
	headers = curl_slist_append(headers, "content-type: application/json");
	headers = curl_slist_append(headers, "accept: application/json");
	curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

	//curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, "{ \"input\": \"Hello world\", \"lang\": \"fr\"}");
	curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, c);

	curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &result);

	curl_easy_perform(hnd);
	system("CLS");
	std::cout << std::flush;
	system("CLS");
	std::cout << "-----------------------------------------------------------------------------------------------------" << std::endl << std::endl;
	result.erase(0, result.find("translate"));
	const char * toreplace = "";
	result = std::regex_replace(result, std::regex("u00e9"), "e");
	result = std::regex_replace(result, std::regex("u00e8"), "e");
	result = std::regex_replace(result, std::regex("&#39;"), "'");
	result = std::regex_replace(result, std::regex("u00e0"), "a");
	result = std::regex_replace(result, std::regex("u00e2"), "a");
	result = std::regex_replace(result, std::regex("u00ea"), "e");
	result = std::regex_replace(result, std::regex("u00c8"), "E");
	result = std::regex_replace(result, std::regex("u00e7"), "c");
	result = std::regex_replace(result, std::regex("u00f4"), "o");
	result = std::regex_replace(result, std::regex("translated\": \""), "");
	result = std::regex_replace(result, std::regex("\", \"target_lang\": \"fr\", \"source_lang\": \"en\", \"response\": \"Successful\", \"status\": 200"), "");
	result.erase(std::remove(result.begin(), result.end(), 92), result.end());
	result.erase(std::remove(result.begin(), result.end(), 125), result.end());
	return result;
}