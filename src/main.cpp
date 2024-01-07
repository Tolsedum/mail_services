#include <iostream>
#include <filesystem>
#include "functions.hpp"
#include "dom/DomDocument.hpp"



int main(int argc, char *argv[]){
    double start_time =  clock();
  
    std::string name_file;
    if(argc > 1){
        for (int i = 0; i < argc; i++){
            std::string line = argv[i];
            
            size_t pos = line.find(" ");
            if(pos != std::string::npos){
                name_file = line.erase(0, pos+1);
            }

            if(name_file.empty() && line == "-f"){
                name_file = argv[++i];
            }
        }
    }
     
    if(name_file.empty()){
        name_file = "api.html";
    }  
    
    name_file = "api.html";
    if(!std::filesystem::exists(name_file)){
        std::string link = "https://www.mail365.ru/api.php";
        std::string content = getHttpPage(link, "api.html");
    }
    std::string output_data;
    dom::DomDocument domDocument;
    domDocument.parsDocument(name_file);
 
    dom::Node *body  = domDocument.getNodeByTagName("body");
    if(body != nullptr){
        dom::Node* methods = (*body).getElementById("methods");
        if(methods != nullptr){
            for(dom::Node* li : methods->getElementsByTagName("li")){
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
                                            request_params.push_back(insertDataInParams(tr));
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
                                        response_params.push_back(insertDataInParams(tr));
                                    }else{
                                        count_iter++;
                                    }
                                }
                            }
                        }
                        
                    }
                    output_data.append(getInfo(
                        methode, 
                        url, 
                        descr_func,
                        request_params,
                        response_params
                    ));
                    break;
                }
            } 
        }
    }

    std::ofstream out("out_file.html", std::ios::trunc);
    if(out.is_open()){
        out << output_data;
    }

    double end_time = clock(); // конечное время
    double search_time = end_time - start_time; 
    std::cout<< "search_time " << search_time/CLOCKS_PER_SEC << " сек." <<std::endl;
    std::cout<< "name_file "<< name_file << " EXIT" <<std::endl;
    return 0; 
}