#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <iterator>
#include "polyvector.h"

enum Type {BaseT, DerivedT};

class Base {
public:
    virtual Type get_type() {
        return BaseT;
    }
};

class Derived : public Base {
public:
    virtual Type get_type() {
        return DerivedT;
    }
};

TEST_CASE("Concepts") {
    CHECK(same_size<Derived, Base>);
    CHECK(!same_size<char, long long int>);

    CHECK(emplaceable_from<Base, Base>);
    CHECK(emplaceable_from<Derived, Base>);
    CHECK(emplaceable_from<Derived, Derived>);
    CHECK(!emplaceable_from<Base, Derived>);

    CHECK(std::forward_iterator<PolyVector<int>::iterator>);
}

TEST_CASE("Initializer list") {
    PolyVector<int> vec{1, 2, 3, 4};

    CHECK(vec[0] == 1);
    CHECK(vec[1] == 2);
    CHECK(vec[2] == 3);
    CHECK(vec[3] == 4);
}

TEST_CASE("Two element Accessors") {
    PolyVector<int> vec{1, 2};

    CHECK(vec[0] == 1);
    CHECK(vec[1] == 2);
    CHECK(vec.front() == 1);
    CHECK(vec.back() == 2);
    CHECK(*vec.data() == 1);
    CHECK(*(vec.data() + 1) == 2);
}

TEST_SUITE_BEGIN("Iterators");
TEST_CASE("Iterator for") {
    PolyVector<int> vec{1, 2, 3, 4};
    
    int i = 0;
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        CHECK(*it == vec[i]);
        ++i;
    }
}

TEST_CASE("Range for") {
    PolyVector<int> vec{1, 2, 3, 4};
    
    int i = 0;
    for (auto item : vec) {
        CHECK(item == vec[i]);
        ++i;
    }
}
TEST_SUITE_END();

TEST_SUITE_BEGIN("Capacity");
TEST_CASE("initial size 0") {
    PolyVector<int> vec;

    CHECK(vec.size() == 0);
    CHECK(vec.capacity() == 0);
}

TEST_CASE("initial size 2") {
    PolyVector<int> vec{1, 2};

    CHECK(vec.size() == 2);
    CHECK(vec.capacity() == 2);
}

TEST_CASE("Reserve no elements") {
    PolyVector<int> vec;
    vec.reserve(1);

    CHECK(vec.size() == 0);
    CHECK(vec.capacity() == 1);
}

TEST_CASE("Reserve with elements") {
    PolyVector<int> vec{1, 2};
    vec.reserve(10);

    CHECK(vec.size() == 2);
    CHECK(vec.capacity() == 10);

    CHECK(vec[0] == 1);
    CHECK(vec[1] == 2);
}

TEST_CASE("Increase size only") {
    PolyVector<int> vec{1, 2, 3};
    CHECK(vec.size() == 3);
    CHECK(vec.capacity() == 4);

    vec.push_back(1);

    CHECK(vec.size() == 4);
    CHECK(vec.capacity() == 4);
}

TEST_CASE("Increase size and capacity empty") {
    PolyVector<int> vec;
    vec.push_back(1);

    CHECK(vec.size() == 1);
    CHECK(vec.capacity() == 1);
}
TEST_CASE("Increase size and capacity non-empty") {
    PolyVector<int> vec{1, 2, 3, 4};
    vec.push_back(1);

    CHECK(vec.size() == 5);
    CHECK(vec.capacity() == 8);
}

TEST_CASE("Reserve less than current") {
    PolyVector<int> vec{1, 2};
    vec.reserve(1);

    CHECK(vec.size() == 2);
    CHECK(vec.capacity() == 2);
}

TEST_SUITE_BEGIN("Modifiers");
TEST_CASE("clear") {
    PolyVector<int> vec {1,2};

    CHECK(vec.size() == 2);
    CHECK(vec.capacity() == 2);

    vec.clear();

    CHECK(vec.size() == 0);
    CHECK(vec.capacity() == 2);
}

TEST_CASE("insert") {
    PolyVector<int> vec{1, 2, 3};
    vec.insert(++vec.begin(), 5);
    
    CHECK(vec[0] == 1);
    CHECK(vec[1] == 5);
    CHECK(vec[2] == 2);
    CHECK(vec[3] == 3);
}

TEST_CASE("erase_value") {
    PolyVector vec {0, 0, 1, 0, 2, 0, 3, 0};
    vec.erase_value(0);

    CHECK(vec[0] == 1);
    CHECK(vec[1] == 2);
    CHECK(vec[2] == 3);
}

TEST_CASE("push_back") {
    PolyVector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    CHECK(vec[0] == 1);
    CHECK(vec[1] == 2);
    CHECK(vec[2] == 3);
}

TEST_CASE("pop_back") {
    PolyVector<int> vec{1, 2, 3};
    vec.pop_back();
    
    CHECK(vec.size() == 2);
    CHECK(vec[0] == 1);
    CHECK(vec[1] == 2);
}
TEST_SUITE_END();
/**


    static_assert(emplaceable_from<Derived, Base>);
    static_assert(std::forward_iterator<PolyVector<int>::iterator>);
**/
