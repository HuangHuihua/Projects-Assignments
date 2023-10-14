#ifndef COMP6771_PIPELINE_H
#define COMP6771_PIPELINE_H

#include<exception>
//#include<stdexcept>
#include<string>
#include<iostream>
#include<vector>
#include<concepts>
#include<cstddef>
#include<map>
#include<utility>
#include <memory>
#include <typeinfo>
#include <tuple>

namespace ppl {

    //we need to correctly construct these type, so user can derive them easily

    // Errors that may occur in a pipeline.
    enum class pipeline_error_kind {
        // An expired node ID was provided.
        invalid_node_id,
        // Attempting to bind a non-existant slot.
        no_such_slot,
        // Attempting to bind to a slot that is already filled.
        slot_already_used,
        // The output type and input types for a connection don't match.
        connection_type_mismatch,
    };

    struct pipeline_error : std::exception {
        explicit pipeline_error(pipeline_error_kind);
        auto kind() -> pipeline_error_kind;
        auto what() -> const char *;
    
    private:
        pipeline_error_kind pplek;
    };

// NODE AND POLL

    // The result of a poll_next() operation.
    enum class poll {
        // A value is available.
        ready,
        // No value is available this time, but there might be one later.
        empty,
        // No value is available, and there never will be again:
        // every future poll for this node will return `poll::closed` again.
        closed,
    };

    class node {
    public:
        virtual auto name() const -> std::string;//pure virtual function

        virtual ~node();

        // struct nodeId{
        //     //char* char_part;
        //     int int_part;
        // };

        //node(){};
        // node(input){};

        // std::vector<std::pair<node*, int>> children;
        // std::vector<std::pair<node*, int>> parents;

    private:
        
        virtual auto poll_next() -> poll;//pure virtual function
        virtual void connect(const node* source, int slot);//pure virtual function

        // You may add any other virtual functions you feel you may want here.


        friend class pipeline;
    };

//PRODUCER 

    template <typename Output>
    struct producer : node {
        using output_type = Output;
        //pure virtual function
        virtual auto value() const -> const output_type&; // only when `Output` is not `void`


        
    };

    //when output equal to void
    template <>
    struct producer<void> : node {
        using output_type = void;

    };       

//COMPONENT

    template <typename Input, typename Output>
    struct component : producer<Output> {
        using input_type = Input; 
        //no private
    };

//SINK

    template <typename Input>
    struct sink : component<std::tuple<Input>, void> {

    };

//SOURCE

    template <typename Output>
    struct source : component<std::tuple<>, Output> {
    private:
        //Provides a default implementation for node::connect,knowing that this function will never be called
        void connect(const node *source, int slot) override;

    };


    // The requirements that a type `N` must satisfy
    // to be used as a component in a pipeline.
    template <typename N>
    // 3.6.0
    concept concrete_node = requires(N){//大概createnode有错是这里没有写好
        //{a}->std::convertible_to<component<std::tuple<void*, void*>,void*>>;
        //requires std::is_same_v<typename N::input_type, typename component<typename N::Input, typename N::Output>::input_type>;

        requires std::is_class_v<N>;
        typename N::input_type;
        //syntax to check tuple type
        //requires std::is_same_v<typename N::input_type, std::tuple<typename N::Args>>; //some tuple function to check, 

        requires( sizeof(std::tuple_size_v<typename N::input_type>) == sizeof(int) );
        //requires (std::is_tuple_v<typename N::input_type> == true);

        typename N::output_type;
        requires std::is_base_of_v<node, N>;
        
        // if constexpr(std::is_same_v<typename N::output_type,void>){
        //     requires std::is_base_of_v<producer<void>,N>;
        // }
        // else{
            requires std::is_base_of_v<producer<typename N::output_type>,N>;
        //}
        
            if constexpr(std::is_same_v<typename N::output_type, void>){
                requires std::is_base_of_v<producer<void>,N>;
            };
        
        

    };

//PIPELINE

    class pipeline {
    private: 
        struct node_info;
    public:
        // 3.6.1
        using node_id = node_info*;/* unspecified */
    private:
        struct slot{
                int sid;
                int usage;// 0 is no connect, 1 is in use
                //char* typeName;
                std::type_info slot_type;
        };
        struct node_info {
            // children
            // children nodes
            //child and slot of the child  
            std::vector<node_info*> children;
            // node
            ppl::node* node_;
            // parent and which slot connect that parent
            std::vector<std::pair<node_info*, slot*>> parents;
            // number slots
            std::vector<slot*> slots;

            std::type_info output;

            //template<typename Input, typename Output>
            // node_info() {

            //     //slots.resize()
            // }
        };

        //need an id for each component
        // struct nodeId{
        //     //char* char_part = "A";
        //     int int_part;
        // };

        //A pair or map
        std::vector<node_id> node_vec;
        //std::map<node_id, node*> m;

        int count = 0;

    public:


        //store nodes, any types including source,sink, component
        //std::vector<std::pair<node_id, node*>> m;

        // 3.6.2
        pipeline();
        pipeline(const pipeline &) = delete;
        pipeline(pipeline&&);
        auto operator=(const pipeline &) -> pipeline& = delete;
        auto operator=(pipeline &&) noexcept-> pipeline&;
        ~pipeline();

        // 3.6.3
        template <typename N, typename... Args>
        requires concrete_node<N> and std::constructible_from<N, Args...>
        auto create_node(Args&& ...args) -> node_id{
            //create something like component

            std::unique_ptr<N> node_ptr = std::make_unique<N>(std::forward<Args>(args)...);
            //node* node_ptr = new node;
            //N* node_ptr = new N{args...};//use smart pointer
            node_id id;
            node_vec.push_back(id);
            id->node_ = node_ptr;


            //id.int_part = count;
            count++;
            //what is the output?
            //is args only input type or it has output type? has output type

            //some variadic function
            // std::va_list args_;
            // std::va_start(args_);

            for(int i = 0; i < std::tuple_size_v<N::input_type>; i++){
                //have slot type, and we need to  
                slot* slot_;
                slot_->sid = i;
                slot_->slot_type = typeid(std::get<i>(N::input_types));//decltype()
                slot_->usage = 0;
                id->slots.push_back(slot_);
            
            }
            
            id->output = typeid(N::output_type);

            //node_vec.insert(std::make_pair(id,dynamic_cast<N*>(node_ptr)));
            return id;

        }

        void erase_node(node_id n_id){
            //diconnect its parents and children
            //auto node = get_node(n_id);
            //find the parent and disconnect and set the used slot = 0
            for(auto p : n_id->parents){
                disconnect(p.first, n_id);
                //p.second->usage = 0;//done in disconnect function
            }
            
            for(auto c : n_id->children){
                //for each child, find n_id in their parents vector and disconnect and set usage of slot = 0
                disconnect(n_id, c);

            }

            for (std::vector<node_id>::iterator it = node_vec.begin(); it != node_vec.end();it++)
            {
                if (*it == n_id)
                    node_vec.erase(it);
            }
            
            //finally remove from sets of nodes
            
            
        }

        auto get_node(node_id n_id) -> node*{
            //auto node = m.find(n_id);
            return n_id->node_;
            //if(node == m.end()) throw ppl::pipeline_error(ppl::pipeline_error_kind::invalid_node_id);
            //return node->second;
        }

        // 3.6.4
        void connect(node_id src, node_id dst, int slot){
            //connect two component
            //auto node_src = get_node(src);
            //auto node_dst = get_node(dst);

            //check slot 
            auto it = dst->slots.begin();
            it+=slot;

                if((*it)->slot_type != src->output){
                    ppl::pipeline_error(ppl::pipeline_error_kind::connection_type_mismatch);
                }
            

            //typeid
            
        }

        void disconnect(node_id src, node_id dst){

            // (void)src;
            // (void)dst;

            
            //get the src out from dst parents vector
            for(auto it_src = dst->parents.begin(); it_src != dst->parents.end(); it_src++){
                if((*it_src).first == src){
                    (*it_src).second->usage = 0;
                    dst->parents.erase(it_src);
                }
            }

            //get the dst out from src children vector
            for(auto j = src->children.begin(); j != src->children.end(); j++){
                if((*j) == dst){
                    // for(auto it_p_j = (*j)->parents.begin(); it_p_j != (*j)->parents.end(); it_p_j++){
                    //     if(){
                            
                    //     }
                    // }
                    src->children.erase(j);
                }
                
                
            }
        }

        //get child, return the connected slots, a-> b01, ->b02
        auto get_dependencies(node_id src) -> std::vector<std::pair<node_id, int>>{
            std::vector<std::pair<node_id, int>> Alldependency;

            for(auto it : src->children){
                for(auto pit : it->parents){
                    if(pit.first == src){
                        Alldependency.push_back(std::make_pair(it, pit.second->sid));
                    }
                }
            }
            
            return Alldependency;
        }

        // 3.6.5
        auto is_valid() -> bool{

            return false;
        }
        auto step() -> bool{

            return false;
        }
        void run(){
            
        }

        // 3.6.6
        friend std::ostream &operator<<(std::ostream &, const pipeline &);
    };


}

#endif  // COMP6771_PIPELINE_H
