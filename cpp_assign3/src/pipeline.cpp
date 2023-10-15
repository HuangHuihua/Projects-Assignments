#include "./pipeline.h"


//pipeline exception
ppl::pipeline_error::pipeline_error(ppl::pipeline_error_kind kind){
    //return kind;
    this->pplek = kind;
}

auto ppl::pipeline_error::kind() -> pipeline_error_kind{


        if(this->pplek == ppl::pipeline_error_kind::invalid_node_id)
            return ppl::pipeline_error_kind::invalid_node_id;

        if(this->pplek == ppl::pipeline_error_kind::no_such_slot)
            return ppl::pipeline_error_kind::no_such_slot;

        if(this->pplek == ppl::pipeline_error_kind::slot_already_used)
            return ppl::pipeline_error_kind::slot_already_used;

        if(this->pplek  == ppl::pipeline_error_kind::connection_type_mismatch)
            return ppl::pipeline_error_kind::connection_type_mismatch;

    return ppl::pipeline_error_kind::invalid_node_id;
}
auto ppl::pipeline_error::what() -> const char *{
    
    if(this->kind() == ppl::pipeline_error_kind::invalid_node_id) return "invalid node ID";
    if(this->kind() == ppl::pipeline_error_kind::no_such_slot) return "no_such_slot";
    if(this->kind() == ppl::pipeline_error_kind::slot_already_used) return "slot_already_used";
    if(this->kind() == ppl::pipeline_error_kind::connection_type_mismatch) return "connection_type_mismatch";
    
    return "";
}

//pipeline

ppl::pipeline::pipeline(pipeline&& other){

    if(this != &other){
       
    }
}


// template <typename N, typename... Args>
// requires ppl::concrete_node<N> and std::constructible_from<N, Args...>
// auto ppl::pipeline::create_node(Args&& ...args) -> ppl::pipeline::node_id {
//     //create something like component

//     // template<typename T, typename... Args>
//     // typename std::enable_if<sizeof...(Args) == 0, T>::type
//     // last_arg(T t, Args... args) {
//     //     return t;
//     // }

//     // template<typename T, typename... Args>
//     // typename std::enable_if<sizeof...(Args) != 0, T>::type
//     // last_arg(T t, Args... args) {
//     //     return last_arg(args...);
//     // }

//     N* node_ = new N{args...};
//     node_id id;
//     id.int_part = count;
//     count++;
//     m.insert(std::make_pair(id,node_));
//     return id;


// }
// void erase_node(ppl::pipeline::node_id n_id){

// }
// auto get_node(ppl::pipeline::node_id n_id) -> node*{
//     auto node = m.find(n_id);
//     if(node == m.end()) throw ppl::pipeline_error(ppl::pipeline_error_kind::invalid_node_id);
//     return node->second;
// }

// // 3.6.4
// void connect(ppl::pipeline::node_id src, ppl::pipeline::node_id dst, int slot){
//     //connect two component
//     auto node_src = get_node(src);
//     auto node_dst = get_node(dst);
    
//     node_src->children.push_back(node_dst);
//     //how to conenct slot with src

//     if(src.... .poll_next() == ppl::poll::ready)

    
// }

// void disconnect(ppl::pipeline::node_id src, ppl::pipeline::node_id dst){

// }

// auto get_dependencies(ppl::pipeline::node_id src) -> std::vector<std::pair<ppl::pipeline::node_id, int>>{

// }
