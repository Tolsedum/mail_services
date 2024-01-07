#ifndef PARSER_INFO_HPP
#define PARSER_INFO_HPP

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

namespace parser{
    class Info{
    public:
        /**
         * @brief Get the list not closing tags object
         * @return std::vector<std::string> 
         */
        static std::vector<std::string> 
        getListNotClosingTags();
        
        /**
         * @brief Get the tag closing instructions object
         * @return std::map<std::string, std::vector<std::string>> 
         */
        static std::map<std::string, std::vector<std::string>>
        getTagClosingInstructions();

        /**
         * @brief Get the list closing tags by tag name object
         * @param tag_name 
         * @return std::vector<std::string> 
         */
        static std::vector<std::string>
            getListClosingTagsByTagName(std::string tag_name);
        
        static bool isClosingTag(std::string tag_name);
    };
}

#endif /* PARSER_INFO_HPP */