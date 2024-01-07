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

void setMethodeAndUrlFromH4(
    std::string h4, 
    std::string &methode,
    std::string &url
){
    bool set_methode = true;
    for(char &var : h4){
        if(var == ':'){
            set_methode = false;
        }else if(var != ' '){
            if(set_methode){
                methode.append(1, var);
            }else{
                url.append(1, var);
            }
        }
    }
}

Params insertDataInParams(dom::Node* tr){
    Params params;
    if(tr != nullptr){
        params.is_optional = false;
        for(dom::Node* td : 
            tr->getElementsByTagName("td")
        ){
            if(td != nullptr){
                if(params.name_.empty()){
                    params.name_ = td->getInnerHtml();
                }else if(params.description_.empty()){
                    params.description_ = td->getInnerHtml();
                }else{
                    params.type_ =  td->getInnerHtml();
                }
            }
        }
    }
    
    return params;
}

std::string getInfo(
    std::string methode, 
    std::string url, 
    std::string descr_func,
    std::vector<Params> request_params,
    std::vector<Params> response_params
){
    std::string ret_value;
    std::string space = "    ";
    ret_value.append(
        space + "\"description\": \"" + descr_func + "\",\n"
        + space + "\"methode\": \"" + methode + "\",\n"
        + space + "\"url\": \"" + url + "\",\n"
    );
    ret_value.append(space + "params: {\n");
    int iter = 0;
    for(auto var : request_params){
        std::string optional = "false";
        if(var.is_optional){
            optional = "true";
        }
        if(request_params.size() -1 > iter){
            ret_value.append(
                space + space + "\'" + var.name_ + "\": " + optional + ",\n"
            );
        }else{
            ret_value.append(
                space + space + "\"" + var.name_ + "\": " + optional + "\n"
            );
        }
        iter++;
    }
    ret_value.append(space + "}");

    return ret_value;
}