#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <bitset>
#include <stdlib.h>
#include <optional>
#include <stdint.h> 
//don't use bitset
// 是不是读漏了，跳了symbol
#define DICT_LEN 0b1000000000000000
uint16_t msb;

namespace LZW{

    auto concatenate(const std::string p, const char &c)->std::string{
        
        if(p == "") return "";
        std::string pc;
        pc = p + c;
        return pc;

    }

    auto isFull(const std::map<std::string, uint16_t> Dict){
        if (Dict.size() == DICT_LEN) return true;
        return false;
    }

    auto addIntoDict(const std::string &pc, std::map<std::string, uint16_t> &Dict, uint16_t index)->void{
        
        msb = ((((uint16_t)-1) >> 1) +1);
        //   std::cout<<"\t"<<"==================index:"<<index<<"---------msb:"<<msb<<"\t";
        index |= msb;
        // std::cout<<"pc:"<<pc<<"\t";
        Dict[pc] = index;
        // std::cout<<"Dict[pc]:"<<Dict[pc]<<"\t";

        // std::cout<<"\t"<<"==================after msb index = "<<index<<"\t";

    }


    auto isASCII(const std::string &pc)->bool{
        if(pc.size() <= 1) return true;
        return false;
    }


    auto code(const std::string &p, const std::map<std::string, uint16_t> &Dict)->uint16_t{
        auto search = Dict.find(p);
        if(search != Dict.end())
            return search->second;
        return false;
    }

    auto Low(const uint16_t &index)->uint8_t{
        uint8_t low8bit_;
        low8bit_ = index & 0xFF;
        // std::cout<<"low"<<std::bitset<8>(low8bit_)<<std::endl;
        return low8bit_;
    }

    auto High(const uint16_t &index)->uint8_t{
        uint8_t high8bit_;
        high8bit_ = (index & 0xFF00) >> 8;
        
        // std::cout<<"high"<<std::bitset<8>(high8bit_)<<std::endl;
        return high8bit_;
    }

    void LZW_encoding(std::string input_path, std::string output_path){

        //read input file from path argv[1]
        std::ifstream InputTXT;
        InputTXT.open(input_path);

        //output the result to output file argv[2]
        std::ofstream OutputTXT(output_path);

        //if fail to open file, exit
        if(InputTXT.fail()){
            std::cerr<<"Error - Failed to open" << input_path <<std::endl;
            exit(-1);
        }

        std::string p = ""; //prev char
        char c; //current char

        uint16_t index = 0; // count index 
        // first is symbol(s), second is binary string
        std::map<std::string, uint16_t> dict; // dictionary with 15-bit length index

        std::cout<<" \t \t      \t|     Dict"<<std::endl;
        std::cout<<"p\t"<<"c\t"<<"output\t"<<"| index\t"<<"symbol"<<std::endl;
        std::cout<<"---------------------------------------------"<<std::endl;

        //read symbol by symbol
        while(InputTXT.good()){ 
            c = InputTXT.get();
            std::string pc = concatenate(p,c);

            std::cout<<p<<"\t"<<c<<"\t";

                if(dict.find(pc) != dict.end()){

                    p = pc;
                    std::cout<<"~~~~~Find pc in dictionary!"<<std::endl;

                }else{

                    // if pc is actually ascii code we don't add to dictionary
                    if(isASCII(pc)){
                        //directly output to file
                        OutputTXT<<pc;
                        std::cout<<std::endl;
                    }else
                    {
                        
                        if(!isFull(dict) && c!=EOF){
                            addIntoDict(pc,dict,index);                        
                            index++;        
                        }
                                            
                        std::string output = p;
                        std::string symbol= pc;
                        //if p is ASCII
                        if(isASCII(p)){
                            OutputTXT<<p;
                            // std::cout<<output<<std::endl;
                            // std::cout<<"in ASCII table ";
                            std::cout<<output<<"\t| "<<dict[pc]<<"\t"<<symbol<<std::endl;
                            
                        }else{

                            OutputTXT<<High(code(p,dict));
                            OutputTXT<<Low(code(p,dict));
                            uint16_t index_ = code(p,dict);
                            // std::string symbol= pc;
                            // // std::cout<<std::endl<<std::bitset<16>(code(p,dict))<<std::endl;
                            // std::cout<<output<<"\t"<<"| "<<std::bitset<8>(High(code(p,dict)))<<"\\"<<std::bitset<8>(Low(code(p,dict)))<<"\t"<<symbol<<std::endl;
                            
                            std::cout<<index_<<"\t| "<<dict[pc]<<"\t"<<symbol<<std::endl;
                            // std::cout<<output<<"\t"<<"| "<<dict[pc]<<"\t"<<std::bitset<8>(High(code(p,dict)))<<"\\"<<std::bitset<8>(Low(code(p,dict)))<<"\t"<<symbol<<std::endl;

                        }
                        
                    }
                        
                    p = c;
                }

        }

        

        std::cout<<std::endl;

        //close the input file
        InputTXT.close();


    }

    

}

int main(int argc, char*argv[]){

    for(auto i = 1; i < argc; ++i)
        std::cout<<"path: "<<argv[i]<<std::endl;

    LZW::LZW_encoding(argv[1], argv[2]);

    std::cout<<std::endl;

    return 0;
}