#include <cassert>
#include <string>
#include <iostream>

#include "single_linked_list.h"

template <typename List>
void PrintList(const List& list_) {
    for (auto it = list_.begin(); it != list_.end(); ++it) {
        std::cout << *it << " ";  
    }
    std::cout << std::endl;
}

void Test_Swap() {
    using namespace std::string_literals; 
    SingleLinkedList<std::string> one; 
    one.PushFront("ku"s); 
    one.PushFront("ku"s);
    one.PushFront("ru"s);  

    SingleLinkedList<std::string> two; 
    two.PushFront("one"s); 
    two.PushFront("two"s); 

    SingleLinkedList<std::string> empty; 

    assert(empty.GetSize() == 0); 
    assert(two.GetSize() == 2);
    empty.swap(two); 
    
    assert(empty.GetSize() == 2); 
    assert(two.GetSize() == 0); 

    one.swap(empty); 
    assert(empty.GetSize() == 3); 
    assert(one.GetSize() == 2); 

    std::cout << "Swap is OK" << std::endl;

}


void Test_Initializer_List() {
    using namespace std::string_literals; 
    /**
    {
        SingleLinkedList<std::string> one {"ku"s, "ku"s, "ru"s, "ku"s}; 
        SingleLinkedList<int> two {1, 2, 3, 4}; 
        SingleLinkedList<std::string> thr {};  
        assert(one.GetSize() == 4); 
        assert(thr.GetSize() == 0); 
        
        PrintList(one); 
        PrintList(two); 
    }
    */

    struct ScreamsCreatedDeleted {
        ScreamsCreatedDeleted() 
        {std::cout << "+++ "s << name_  << std::endl;}; 

        ScreamsCreatedDeleted(std::string name) : name_(name) 
        {
            std::cout << "+++ "s << name_  << std::endl;
        }

        ScreamsCreatedDeleted(const ScreamsCreatedDeleted& other) : name_(other.name_) 
        {
            std::cout << "=== "s << name_  << std::endl;
            if (name_ == "crash"s) { throw std::logic_error("privet"s); }
        }
        
        ~ScreamsCreatedDeleted() 
        {std::cout << "--- "s  << name_ << std::endl; }
        
        std::string name_ = "no *** name"s;
    }; 

    { 
        SingleLinkedList<ScreamsCreatedDeleted> spys_2 {ScreamsCreatedDeleted("First"), ScreamsCreatedDeleted("Second")}; 
        std::cout << "A list shoul be alive"s << std::endl;
        assert(spys_2.begin()->name_ == "First"); 
    }

    try {
        SingleLinkedList<ScreamsCreatedDeleted> spys_2 {ScreamsCreatedDeleted("First"), ScreamsCreatedDeleted("Second"), ScreamsCreatedDeleted("crash")}; 
        assert(spys_2.GetSize() == 0); 
    } catch (const std::logic_error & e) {
        std::cout << "Поймали ошибку. Должно сломаться" << std::endl;
    }

    std::cout << "Initializer List is OK" << std::endl;
    
    try {
        SingleLinkedList<ScreamsCreatedDeleted> spys_2 {ScreamsCreatedDeleted("First"s), ScreamsCreatedDeleted("Second"), ScreamsCreatedDeleted("Third")}; 
        assert(spys_2.GetSize() == 3); 
        assert(spys_2.begin()->name_ == "First"s);
    } catch (const std::logic_error & e) {
        std::cout << "Не должно ломаться" << std::endl;
    }

    std::cout << "Initializer list is OK-------------" << std::endl;
}

void MyTest_Copy_constructor() {
    using namespace std::string_literals; 

    {
        SingleLinkedList<std::string> one {"ku"s, "ku"s, "ru"s, "ku"s}; 
        SingleLinkedList<std::string> two (one); 
        
        assert(two.GetSize() == 4); 
        assert(*two.begin() == "ku"s); 
        assert(two.begin() != one.begin()); 

        PrintList(one); 
        PrintList(two); 
    }

    std::cout << "Copy is OK-------------" << std::endl;

}


void MyTest_PopFront() {
    using namespace std::string_literals; 
    SingleLinkedList<std::string> one {"ku"s, "ku"s, "ru"s, "ku"s}; 

    assert(one.GetSize() == 4); 
    one.PopFront(); 
    assert(one.GetSize() == 3); 
    one.PopFront(); 
    assert(one.GetSize() == 2); 
    assert(*one.begin() == "ru"s);



    struct ScreamsDeletion {
        ScreamsDeletion() = default; 
        ScreamsDeletion(std::string name) : name_(name) {}       
        ~ScreamsDeletion() {std::cout << "--- "s  << name_ << std::endl; }
        std::string name_ = "no *** name"s;
    }; 

    SingleLinkedList<ScreamsDeletion> two; 
    two.PushFront(ScreamsDeletion("First"s));
    two.PushFront(ScreamsDeletion("Second"s));
    two.PushFront(ScreamsDeletion("Third"s));
    two.PushFront(ScreamsDeletion("Fourth"s));

    two.PopFront(); 

    std::cout << "CHECK: /Fourth/ should've just been deleted"s << std::endl; 
}

