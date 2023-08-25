#include "http_response.hpp"
#include <iostream>

string http_response::str() {
    return string(body_memory, body_size);
}