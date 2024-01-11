#include "dom/Node.hpp"

/* -------------- dom::Element -------------- */
dom::Element::Element(std::string value){
    list_data_.push_back(value);
}

dom::Element::Element(std::vector<std::string> value){
    list_data_ = value;
};

std::vector<std::string> dom::Element::getList(){
    return list_data_;
}

bool dom::Element::empty(){
    return list_data_.empty();
}

bool dom::Element::has(
    const std::string& value
){
    return std::find(
        list_data_.begin(),
        list_data_.end(),
        value
    ) != list_data_.end();
}

std::string 
dom::Element::toString(){
    std::string ret_value;
    if(!list_data_.empty()){
        for(auto &str : list_data_){
            ret_value.append(str + " ");
        }
        ret_value.erase(ret_value.size()-1, 1);
    }
    return ret_value;
}

/* -------------- dom::Element END -------------- */

/* ---------------------------- PRIVATE ------------------------------ */

int dom::Node::getCurrentNum(){
    return current_num_;
};
std::string dom::Node::getTagName(){
    return tag_name_;
};
std::string dom::Node::getInnerHtml(){
    return inner_HTML_;
};
void dom::Node::setTagName(std::string tag_name){
    tag_name_ = tag_name;
};
void dom::Node::setInnerHtml(std::string inner_HTML){
    inner_HTML_ = inner_HTML;
};
void dom::Node::setCurrentNum(int num){
    current_num_ = num;
};
bool dom::Node::isNotDeleted(){
    return !is_deleted_;
};

std::shared_ptr<dom::Node> 
dom::Node::extractTagFromInnerHtml(
    parser::DataElement dataElement, parser::ParserStr pars
){
    std::shared_ptr<Node> ret_node = nullptr;
    if(
        inner_HTML_.size() >= dataElement.pos_erase
        && inner_HTML_.size() >= dataElement.pos_begin
    ){
        parser::ContentElement element = pars.getFirstElement();
        if(element.getType() != parser::ContentType::is_empty){
            ret_node = appendInNodeList(element);
            inner_HTML_.replace(
                dataElement.pos_begin,
                dataElement.pos_erase,
                "%element_" + std::to_string(ret_node->getCurrentNum()) + "%"
            );
            
        }
    }
    
    return ret_node;
}

std::string dom::Node::attrToString(){
    std::string attributs;
    if(!list_elements_.empty()){
        for(std::pair<std::string, Element> var : list_elements_){
            attributs.append(var.first);
            if(!var.second.empty()){
                attributs.append(1, '=');
                attributs.append(1, '"');
                attributs.append(var.second.toString());
                attributs.append(1, '"');
            }
            attributs.append(1, ' ');
        }
        if(!attributs.empty())
        attributs.erase(attributs.size()-1, 1);
    }
    return attributs;
}

std::shared_ptr<dom::Node>  
dom::Node::appendInNodeList(
    parser::ContentElement element
){
    std::shared_ptr<Node> node = std::make_shared<Node>(
        element.getTagName(), 
        element.getContent(), 
        element.getAttributs()
    );
    // Node* ret_node;
    // int position = node_list_.size();
    node->setCurrentNum(node_list_.size());
    node_list_.push_back(std::move(node));
    // ret_node = &node_list_.back();
    // ret_node->setCurrentNum(position);
    return node_list_.back();
}
/* ---------------------------- PRIVATE END ------------------------------ */
/* ---------------------------- PUBLIC ------------------------------ */
dom::Node::Node(
    std::string tag_name,
    std::string inner_HTML,
    std::string attributs
)
    : is_deleted_(false)
    , tag_name_(tag_name)
    , inner_HTML_(inner_HTML)
{
    static int num_element_ = 0;
    for(
        std::pair<std::string, std::string> var : 
        parser::ParserStr::getAttributs(attributs)
    ){
        list_elements_[var.first] = Element(var.second);
    }
    current_num_ = num_element_;
    num_element_++;
}

bool dom::Node::hasElementByName(std::string name_element){
    for(std::pair<const std::string, dom::Element> &element : list_elements_){
        if(element.first == "id" && element.second.has(name_element)){
            return true;
        }
    }
    return false;
}

std::shared_ptr<dom::Node> 
dom::Node::getElementById(std::string id){
    if(hasElementByName(id)){
        return shared_from_this();
    }

    std::shared_ptr<Node> node = nullptr;
    if(!node_list_.empty()){
        for(std::shared_ptr<Node>& var : node_list_){
            node = var->getElementById(id);
            if(node != nullptr){
                return node;
            }
        }
    }
    parser::ParserStr pars(inner_HTML_);
    parser::DataElement dataElement = pars.getTagById(id);
    if(dataElement.valid){
        node = extractTagFromInnerHtml(dataElement, pars);
    }
    return node;
}

void dom::Node::extractAllTagsByName(std::string tag_name){
    if(!inner_HTML_.empty()){
        while (true){
            parser::ParserStr pars(inner_HTML_);
            parser::DataElement data = pars.getTagByName(tag_name);    
            if(data.valid){
                extractTagFromInnerHtml(data, pars);
            }else{
                break;
            }
        }
    }
}

std::shared_ptr<dom::Node>
dom::Node::getElementByTagName(std::string tag_name){
    std::shared_ptr<dom::Node> ret_node = nullptr;
    extractAllTagsByName(tag_name);
    if(!node_list_.empty()){
        for(std::shared_ptr<Node>& node : node_list_){
            if(node->isNotDeleted()){
                if(node->getTagName() == tag_name){
                    ret_node = node;
                }else{
                    std::shared_ptr<Node> node_ = 
                        node->getElementByTagName(tag_name);
                    if(node_ != nullptr){
                        ret_node = node_;
                        break;
                    }
                }
            }
            if(ret_node != nullptr)
                break;
        }
    }
    return ret_node;
}

std::list<std::shared_ptr<dom::Node>> 
dom::Node::getElementsByTagName(std::string tag_name){
    std::list<std::shared_ptr<Node>> list;
    extractAllTagsByName(tag_name);
    if(!node_list_.empty()){
        for(std::shared_ptr<Node> &node : node_list_){
            std::list<std::shared_ptr<Node>> list_ = 
                node->getElementsByTagName(tag_name);
            if(node->getTagName() == tag_name){
                list.push_back(node);
            }
            for(std::shared_ptr<Node> var : list_){
                if(var != nullptr){
                    list.push_back(var);
                }
            }
        }
    }
    return list;
}


std::string dom::Node::toString(){
    std::string content, attributs;
    if(isNotDeleted()){
        attributs = attrToString();
        std::string inner_HTML = inner_HTML_;
        if(!node_list_.empty()){
            for(std::shared_ptr<Node>& node : node_list_){
                std::string lable = "%element_" 
                    + std::to_string(node->getCurrentNum()) 
                    + "%";
                std::size_t pos = inner_HTML.find(lable);
                if(pos != std::string::npos){
                    inner_HTML.replace(pos, lable.size(), node->toString());
                }
            }
        }
        content.append(inner_HTML);
        if(!tag_name_.empty()){
            std::vector<std::string> list = parser::Info::getListNotClosingTags();
            if(std::find(list.begin(), list.end(), tag_name_) == list.end()){
                std::string spase;
                if(!attributs.empty()){
                    spase = " ";
                }
                content = "<" + tag_name_ + spase
                    + attributs + ">" 
                    + content 
                    + "</" + tag_name_ + ">";
            }else{
                content = "<" + tag_name_ + " " + attributs + "/>";
            }
        }
    }
    return content;
}

std::string dom::Node::getAttributByName(std::string name_attr){
    std::string value;
    if(
        auto search = list_elements_.find(name_attr);
        search != list_elements_.end()
    ){
        value = search->second.toString();
    }
    return value;
}

std::list<std::string> 
dom::Node::getListAttributsName(){
    std::list<std::string> list;
    for(std::pair<std::string, Element> p : list_elements_){
        list.push_back(p.first);
    }
    return list;
}

void dom::Node::remove(){
    is_deleted_ = true;
    if(!node_list_.empty()){
        for(auto &var : node_list_){
            var->remove();
        }
    }
}

/* ---------------------------- PUBLIC END  ------------------------------ */