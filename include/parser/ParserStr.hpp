#ifndef PARSER_PARSER_STR_HPP
#define PARSER_PARSER_STR_HPP

#include "info.hpp"

namespace parser{
    enum class ContentType { is_tag, is_content, is_empty };
    static const std::vector<std::string> contentTypeDesct{"tag", "content", "empty"};
    
    struct ContentElement{
    private:
        ContentType type_;
        std::string tag_name_;
        std::string attributs_;
        std::string content_;
        
    public:
        ContentElement(){type_ = ContentType::is_empty;};
        ContentElement(std::string content)
            : type_(ContentType::is_empty)
            , tag_name_("")
            , attributs_("")
            , content_(content)
        {};
        ~ContentElement(){};

        friend std::ostream &operator<<(
            std::ostream &os, 
            const ContentElement &element
        ){
            os
                << "content_: " << element.content_ <<std::endl
                << "type: " << contentTypeDesct[(int)element.type_] <<std::endl
                    
            << std::endl;
            return os;
        }

        void setType(ContentType type){type_ = type;};
        void setContent(const std::string &content){content_ = content;};
        void setTagName(const std::string &tag_name){tag_name_ = tag_name;};
        void setAttributs(const std::string &attributs){attributs_ = attributs;};

        ContentType getType(){return type_;};
        std::string getTagName(){return tag_name_;};
        std::string getAttributs(){return attributs_;};
        std::string getContent(){return content_;};
        
        std::string getAttributValue(std::string attr_name);
    };

    struct DataElement{
        std::size_t pos_begin;
        std::size_t pos_end;
        std::size_t pos_erase;
        std::string name_tag;
        std::string tag_content;
        bool valid;
    };

    class ParserStr{
    private:
        /**
         * @brief Parsing content 
         * 
         */
        std::string content_;

        /**
         * @brief If first element is not tag then get the first content from content_
         * @return std::string 
         */
        std::string getFirstContent();

        /**
         * @brief If first element in content_ is tag then get the first tag object
         * @return std::string 
         */
        parser::ContentElement getFirstTag();

        /**
         * @brief Use if next elements in content_ id attributs
         * @return std::string 
         */
        std::string getAttributsFromContent();

        /**
         * @brief Get name next tag element
         * @return std::string 
         */
        std::string getTagNextName(std::size_t pos = 0);

        /**
         * @brief if the cursor position is further than the tag name, 
         * find the tag name
         * 
         * @param content 
         * @param pos 
         * @return DataElement 
         */
        DataElement getTagNameByPositionInFront(
            std::string content, 
            std::size_t pos
        );
        /**
         * @brief Identify the position of the tag to be extracted
         * 
         * @param tagData data tag for extracted
         * @param tag_cut tag name
         * @return std::size_t size closing tag
         */
        std::size_t checkPositionForCutElement(
            DataElement &tagData, 
            std::string tag_cut
        );

        /**
         * @brief 
         * Cut element for data
         * @param tagData data tag for extracted
         */
        void cutElement(DataElement &tagData);
    public:
        ParserStr(std::string content):content_(content){};
        ~ParserStr(){};

        /**
         * @brief Get the content_ object
         * @return std::string 
         */
        std::string getContent();
        
        /**
         * @brief Get the first element from content_
         * @return ContentElement 
         */
        ContentElement getFirstElement();

        /**
         * @brief Extract attributs in array object from content
         * @param content 
         * @return std::map<std::string, std::string> 
         */
        static std::map<std::string, std::string> 
        getAttributs(std::string content);

        /**
         * @brief Extrack tag by name from content_
         * 
         * @param tag_name 
         * @return DataElement 
         */
        DataElement getTagByName(
            std::string tag_name,
            std::size_t pos = 0
        );

        /**
         * @brief Extrack tag by id from content_
         * 
         * @param tag_id 
         * @return DataElement 
         */
        DataElement getTagByNameAttribut(
            std::string tag_id,
            std::string name_attribut = "id",
            std::size_t pos = 0
        );

        /**
         * @brief Get the next tag by position
         * 
         * @param pos 
         * @return std::DataElement 
         */
        DataElement getNextTagByPosition(std::size_t pos = 0);
    };    
}
#endif /* PARSER_PARSER_STR_HPP */