#include "./filtered_string_view.h"



// Implement here
//fsv::filter fsv::filtered_string_view::default_predicate = [](const char&){return true;};

fsv::filter fsv::filtered_string_view::default_predicate = [](const char&){return true;};

size_t fsv::filtered_string_view::stringLen =0;

auto fsv::filtered_string_view::predicate() const -> const fsv::filter& {

    //return static_cast<const fsv::filter&>(fsv::filtered_string_view::default_predicate);
    // std::cout<<"return predicate() func"<<std::endl;
    return fsv::filtered_string_view::default_predicate;
}


fsv::filtered_string_view::filtered_string_view(){
    
    this->rawPointer = nullptr;

    //this->size();

    this->stringLen = 0;
    this->default_predicate = [](const char&){return true;};
}


std::size_t fsv::filtered_string_view::size() const{

    const char * p = this->rawPointer;
    this->stringLen = 0;
    
    while(p != nullptr && *p!='\0'){
               
        if(this->default_predicate(*p)){
        ++this->stringLen;            
        }
        p++; 
    }
    
    return this->stringLen;
}

fsv::filtered_string_view::filtered_string_view(const std::string &str){

    this->rawPointer = str.c_str();
    this->default_predicate = [](const char&){return true;};
    this->stringLen = this->size();
}

fsv::filtered_string_view::filtered_string_view(const std::string &str, filter predicate){

    // std::cout<<"constructor with str && predicate "<<std::endl;
    this->rawPointer = str.c_str();
    this->default_predicate = predicate;
    this->stringLen = this->size();
}


//Implicit Null-Terminated String Constructor
fsv::filtered_string_view::filtered_string_view(const char *str){

    this->rawPointer = str;
    this->default_predicate = [](const char&){return true;};
    this->stringLen = this->size();
}

fsv::filtered_string_view::filtered_string_view(const char *str, filter predicate){
    // std::cout<<"constructor with char* && predicate "<<std::endl;
    this->rawPointer = str;
    this->default_predicate = predicate;
    this->stringLen = this->size();
}

//Copy and Move Constructors
fsv::filtered_string_view::filtered_string_view(const filtered_string_view &other)
    :rawPointer{other.rawPointer} {
    
    //this->rawPointer = new char(*other.rawPointer);
    // this->rawPointer = other.rawPointer;
    this->default_predicate = other.default_predicate;
    this->stringLen = other.stringLen;
}


fsv::filtered_string_view::filtered_string_view(filtered_string_view &&other) noexcept
    : rawPointer{std::exchange(other.rawPointer, nullptr)}
      {

    //should not move to same object itself
    if(this != &other){
        
        // this->rawPointer = other.rawPointer;

        this->stringLen = other.stringLen;
        this->default_predicate = other.default_predicate;

        // std::swap(this->rawPointer, other.rawPointer);
        // std::swap(this->stringLen, other.stringLen);
        // std::swap(this->default_predicate, other.default_predicate);

        // other.rawPointer = nullptr;
        // other.stringLen = 0;
        other.default_predicate = [](const char&){return true;};


    }else{
        std::cout<<"Could not move object itself"<<std::endl;
    }



}


const char* fsv::filtered_string_view::data() const{

    return this->rawPointer;

}

fsv::filtered_string_view &fsv::filtered_string_view::operator=(const filtered_string_view &other){

    //clean up destination
    this->default_predicate = [](const char&){return true;};
    this->rawPointer = nullptr;
    this->stringLen = 0;
    
    this->default_predicate = other.default_predicate;
    this->rawPointer = other.rawPointer;
    this->stringLen = other.stringLen;

    return *this;
}

fsv::filtered_string_view &fsv::filtered_string_view::operator=(filtered_string_view &&other)noexcept{
    if(this!= &other){
        std::swap(this->rawPointer, other.rawPointer);
        std::swap(this->stringLen, other.stringLen);
        std::swap(this->default_predicate, other.default_predicate);

        other.rawPointer = nullptr;
        other.stringLen = 0;
        other.default_predicate = [](const char&){return true;};

        
    }

    return *this;
}

//read character from filtered string
auto fsv::filtered_string_view::operator[](int n) -> const char &{

    const char * p = this->rawPointer;
    // std::cout<<this->stringLen<<std::endl;

    int i = -1;
    
    while(p != nullptr && *p != '\0'){
        if(this->default_predicate(*p)) i++;
        
        if( i == n) break;
        p++;

    }

    return *p;

}

fsv::filtered_string_view::operator std::string(){
    std::string str;
    std::size_t len = this->stringLen+1;
    const char * p = this->rawPointer;    
    while(len != 0){

        if(this->default_predicate(*p)){
            str+=*p;        
        }
        p++;
        --len;
    }

    return str;

}


const char & fsv::filtered_string_view::at(int index) const{

        const char * p = this->rawPointer;
        int i = -1;
        
        std::string str = std::to_string(index);

        
        if(this->size() == 0 || static_cast<std::size_t>(index) > this->size() -1)
        throw std::domain_error{"filtered_string_view::at("+str+"): invalid index"};

        while(p != nullptr && *p != '\0'){
            if(this->default_predicate(*p)) i++;
            
            if( i == index) break;
            p++;

        }

        return *p;


}


auto fsv::filtered_string_view::empty() -> bool{

    if(this->size() == 0) return true;

    return false;
}



// auto operator==(const fsv::filtered_string_view &lhs, const fsv::filtered_string_view &rhs) -> bool
// {
//     if(lhs.size() != rhs.size()) return false;
//     for(int i = 0; static_cast<std::size_t>(i)< lhs.size(); i++){
//         if(lhs.at(i) != rhs.at(i)) return false;
//     }

//     return true;
// }


// auto operator!=(const fsv::filtered_string_view &lhs, const fsv::filtered_string_view &rhs) -> bool{
//     return !(lhs==rhs);
// }


// auto operator<<(std::ostream &os, const fsv::filtered_string_view &fsv) -> std::ostream&{

// }

//comparison
// auto compose(const fsv::filtered_string_view &fsv, const std::vector<fsv::filter> &filts) -> fsv::filtered_string_view{

//     //fsv::filtered_string_view fsv1 = fsv;
//     //fsv.default_predicate = [](const char&){return true;};

//     for(std::vector<fsv::filter>::const_iterator it = filts.begin(); it != filts.end(); ++it){
        
//         auto predicate = *it;
//         for(int i = 0; static_cast<std::size_t>(i) < std::strlen(fsv.data()); i++){
//             if(predicate(fsv.at(i))) continue;
//         }
        
//     }

//     fsv.default_predicate = [](const char&){return true;};

//     return fsv;
// }
