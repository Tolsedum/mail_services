#include "mail365.hpp"
std::string mail365::getMainBlockName(std::string name){
    std::size_t pos = name.find("/");
    if(pos != std::string::npos){
        name.erase(pos, name.size());
    }
    return name;
}

void mail365::createJsonApiFile(std::string file_name){
    std::string path = "mail365";
    if(!std::filesystem::exists(path)){
        std::filesystem::create_directory(path);
    }
    file_name = path + "/" + file_name;
    if(!std::filesystem::exists(file_name)){
        std::string link = "https://www.mail365.ru/api.php";
        std::string content = getHttpPage(link, file_name);
    }
    
    dom::DomDocument domDocument;
    domDocument.parsDocument(file_name);
 
    std::shared_ptr<dom::Node> body  = domDocument.getNodeByTagName("body");
    if(body != nullptr){
        std::shared_ptr<dom::Node> methods = (*body).getElementById("methods");
        if(methods != nullptr){
            std::map<std::string, std::vector<ReturnsParams>> conteiner;
            for(std::shared_ptr<dom::Node> li : methods->getElementsByTagName("li")){
                ReturnsParams params = parsStepByLi(li);
                std::string block_name = getMainBlockName(params.block_name);
                if(
                    auto search = conteiner.find(block_name);
                    search != conteiner.end()
                ){
                    conteiner[block_name].push_back(params);
                }else{
                    conteiner[block_name] = std::vector<ReturnsParams>{params};
                }
            }
            for(auto var : conteiner){
                std::string content("[\n");
                for(auto params : var.second){
                    content.append(params.content);
                    content.append(",\n");
                }
                content.erase(content.size() - 2, content.size());
                content.append(getSpace(1) + "\n]");

                std::cout<< path + "/" + var.first + ".json" << std::endl;
                
                std::ofstream out(path + "/" + var.first + ".json", std::ios::trunc);
                if(out.is_open()){
                    out << content;
                }
                out.close();
            }
        }
    }
}

mail365::ReturnsParams mail365::parsStepByLi(std::shared_ptr<dom::Node> li){
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

        std::shared_ptr<dom::Node> blockquote = li->getElementByTagName("blockquote");
        if(blockquote != nullptr){
            std::list<std::shared_ptr<dom::Node>> ps = blockquote
                ->getElementsByTagName("p");
            
            if(!ps.empty()){
                std::list<std::shared_ptr<dom::Node>> tab = 
                    blockquote->getElementsByTagName("tbody");
                if(!tab.empty()){
                    descr_func = removeSymbolse(ps.front()->getInnerHtml(), {"\n", "\r", "\t", "\v"});
                    
                    int count_iter = 0;
                    auto iter = ps.begin();
                    iter++;
                    if(iter->get()->getInnerHtml() != "Ответ"){
                        for(std::shared_ptr<dom::Node> tr : tab.front()
                            ->getElementsByTagName("tr")
                        ){
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
                    for(std::shared_ptr<dom::Node> tr : tab.back()
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

Params mail365::insertDataInParams(std::shared_ptr<dom::Node> tr){
    Params params;
    if(tr != nullptr){
        params.is_optional = false;
        int iter = 0;
        for(std::shared_ptr<dom::Node> td : 
            tr->getElementsByTagName("td")
        ){
            if(td != nullptr){
                if(iter == 0){
                    params.name_ = td->getInnerHtml();
                }else if(iter == 1){
                    std::string descr = removeSymbolse(td->getInnerHtml(), {"\n", "\r", "\t", "\v"});

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
            iter++;
        }
    }
    
    return params;
}

std::string mail365::getSpace(short count){
    std::string space = "    ";
    std::string ret_value;
    for (short i = 0; i < count; i++){
        ret_value.append(space);
    }
    return ret_value;
}

std::string mail365::getInfo(
    std::string methode, 
    std::string url, 
    std::string descr_func,
    std::vector<Params> request_params,
    std::vector<Params> response_params
){
    std::string ret_value = getSpace(1) + "{\n";
    ret_value.append(
        getSpace(2) + "\"description\": \"" + descr_func + "\",\n"
            + getSpace(2) + "\"methode\": \"" 
            + boost::algorithm::to_lower_copy(methode)
            + "\",\n"
        + getSpace(2) + "\"url\": \"" + url + "\",\n"
    );
    ret_value.append(getSpace(2) + "\"patern\": {\n");
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
            getSpace(3) + "\"" + var.name_ + "\": {\n" 
                + getSpace(4) + "\"optional\": " +  optional + ",\n"
                + getSpace(4) + "\"description\": " + "\"" + var.description_ + "\""
                + "\n" + getSpace(3) + "}" + end + "\n"
        );
        iter++;
    }
    ret_value.append(getSpace(2) + "}");

    if(!response_params.empty()){
        ret_value.append(",\n" + getSpace(2) + "\"return_value\":{\n");
        iter = 0;
        for(Params var : response_params){
            std::string end = "";
            if(response_params.size() -1 > iter){
                end = ",";
            }
            if(var.name_.empty()){
                ret_value.append(
                    getSpace(3) + "\"description\": " 
                        + "\"" + var.description_ + "\""
                        + end + "\n"
                );
            }else{
                ret_value.append(
                    getSpace(3) + "\"" + var.name_ + "\": {\n" 
                        + getSpace(4) + "\"description\": " 
                        + "\"" + var.description_ + "\""
                        + "\n" + getSpace(3) + "}" + end + "\n"
                );
            }
            iter++;
        }
        ret_value.append(getSpace(2) + "}\n" + getSpace(1)+ "}");
    }
    return ret_value;
}
