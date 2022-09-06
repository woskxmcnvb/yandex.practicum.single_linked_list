#include <cassert>
#include <cstddef>
#include <string>
#include <utility>

#include "single_linked_list.h"

#include "yandex_tests.h"
#include "my_tests.h"



int main() {
    YTest_GetSize_IsEmpty();
    YTest_PushFront_Clear(); 
    YTest_Iterator(); 

    MyTest_Swap(); 

    MyTest_Initializer_List(); 
    MyTest_Copy_constructor(); 

    MyTest_PopFront();
    MyTest_EraseAfter();

    Test3();
    Test4(); 

}