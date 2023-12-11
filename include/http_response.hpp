#pragma once
#include <curl/curl.h>
#include <string>
#include <map>
using std::string;
using std::map;

class http_response {
public:
    CURLcode curlCode;

    char* body_memory;
    size_t body_size = 0;
    int statusCode;
    map<string, string> headers;

    string str();

    http_response() {
        body_memory = (char*)malloc(1);        
    };

    ~http_response() {
        free(body_memory);
    };    
};