#include "mail365.hpp"

int main(){
    double start_time =  clock();
    
    mail365::createJsonApiFile("api_mail365.html");
    
    double end_time = clock(); // конечное время
    double search_time = end_time - start_time; 
    std::cout<< "search_time " << search_time/CLOCKS_PER_SEC << " сек." <<std::endl;
    return 0; 
}