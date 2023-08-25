#pragma once
#include <cstring>
#include "http_request.hpp"
#include "http_response.hpp"

class http {
public:
    static http_response send(http_request req);
    static CURL* handle;
private:
    static size_t write_callback(void* data, size_t size, size_t nmemb, void* userdata);
    static size_t header_callback(char* buffer, size_t size, size_t nitems, void* userdata);
};