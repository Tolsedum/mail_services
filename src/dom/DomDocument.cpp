#include "dom/DomDocument.hpp"

void dom::DomDocument::parsDocument(std::string name_file){
    std::ifstream ifs(name_file);
    if(ifs.is_open()){
        std::string content = "";
        while (!ifs.eof()){
            std::string buffer;
            std::getline(ifs, buffer);
            content.append(buffer);
        }
        parser::ParserStr parse(content);
        while (!parse.getContent().empty()){
            parser::ContentElement element = parse.getFirstElement();
            if(element.getType() != parser::ContentType::is_empty){
                if(element.getTagName() != "!DOCTYPE"){
                    if(element.getTagName() == "html"){
                        type_document_ = "html";
                        parser::ParserStr parse_(element.getContent());
                        while (!parse_.getContent().empty()){
                            parser::ContentElement element_ = 
                                parse_.getFirstElement();
                            if(element_.getType() != parser::ContentType::is_empty){
                                setContentElement(element_);
                            }else{
                                break;
                            }
                        }
                    }else{
                        setContentElement(element);
                    }
                }
            }else{
                break;
            }
        }
    }
}

std::shared_ptr<dom::Node> 
dom::DomDocument::getNodeByTagName(std::string tag_name){
    for(std::shared_ptr<Node>& var : dom_list_){
        if(var->getTagName() == tag_name){
            return var;
        }
    }
    return nullptr;
}

void dom::DomDocument::setContentElement(parser::ContentElement element){
    static int num_element = 0;
    std::shared_ptr<Node> dom = std::make_shared<Node>(
        element.getTagName(), 
        element.getContent(), 
        element.getAttributs()
    );
    
    if(!element.getTagName().empty()){
        dom->setCurrentNum(dom_list_.size());
        dom_list_.push_back(std::move(dom));
    }
    num_element++;
}

std::string dom::DomDocument::getContent(){
    std::string content("<!DOCTYPE " + type_document_ + ">");
    content.append("<" + type_document_ + ">");
    for(std::shared_ptr<Node>& var : dom_list_){
        if(var->isNotDeleted()){
            content.append(var->toString());
        }
    }
    content.append("</" + type_document_ + ">");
    return content;
}