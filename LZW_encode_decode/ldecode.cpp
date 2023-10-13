

#include <iostream>
#include <fstream>
#include <bitset>
#include <map>

uint8_t msb = 0b10000000;
uint8_t mask = 0b00000000;
uint16_t msb16 = 0b1000000000000000;

namespace LZW_decode{

    auto isASCII(const char &c)->bool{
        //check if the first bit is 0 or 1
        // 0 ---> in ASCII table
        // 1 ---> in Dictionary
        uint8_t tmp = 0;
        tmp = mask | c;
        tmp = tmp >> 7;
        // if the left most bit is still 1
        if(tmp == 0)return true;
        return false;

    }

    // auto concatenate(const char &lowPart, const char &c)->std::string{
    //     std::string pc;
    //     pc = std::string(1,lowPart) + c;
    //     return pc;
    // }

    // auto convertStrtoUINT16()->uint16_t{

    // }

    auto concatenate(std::string &p, char &c)->std::string{
        std::string pc;
        pc = p + c;
        return pc;
    }

    auto isLenTwo(const std::string &pc)->bool{
        
        return false;
    }

    auto inDict(const std::string &pc,const std::map<uint16_t, std::string> Dict, const std::map<std::string, uint16_t> &CheckDuplicateDict)->bool{
        auto it = CheckDuplicateDict.find(pc);
        if(it != CheckDuplicateDict.end()) return true;
        return false;
    }

    auto addtoDict(const std::string &pc,const uint16_t &twoByteIndex, std::map<uint16_t, std::string> &Dict, std::map<std::string, uint16_t> &CheckDuplicateDict)->void{
        Dict[twoByteIndex] = pc;
        CheckDuplicateDict[pc] = twoByteIndex;

    }

    // auto addtoCheckDuplicateDict(const std::string &pc, uint16_t &twoByteIndex, std::map<std::string, uint16_t> &CheckDuplicateDict)->void{
    //     CheckDuplicateDict[pc] = twoByteIndex;
    // }

    auto printDictionary(const std::map<uint16_t, std::string> &Dict)->void{
        std::cout<<"===================Dictionary====================="<<std::endl;
        std::cout<<"\tindex\t"<<"\t|\t"<<"Symbol\t"<<std::endl;
        std::cout<<"------------------------+-------------------------"<<std::endl;
        for(auto& obj: Dict){
            std::cout<<"\t"<<obj.first<<"\t\t\t"<<obj.second<<std::endl;
        }
    }

    auto printDictionary(const std::map<std::string, uint16_t> &Dict)->void{
        std::cout<<"===================Dictionary====================="<<std::endl;
        std::cout<<"\tindex\t"<<"\t|\t"<<"Symbol\t"<<std::endl;
        std::cout<<"------------------------+-------------------------"<<std::endl;
        for(auto& obj: Dict){
            std::cout<<"\t"<<obj.second<<"\t\t\t"<<obj.first<<std::endl;
        }
    }

    auto LZW_decoding(std::string input_path, std::string output_path)->void{
        std::ifstream InputTXT;
        InputTXT.open(input_path);

        std::ofstream OutputTXT(output_path);

        if(InputTXT.fail()){
            std::cerr<<"Error - Failed to open" << input_path <<std::endl;
            exit(-1);
        }

        char c = InputTXT.get();
        std::string p;
        std::string pc;
        std::string pc_0;
        uint8_t mask = 0b10000000;
        uint16_t twoByteIndex = 0; // initially is 0b1000 0000 0000 0000
        // uint16_t indexBeforeConvert = 0;
        std::map<uint16_t, std::string> Dict;
        std::map<std::string, uint16_t> CheckDuplicateDict;

        if(c !=EOF)
            OutputTXT<<c;
        std::cout<<c<<std::endl;
        p = c;

        while(InputTXT.good()){
            c = InputTXT.get();
            std::cout<<"c = "<<c<<std::endl<<"\tcurrent index is "<<twoByteIndex<<"|\t";

            if(isASCII(c)){
                //directly output char we read
                OutputTXT<<c;
                pc = concatenate(p, c);
                std::cout<<"isASCII(c)"<<"\tpc: "<<pc<<std::endl;
                //check if pc in dictionary already
                if(inDict(pc, Dict, CheckDuplicateDict)){
                    //we don't add pc into dictionary
                    //but we make p = pc
                    p = pc;
                    std::cout<<"\tpc exist in dict, p = pc: "<<p<<std::endl;
                }else{
                    //if pc isn't in dictionary, we add pc 
                    uint16_t idx = (mask << 8) | twoByteIndex;
                    addtoDict(pc, idx, Dict, CheckDuplicateDict);
                    twoByteIndex++;

                    //p = c
                    p = c;
                    std::cout<<"\tpc not exist in dict, p = c: "<<p<<std::endl;

                }
                std::cout<<"\t\t\t in ASCII(c), p is "<<p<<std::endl;

            }else{//if we meet 1-msb byte, it means its an index
                //calculate index
                char lowBits = InputTXT.get();
                uint16_t conv = lowBits & 0b0000000011111111;
                std::cout<<std::endl<<"------------ c = "<<std::bitset<8>(c)<<"------------"<<std::endl;
                uint16_t index = (c << 8) | conv;   
                std::cout<<"is index"<<"\tc: "<<std::bitset<16>(c << 8)<<std::endl;
                std::cout<<"is index"<<"\tlowBits: "<<std::bitset<8>(lowBits)<<std::endl;
                std::cout<<"is index"<<"\tindex: "<<std::bitset<16>(index)<<std::endl;
                std::cout<<"is index"<<"\tindex: "<<(index & 0b0111111111111111)<<std::endl;
                // when it haven't got <index> in our dictionary 
                auto search = Dict.find(index);
                if(search != Dict.end()){
                    std::cout<<"\t\tThe index exist and "<<"p + Dict[index][0] = ";
                    pc_0 = concatenate(p, Dict[index][0]);
                    std::cout<<pc_0<<std::endl;
                }else{
                    //it's a special case
                    
                    std::cout<<"\t\tOH NO, the index not exist in dictionary!!"<<std::endl;
                    //we add the result into dictionary
                    std::string pnc;
                    // since p + Dict[<5>][0] = Dict[<5>]; which means p[0] = Dict[5][0]; Dict[<5>] = p + p[0]
                    pnc = concatenate(p, p[0]);
                    std::cout<<"\t\tWe add Dict["<<(index & 0b0111111111111111)<<"], and p + Dict[index][0] = ";
                    addtoDict(pnc, index, Dict, CheckDuplicateDict);
                    twoByteIndex++;
                    pc_0 = concatenate(p, Dict[index][0]);
                    std::cout<<pc_0<<std::endl;

                }
                OutputTXT<<Dict[index];

                // check p + Dict[c][0] if exist in dictionary
                std::cout<<"Check if p + Dict[index][0] exist? ----- ";
                if(inDict(pc_0,Dict,CheckDuplicateDict)){
                    //if p + Dict[c][0] exist in dictionary, we don't add
                    // make p = p + Dict[c][0]
                    // p = pc_0;
                    std::cout<<"Yes, "<<pc_0<<std::endl;
                }else {
                    uint16_t idx = (mask << 8) | twoByteIndex;
                    addtoDict(pc_0, idx, Dict, CheckDuplicateDict);
                    twoByteIndex++;
                    // make p = Dict[c]
                    // p = Dict[index];
                    std::cout<<"No, we add p + Dict["<<(idx & 0b0111111111111111)<<"][0] into dict: "<<pc_0<<std::endl;
                    
                }
                p = Dict[index];
                std::cout<<"\t\t\tp is "<<p<<", Dict["<<(index & 0b0111111111111111)<<"]="<<Dict[index]<<std::endl;

            }


        }

        printDictionary(Dict);
        // printDictionary(Dict);

        InputTXT.close();

    }
}

int main(int argc, char *argv[]){
    // std::cout<<"Hello"<<std::endl;

    for(auto i = 1; i < argc; ++i)
        std::cout<<"path: "<<argv[i]<<std::endl;

    LZW_decode::LZW_decoding(argv[1],argv[2]);

    return 0;
}