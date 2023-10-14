#include "./filtered_string_view.h"



// Implement here
//fsv::filter fsv::filtered_string_view::default_predicate = [](const char&){return true;};

//fsv::filter fsv::filtered_string_view::default_predicate = [](const char&){return true;};

//size_t fsv::filtered_string_view::stringLen =0;

auto fsv::filtered_string_view::predicate() const -> const fsv::filter& {

    //return static_cast<const fsv::filter&>(fsv::filtered_string_view::default_predicate);
    // std::cout<<"return predicate() func"<<std::endl;
    return fsv::filtered_string_view::default_predicate;
}

fsv::filtered_string_view::filtered_string_view():rawPointer(nullptr),stringLen(0),default_predicate([](const char&){return true;}){

    // std::cout<<"default constructor"<<std::endl;
    
    // this->rawPointer = nullptr;

    // //this->size();

    // this->stringLen = 0;
    // this->default_predicate = [](const char&){return true;};

            begin_ = this->rawPointer;
            end_ = this->rawPointer+stringLen;
            //end_ = &this->at(0+static_cast<int>(stringLen));
}


std::size_t fsv::filtered_string_view::size() const{

    // std::cout<<"size()"<<std::endl;


    const char * p = this->rawPointer;
    std::size_t len = 0;
    
    while(p != nullptr && *p!='\0'){
               
        if(this->default_predicate(*p)){
        ++len;            
        }
        p++; 
    }
    
    return len;
}

fsv::filtered_string_view::filtered_string_view(const std::string &str){

    // std::cout<<"1 para str constructor"<<std::endl;


    this->rawPointer = str.c_str();
    this->default_predicate = [](const char&){return true;};
    this->stringLen = str.size();
    begin_ = this->rawPointer;
    end_ = this->rawPointer+stringLen;
}

fsv::filtered_string_view::filtered_string_view(const std::string &str, filter predicate){
    

    // std::cout<<"constructor with str && predicate "<<std::endl;
    this->rawPointer = str.c_str();
    this->default_predicate = predicate;
    this->stringLen = str.size();
    begin_ = this->rawPointer;
    end_ = this->rawPointer+stringLen;
}


//Implicit Null-Terminated String Constructor
fsv::filtered_string_view::filtered_string_view(const char *str){


    // std::cout<<"1 para char* constructor"<<std::endl;

    this->rawPointer = str;
    this->default_predicate = [](const char&){return true;};
    int i = 0;
    while(str[i] != '\0'){
        this->stringLen++;
        i++;
    }
    begin_ = this->rawPointer;
    end_ = this->rawPointer+stringLen;
        
}

fsv::filtered_string_view::filtered_string_view(const char *str, filter predicate){
    // std::cout<<"constructor with char* && predicate "<<std::endl;
    this->rawPointer = str;
    this->default_predicate = predicate;
    int i = 0;
    while(str[i] != '\0'){
        this->stringLen++;
        i++;
    }
    begin_ = this->rawPointer;
    end_ = this->rawPointer+stringLen;
        
}

//Copy and Move Constructors
fsv::filtered_string_view::filtered_string_view(const filtered_string_view &other)
    :rawPointer{other.rawPointer} {

    // std::cout<<"copy constructor"<<std::endl;

    //this->rawPointer = new char(*other.rawPointer);
    // this->rawPointer = other.rawPointer;
    this->default_predicate = other.default_predicate;
    this->stringLen = other.stringLen;
    begin_ = this->rawPointer;
    end_ = this->rawPointer+stringLen;
}


fsv::filtered_string_view::filtered_string_view(filtered_string_view &&other) noexcept
    : rawPointer{std::exchange(other.rawPointer, nullptr)}
      {
    
    // std::cout<<"move constructor"<<std::endl;

    //should not move to same object itself
    if(this != &other){
        

        // this->rawPointer = other.rawPointer;

        this->stringLen = other.stringLen;
        this->default_predicate = other.default_predicate;

        // std::swap(this->rawPointer, other.rawPointer);
        // std::swap(this->stringLen, other.stringLen);
        // std::swap(this->default_predicate, other.default_predicate);

        // other.rawPointer = nullptr;
        other.stringLen = 0;
        other.default_predicate = [](const char&){return true;};


    }else{
        // std::cout<<"Could not move object itself"<<std::endl;
    }
    begin_ = this->rawPointer;
    end_ = this->rawPointer+stringLen;  

}


const char* fsv::filtered_string_view::data() const{

    return this->rawPointer;

}

fsv::filtered_string_view &fsv::filtered_string_view::operator=(const filtered_string_view &other){
    
    // std::cout<<"copy assignment"<<std::endl;

    //clean up destination
    //this->default_predicate = [](const char&){return true;};
    //this->rawPointer = nullptr;
    //this->stringLen = 0;
    
    this->default_predicate = other.default_predicate;
    this->rawPointer = other.rawPointer;
    this->stringLen = other.stringLen;

        // std::cout<<"output *this in copy"<<*this<<std::endl;
        // std::cout<<"output other in copy"<<other<<std::endl;
        begin_ = this->rawPointer;
        end_ = this->rawPointer+stringLen;

    return *this;
}

fsv::filtered_string_view &fsv::filtered_string_view::operator=(filtered_string_view &&other)noexcept{

    // std::cout<<"move assignment"<<std::endl;
    if(this!= &other){
        // std::cout<<"in move assign"<<*this<<std::endl;
        std::cout<<other<<std::endl;
        std::swap(this->rawPointer, other.rawPointer);
        std::swap(this->stringLen, other.stringLen);
        std::swap(this->default_predicate, other.default_predicate);
        // this->rawPointer = other.rawPointer;
        // this->stringLen = other.stringLen;
        // this->default_predicate = other.default_predicate;

        // other.rawPointer = nullptr;
        // other.stringLen = 0;
        // other.default_predicate = [](const char&){return true;};
        // std::cout<<"output *this in move"<<*this<<std::endl;
        // std::cout<<"output other in move"<<other<<std::endl;
        begin_ = this->rawPointer;
        end_ = this->rawPointer+stringLen;
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
    std::size_t len = this->size()+1;
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


auto fsv::compose(const fsv::filtered_string_view &fsv, const std::vector<fsv::filter> &filts) -> fsv::filtered_string_view{

       //Accepts a filtered_string_view and a vector of filtering predicates and returns a new filtered_string_view 
       //which filters the same underlying string as fsv but with a predicate which only returns true 
       //if all of the filters in filts (called in order of the vector from left to right) also return true, 
       //that is, all the filters would have filtered the same letter. 
       //If any filter in the chain of function calls returns false, 
       //the new predicate should short-circuit and not call the subsequent functions and return false.

        //new predicate to store all the predicate in vector
        //how to compose all the predicate?

        fsv::filter new_Predicate = [&filts](const char& c){
            for(auto it = filts.begin(); it != filts.end(); ){
                auto tmp_pre = *it;

                if(tmp_pre(c)){
                    it++;
                }else{
                    return false;
                    break;
                }

                // if(count == static_cast<int>(filts.size())){
                //     return true;
                // }
            }


            return true;
        };

        

    return fsv::filtered_string_view{fsv.data(), new_Predicate};
}



auto fsv::split(const fsv::filtered_string_view &fsv, const fsv::filtered_string_view &tok) -> std::vector<fsv::filtered_string_view>{

    // std::cout<<fsv<<std::endl;
    // std::cout<<tok.size()<<std::endl;
    std::vector<fsv::filtered_string_view> vec;
    //this can be done by initialising each slice of the split 
    //with the original underlying data of fsv and a new predicate 
    //which calculates the extent of the substring within the original string
    // auto predicate_fsv = fsv.predicate();

    // auto predicate_tok = tok.predicate();

    fsv::filter slice_predicate;

    std::vector<fsv::filter> filts;

    //filts.reserve(4);
    filts.push_back(fsv.predicate());

    int Slice_begin = 0;//each slice start index position
    int begin_i = Slice_begin;
    int end_i =static_cast<int>( fsv.size())-1;
    int count_c = 0;

    int tmp = 0;
    int j = 0;
    int count = 0;

    //check the type of fsv underlying data
    // const std::type_info &t1 = typeid(std::string);
    // const std::type_info &t2 = typeid(const char*);
    // const std::type_info &t3 = typeid(fsv.data());

    // if(t1 == t3){

    // }else if(t2 == t3){
        
    // }


    for(int i = 0; i < static_cast<int>(fsv.size());){

        // std::cout<<i<<std::endl;
        count = 0;
        j = 0;
        tmp = i;

        if(fsv.at(i)==tok.at(0)){
            // std::cout<<"when at(i) == tok.first, i: "<<i<<std::endl;
            
            while(i < static_cast<int>(fsv.size())&& j < static_cast<int>(tok.size()) && fsv.at(i)==tok.at(j)){
            count++;
            // std::cout<<"count "<<count<<std::endl;           
            // std::cout<<"in while loop, i: "<<i<<std::endl;
            if(i+1>static_cast<int>(fsv.size())&& j+1> static_cast<int>(tok.size()) &&fsv.at(i+1)!=tok.at(j+1)){
                
                i = tmp;
                break;  
            } 
            i++;
            // std::cout<<" i: "<<i<<std::endl;
            j++;
            // std::cout<<" j: "<<j<<std::endl;
            
            
            }


        }

        if(count == static_cast<int>(tok.size())){
            // std::cout<<"when count == tok.size(), i: "<<i<<std::endl;
            begin_i = Slice_begin;
            end_i = i-count+1;
           
           //define a temporary new predicate for each slice
            fsv::filter slice_predicate_tmp = [begin_i, end_i, &count_c](const char& c){ 
                // std::cout<<"count_c"<<count_c<<std::endl;
                //if the letter position is between the slice begin index and the tok, then return true
                if(c) count_c++;
                if(count_c>=begin_i && count_c<end_i ) return true;
                else return false;
                };

            count_c = 0;
            filts.push_back(slice_predicate_tmp);

            //use compose here to make sure new predicate and fsv->predicate can be all applied
            auto fsv_tmp = compose(fsv,std::vector<fsv::filter>{filts[0] ,filts.back(), tok.predicate()});
            
            vec.push_back(fsv_tmp);
            
            if(i < static_cast<int>(fsv.size())){
                //i+= static_cast<int>(tok.size());
                Slice_begin = i+1;
            }

            count = 0; 
            j=0;
        }else{
            // std::cout<<"else, i: "<<i<<std::endl;
            // if((i+1) < static_cast<int>(fsv.size())){
            i++;

            // }
            // std::cout<<"else, i: "<<i<<std::endl;

            j=0; 
        }

        if(i==static_cast<int>(fsv.size())-1){
            begin_i = Slice_begin;//the begin_i actually need to be the underlying data index
            end_i = static_cast<int>(fsv.size())+1;

            fsv::filter slice_predicate_tmp_i = [begin_i, end_i, &count_c](const char& c){ 
                // std::cout<<"count_c"<<count_c<<std::endl;
                if(c) count_c++;
                if(count_c>=begin_i && count_c<end_i ) return true;
                return false;
            };
            count_c = 0;
            filts.push_back(slice_predicate_tmp_i);
            
            auto fsv_tmp_i = compose(fsv, std::vector<fsv::filter>{filts[0] ,filts.back()});

            vec.push_back(fsv_tmp_i);
        }
        
    }

    for(auto it = vec.begin(); it!=vec.end();it++){
        std::cout<<*it<<std::endl;
    }
    // std::cout<<fsv.size()<<std::endl;
    // std::cout<<std::strlen(fsv.data())<<std::endl;

    return vec;
}


auto fsv::substr(const fsv::filtered_string_view &fsv, int pos, int count) -> fsv::filtered_string_view{
 
    //so for the given position we can crate a new predicate for given scope
    int rcount = count <= 0 ? static_cast<int>(fsv.size()) - pos : count;

    if(rcount == 0) return fsv::filtered_string_view{fsv.data(), [](const char &){return false;}};
    
    int count_c = 0; //calculate the psoition in underlying data

    fsv::filter predicate_sub = [pos, rcount,&count_c](const char& c){

        if(c) count_c++;
        if(count_c < pos+rcount+1 && count_c >pos) return true; 
        else return false;
    };

    std::vector<fsv::filter> vec;
    vec.push_back(fsv.predicate());
    vec.push_back(predicate_sub);
    
    return fsv::compose(fsv, vec);
    
}

