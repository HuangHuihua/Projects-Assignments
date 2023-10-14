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

namespace fsv {
    using filter = std::function<bool(const char &)>;

    class filtered_string_view {
        class iter {
        public:
            using MEMBER_TYPEDEFS_GO_HERE = void;

            iter();

            auto operator*() const -> void; // change this
            auto operator->() const -> void; // change this

            auto operator++() -> iter&;
            auto operator++(int) -> iter;
            auto operator--() -> iter&;
            auto operator--(int) -> iter;

            friend auto operator==(const iter &, const iter &) -> bool;
            friend auto operator!=(const iter &, const iter &) -> bool;

        private:
            /* Implementation-specific private members */
        };
    public:
    
        static filter default_predicate;

        //default constructor
        filtered_string_view();

        //Implicit String Constructor 
        filtered_string_view(const std::string &str);


        //String Constructor with Predicate
        filtered_string_view(const std::string &str, filter predicate);

        //Implicit Null-Terminated String Constructor
        filtered_string_view(const char *str);

        //Null-Terminated String with Predicate Constructor
        filtered_string_view(const char *str, filter predicate);


        //Copy and Move Constructors
/* 1 */ filtered_string_view(const filtered_string_view &other);
/* 2 */ filtered_string_view(filtered_string_view &&other);

        const char* data() const;


        auto size() -> std::size_t;   


    private:
        //pointer point to stirng
        const char * rawPointer;
        //lenght of string
        std::size_t stringLen;

    };
}


#endif // COMP6771_ASS2_FSV_H
