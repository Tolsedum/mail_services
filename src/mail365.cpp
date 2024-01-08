#include "mail365.hpp"

void mail365::createJsonApiFile(std::string file_name){
    std::string path = "mail365";
    if(!std::filesystem::exists(file_name)){
        std::string link = "https://www.mail365.ru/api.php";
        std::string content = getHttpPage(link, file_name);
    }
    if(!std::filesystem::exists(path)){
        std::filesystem::create_directory(path);
    }
    // std::string output_data;
    dom::DomDocument domDocument;
    domDocument.parsDocument(file_name);
 
    dom::Node *body  = domDocument.getNodeByTagName("body");
    if(body != nullptr){
        dom::Node* methods = (*body).getElementById("methods");
        if(methods != nullptr){
            for(dom::Node* li : methods->getElementsByTagName("li")){
                ReturnsParams params = parsStepByLi(li);
                
                // std::ofstream out(path + "/" + params.block_name + ".html", std::ios::trunc);
                // if(out.is_open()){
                //     out << params.content;
                // }
                // out.close();
                // break;
            } 
        }
    }
}

mail365::ReturnsParams mail365::parsStepByLi(dom::Node* li){
    ReturnsParams ret_value;
    if(li != nullptr){
        std::string methode, url, descr_func;
        std::vector<Params> request_params;
        std::vector<Params> response_params;

        setMethodeAndUrlFromH4(
            li->getElementByTagName("h4")
                ->getInnerHtml(),
            methode,
            url
        ); 
        ret_value.block_name = url.replace(0,1, "");

        std::cout<< ret_value.block_name << std::endl;

        dom::Node *blockquote = li->getElementByTagName("blockquote");
        if(blockquote != nullptr){
            std::vector<dom::Node*> ps = blockquote
                ->getElementsByTagName("p");
            
            if(!ps.empty()){
                std::vector<dom::Node*> tab = 
                    blockquote->getElementsByTagName("tbody");
                if(!tab.empty()){
                    descr_func = ps[0]->getInnerHtml();
                    
                    int count_iter = 0;
                    for(dom::Node* tr : tab[0]
                        ->getElementsByTagName("tr")
                    ){
                        if(tr != nullptr){
                            if(count_iter > 0){
                                request_params.push_back(
                                    insertDataInParams(tr)
                                );
                            }else{
                                count_iter++;
                            }
                        }
                    }
                    count_iter = 0;
                    for(dom::Node* tr : tab[1]
                        ->getElementsByTagName("tr")
                    ){
                        if(count_iter > 0){
                            response_params.push_back(
                                insertDataInParams(tr)
                            );
                        }else{
                            count_iter++;
                        }
                    }
                }
            }
        }
        ret_value.content.append(getInfo(
            methode, 
            url, 
            descr_func,
            request_params,
            response_params
        ));
        
    }
    return ret_value;
}


void mail365::setMethodeAndUrlFromH4(
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

Params mail365::insertDataInParams(dom::Node* tr){
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
                    std::string descr = td->getInnerHtml();
                    std::size_t pos = descr.find('"');
                    if(pos != std::string::npos){
                        while (pos != std::string::npos){
                            descr.replace(pos, 1, "'");
                            pos = descr.find('"');
                        }
                    }
                    params.description_ = descr;
                }else{
                    params.type_ =  td->getInnerHtml();
                }
            }
        }
    }
    
    return params;
}

std::string mail365::getInfo(
    std::string methode, 
    std::string url, 
    std::string descr_func,
    std::vector<Params> request_params,
    std::vector<Params> response_params
){
    std::string ret_value = "{\n";
    std::string space = "    ";
    ret_value.append(
        space + "\"description\": \"" + descr_func + "\",\n"
            + space + "\"methode\": \"" 
            + boost::algorithm::to_lower_copy(methode)
            + "\",\n"
        + space + "\"url\": \"" + url + "\",\n"
    );
    ret_value.append(space + "\"patern\": {\n");
    std::size_t iter = 0;
    for(auto var : request_params){
        std::string end = "";
        std::string optional = "false";
        if(var.is_optional){
            optional = "true";
        }
        if(request_params.size() -1 > iter){
            end = ",";
        }
        ret_value.append(
            space + space + "\'" + var.name_ + "\": {\n" 
                + space + space + space + "\"optional:\" " +  optional + ",\n"
                + space + space + space + "\"description\": " + "\"" + var.description_ + "\""
                + "\n" + space + space + "}" + end + "\n"
        );
        iter++;
    }
    ret_value.append(space + "}");

    if(!response_params.empty()){
        ret_value.append(",\n" + space + "return_value:{\n");
        iter = 0;
        for(Params var : response_params){
            std::string end = "";
            if(request_params.size() -1 > iter){
                end = ",";
            }
            ret_value.append(
                space + space + "\'" + var.name_ + "\": {\n" 
                    + space + space + space + "\"description\": " + "\"" + var.description_ + "\""
                    + "\n" + space + space + "}" + end + "\n"
            );
            iter++;
        }
        ret_value.append(space + "}\n}");
    }
    return ret_value;
}
