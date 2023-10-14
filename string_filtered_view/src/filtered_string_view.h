#ifndef COMP6771_ASS2_FSV_H
#define COMP6771_ASS2_FSV_H

#include <compare>
#include <exception>
#include <functional>
#include <iterator>
#include <string>
#include <vector>

#include<iostream>
#include<optional>

#include <utility>
#include<algorithm>

#include <set>
#include<cstring>

namespace fsv {
    using filter = std::function<bool(const char &)>;

    using cvec = const char;

    class filtered_string_view {
        class iter {//add predicate
        public:
        

            using iterator_category = std::bidirectional_iterator_tag;

            using value_type = char;

            using reference = const char &;

            using pointer = void;

            using difference_type = std::ptrdiff_t;

            iter() = default;
            //iter(const cvec*bwgib() ) 

            iter(const cvec *cb, const cvec *ce, fsv::filter predicate,std::size_t ptr):b(cb),e(ce),predicate_(predicate), ptr_(ptr){

                
            }
            //iter(const cvec *cb, std::size_t ptr):b(cb), ptr_(ptr){}
            //iter(const cvec *ce, std::size_t ptr):e(ce), ptr_(ptr){}

            iter(const iter&other){
                this->b = other.b;
                this->e = other.e;
                this->ptr_ = other.ptr_;
            }
            // iter(const cvec *cb,std::size_t ptr):b(cb), ptr_(ptr){

            // }
            iter(std::size_t ptr): ptr_(ptr){}

            //auto operator=(const iter&)->iter&;

            reference operator*() const// change this
            {
                
                return *b;
            }
            pointer operator->() const{

            }

            auto operator++() -> iter&{
                ptr_++;
                while(!predicate_(b[ptr_])){
                    ptr_++;
                }
                return *this;
            }
            auto operator++(int) -> iter{
                auto tmp = *this;
                while(!predicate_(b[ptr_])){
                    ptr_++;
                }
                ++(*this);
                return tmp;

            }
            auto operator--() -> iter&{
                ptr_--;
                while(!predicate_(b[ptr_])){
                    ptr_--;
                }
                return *this;
            }
            auto operator--(int) -> iter{
                auto tmp = *this;
                while(!predicate_(b[ptr_])){
                    ptr_--;
                }
                --(*this);
                return tmp;             
            }

            friend auto operator==(const iter &a, const iter &b) -> bool{
                return a.ptr_ == b.ptr_;
            }
            friend auto operator!=(const iter &a, const iter &b) -> bool{
                return !(a == b);
            }

            
        private:
            /* Implementation-specific private members */
            const cvec* b;  //string begin
            const cvec* e;  //string end
            std::size_t ptr_;//len 

            fsv::filter predicate_;

            //iter();
            
        };
    private:
        //pointer point to stirng
        const char * rawPointer;
        //length of string
        std::size_t stringLen;//

        const cvec* begin_;
        const cvec* end_;
    public:

        //using iterator = iter;
        using const_iterator = iter;
        using iterator = const_iterator;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;


        //constructor to init begin end
        filtered_string_view(const cvec&begin, const cvec&end): begin_(&begin), end_(&end){
            // begin_ = &this->at(0);
            // end_ = &this->at(0+static_cast<int>(stringLen));
        }     

        auto begin()->iterator{
            return iterator{begin_,end_,0,};
        }
        auto end()->iterator{
            return iterator{begin_,end_,stringLen};
        }

        auto begin() const->const_iterator{
            return iterator{begin_,end_,0};

        }
        auto end() const -> const_iterator{
            return iterator{begin_,end_,stringLen};

        }

        auto cbegin()->iterator{
            return begin();
        }
        auto cend()->iterator{
            return end();
        }

        auto cbegin() const ->const_iterator{
            return begin();
        }
        auto cend() const -> const_iterator{
            return end();
        }

        auto rbegin() ->reverse_iterator{
            return reverse_iterator{end()};
        }
        auto rend() ->reverse_iterator{
            return reverse_iterator{begin()};
        }

        auto rbegin() const -> const_reverse_iterator{
            return const_reverse_iterator{end()};
        }
        auto rend() const -> const_reverse_iterator{
            return const_reverse_iterator{begin()};
        }

        auto crbegin() ->reverse_iterator{
            return rbegin();
        }
        auto crend() const -> const_reverse_iterator{
            return rend();
        }

    
    
        filter default_predicate = [](const char&){return true;};

        //default constructor
        //filtered_string_view() = default;
        filtered_string_view();

        //Implicit String Constructor 
        explicit filtered_string_view(const std::string &str);


        //String Constructor with Predicate
        filtered_string_view(const std::string &str, filter predicate);

        //Implicit Null-Terminated String Constructor
        explicit filtered_string_view(const char *str);

        //Null-Terminated String with Predicate Constructor
        filtered_string_view(const char *str, filter predicate);


        //Copy and Move Constructors
        filtered_string_view(const filtered_string_view &other);
        filtered_string_view(filtered_string_view &&other) noexcept;

        //operator overload
        //Copy Assignment
        filtered_string_view &operator=(const filtered_string_view &other);

        // auto operator==();

        //Move assignment 
        filtered_string_view &operator=(filtered_string_view &&other) noexcept;


        auto operator[](int n) -> const char &;

        explicit operator std::string();

        //auto at(int index) -> const char &;
        const char & at(int index) const;

        friend auto operator==(const filtered_string_view &lhs, const filtered_string_view &rhs) -> bool{
            if(lhs.size() != rhs.size()) return false;
            for(int i = 0; static_cast<std::size_t>(i)< lhs.size(); i++){
            if(lhs.at(i) != rhs.at(i)) return false;
            }

            return true;
        }

        
        friend auto operator!=(const fsv::filtered_string_view &lhs, const fsv::filtered_string_view &rhs) -> bool{
            return !(lhs==rhs);
        }


        friend auto operator<=>(const filtered_string_view &lhs, const filtered_string_view &rhs) -> std::strong_ordering{

            //std::size_t len = lhs.size() <= rhs.size()? lhs.size() : rhs.size();

            filtered_string_view lstr = lhs, rstr=rhs;
            
            std::string ls = std::string(lstr);
            std::string rs = std::string(rstr);

            //for(int i = 0; i < len; i++){
                //std::lexicographical_compare(rhs, rhs, lhs, lhs )
                //return lhs.at(i) <=> rhs.at(i);
                //if(std::lexicographical_compare(ls.begin(), ls.end(), rs.begin(), rs.end(), comp)) 
                

            //}

            return ls <=>rs;
        }


        //Outputstream
        friend auto operator<<(std::ostream &os, const filtered_string_view &fsv) -> std::ostream&{
            
            std::string str;
            const char*p = fsv.rawPointer;
            const auto predicate = fsv.predicate();
            while(p != nullptr && *p !='\0'){

                if(predicate(*p)) str+=*p;

                p++;
            }
            //filtered_string_view str = fsv;
            //std::cout<<std::string(str);
            std::cout<<str;
            return os;
        }


        auto empty() -> bool;


        const char* data() const;

        //auto size() -> std::size_t;   
        std::size_t size() const;   

        auto predicate() const -> const filter&;


        //deconstructor
        ~filtered_string_view() = default;


    };

    auto compose(const fsv::filtered_string_view &fsv, const std::vector<fsv::filter> &filts) -> fsv::filtered_string_view;
    auto split(const fsv::filtered_string_view &fsv, const fsv::filtered_string_view &tok) -> std::vector<fsv::filtered_string_view>;
    auto substr(const filtered_string_view &fsv, int pos = 0, int count = 0) -> filtered_string_view;
}

// auto operator==(const fsv::filtered_string_view &lhs, const fsv::filtered_string_view &rhs) -> bool;
// auto operator!=(const fsv::filtered_string_view &lhs, const fsv::filtered_string_view &rhs) -> bool;



#endif // COMP6771_ASS2_FSV_H
