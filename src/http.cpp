#include "http.hpp"
#include "http_response.hpp"
#include "http_request.hpp"
#include <cstring>
#include <curl/curl.h>
#include <curl/easy.h>
#include <iostream>

CURL* http::handle = curl_easy_init();

/**
 * @brief Writes response body to a http_response object
 * 
 * @param data The data to be written
 * @param size The size of one data item
 * @param nmemb The number of data items
 * @param userdata A pointer to the http_response object to be written to
 * @return size_t The total size of the chunk written
 */

size_t http::write_callback(void* data, size_t size, size_t nmemb, void* userdata) {
    size_t rsize = size * nmemb;
    http_response* res = (http_response*)userdata;

    char* mem = (char*)realloc(res->body_memory, res->body_size + rsize + 1);

    if (mem == nullptr) {
        return 0;
    }

    res->body_memory = mem;
    memcpy(&(res->body_memory[res->body_size]), data, rsize);
    res->body_size += rsize;
    res->body_memory[res->body_size] = 0;
    
    return rsize;
}

/**
 * @brief Writes header data to a http_response object
 * 
 * @param buffer The buffer to read from
 * @param size The size of one data item
 * @param nitems The number of data items
 * @param userdata A pointer to the http_response object to be written to
 * @return size_t The total size of the chunk read
 */

size_t http::header_callback(char* buffer, size_t size, size_t nitems, void* userdata) {
    size_t rsize = size * nitems;
    http_response* res = (http_response*)userdata;
    
    string header(buffer, rsize);
    int name_end = header.find(':');
    int header_end = std::min(header.find('\n'), header.find('\r'));
    
    res->headers[header.substr(0, name_end)] = header.substr(name_end + 2, header_end - name_end);
    
    return rsize;
}

size_t http::download_write_callback(void *ptr, size_t size, size_t nmemb, void* userdata) {
    return fwrite(ptr, size, nmemb, (FILE*)userdata);
}

http_response http::send(http_request req) {
    // Basic options
    curl_easy_setopt(handle, CURLOPT_URL, req.url.c_str());
    curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, httpMethodNames[req.method].c_str());
    curl_easy_setopt(http::handle, CURLOPT_FOLLOWLOCATION, true);

    // Set headers
    curl_slist* headers = NULL;
    
    for (const auto& [name, value] : req.headers) {
        headers = curl_slist_append(headers, (name + ":" + value).c_str());
    }

    curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headers);

    // Set request body
    if (methodIncludesBody[req.method]) {
        curl_easy_setopt(handle, CURLOPT_POSTFIELDS, req.body.c_str());
    }

    // Setup response body
    http_response res;
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &res);
    
    // Setup response headers (disabled because headers are unused in this project and this would slightly impact performance)
    // curl_easy_setopt(handle, CURLOPT_HEADERFUNCTION, header_callback);
    // curl_easy_setopt(handle, CURLOPT_HEADERDATA, &res);

    res.code = curl_easy_perform(handle);

    // Cleanup
    if (headers != NULL) {
        curl_slist_free_all(headers);
    }

    curl_easy_reset(handle);

    return res;
}

void http::download(const char* url, const char* path) {
    // Basic options
    curl_easy_setopt(handle, CURLOPT_URL, url);
    curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(http::handle, CURLOPT_FOLLOWLOCATION, true);

    // Setup response body
    FILE* file = fopen(path, "wb");

    if (file) {
        http_response res;
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, download_write_callback);
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, file);
        curl_easy_perform(handle);
        fclose(file);
    }
    

    curl_easy_reset(handle);
}