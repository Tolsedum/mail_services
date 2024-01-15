#include "functions.hpp"

size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}

std::string getHttpPage(std::string url, std::string file_name){
    std::string content;
    CURL* curl;
    FILE* fp;
    curl = curl_easy_init();
    if (curl) {
        fp = fopen(file_name.c_str(), "wb");
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
    }
    return content;
}

std::string removeSymbolse(std::string str, std::string simbol){
    std::size_t pos = 0;;
    while ((pos = str.find(simbol)) != std::string::npos)
        str.replace(pos, 1, "");
    return str;
}

std::string removeSymbolse(std::string str, std::vector<std::string> simbol){
    for(auto c : simbol){
        str = removeSymbolse(str, c);
    }
    return str;
}