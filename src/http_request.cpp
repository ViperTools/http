#include "http_request.hpp"

map<HttpMethod, string> httpMethodNames {
    { HttpMethod::GET, "GET" },
    { HttpMethod::HEAD, "HEAD" },
    { HttpMethod::POST, "POST" },
    { HttpMethod::PUT, "PUT" },
    { HttpMethod::DELETE, "DELETE" },
    { HttpMethod::CONNECT, "CONNECT" },
    { HttpMethod::OPTIONS, "OPTIONS" },
    { HttpMethod::TRACE, "TRACE" },
    { HttpMethod::PATCH, "PATCH" }
};

map<HttpMethod, bool> methodIncludesBody {
    { HttpMethod::GET, false },
    { HttpMethod::HEAD, false },
    { HttpMethod::POST, true },
    { HttpMethod::PUT, true },
    { HttpMethod::DELETE, false },
    { HttpMethod::CONNECT, false },
    { HttpMethod::OPTIONS, true },
    { HttpMethod::TRACE, false },
    { HttpMethod::PATCH, true }
};