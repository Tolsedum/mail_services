#include "parser/info.hpp"

std::vector<std::string> 
parser::Info::getListNotClosingTags(){
    return {
        "!doctype", "area", "base",
        "br", "col", "embed",
        "hr", "img", "input",
        "keygen", "link", "meta",
        "param", "source", "track",
        "wbr"
    };
}

std::map<
std::string, std::vector<std::string>>
parser::Info::getTagClosingInstructions(){
    return {
        {"li", {"</li>", "<li", "</ul>", "</ol>"}},
        {"dd", {"</dd>", "<dt, <dd"}},
        {"p", {"</p>", "<p"}},
        {"rt", {"</rt>", "<rt", "<rp", "</ruby>"}},
        {"rp", {"</rp>", "<rt", "<rp", "</ruby>"}},
        {"optgroup", {"</optgroup>", "<optgroup", "</select>"}},
        {"option", {
            "</option>", "<option", "<optgroup", 
            "</optgroup>", "</select>"
        }},
        {"thead", {"</thead>", "<tbody", "<tfoot"}},
        {"tbody", {"</tbody>", "<tbody", "<tfoot", "</table>"}},
        {"tfoot", {"</tfoot>", "</table>"}},
        {"tr", {"</tr>", "<tr", "<th", "</table>"}},
        {"th", {"</th>", "<th", "<tr", "</thead>", "</table>"}},
        {"td", {"</td>", "<td", "<tr", "<th", "</table>"}}
    };
}

std::vector<std::string> parser::Info::getListClosingTagsByTagName(
    std::string tag_name
){
    std::vector<std::string> data;
    std::map<std::string, std::vector<std::string>>
    list_closing_tags = Info::getTagClosingInstructions();
    if(
        auto search = list_closing_tags.find(tag_name);
        search != list_closing_tags.end()
    ){
        data = search->second;
    }
    return data;
}

std::string str_tolower(std::string s){
    std::transform(s.begin(), s.end(), s.begin(), 
        [](unsigned char c){ return std::tolower(c); }
    );
    return s;
}

bool parser::Info::isClosingTag(std::string tag_name){
    std::vector<std::string> list = getListNotClosingTags();
    return std::find(list.begin(), list.end(), str_tolower(tag_name)) == list.end();
    
}