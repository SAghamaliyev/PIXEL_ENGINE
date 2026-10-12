#include <iostream>

#include "Engine.h"

int main() {
    Engine engine;
    
    if (!engine.initialize()) {
        std::cout << "Something went wrong";
        return -1;
    }

    engine.run();

    engine.terminate();
    return 0;
}