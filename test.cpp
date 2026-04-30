#include <iostream>
#include <ostream>

#include "smvector.hpp"

int main() {
    smv::smvector<int> a;
    for (int i = 10; i < 20; i++) {
        a.push_front(i);
    }
    for (const auto& i : a) {
        std::cout << i << ",";
    }
}