#pragma once
#define CURL_STATICLIB
#include <iostream>
#include <string>
#include <curl/curl.h>
#include <regex>

class translate
{
public:
	std::string translateTO(std::string value);
};

