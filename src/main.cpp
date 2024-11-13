#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <map>
#include <chrono>
#include <vector>

using namespace std::chrono;

class Dice{
public:
    inline Dice(uint8_t _die)
    : _dies_number(_die){}
    inline ~Dice(){}

    inline uint8_t get_dies_number(){
        return _dies_number;
    }
    inline uint8_t roll(){
        double rd = ((double)rand()) / ((double)RAND_MAX);
        int ret = 1 + rd * _dies_number;
        if(ret >= _dies_number + 1){
            // Avoid Extreme Value
            ret = _dies_number;
        }
        return (uint8_t)ret;
    }
private:
    inline static bool _is_rand_init = false;
    inline void _init(){
        if(!_is_rand_init){
            milliseconds ms = duration_cast< milliseconds >(
                system_clock::now().time_since_epoch()
            );
            srand(ms.count());
            _is_rand_init = true;
        }
    }

    uint8_t _dies_number = 0;
};

class DashBoard{
public:
    DashBoard(){}
    ~DashBoard(){}

    inline void mark_tally(int _col){
        if(_is_col_exists(_col)){
            _add_mark_to_col(_col);
        }else{
            _create_col(_col);
            _add_mark_to_col(_col);
        }
    }
    inline void print(){
        std::cout << "Total Point Result : " << std::endl;
        for(auto& it : count_ret){
            std::cout << it.first << " : " << it.second << " (" << (double)it.second / (double)100000 * 100 << "%)" << std::endl;
        }
    }
    inline void try_create_col(int _col){
        if(!_is_col_exists(_col)){
            _create_col(_col);
        }
    }
private:
    std::map<int, int> count_ret;
    inline bool _is_col_exists(int _col){
        return count_ret.contains(_col);
    }
    inline void _add_mark_to_col(int _col){
        count_ret[_col]++;
    }
    inline void _create_col(int _col){
        count_ret.emplace(_col,0);
    }
};


int main(int argc, char *argv[]) {
    // Init the dice number and dice dies
    int dice_number = 1;
    int dice_die_number = 6;
    if(argc > 1){
        dice_number = std::atol(argv[1]);
    }
    if(argc > 2){
        dice_die_number = std::atol(argv[2]);
    }

    // Create Dice(s) for Rolling
    std::vector<Dice*> dices;
    for(size_t i = 0 ; i < dice_number ; i++){
        dices.push_back(new Dice(dice_die_number));
    }

    // Create Dash Board for Remark Result
    DashBoard dash_board;
    for(size_t i = 1 * dice_die_number ; i <= dice_number * dice_die_number ; i++){
        dash_board.try_create_col(i);
    }

    // Start generate result
    for(size_t i = 0; i < 100000 ; i++){
        int ret = 0;
        for(auto& dice : dices){
            ret += dice->roll();
        }
        dash_board.mark_tally(ret);
    }

    // Display result
    //system("cls");
    std::cout << std::endl;
    std::cout << "Dice(s) Number : " << dice_number << std::endl;
    dash_board.print();

    return 0;
}