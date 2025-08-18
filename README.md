# PolyVector
A polymorphic vector for c++.
This container allows storage of base and derived classes in-place as long as they are of the same size.

# Purpose
While std::vector is great, it does not play well with derived classes due to object slicing. Because we can only insert items via copy or emplacing the base object, there is no way to get a derived object directly into the vector.
While we can avoid this by using pointers to the base class, we pay the penalty of pointer indirection. PolyVector seeks to remedy this.

# Example usage 
```c++
#include "polyvector.h"
#include <iostream>

class Base {
public:
    virtual void print_type() {
        std::cout << "Base" << std::endl;
    }
};

class Derived : public Base {
public:
    virtual void print_type() {
        std::cout << "Derived" << std::endl;
    }
};

int main() {
    PolyVector<Base> polyvec;
    polyvec.emplace_back<Base>();
    polyvec.emplace_back<Derived>();
    polyvec[0].print_type();
    polyvec[1].print_type();
    return 0;
}
```
Output:
```
Base
Derived
```

# Template Parameters
|||
| --- | --- |
| Base | Base type to insert into vector |
| Allocator | same as std::vector |
| Derived (emplace_back) | Derived type of Base with the same size as Base |

# Member Types
|||
| --- | --- |
| value_type | Base |
| allocator_type | Allocator |
| size_type | size_t |
| reference | Base& |
| pointer | Base* |

# Member functions
||
| --- |
| PolyVector |
| ~PolyVector |

Copy/move assignment/constructor are deleted

# Element access
||
| --- |
| operator[] |
| front |
| back |
| data |

# Iterators
||
| --- |
| begin |
| end |

# Capacity
||
| --- |
| size |
| reserve |
| capacity |

# Modifiers
||
| --- |
| clear |
| insert | 
| erase_value |
| push_back |
| emplace_back |
| pop_back |

