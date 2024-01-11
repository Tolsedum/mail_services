#include "parser/ParserStr.hpp"

/* ---------------------------- PRIVATE  ------------------------------ */

parser::ContentElement parser::ParserStr::getFirstTag(){
    ContentElement element;
    std::string tag_name = getTagNextName();
    if(tag_name.empty()){
        element.setType(ContentType::is_empty);
    }else{
        std::size_t end_tag_size = 0;
        size_t pos_begin = content_.find("<" + tag_name);
        size_t pos_end;
        if(pos_begin != std::string::npos){
            element.setType(ContentType::is_tag);
            element.setTagName(tag_name);
            content_.erase(0, pos_begin + std::string("<" + tag_name).size());
            if(content_[0] != '>'){
                element.setAttributs(getAttributsFromContent());
            }else{
                content_.erase(0, 1);
            }
            
            if(Info::isClosingTag(tag_name)){
                std::vector<std::string> data = Info::getListClosingTagsByTagName(tag_name);
                if(data.empty()){
                    std::string closing_tag = "</" + tag_name + ">";
                    end_tag_size = closing_tag.size();
                    pos_end = content_.find(closing_tag);
                    if(pos_end == std::string::npos){
                        pos_end = content_.size();
                    }
                }else{
                    for(std::string &var : data){
                        pos_end = content_.find(var);
                        if(pos_end != std::string::npos){
                            end_tag_size++;
                            break;
                        }
                    }
                    if(end_tag_size == 0){
                        pos_end = content_.size();
                    }
                }
                if(pos_end != std::string::npos){
                    size_t size = pos_end - pos_begin;
                    element.setContent(content_.substr(
                        pos_begin, 
                        size
                    ));
                    content_.erase(0, size + end_tag_size);
                }
            }
        }
    }
    
    return element;
}

std::string parser::ParserStr::getAttributsFromContent(){
    std::string attrib;
    int iter = 0;
    
    for (size_t i = 0; i < content_.size(); i++){
        if(content_[i] == '>' || (content_[i] == '/' && content_[i+1] == '>')){
            if(content_[i] == '>'){
                iter++;
            }else
                iter += 2;
            break;
        }else{
            attrib.append(1, content_[i]);
        }
        iter++;
    }
    
    content_.erase(0, iter);
    return attrib;
}

std::string parser::ParserStr::getFirstContent(){
    std::string ret_content = "";
    for(char var : content_){
        if(var == '<'){
            break;
        }else{
            ret_content.append(1, var);
        }
    }
    return ret_content;
}

/**
 * @brief Get the Tag Name By Position In Front object
 * 
 * @param content content
 * @param pos front position
 * @return std::string 
 */
parser::DataElement parser::ParserStr::getTagNameByPositionInFront(
    std::string content, 
    std::size_t pos
){
    parser::DataElement ret_value;
    if(content.size() >= pos){
        for (size_t i = pos; i > 0; i--){
            if(content[i] == ' '){
                ret_value.name_tag = "";
            }else if(content[i] == '<'){
                ret_value.pos_begin = i;
                break;
            }else{
                ret_value.name_tag = content[i] + ret_value.name_tag;
            }
        }
    }
    ret_value.valid = true;
    return ret_value;
}

std::size_t parser::ParserStr::
checkPositionForCutElement(parser::DataElement &tagData, std::string tag_cut = ""){
    std::string close_tag;
    if(tag_cut.empty()){
        close_tag = "</" + tagData.name_tag + ">";
    }else{
        close_tag = tag_cut;
    }
    
    std::size_t pos_end = content_.find(
        close_tag, 
        tagData.pos_begin + 1
    );
    std::size_t pos_open_tag = tagData.pos_begin + 1;
    std::size_t cont_open_tag = 0;

    while (pos_open_tag != std::string::npos){
        if(pos_open_tag < pos_end){
            pos_open_tag = content_.find(
                "<" + tagData.name_tag, 
                pos_open_tag
            );
            if(pos_open_tag == std::string::npos){
                break;
            }
            pos_open_tag++;
            cont_open_tag++;
        }else{
            break;
        }
    }
    if(cont_open_tag > 1){
        for (std::size_t i = 0; i < cont_open_tag; i++){
            pos_end++;
            pos_end = content_.find(
                close_tag, 
                pos_end
            );
        }
    }
    
    tagData.pos_end = pos_end;
    if(pos_end != std::string::npos){
        if("</" + tagData.name_tag + ">" == tag_cut){
            return close_tag.size();
        }else {
            return 0;
        }
    }
    return std::string::npos;
}

void parser::ParserStr::cutElement(parser::DataElement &tagData){
    std::vector<std::string> list = parser::Info::getListNotClosingTags();
    std::size_t size_erase = 0;
    if(std::find(list.begin(), list.end(), tagData.name_tag) == list.end()){
        std::vector<std::string> data = 
            Info::getListClosingTagsByTagName(tagData.name_tag);
        if(!data.empty()){
            for(std::string &var : data){
                size_erase = checkPositionForCutElement(tagData, var);
                if(size_erase != std::string::npos){
                    break;
                }
            }
        }else
            size_erase = checkPositionForCutElement(tagData);
        
    }else{
        tagData.pos_end = content_.find(
            ">", 
            tagData.pos_begin
        );
        size_erase = 1;
    }
    if(
        tagData.pos_end != std::string::npos 
        && tagData.pos_begin != std::string::npos
    ){
        tagData.pos_erase = tagData.pos_end - tagData.pos_begin + size_erase;
    
        content_.erase(0, tagData.pos_begin);
        content_.erase(tagData.pos_erase);
        tagData.tag_content = content_;
    }else{
        tagData.valid = false;
    }
        
}

/* ---------------------------- PRIVATE END ------------------------------ */

/* ---------------------------- PUBLIC  ------------------------------ */

std::string parser::ParserStr::getContent(){
    return content_;
};

std::string parser::ParserStr::getTagNextName(){
    std::string tag_name;
    if(content_[0] == '<'){
        for(char var : content_){
            if(var == '>' || var == ' '){
                break;
            }else if(var != '<'){
                tag_name.append(1, var);
            }
        }
    }
    return tag_name;
}

parser::ContentElement parser::ParserStr::getFirstElement(){
    ContentElement element;
    std::string content_element = getFirstContent();
    if(content_element.empty()){
        element = getFirstTag();
    }else{
        element.setContent(content_element);
        element.setType(ContentType::is_content);
        content_.erase(0, content_element.size());
    }
    return element;
}

std::map<std::string, std::string> 
parser::ParserStr::getAttributs(
    std::string content
){
    std::map<std::string, std::string> attributs;
    
    std::string name, value;
    int quote_count = 0;
    char quote = '"';
    bool set_name = true;

    for (size_t i = 0; i < content.size(); i++){
        if(content[i] == '='){
            i++;
            if(content[i] != quote){
                if(
                    (content[i] == '"' || content[i] == '\'')
                    && content[i] != quote
                ){
                    quote = content[i];
                    set_name = false;
                }else{
                    quote = ' ';
                    quote_count++;
                    set_name = false;
                }
            }
        }
        
        if(quote == content[i]){
            quote_count++;
            if(quote_count == 2){
                quote_count = 0;
                set_name = true;
                if(!name.empty()){
                    attributs[name] = value;
                }
                name.clear();
                value.clear();
            }else
                set_name = false;
        }else if(quote_count == 0){
            if(content[i] == ' '){
                set_name = false;
            }
            if(set_name){
                name.append(1, content[i]);
            }else{
                set_name = true;
            }
        }else{
            value.append(1, content[i]);
        }
    }
    if(!name.empty()){
        attributs[name] = value;
    }
    return attributs;
}

parser::DataElement parser::ParserStr::getTagByName(
    std::string tag_name
){
    DataElement tagData;
    std::size_t pos = content_.find("<" + tag_name);
    if(pos == std::string::npos){
        tagData.valid = false;
    }else{
        tagData.pos_begin = pos;
        tagData.name_tag = tag_name;
        tagData.valid = true;
        cutElement(tagData);
    }
    return tagData;
}

parser::DataElement parser::ParserStr::getTagById(
    std::string tag_id
){
    std::size_t pos = content_.find("id=\"" + tag_id + '"');
    DataElement tagData;
    if(pos == std::string::npos){
        tagData.valid = false;
    }else{
        tagData = getTagNameByPositionInFront(
            content_, 
            pos
        );
        cutElement(tagData);
    }
    return tagData;
}

/* ---------------------------- PUBLIC END  ------------------------------ */
