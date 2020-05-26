#include <algorithm>
#include <iostream>
#include <sstream>
#include "string"
#include <cassert>
template <typename T>
class List {
public:
    struct Node{
        T value;
        Node* prev;
        Node* next;
        Node(){}
        Node(T node_val): value(node_val) {}
        ~Node() {}
    };
    class iterator{
        Node* curr_node;
    public:
        iterator() : curr_node(0) {}
        iterator(Node *node): curr_node(node) {}
        iterator(const iterator &it): curr_node(it.curr_node) {}
        iterator& operator = (const iterator &it) {
            curr_node=it.curr_node;
            return *this;
        }
        bool operator == (const iterator it) const {
            return (curr_node == it.curr_node);
        }
        bool operator != (const iterator it) const {
            return !(it == *this);
        }
        iterator& operator ++ () {
            curr_node=curr_node -> next;
            return *this;
        }
        iterator& operator -- () {
            curr_node = curr_node -> prev;
            return *this;
        }
        T& operator * () const {
            return curr_node -> value;
        }
    };
private:
    Node* head;
    Node* tail;
    iterator head_iterator;
    iterator tail_iterator;
    size_t s = 0;
public:
    List() {
        s = 0;
        head = tail = new Node;
        tail -> next = nullptr;
        tail -> prev = nullptr;
        head_iterator = iterator(head);
        tail_iterator = iterator(tail);
    }
    List(size_t count, const T& value = T()) {
        s = 0;
        head = tail = new Node;
        tail -> next = nullptr;
        tail -> prev = nullptr;
        head_iterator = iterator(head);
        tail_iterator = iterator(tail);
        for (size_t i = 0; i < count; ++i) {
            push_back(value);
        }
    }
    List(const List &list) {
        s = 0;
        head = tail = new Node;
        tail -> next = nullptr;
        tail -> prev = nullptr;
        head_iterator = iterator(head);
        tail_iterator = iterator(tail);
        for (iterator i = list.head_iterator; i!=list.tail_iterator; ++i) {
            push_back(*i);
        }
    }
    List(List&& list) {
        head = list.head;
        tail = list.tail;
        head_iterator = list.head_iterator;
        tail_iterator = list.tail_iterator;
        list.clear();
    }
    ~List() {}
    List& operator = (const List& list) {
        if (&list == this)
            return *this;
        this -> clear();
        s = list.size();
        head = list.head;
        tail = list.tail;
        head_iterator = list.head_iterator;
        tail_iterator = list.tail_iterator;
        return *this;
    }
    List& operator = (List&& list) {
        if (&list == this)
            return *this;
        this -> clear();
        s = list.size();
        head = list.head;
        tail = list.tail;
        head_iterator = list.head_iterator;
        tail_iterator = list.tail_iterator;
        list.clear();
        return *this;
    }
    size_t size() const{
        return s;
    }
    T front() {
        return *head_iterator;
    }
    T back() {
        auto c = --tail_iterator;
        ++tail_iterator;
        return *(c);
    }
    void clear() {
        while(s > 0){
            pop_back();
        }
        s = 0;
        head -> value = 0;
    }
    bool empty() {
        return s == 0;
    }
    void push_back(T node_val) {
        ++s;
        tail -> value = node_val;
        Node* node = new Node;
        node -> prev = tail;
        node -> next = nullptr;
        tail -> next = node;
        tail = node;
        tail_iterator = iterator(tail);
    }
    void pop_back() {
        --s;
        Node* node_remove = tail->prev;
        tail->prev->prev->next=tail;
        tail->prev = node_remove->prev;
        delete node_remove;
    }
    void push_front(T node_val) {
        if (empty()) {
            ++s;
            push_back(node_val);
        } else {
            ++s;
            Node* node = new Node(node_val);
            node -> next = head;
            node -> prev = nullptr;
            head -> prev = node;
            head = node;
            head_iterator = iterator(head);
        }
    }
    void pop_front() {
        --s;
        Node *node_remove = head;
        head = node_remove -> next;
        head -> prev = 0;
        head_iterator = iterator(head);
        delete node_remove;
    }
    //void insert(const_iterator it, const T&) {}
    //void insert(const_iterator it, T&&) {}
    //void insert(const_iterator it, InputIter frist, InputIter last) {}
    iterator begin() const {
        return head_iterator;
    }
    iterator end() const {
        return tail_iterator;
    }
    iterator cbegin() const {
        return head_iterator;
    }
    iterator cend() const {
        return tail_iterator;
    }

};
