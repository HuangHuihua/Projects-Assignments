#include "./pipeline.h"

#include <catch2/catch.hpp>

TEST_CASE("Ass3 -- it's piping hot") {
    CHECK(false);
}


TEST_CASE("pipeline_error test") {
    
    try{
        throw ppl::pipeline_error(ppl::pipeline_error_kind::no_such_slot);
    }catch(ppl::pipeline_error &pe){//### what() cannot be const
        std::cout<<pe.what()<<std::endl;
    }
}

TEST_CASE("pipeline move construstor") {
    
    ppl::pipeline p1;
    auto p2 = std::move(p1);

}

TEST_CASE("component"){
    
}


TEST_CASE("Test the concrete_node<N>, create_node function"){

}


TEST_CASE("pipeline Image rotation") {
    
    //Rectangle source and rotation source

    //take image three corner and times double values to do tramformation/rotation

    //then produce the output
}
