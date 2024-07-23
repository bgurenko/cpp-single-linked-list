#pragma once
#include <cassert>
#include <cstddef>
#include <string>
#include <utility>
#include <algorithm>

template <typename Type>
class SingleLinkedList {
    // Узел списка
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };

public:
    SingleLinkedList(){
        size_ = 0;
    }

    ~SingleLinkedList(){
        Clear();
    }

    template<typename InputIt> 
    explicit SingleLinkedList(InputIt begin, InputIt end, size_t size){
        if (size != 0){
                        
            SingleLinkedList tmp;
            tmp.size_ = size;
            Node *p = &tmp.head_;
            
            for (auto it = begin; it != end; ++it){
                p->next_node = new Node(*it,nullptr);
                p = p->next_node;
            }
            swap(tmp);
        }
    }

    SingleLinkedList(std::initializer_list<Type> values):SingleLinkedList(values.begin(), values.end(), values.size()){
       
        // SingleLinkedList(values.begin(), values.end(), values.size());
    }

    // SingleLinkedList(std::initializer_list<Type> values){
        
    //     if (values.size() != 0){
                        
    //         SingleLinkedList tmp;
    //         tmp.size_ = values.size();
    //         Node *p = &tmp.head_;
            
    //         for (auto it = values.begin(); it != values.end(); ++it){
    //             p->next_node = new Node(*it,nullptr);
    //             p = p->next_node;
    //         }
    //         swap(tmp);
    //     }

    // }

    // SingleLinkedList(const SingleLinkedList& other) {

    //     size_ = other.GetSize();
    //     if (other.GetSize() != 0 && other.head_.next_node != nullptr){
    //         try{
    //             SingleLinkedList tmp;
    //             tmp.size_ = other.size_;
    //             Node *p = &tmp.head_;
                
    //             for (auto it = other.begin(); it != other.end(); ++it){
    //                 p->next_node = new Node(*it,nullptr);
    //                 p = p->next_node;
    //             }
    //             swap(tmp);
    //         } catch (...){
    //             Clear();
    //             throw ;
    //         }
    //     }
    // }

    SingleLinkedList(const SingleLinkedList& other): SingleLinkedList(other.cbegin(), other.cend(), other.GetSize()){}


    SingleLinkedList& operator=(const SingleLinkedList& rhs) {

        if (this != &rhs){
            SingleLinkedList tmp(rhs);
            swap(tmp);
        }
        return *this;
    }

    // Обменивает содержимое списков за время O(1)
    void swap(SingleLinkedList& other) noexcept {
        // обменяем размеры
        std::swap (other.size_, size_);
        // size_t tmp_size = other.size_;
        // other.size_ = this->size_;
        // this->size_ = tmp_size;

        // обменяем указатели
        auto tmp_pointer = other.head_.next_node;
        other.head_.next_node = this->head_.next_node;
        this->head_.next_node = tmp_pointer;
    }

    // Возвращает количество элементов в списке за время O(1)
    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    // Сообщает, пустой ли список за время O(1)
    [[nodiscard]] bool IsEmpty() const noexcept {
        return size_ == 0;
    }

     // Вставляет элемент value в начало списка за время O(1)
    void PushFront(const Type& value) {

        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }

    // Очищает список за время O(N)
    void Clear() noexcept {
        
        while (head_.next_node) {
            auto tmp = head_.next_node->next_node;
            delete head_.next_node;
            head_.next_node = tmp;
        }
        size_ = 0;
    
    }
    
    // Шаблон класса «Базовый Итератор».
    // Определяет поведение итератора на элементы односвязного списка
    // ValueType — совпадает с Type (для Iterator) либо с const Type (для ConstIterator)
    template <typename ValueType>
    class BasicIterator {
        // Класс списка объявляется дружественным, чтобы из методов списка
        // был доступ к приватной области итератора
        friend class SingleLinkedList;

        // Конвертирующий конструктор итератора из указателя на узел списка
        explicit BasicIterator(Node* node)
        :node_(node)
        {}

    public:
        // Объявленные ниже типы сообщают стандартной библиотеке о свойствах этого итератора

        // Категория итератора — forward iterator
        // (итератор, который поддерживает операции инкремента и многократное разыменование)
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
        BasicIterator(const BasicIterator<Type>& other) noexcept : BasicIterator(other.node_)
        {}
        // Чтобы компилятор не выдавал предупреждение об отсутствии оператора = при наличии
        // пользовательского конструктора копирования, явно объявим оператор = и
        // попросим компилятор сгенерировать его за нас
        BasicIterator& operator=(const BasicIterator& rhs) = default;

        // Оператор сравнения итераторов (в роли второго аргумента выступает константный итератор)
        // Два итератора равны, если они ссылаются на один и тот же элемент списка либо на end()
        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            
            return node_ == rhs.node_;
        }

        // Оператор проверки итераторов на неравенство
        // Противоположен !=
        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            
            return !(node_ == rhs.node_);
        }

        // Оператор сравнения итераторов (в роли второго аргумента итератор)
        // Два итератора равны, если они ссылаются на один и тот же элемент списка либо на end()
        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            
            return node_ == rhs.node_;
        }

        // Оператор проверки итераторов на неравенство
        // Противоположен !=
        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            
            return !(node_ == rhs.node_);
        }

        // Оператор прединкремента. После его вызова итератор указывает на следующий элемент списка
        // Возвращает ссылку на самого себя
        // Инкремент итератора, не указывающего на существующий элемент списка, приводит к неопределённому поведению
        BasicIterator& operator++() noexcept {
            
            assert(node_ != nullptr);
            node_ = node_->next_node;
            return *this;
        }

        // Оператор постинкремента. После его вызова итератор указывает на следующий элемент списка
        // Возвращает прежнее значение итератора
        // Инкремент итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        BasicIterator operator++(int) noexcept {
            
            auto old_value(*this);
            ++(*this);
            return old_value;
        }

        // Операция разыменования. Возвращает ссылку на текущий элемент
        // Вызов этого оператора у итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        [[nodiscard]] reference operator*() const noexcept {
            
            assert (node_ != nullptr);
            return node_->value;
        }

        // Операция доступа к члену класса. Возвращает указатель на текущий элемент списка
        // Вызов этого оператора у итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        [[nodiscard]] pointer operator->() const noexcept {
            
            assert (node_ != nullptr);
            return &(node_->value);
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

    // Возвращает итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен end()
    [[nodiscard]] Iterator begin() noexcept {
        
        // if (this->IsEmpty()){
        //     return this->end();
        // } else{
        return Iterator(head_.next_node);
        // }
    }

    // Возвращает итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] Iterator end() noexcept {
       
        auto *tmp = head_.next_node;
        while (tmp)
        {
            tmp = tmp->next_node;
        }
        return Iterator(tmp);
    }

    // Возвращает константный итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен end()
    // Результат вызова эквивалентен вызову метода cbegin()
    [[nodiscard]] ConstIterator begin() const noexcept {
 
        // if (this->IsEmpty()){
        //     return this->cend();
        // } else{
        return ConstIterator(head_.next_node);
        // }
    }

    // Возвращает константный итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    // Результат вызова эквивалентен вызову метода cend()
    [[nodiscard]] ConstIterator end() const noexcept {
 
        Node *tmp = head_.next_node;
        while (tmp)
        {
            tmp = tmp->next_node;
            
        }
        return ConstIterator(tmp);
    }

    // Возвращает константный итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен cend()
    [[nodiscard]] ConstIterator cbegin() const noexcept {
        if (this->IsEmpty()){
            return this->cend();
        } else{
            return ConstIterator(head_.next_node);
        }
    
    }

    // Возвращает константный итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator cend() const noexcept {

        return ConstIterator(this->end());
    }

    // Возвращает итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] Iterator before_begin() noexcept {
        
        return Iterator(&head_);
    }

    // Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
 
        // Node *p = &head_;
        return ConstIterator (const_cast<Node*>(&head_));
    }

    // Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator before_begin() const noexcept {
 
        return ConstIterator (const_cast<Node*>(&head_));
    }

    /*
     * Вставляет элемент value после элемента, на который указывает pos.
     * Возвращает итератор на вставленный элемент
     * Если при создании элемента будет выброшено исключение, список останется в прежнем состоянии
     */
    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        // создаем новый узел
        assert(pos.node_ != nullptr);
        Node* new_node = new Node(value, pos.node_->next_node);
        pos.node_->next_node = new_node;
        ++size_ ;
        return Iterator(new_node);
    }

    void PopFront() noexcept {

        assert(!this->IsEmpty());
        // определим ссылку на элемент, кото удаляем
        Node *tmp = (head_.next_node);
        // переназначем ссылку
        head_.next_node = tmp->next_node;
        //удаляем элемент

        delete tmp;
        --size_ ;

    }

    /*
     * Удаляет элемент, следующий за pos.
     * Возвращает итератор на элемент, следующий за удалённым
     */
    Iterator EraseAfter(ConstIterator pos) noexcept {
        // узел для удаления
        
        assert(pos.node_ != nullptr);
        assert(pos.node_->next_node != nullptr);
        assert(size_ > 0);


        auto del_node = pos.node_->next_node;
        //перезначаем ссылку

        pos.node_->next_node = del_node->next_node; 
        delete del_node;

        size_ -= 1;
        return Iterator (pos.node_->next_node);
    }

private:
    // Фиктивный узел, используется для вставки "перед первым элементом"
    Node head_;
    size_t size_;
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    if (lhs.begin() == rhs.begin() && lhs.end() == rhs.end()){
        return true;
    } else if (lhs.GetSize() != rhs.GetSize()){
        return false;
    } else{
        return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }
    
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    
    return !(rhs < lhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {

     return (rhs < lhs);
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {

    return !(lhs < rhs);
}

    void Test0() {
        using namespace std;
        {
            const SingleLinkedList<int> empty_int_list;
            assert(empty_int_list.GetSize() == 0u);
            assert(empty_int_list.IsEmpty());
        }

        {
            const SingleLinkedList<string> empty_string_list;
            assert(empty_string_list.GetSize() == 0u);
            assert(empty_string_list.IsEmpty());
        }
    }

    
