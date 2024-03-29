#ifndef DOM_DOM_DOCUMENT_HPP
#define DOM_DOM_DOCUMENT_HPP

#include <fstream>
#include <list>
#include <memory>
#include "parser/info.hpp"
#include "dom/Node.hpp"

namespace dom{
    class DomDocument{
        std::string type_document_;
        std::list<std::shared_ptr<Node>> dom_list_;

    public:
        DomDocument(){};
        ~DomDocument(){};
            
        /**
         * @brief Pars file by name name_file and create dom_list_
         * 
         * @param name_file 
         */
        void parsDocument(std::string name_file);
        void setContentElement(parser::ContentElement element);

        /**
         * @brief Get the node by tag name from dom_list_
         * 
         * @param tag_name 
         * @return Node* 
         */
        std::shared_ptr<dom::Node> getNodeByTagName(std::string tag_name);

        /**
         * @brief Convert node list to string html document
         * 
         * @return std::string 
         */
        std::string getContent();
    }; 
}

#endif /* DOM_DOM_DOCUMENT_HPP */
