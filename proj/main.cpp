#include "MainGame.hh"

#include <random>
#include <time.h>
#include <fstream>
#include <iostream>
#include <filesystem>

using namespace std;

int main(int argc, char** argv) {
    if (argc > 1) {
        MainGame mg(argv[1], 1);
        std::filesystem::create_directories("sim/");
        if (argc > 2){
            mg.Play("sim/", jsonReading, std::atoi(argv[2]));
        }
        mg.Play("sim/", jsonReading, 10);
    }
    else{
        MainGame mg("test-board.json", 1);
        std::filesystem::create_directories("sim/");
        mg.Play("sim/", jsonReading, 10);
    }
    return 0;
}