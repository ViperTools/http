#pragma once
#include <string>
#include <map>
using std::string;
using std::map;

enum class HttpMethod {
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE,
    PATCH
};

extern map<HttpMethod, string> httpMethodNames;
extern map<HttpMethod, bool> methodIncludesBody;

class http_request {
    public:
        HttpMethod method;
        string url;
        map<string, string> headers;
        string body;
        string content_type;

        void set_header(string name, string value) {
            headers[name] = value;
        }

        http_request(HttpMethod method, string url)
            : method(method),
            url(url)
        {
            if (!content_type.empty()) {
                headers["Content-Type"] = content_type;
            }
        };
};