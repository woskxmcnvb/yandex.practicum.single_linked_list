#pragma once
#include <iostream>

using namespace std::string_literals; 

template <typename Type>
class SingleLinkedList {
    // Узел списка
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next) : value(val), next_node(next) {}

        Type value;
        Node* next_node = nullptr;
    };


    // Шаблон класса «Базовый Итератор».
    // Определяет поведение итератора на элементы односвязного списка
    // ValueType — совпадает с Type (для Iterator) либо с const Type (для ConstIterator)
    template <typename ValueType>
    class BasicIterator {
        // Класс списка объявляется дружественным, чтобы из методов списка был доступ к приватной области итератора
        friend class SingleLinkedList;

        // Конвертирующий конструктор из указателя на узел списка
        explicit BasicIterator(Node* node) : node_(node) {}

    public:
        // Объявленные ниже типы сообщают стандартной библиотеке о свойствах этого итератора
        // Категория итератора — forward iterator (итератор, который поддерживает операции инкремента и многократное разыменование)
        using iterator_category = std::forward_iterator_tag;
        // Тип элементов, по которым перемещается итератор
        using value_type = Type;
        // Тип, используемый для хранения смещения между итераторами
        using difference_type = std::ptrdiff_t;
        // Тип указателя на итерируемое значение
        using pointer = ValueType*;
        // Тип ссылки на итерируемое значение
        using reference = ValueType&;

        BasicIterator() = default;

        // Конвертирующий конструктор/конструктор копирования
        // При ValueType, совпадающем с Type, играет роль копирующего конструктора
        // При ValueType, совпадающем с const Type, играет роль конвертирующего конструктора
        BasicIterator(const BasicIterator<Type>& other) noexcept { node_ = other.node_; }

        // Операторы ++ * -> для несуществующих элементов приводят к неопределенному поведению 

        BasicIterator& operator=(const BasicIterator& rhs) = default;

        // Оператор сравнения итераторов (в роли второго аргумента *константный* итератор)
        // Два итератора равны, если они ссылаются на один и тот же элемент списка либо на end()
        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return (node_ == rhs.node_); 
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return !(*this == rhs); 
        }

        // Оператор сравнения итераторов (в роли второго аргумента итератор)
        // Два итератора равны, если они ссылаются на один и тот же элемент списка либо на end()
        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return (node_ == rhs.node_); 
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return !(*this == rhs); 
        }

        // Оператор прединкремента. После его вызова итератор указывает на следующий элемент списка
        // Возвращает ссылку на самого себя
        BasicIterator& operator++() noexcept {
            node_ = node_->next_node; 
            return *this; 
        }

        // Оператор постинкремента. После его вызова итератор указывает на следующий элемент списка
        // Возвращает прежнее значение итератора
        BasicIterator operator++(int) noexcept {
            BasicIterator temp_it (node_->next_node); 
            std::swap(*this, temp_it); 
            return temp_it; 
        }

        [[nodiscard]] reference operator*() const noexcept {
            return node_->value; 
        }

        [[nodiscard]] pointer operator->() const noexcept {
            return &node_->value; 
        }

    private:
        Node* node_ = nullptr;
    };

public:
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    // Итератор, допускающий изменение элементов списка
    using Iterator = BasicIterator<Type>;
    // Константный итератор, предоставляющий доступ для чтения к элементам списка
    using ConstIterator = BasicIterator<const Type>;

    // Нельзя разыменовывать .end() и .before_begin() - неопределенное поведение
    // Если список пустой, begin() == end()

    // Возвращает итератор, ссылающийся на первый элемент
    [[nodiscard]] Iterator begin() noexcept {
        return Iterator(head_->next_node); 
    }
    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return Iterator(head_->next_node);
    }
    // эквивалентен cbegin()
    [[nodiscard]] ConstIterator begin() const noexcept {
        return Iterator(head_->next_node);
    }

    // Возвращает итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    [[nodiscard]] Iterator end() noexcept {
        return Iterator(nullptr); 
    }
    [[nodiscard]] ConstIterator cend() const noexcept {
        return Iterator(nullptr); 
    }
    // эквивалентен cend()
    [[nodiscard]] ConstIterator end() const noexcept {
        return Iterator(nullptr); 
    }


    // Возвращает итератор, указывающий на позицию перед первым элементом односвязного списка.
    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator(head_);
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return Iterator(head_);
    }
    
    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return Iterator(head_);
    }



public:
    SingleLinkedList() : head_(new Node()) {}

    SingleLinkedList(std::initializer_list<Type> values) : head_(new Node()) {
        FillWithValues(values.begin(), values.end());
    }

    SingleLinkedList(const SingleLinkedList& other) : head_(new Node()) {
        FillWithValues(other.begin(), other.end());
    }
    
    ~SingleLinkedList() {
        Clear();
        delete head_; 
    }

    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    void PushFront(const Type& value) {
        head_->next_node = new Node(value, head_->next_node); 
        ++size_; 
    }

    void Clear() noexcept {
        while (head_->next_node) {
            Node* tmp = head_->next_node;
            head_->next_node = head_->next_node->next_node; 
            delete tmp;      
        }
        size_ = 0;
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if (this != &rhs) {
            SingleLinkedList temp(rhs); 
            swap(temp); 
        }
        return *this; 
    }

    // Обменивает содержимое списков за O(1)
    void swap(SingleLinkedList& other) noexcept {
        std::swap(head_->next_node, other.head_->next_node); 
        std::swap(size_, other.size_); 
    }


    // Возвращает итератор на вставленный элемент
    // Если при создании элемента будет выброшено исключение, список останется в прежнем состоянии
    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        pos.node_->next_node = new Node(value, pos.node_->next_node);  
        return Iterator(pos.node_->next_node); 
    }

    void PopFront() noexcept {
        EraseAfter(before_begin()); 
    }

    //Возвращает итератор на элемент, следующий за удалённым
    Iterator EraseAfter(ConstIterator pos) noexcept {
        if (pos != end()) {
            Node * to_drop = pos.node_->next_node; 
            pos.node_->next_node = to_drop->next_node; 
            delete to_drop; 
            --size_;
        }
        return Iterator(pos.node_->next_node); 
    }

private:

    // темплейтный филлер по итератору - для списка инициализации и для конструктора копирования
    template <typename SourceIterator>
    void FillWithValues(SourceIterator begin_, SourceIterator end_) {
        // пытаемся построить временный список, в процессе все может сломаться
        try {
            SingleLinkedList temp;
            Node* last_node = temp.head_; 
            for (auto it = begin_; it != end_; ++it) {
                last_node->next_node = new Node(*it, nullptr); 
                last_node = last_node->next_node;  
                ++temp.size_; 
            } 
            
            // если ничего не сломалось, записываем его в основной
            swap(temp); 
        } catch (...) {
            throw;
        }   
    }

    // Фиктивный узел, используется для вставки "перед первым элементом"
    Node *head_;
    size_t size_ = 0;
};


template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    // сравниваем размеры
    if (lhs.GetSize() != rhs.GetSize()) {
        return false;
    } 
    
    //сравниваем поэлементно
    auto it_lhs = lhs.begin();
    auto it_rhs = rhs.begin(); 
    while(it_lhs != lhs.end()) {
        if (*it_lhs != *it_rhs) {
            return false; 
        }
        ++it_lhs; 
        ++it_rhs; 
    }

    return true;
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.cbegin(), lhs.cend(), 
                                        rhs.cbegin(), rhs.cend());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return (lhs < rhs) || (lhs == rhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs <= rhs);
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs < rhs);
} 

