#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>


struct Params{
    bool is_optional;
    std::string name_;
    std::string description_;
    std::string type_;
};

size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream);
std::string getHttpPage(std::string url, std::string file_name);


#endif /* FUNCTIONS_HPP */
