#include <iostream>
#include <cstddef>
#include "vector.h"

using std::cout, std::endl;

int main() {
    int a[2] {3,4};
    int *p = static_cast<int*>(a);
    cout << "dereference check: " << p[1] << endl;
    cout << "start of program" << endl;
    PolyVector<int> vec;
    cout << "initialized vec" << endl;
    vec.push_back(1);
    cout << "first element: " << vec[0] << endl;
    vec.push_back(3);
    cout << "size: " << vec.size() << endl;

    cout << "elements: ";
    for (size_t i = 0; i < vec.size(); i++) {
        cout << i << ", ";
    }

    cout << endl;
}
