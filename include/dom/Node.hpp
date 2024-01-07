#ifndef DOM_ELEMENT_HPP
#define DOM_ELEMENT_HPP

#include <map>
#include <vector>
#include <list>
#include <stack>
#include <string>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include "parser/ParserStr.hpp"

namespace dom{

    /**
     * @brief Object for worck with a list of data
     */
    struct Element{
    private:
        /**
         * @brief List params in element (attributs)
         */
        std::vector<std::string> list_data_;

    public:
        Element(){};
        Element(std::string value);
        Element(std::vector<std::string> value);
        ~Element(){};
        /**
         * @brief Has value in list_data
         * 
         * @return true if has
         * @return false if not has 
         */
        bool has(const std::string& value);

        /**
         * @brief Get list data for string line
         * 
         * @return std::string 
         */
        std::string toString();

        /**
         * @brief list_data is empty
         * 
         * @return true if empty
         * @return false if not empty
         */
        bool empty();

        std::vector<std::string> getList();

        friend std::ostream & operator<<(
            std::ostream &os, const Element &e
        ){
            for(auto var : e.list_data_){
                os << var << "; ";
            }
            os << std::endl;
            return os;
        }
    };

    /**
     * @brief Node of document
     * @param tag_name tag name node
     * @param list_elements all elements node. (attributs, class ...)
     * @param node_list all nodes locate inside with this node
     */
    class Node{
    protected:
        bool is_deleted_;
        
        int current_num_;
        /**
         * @brief tag name node
         */
        std::string tag_name_;
        /**
         * @brief all elements node. (attributs, class ...)
         */
        std::map<std::string, Element> list_elements_;
        /**
         * @brief node content (innerHTML)
         */
        std::string inner_HTML_;
        /**
         * @brief all nods locate inside with this node
         */
        std::list<Node> node_list_;

        /**
         * @brief Convert attributs to string
         * 
         * @return std::string 
         */
        std::string attrToString();

        /**
         * @brief append Node in node_list_
         * 
         * @param node 
         * @return Node* inserted in node_list_
         */
        Node* appendInNodeList(Node node);

        /**
         * @brief Extract tag from inner_HTML_
         * 
         * @param dataElement 
         * @param pars 
         * @return Node* 
         */
        Node* extractTagFromInnerHtml(
            parser::DataElement dataElement,
            parser::ParserStr pars
        );

        /**
         * @brief Extract all tags by name from inner_HTML_
         * 
         * @param tag_name 
         */
        void extractAllTagsByName(std::string tag_name);
        
    public:
        Node(){};
        Node(
            std::string tag_name,
            std::string inner_HTML,
            std::string attributs
        );
        virtual ~Node(){};

        int getCurrentNum();
        std::string getTagName();
        std::string getInnerHtml();
        
        void setTagName(std::string tag_name);
        void setInnerHtml(std::string inner_HTML);
        void setCurrentNum(int num);

        bool isNotDeleted();
        bool hasElementByName(std::string name_element);

        /**
         * @brief Remove element from content
         */
        void remove();

        /**
         * @brief Convert nodes content in string
         * 
         * @return std::string 
         */
        std::string toString();

        /**
         * @brief Get the attribut by name from innerHTML_ or node_list_
         * 
         * @param name_attr 
         * @return std::string 
         */
        std::string getAttributByName(std::string name_attr);

        /**
         * @brief Get the element by id  from innerHTML_ or node_list_
         * 
         * @param id 
         * @return Node* 
         */
        Node* getElementById(std::string id);

        /**
         * @brief Get the element by tag name from innerHTML_ or node_list_
         * 
         * @param tag_name 
         * @return Node* 
         */
        Node* getElementByTagName(std::string tag_name);

        /**
         * @brief Get the elements by tag name from innerHTML_ or node_list_
         * 
         * @param tag_name 
         * @return std::vector<Node*> 
         */
        std::vector<Node*> getElementsByTagName(std::string tag_name);

        /**
         * @brief Get the list attributs name from innerHTML_ or node_list_
         * 
         * @return std::vector<std::string> 
         */
        std::vector<std::string> getListAttributsName();
    };
};


#endif /* DOM_ELEMENT_HPP */
