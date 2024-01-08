#ifndef MAIL_365_HPP
#define MAIL_365_HPP

#include <string>
#include <filesystem>
#include <boost/algorithm/string.hpp>
#include "dom/DomDocument.hpp"
#include "functions.hpp"

namespace mail365{
    struct ReturnsParams{
        std::string block_name;
        std::string content;
    };

    void createJsonApiFile(std::string file_name);
    ReturnsParams parsStepByLi(dom::Node* node);
    std::string genereteContentFromLi(dom::Node* node);

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
}


#endif /* MAIL_365_HPP */
