#include <iostream>
#include <cstddef>
#include <iterator>
#include "vector.h"

using std::cout, std::endl;



class Base {
public:
    virtual void print() {
        cout << "Base" << endl;
    }
};

class Derived : public Base {
public:
    virtual void print() {
        cout << "Derived" << endl;
    }
};


int main() {
    static_assert(emplaceable_from<Derived, Base>);
    static_assert(std::forward_iterator<PolyVector<int>::iterator>);



    int a[2] {3,4};
    int *p = static_cast<int*>(a);
    cout << "dereference check: " << p[1] << endl;
    cout << "start of program" << endl;
    PolyVector<long> vec;
    cout << "initialized vec" << endl;
    vec.push_back(1);
    cout << "first element: " << vec[0] << endl;
    vec.push_back(3);
    cout << "size: " << vec.size() << endl;

    cout << "elements: ";
    for (size_t i = 0; i < vec.size(); i++) {
        cout << vec[i] << ", ";
    }
    cout << endl;


    cout << "elements iterator: ";
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        cout << *it << ", ";
    }
    cout << endl;
    cout << "elements range: ";
    for (auto item : vec) {
        cout << item << ", ";
    }
    cout << endl;


    PolyVector<Base> polyvec;
    polyvec.emplace_back<Base>();
    polyvec.emplace_back<Derived>();

    for (size_t i = 0; i < polyvec.size(); i++) {
        polyvec[i].print();
    }

    polyvec.reserve(16);
    for (size_t i = 0; i < polyvec.size(); i++) {
        polyvec[i].print();
    }



}
