#include <iostream>
#include <cstddef>
#include <iterator>
#include "polyvector.h"
#include "doctest.h"


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

    PolyVector<int> lol{1,2,3,4,5};

    cout << "elements: ";
    for (size_t i = 0; i < lol.size(); i++) {
        cout << lol[i] << ", ";
    }
    cout << endl;


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
    cout << "range initial capacity: " << vec.capacity() << std::endl;
    cout << "elements range: ";
    for (auto item : vec) {
        cout << item << ", ";
    }
    cout << endl << "final capacity: " << vec.capacity() << std::endl;
    cout << endl;

    cout << "insert initial capacity: " << vec.capacity() << std::endl;
    vec.insert(++vec.begin(), 2);
    for (auto item : vec) {
        cout << item << ", ";
    }
    cout << endl << "final capacity: " << vec.capacity() << std::endl;

    cout << "insert initial capacity: " << vec.capacity() << std::endl;
    vec.insert(++vec.begin(), 7);
    vec.insert(++vec.begin(), 7);
    for (auto item : vec) {
        cout << item << ", ";
    }
    cout << endl << "final capacity: " << vec.capacity() << std::endl;

    vec.erase_value(7);
    for (auto item : vec) {
        cout << item << ", ";
    }
    cout << endl << "final capacity: " << vec.capacity() << std::endl;

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
