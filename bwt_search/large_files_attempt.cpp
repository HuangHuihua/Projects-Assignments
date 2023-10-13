#include <iostream>
#include <fstream>
#include <bitset>
#include <map>
// #include <vector>
// #include <sstream>
#include <string>
#include <filesystem>
#include <cmath>
#include <cstdint>
#include <algorithm>

int gap = 1000; // the rank gap

std::string rlb_file;
std::string index_file;
std::string query_pattern;

int num_leftbracket = 0;
std::map<char, int> C_map; // in case we don't need index file

std::string sequence_symbol = "";

int posOfC_IndexFile = 0;

int thesizeofbwt = 0;

bool Fir = false, Las = true; // to check currently execute First pointer or Last pointer
auto occ(const char &c, const int &fl_ptr) -> int
{

    if (Fir == false && Las == true)
    {
        Fir = true;
        Las = false;
    }
    else
    {
        Fir = false;
        Las = true;
    }

    std::ifstream rlb_fin(rlb_file, std::ios::binary);
    if (fl_ptr == 0)
    { // just return if point to the first or before first
        if (c == rlb_fin.get())
            return 1;
        else
            return 0;
    }
    else if (fl_ptr < 0)
        return 0;

    int block_location = 0;
    int offset = 0;
    int rank_of_c = 0;

    // read index file to get information of that row (the end of last block. e.g. if gap = 794, the end of first block has bwt index is 794)
    // std::uintmax_t index_file_size = std::filesystem::file_size(index_file);
    if (gap!=-1)
    {

        int rank_row_in_index = 0;
        if(fl_ptr/gap > 0){
            std::ifstream idx_fin(index_file, std::ios::binary);
            rank_row_in_index = ((fl_ptr / gap) - 1) * 127 * sizeof(int); // 0-126 is the first row.
            // std::cout<<"rank_row_in_index: "<<rank_row_in_index<<std::endl;

            idx_fin.seekg(rank_row_in_index);
            idx_fin.read(reinterpret_cast<char *>(&block_location), sizeof(int));
            // std::cout<<"block_location: "<<block_location<<std::endl;// check the read is correct?

            idx_fin.seekg(rank_row_in_index + 1 * sizeof(int));
            idx_fin.read(reinterpret_cast<char *>(&offset), sizeof(int));
            // std::cout<<"offset"<<offset<<std::endl;

            idx_fin.seekg(rank_row_in_index + (int)c * sizeof(int));
            idx_fin.read(reinterpret_cast<char *>(&rank_of_c), sizeof(int));
            // std::cout<<"rank_of_c"<<rank_of_c<<std::endl;
            idx_fin.close();

        }


        if (fl_ptr % gap == 0 && gap != -1)
            return rank_of_c;

        // open rlb file to scan block untill fl_ptr position(including)
        int cur_pos = -1;
        if (gap != -1)
        {
            cur_pos = (fl_ptr / gap) * gap - offset - 1; // since we want to make cur_pos be consistent with the position of the symbol when reading, so minus 1
        }
        // else
        // {
        //     cur_pos = -1; // gap == -1 means no index file, we scan from the beginning of rlb file
        // }

        // rlb_fin.open(rlb_file, std::ios::binary);
        rlb_fin.seekg(block_location, std::ios::beg);
        std::string symbolcount = "";
        while (cur_pos != fl_ptr)
        {
            char sym = rlb_fin.get(); // from the gap position, if gap=100, from 100, 200...
            char pre_sym;
            if (sym >> 7 == 0)
            {
                cur_pos++;
                pre_sym = sym;
                if(fl_ptr/gap == 0 && cur_pos>=0){
                    if (sym == c)
                        rank_of_c++;
                }
                else if (cur_pos > (fl_ptr / gap) * gap)
                {
                    if (sym == c)
                        rank_of_c++;
                }
            }
            else
            {
                std::string tmpsymbolcount = std::bitset<8>(sym).to_string();
                symbolcount = tmpsymbolcount.substr(1) + symbolcount;
                if (rlb_fin.peek() >> 7 == 0 || (std::stoi(symbolcount, nullptr, 2) + 3 - 1 + cur_pos) >= thesizeofbwt - 1)
                {
                    int symbolcount_int = std::stoi(symbolcount, nullptr, 2) + 3 - 1;

                    if (symbolcount_int + cur_pos >= fl_ptr)
                    { // 0 + 5 = 5, <count> start from 1, cur_pos = 0
                        if (cur_pos >= (fl_ptr / gap) * gap)
                        { // if in the end of previous block or during the current block
                            if (pre_sym == c)
                                return ((fl_ptr - cur_pos) + rank_of_c);
                            else break;
                        }
                        else
                        {
                            if (pre_sym == c)
                                return ((fl_ptr - (fl_ptr / gap) * gap) + rank_of_c); // cur_pos < the end of pre block, so just count from next one after the end of the block
                            else break;
                        }
                    }
                    else
                    { // not reach the first or last pointer position
                        if (cur_pos >= (fl_ptr / gap) * gap)
                        {
                            if (pre_sym == c)
                                rank_of_c = (symbolcount_int + rank_of_c);
                        }
                        else
                        { // if cur_pos < the end of previous block
                            if (cur_pos + symbolcount_int > (fl_ptr / gap) * gap)
                            {
                                if (pre_sym == c)
                                    rank_of_c = (symbolcount_int - ((fl_ptr / gap) * gap - cur_pos) + rank_of_c); // the <count> miuns the offset from cur_pos to the beginning of the block
                            }
                            // else if (cur_pos + symbolcount_int == (fl_ptr / gap) * gap)
                            // {
                            //     // 0 need to be added
                            // }
                            // it will not have the case that cur_pos + symbolcount_int < (fl_ptr/gap)*gap, since we read each block
                        }
                    }

                    symbolcount.clear();
                    cur_pos += symbolcount_int;
                }
            }
        }
    }
    else
    {
        rank_of_c = 0;
        int cur_pos = -1;
        std::string symbolcount = "";
        while (cur_pos != fl_ptr)
        {
            char sym = rlb_fin.get(); // from the gap position, if gap=100, from 100, 200...
            char pre_sym;
            if (sym >> 7 == 0)
            {
                cur_pos++;
                pre_sym = sym;
                if (sym == c)
                    rank_of_c++;
            }
            else
            {
                std::string tmpsymbolcount = std::bitset<8>(sym).to_string();
                symbolcount = tmpsymbolcount.substr(1) + symbolcount;
                if (rlb_fin.peek() >> 7 == 0 || (std::stoi(symbolcount, nullptr, 2) + 3 - 1 + cur_pos) >= thesizeofbwt - 1)
                {
                    int symbolcount_int = std::stoi(symbolcount, nullptr, 2) + 3 - 1;

                    if (symbolcount_int + cur_pos >= fl_ptr)
                    { // 0 + 5 = 5, <count> start from 1, cur_pos = 0
                        if (pre_sym == c)
                            return ((fl_ptr - cur_pos) + rank_of_c);
                        else break;
                    }
                    else
                    { // not reach the first or last pointer position
                        if (pre_sym == c)
                            rank_of_c = (symbolcount_int + rank_of_c);
                    }

                    symbolcount.clear();
                    cur_pos += symbolcount_int;
                }
            }
        }
    }
    return rank_of_c;
}

auto backwardsearch(const std::string &pattern) -> std::pair<int, int>
{
    int i = pattern.size() - 1;
    char c = pattern[i];

    int pattern_C[pattern.size()];

    int First; // point to the first occur posiition of queried symbol in first column, but always -1 to get the rank before first symbol
    int Last;  // always point to the end of sequence of queried symbol in the first column

    // read C from index file
    // std::uintmax_t index_file_size = std::filesystem::file_size(index_file);
    if (gap!=-1)
    {

        std::ifstream idx_fin(index_file, std::ios::binary);

        for (int j = 0; j < static_cast<int>(pattern.size()); j++)
        {

            idx_fin.seekg(posOfC_IndexFile + (int)pattern[j] * sizeof(int));
            idx_fin.read(reinterpret_cast<char *>(&pattern_C[j]), sizeof(int));

            if (j == static_cast<int>(pattern.size()) - 1)
            { // backward search, initilize the Last ptr be the position of the ascii symbol after last letter of pattern and -1
                if (pattern[j] + 1 > 126)
                { // if it's the last symbol in C table, just make it be the size of bwt length
                    Last = thesizeofbwt - 1;
                }
                else
                {
                    int symbolOrder = sequence_symbol.find(pattern[j]);
                    if(static_cast<std::size_t>(symbolOrder) == sequence_symbol.size()-1)  Last = thesizeofbwt - 1;
                    else{
                        symbolOrder++;
                        idx_fin.seekg(posOfC_IndexFile + (int)(sequence_symbol[symbolOrder]) * sizeof(int)); // next symbol after c
                        idx_fin.read(reinterpret_cast<char *>(&Last), sizeof(int));
                    }
                }
            }
        }
        Last--;

        First = pattern_C[i];
        // std::cout<<"First: "<<First<<std::endl;

        // std::cout<<"Last: "<<Last<<std::endl;

        while (First <= Last && i >= 1)
        {
            c = pattern[i - 1];
            int C_c = pattern_C[i - 1];
            First = C_c + occ(c, First - 1);
            // std::cout<<"First: "<<First<<std::endl;
            // flag -o3 optimazation
            Last = C_c + occ(c, Last) - 1;
            // std::cout<<"Last: "<<Last<<std::endl;
            i--;
        }
    }
    else
    {

        First = C_map[c]; //

        for (auto it = C_map.begin(); it != C_map.end(); it++)
        {
            if ((*it).first == c)
            {

                it++;
                Last = C_map[(*it).first] - 1;
                // std::cout<<"First: "<<First<<std::endl;

                // std::cout<<"Last: "<<Last<<std::endl;
                break;
            }
        }

        while ((First <= Last) && (i >= 1))
        { // the position is counted from 1 but the C[] is 0-based.
            c = pattern[i - 1];
            First = C_map[c] + occ(c, First - 1); // should be
            // std::cout << "First" << First << std::endl;
            Last = C_map[c] + occ(c, Last) - 1; // should be
            // std::cout << "Last" << Last << std::endl;
            i = i - 1;
        }
    }

    if (Last < First)
    {
        // std::cout << "no row prefix by " << pattern << std::endl;
        return std::make_pair(-2, -2);
    }
    // std::cout << "The number of matching query is " << Last - First + 1 << std::endl;
    // std::cout<<"First: "<<First<<"\tLast: "<<Last<<std::endl;
    return std::make_pair(First, Last);
}

auto find_symbol_L(const int &fl_ptr) -> char
{
    char Lsymbol;

    // position ==> fl_ptr, fl_ptr/gap * gap --->block location (offset need to be considered),
    // first, the block location is the symbol not count, so offset means we need to backward to the symbol pos(current block position is during count)
    // from that position, cur_pos, forward and reach the fl_ptr and get the symbol

    // open the file and locate at the position
    std::ifstream rlb_fin(rlb_file, std::ios::binary);
    if (fl_ptr == 0) // just return if point to the first
        return rlb_fin.get();

    int rank_row_in_index = 0;
    int block_location = 0;
    int offset = 0;

    // read index file to get information of that row (the end of last block. e.g. if gap = 794, the end of first block has bwt index is 794)
    // std::uintmax_t index_file_size = std::filesystem::file_size(index_file);
    if (gap != -1)
    {

        if(fl_ptr/gap > 0){//only read index file when it is upper or equal to the 1st (0-based) block

            std::ifstream idx_fin(index_file, std::ios::binary);
            rank_row_in_index = ((fl_ptr / gap) - 1) * 127 * sizeof(int); // 0-126 is the first row. miuns one because the first row at 0 poition we discard, next one fl_ptr/gap = 1 is 0 row
            // std::cout<<"rank_row_in_index: "<<rank_row_in_index<<std::endl;

            idx_fin.seekg(rank_row_in_index);
            idx_fin.read(reinterpret_cast<char *>(&block_location), sizeof(int));
            // std::cout<<"block_location: "<<block_location<<std::endl;// check the read is correct?

            idx_fin.seekg(rank_row_in_index + 1 * sizeof(int));
            idx_fin.read(reinterpret_cast<char *>(&offset), sizeof(int));
            // std::cout<<"offset"<<offset<<std::endl;

            idx_fin.close();
        }

        rlb_fin.seekg(block_location, std::ios::beg);
        if (fl_ptr % gap == 0 && gap != -1)

        {
            // no matter have offset or not, we directly get the symbol
            return rlb_fin.get();
        }

        // open rlb file to scan block untill fl_ptr position(including)
        int cur_pos = (fl_ptr / gap) * gap - offset - 1;

        // rlb_fin.open(rlb_file, std::ios::binary);
        rlb_fin.seekg(block_location, std::ios::beg);
        std::string symbolcount = "";
        while (cur_pos != fl_ptr)
        {
            char sym = rlb_fin.get(); // from the gap position, if gap=100, from 100, 200...
            char pre_sym;
            if(rlb_fin.peek())
            {

            if (sym >> 7 == 0)
            {
                cur_pos++;
                pre_sym = sym;
                Lsymbol = sym;
                if (cur_pos == fl_ptr)
                {
                    return sym;
                }
            }
            else
            {
                std::string tmpsymbolcount = std::bitset<8>(sym).to_string();
                symbolcount = tmpsymbolcount.substr(1) + symbolcount;
                if (rlb_fin.peek() >> 7 == 0 || (std::stoi(symbolcount, nullptr, 2) + 3 - 1 + cur_pos) >= thesizeofbwt - 1)
                {
                    int symbolcount_int = std::stoi(symbolcount, nullptr, 2) + 3 - 1;

                    if (symbolcount_int + cur_pos >= fl_ptr)
                    { // 0 + 5 = 5, <count> start from 1, cur_pos = 0
                        return pre_sym;
                    }

                    symbolcount.clear();
                    cur_pos += symbolcount_int;
                }
            }
            }
        }
    }
    else
    { // for no index file
        int cur_pos = -1;
        std::string symbolcount = "";
        while (cur_pos != fl_ptr)
        {
            char sym = rlb_fin.get(); // from the gap position, if gap=100, from 100, 200...
            char pre_sym;
            if (sym >> 7 == 0)
            {
                cur_pos++;
                pre_sym = sym;
                Lsymbol = sym;
                if (cur_pos == fl_ptr)
                    return sym;
            }
            else
            {
                std::string tmpsymbolcount = std::bitset<8>(sym).to_string();
                symbolcount = tmpsymbolcount.substr(1) + symbolcount;
                if (rlb_fin.peek() >> 7 == 0 || (std::stoi(symbolcount, nullptr, 2) + 3 - 1 + cur_pos) >= thesizeofbwt - 1)
                {
                    int symbolcount_int = std::stoi(symbolcount, nullptr, 2) + 3 - 1;

                    if (symbolcount_int + cur_pos >= fl_ptr) // 0 + 5 = 5, <count> start from 1, cur_pos = 0
                        return pre_sym;

                    symbolcount.clear();
                    cur_pos += symbolcount_int;
                }
            }
        }
    }

    return Lsymbol;
}

auto print_query_item(const int &first, const int &last) -> void;

int main(int argc, char *argv[])
{
    query_pattern = argv[3];
    rlb_file = argv[1];
    std::ifstream read_RLB;
    read_RLB.open(argv[1]);
    if (read_RLB.fail())
    {
        std::cerr << "Error - Failed to open input file " << argv[1] << std::endl;
        exit(-1);
    }
    // std::ofstream fout_ind_txt("index_txt");

    // Use file_size to get the size of the input file
    std::uintmax_t file_size = std::filesystem::file_size(rlb_file);
    // how to vary the gap?

    // we have C table and we know the total block we need rlb-10m, S is 0-126 = 127 is sizeof(int) bytes
    //
    // we know the length of block and number of C, when it comes to the char-->1 byte,
    // estimate the possible length of bwt file

    std::ifstream scan_rlb;
    scan_rlb.open(rlb_file);
    int sizeofbwt = 0;
    std::string tmp_readcount = "";
    // char tmp_pre_sym;
    while (scan_rlb.good())
    {

        char sym = scan_rlb.get();
        if (scan_rlb.peek())
        {
            if (sym >> 7 == 0)
            {
                // tmp_pre_sym = sym;
                sizeofbwt++;
                if(sequence_symbol.find(sym)== std::string::npos) sequence_symbol += sym;
            }
            else if (sym >> 7 == -1)
            {
                std::string tmp_tmp_readcount = std::bitset<8>(sym & 0b01111111).to_string();
                tmp_readcount = tmp_tmp_readcount.substr(1) + tmp_readcount;

                if (scan_rlb.peek() >> 7 == 0 || scan_rlb.peek() == '\377')
                {

                    // if(scan_rlb.peek() == '\377'){
                    //     tmp_readcount = tmp_readcount.substr(7);
                    // }
                    int tmp_readcount_int;
                    if (tmp_readcount != "")
                        tmp_readcount_int = std::stoi(tmp_readcount, nullptr, 2) + 3 - 1;
                    else
                        tmp_readcount_int = 0;

                    sizeofbwt += tmp_readcount_int;
                    tmp_readcount.clear();
                }
            }
        }
    }
    scan_rlb.close();

    std::sort(sequence_symbol.begin(),sequence_symbol.end(),[](char a, char b) {
            return (int(a) < int(b));
        } );

    // std::cout << sizeofbwt << std::endl;
    thesizeofbwt = sizeofbwt;
    // std::cout << file_size << std::endl;
    int num_of_block = file_size / (128 * sizeof(int)); //128 is correct result but here actually should be 127??
    if (num_of_block > 0)
        num_of_block--; // last row for storing C[]
    // std::cout << num_of_block << std::endl;
    if (num_of_block >= 1)
    {
        gap = floor(sizeofbwt / num_of_block);
    }
    else
        gap = -1;
    // std::cout << "gap: " << gap << std::endl;

    index_file = argv[2];
    std::ofstream write_idxfile(index_file, std::ios::binary | std::ios::out);

    int *tmparray = new int[127]; // for output occurrence, 0 for block location, 1 for offset ,2 for bwt index, 2-126 for ASCII
    int *C = new int[127];

    for (int i = 0; i < 127; i++)
    {
        tmparray[i] = 0;
        C[i] = 0;
    }

    char c;
    char pre_c;
    std::string symcount = "";
    int cur_pos = -1;        // bwt index
    int block_location = -1; // record block begin, only record the symbol 0-msb start
    int pre_block_location = block_location;
    int offset = 0; // the symbol position and current bwt position (for rl)
    // int bytesnum = 0;

    // compute the C table first
    // then decide the length

    // clear the C table
    // std::cout << "size of int: " << sizeof(int) << std::endl;

    while (read_RLB.good())
    {

        c = read_RLB.get();
        block_location++;
        if (read_RLB.peek())
        {
            if (c >> 7 == 0)
            {

                cur_pos++;
                // std::cout<<cur_pos<<"  ";
                pre_c = c;
                pre_block_location = block_location;
                C[(int)c] += 1;
                C_map[c] += 1;
                if (gap != -1)
                {
                    if (cur_pos % gap == 0 && cur_pos != 0)
                    {
                        tmparray[0] = block_location;
                        write_idxfile.write(reinterpret_cast<const char *>(&block_location), sizeof(int));
                        offset = 0;
                        tmparray[1] = offset;
                        write_idxfile.write(reinterpret_cast<const char *>(&offset), sizeof(int));
                        tmparray[2] = cur_pos;
                        write_idxfile.write(reinterpret_cast<const char *>(&tmparray[2]), sizeof(int));
                        // fout_ind_txt << tmparray[0] << " " << tmparray[1] << " " << tmparray[2] << " ";
                        for (int i = 3; i < 127; i++)
                        {
                            tmparray[i] = C[i];
                            // fout_ind_txt << tmparray[i] << " ";
                            write_idxfile.write(reinterpret_cast<const char *>(&tmparray[i]), sizeof(int));
                        }
                        // std::cout<<cur_pos<<"  ";
                        // fout_ind_txt << std::endl;
                    }
                }
            }
            else if (c >> 7 == -1)
            {
                std::string tmp_symcount = std::bitset<8>(c & 0b01111111).to_string();
                symcount = tmp_symcount.substr(1) + symcount;

                if (read_RLB.peek() >> 7 == 0 || read_RLB.peek() == '\377')
                {
                    int symcount_int;

                    // if(cur_pos + std::stoi(symcount, nullptr, 2)+ 3 -1 > sizeofbwt-1){ // in case we read EOF=11111111
                    //     symcount = symcount.substr(7);
                    // }

                    // if(symcount!="")
                    symcount_int = std::stoi(symcount, nullptr, 2) + 3 - 1;
                    // else symcount_int = 0;

                    if (gap != -1)
                    {
                        int nextblock = (cur_pos / gap + 1) * gap;
                        if (cur_pos + symcount_int >= nextblock)
                        {
                            for (int i = 3; i < 127; i++)
                            {
                                tmparray[i] = C[i];
                            }
                            int beforenextblock_offset = nextblock - cur_pos;
                            // std::cout<<"offset: "<<beforenextblock_offset<<std::endl;
                            tmparray[(int)pre_c] += beforenextblock_offset;

                            // write_idxfile.write(reinterpret_cast<const char*>(&pre_block_location), sizeof(int));
                            tmparray[0] = pre_block_location;
                            offset = beforenextblock_offset;
                            tmparray[1] = offset;
                            // std::cout<<pre_block_location<<" ";
                            // write_idxfile.write(reinterpret_cast<const char*>(&offset), sizeof(int));
                            // std::cout<<offset<<" ";
                            tmparray[2] = nextblock;
                            for (int j = 0; j < 127; j++)
                            {
                                // fout_ind_txt << tmparray[j] << " ";
                                write_idxfile.write(reinterpret_cast<const char *>(&tmparray[j]), sizeof(int));
                            }
                            // fout_ind_txt << std::endl;

                            int rest = symcount_int - beforenextblock_offset;
                            int numgap = rest / gap;
                            offset = beforenextblock_offset;
                            for (int j = 0; j < numgap; j++)
                            {
                                tmparray[(int)pre_c] += gap;
                                tmparray[0] = pre_block_location;
                                // write_idxfile.write(reinterpret_cast<const char*>(&pre_block_location), sizeof(int));
                                // std::cout<<pre_block_location<<" ";
                                offset += gap;
                                tmparray[1] = offset;
                                // write_idxfile.write(reinterpret_cast<const char*>(&offset), sizeof(int));
                                // std::cout<<offset<<" ";
                                tmparray[2] += gap;
                                for (int i = 0; i < 127; i++)
                                {
                                    // fout_ind_txt << tmparray[i] << " ";
                                    write_idxfile.write(reinterpret_cast<const char *>(&tmparray[i]), sizeof(int));
                                }
                                // fout_ind_txt << std::endl;
                            }
                        }
                    }
                    C_map[pre_c] += symcount_int;
                    C[(int)pre_c] += symcount_int;
                    cur_pos += symcount_int;

                    symcount.clear();
                }
            }
        }
    }

    // write_idxfile.close();
    std::uintmax_t index_file_size = std::filesystem::file_size(index_file);
    // std::cout << "index file size: " << index_file_size << std::endl;
    // for(auto& cc: C_map)
    //     std::cout<<cc.first<<"\t"<<cc.second<<std::endl;
    if (index_file_size > 0)
        C_map.clear();
    else
    {
        int tmp_cur_map, tmp_sum_map = 0;
        for (auto &c : C_map)
        {
            tmp_cur_map = c.second;
            c.second = tmp_sum_map;
            tmp_sum_map = tmp_sum_map + tmp_cur_map;
        }
        C_map[127] = tmp_sum_map;
    }

    // for(int i = 9; i < 127; i++)
    //     std::cout<<(char)i<<"\t"<<C[i]<<std::endl;

    num_leftbracket = C['['];
    // reorganize C
    int tmp_cur, tmp_sum = 0;
    for (int i = 9; i < 127; i++)
    {
        tmp_cur = C[i];
        C[i] = tmp_sum;
        tmp_sum = tmp_sum + tmp_cur;
    }
    // C[127] = tmp_sum;

    // store C into index file
    if (num_of_block > 0)
    {
        for (int i = 0; i < 127; i++)
        {
            write_idxfile.write(reinterpret_cast<const char *>(&C[i]), sizeof(int));
            // fout_ind_txt << C[i] << " ";
        }
    }
    // the end of last occ row is the start of C
    num_of_block = thesizeofbwt/gap;
    // std::cout << num_of_block << std::endl;
    if (num_of_block > 0)
    {
        posOfC_IndexFile = num_of_block * 127 * sizeof(int);
    }
    index_file_size = std::filesystem::file_size(index_file);
    // std::cout << "index file size after inserting C: " << index_file_size << std::endl;

    delete[] tmparray;
    delete[] C;

    read_RLB.close();
    write_idxfile.close();

    // backwardsearch(argv[3]);

    std::pair<int, int> FL = backwardsearch(argv[3]);
    print_query_item(FL.first, FL.second);

    // int iblock_location = 0;
    // int ioffset = 0;
    // int irank_of_c = 0;

    // //read index file to get rank
    // if(std::filesystem::file_size(index_file) > 0){

    //     std::ifstream idx_fin(index_file, std::ios::binary);
    //     int block_in_index = 1*127*sizeof(int); //0-128 is the first block. for gap==1000, if 0< fl_ptr < 1000, fl_ptr/gap = 0, fl_ptr = 1001, from 129

    //     idx_fin.seekg(block_in_index);
    //     idx_fin.read(reinterpret_cast<char*>(&iblock_location), sizeof(int));

    //     idx_fin.seekg(block_in_index + 1*sizeof(int));
    //     idx_fin.read(reinterpret_cast<char*>(&ioffset), sizeof(int));

    //     idx_fin.seekg(block_in_index + 99*sizeof(int));
    //     idx_fin.read(reinterpret_cast<char*>(&irank_of_c), sizeof(int));
    //     idx_fin.close();
    // }

    // std::cout<<"test read index file "<<iblock_location<<std::endl;
    // fout_ind_txt.close();
    return 0;
}

auto print_query_item(const int &first, const int &last) -> void
{
    std::map<int, std::string> Items;
    if (first == -2)
        return;
    if (gap != -1)
    {
        std::ifstream idx_fin(index_file, std::ios::binary);
        for (int i = first; i <= last; i++)
        {
            int ptr = i;

            char sym;
            std::string tm_item = "";
            int C_c = 0; // position of symbol in C table
            // scan the index file and find the symbol
            std::string num_str = "";
            int num;
            // std::cout<<find_symbol_L(ptr)<<std::endl;
            //check the query string is index or not
            if(((*query_pattern.begin()) == '[' || (*query_pattern.end()) == ']')&&(query_pattern[1]>=48&&query_pattern[1]<=57))
            {
                while (sym != ']')
                {
                    // find the [<num>] number and use [<num>+1] backward decode
                    // if no [<num+1>], it means it's the last, so we use C[<last letter>] backward decode
                    sym = find_symbol_L(ptr);
                    tm_item = sym + tm_item;
                    idx_fin.seekg(posOfC_IndexFile + (int)sym * sizeof(int));
                    idx_fin.read(reinterpret_cast<char *>(&C_c), sizeof(int));
                    ptr = C_c + occ(sym, ptr-1);
                }
                tm_item.clear();

                while (sym != '[')
                {
                    // get the [<num>]
                    sym = find_symbol_L(ptr);
                    num_str = sym + num_str;
                    idx_fin.seekg(posOfC_IndexFile + (int)sym * sizeof(int));
                    idx_fin.read(reinterpret_cast<char *>(&C_c), sizeof(int));
                    ptr = C_c + occ(sym, ptr-1) ;
                }
                num_str = num_str.substr(1); // substr the '['
                num = std::stoi(num_str, nullptr, 10);
                num++;
                num++;
                Items[num] ="";
                // std::cout<<num<<std::endl;
                
                num_str = '[' + std::to_string(num) + ']';
            }else{

                while (sym != ']')
                {
                    sym = find_symbol_L(ptr);
                    tm_item = sym + tm_item;
                    idx_fin.seekg(posOfC_IndexFile + (int)sym * sizeof(int));
                    idx_fin.read(reinterpret_cast<char *>(&C_c), sizeof(int));
                    ptr = C_c + occ(sym, ptr-1);
                }
                tm_item.clear();

                // std::string num_str;
                while (sym != '[')
                {
                    // get the [<num>]
                    sym = find_symbol_L(ptr);
                    num_str = sym + num_str;
                    idx_fin.seekg(posOfC_IndexFile + (int)sym * sizeof(int));
                    idx_fin.read(reinterpret_cast<char *>(&C_c), sizeof(int));
                    ptr = C_c + occ(sym, ptr-1) ;
                }
                num_str = num_str.substr(1); // substr the '['
                num = std::stoi(num_str, nullptr, 10);
                num++;
                Items[num] = "";
                // num_str = '[' + std::to_string(num) + ']';
                // std::cout<<"num_str: "<<num_str<<std::endl;
            }
        }
            for(auto& It: Items){
                int C_c = 0;
                int num = It.first;
                // num++;
                std::string num_str = '[' + std::to_string(num) + ']';
                std::pair<int, int> pfl = backwardsearch( num_str );
                num_str.clear();
                char sym = 'u';
                std::string item = "";
                if (pfl.first == -2)
                { // means last item

                    // the number of [ decide how many consective [<index>]
                    num -= num_leftbracket; // e.g in small1.txt, 1-7 index => 7*[, num = 8 now, num-num_leftbracket => first index
                    num_str = '[' + std::to_string(num) + ']';
                    pfl = backwardsearch(num_str);
                    num_str.clear();
                    int ptr = pfl.first;
                    while (sym != '[')
                    {
                        sym = find_symbol_L(ptr);
                        item = sym + item;
                        idx_fin.seekg(posOfC_IndexFile + (int)sym * sizeof(int));
                        idx_fin.read(reinterpret_cast<char *>(&C_c), sizeof(int));
                        ptr = C_c + occ(sym, ptr-1);
                    }

                    Items[(num+num_leftbracket)] = item;  
                    item.clear();
                }
                else
                {
                    int ptr = pfl.first;
                    while (sym != '[')
                    {
                        sym = find_symbol_L(ptr);//find the last column symbol in this position 0-based
                        item = sym + item;
                        idx_fin.seekg(posOfC_IndexFile + (int)sym * sizeof(int));
                        idx_fin.read(reinterpret_cast<char *>(&C_c), sizeof(int));
                        ptr = C_c + occ(sym, ptr-1);//up to previous position, how many sym we have 
                                                    //miuns 1 'cause 0-based
                    }
                    
                    Items[num] = item;
                    item.clear();
                }
            }
    }
    else //no index file decode
    {
        for (int i = first; i <= last; i++)
        {

            int ptr = i;

            char sym;
            std::string tm_item;
            // scan the index file and find the symbol
            while (sym != ']')
            {
                // find the [<num>] number and use [<num>+1] backward decode
                // if no [<num+1>], it means it's the last, use last + 1 - num of items to get the first index
                sym = find_symbol_L(ptr);
                tm_item = sym + tm_item;
                ptr = C_map[sym] + occ(sym, ptr) - 1;
            }

            std::string num_str;
            while (sym != '[')
            {
                // get the [<num>]
                sym = find_symbol_L(ptr);
                num_str = sym + num_str;
                ptr = C_map[sym] + occ(sym, ptr) - 1;
            }
            num_str = num_str.substr(1);
            int num = std::stoi(num_str, nullptr, 10);
            num++;
            Items[num] = "";
        }

            for(auto& It: Items){
                int num = It.first;
                std::string num_str = '[' + std::to_string(num) + ']';
                std::pair<int, int> pfl = backwardsearch(num_str);

                char sym = 'u';
                std::string item = "";
                if (pfl.first == -2)
                { // means last item

                    // the number of [ decide how many consective [<index>]
                    num -= num_leftbracket; // e.g in small1.txt, 1-7 index => 7*[, num = 8 now, num-num_leftbracket => first index
                    num_str = '[' + std::to_string(num) + ']';
                    pfl = backwardsearch(num_str);
                    int ptr = pfl.first;
                    while (sym != '[')
                    {
                        sym = find_symbol_L(ptr);
                        item = sym + item;
                        ptr = C_map[sym] + occ(sym, ptr) - 1;
                    }

                    Items[num+num_leftbracket] = item;
                    item.clear();
                }
                else
                {
                    int ptr = pfl.first;
                    while (sym != '[')
                    {
                        sym = find_symbol_L(ptr);
                        item = sym + item;
                        ptr = C_map[sym] + occ(sym, ptr) - 1;
                    }

                    Items[num] = item;

                    item.clear();
                }
            }
    }

    for (auto &item : Items)
    {
        std::cout << item.second << std::endl;
    }

    //store the result items index in array, and do decode together to get the correct result


}
