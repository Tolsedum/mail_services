#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include "dom/DomDocument.hpp"

struct Params{
    bool is_optional;
    std::string name_;
    std::string description_;
    std::string type_;
};

size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream);
std::string getHttpPage(std::string url, std::string file_name);

void setMethodeAndUrlFromH4(
    std::string h4, 
    std::string &methode,
    std::string &url
);

Params insertDataInParams(dom::Node* tr);

std::string getInfo(
    std::string methode, 
    std::string url, 
    std::string descr_func,
    std::vector<Params> request_params,
    std::vector<Params> response_params
);

#endif /* FUNCTIONS_HPP */
