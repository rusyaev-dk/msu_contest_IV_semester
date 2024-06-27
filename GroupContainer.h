#pragma once
#include "Container.h"
#include "LinkedList1.h"

using namespace std;

enum ContainerType {
    set,
    associative_table,
};

// Базовый класс для некоторой группы абстрактных контейнеров
class GroupContainer: public Container
{   
protected:
    ContainerType _type;

    LinkedList1** _data_array;
    
    size_t _data_array_size = 5000;
    
    size_t _elem_count; // actual quantity of elems in container
    
    size_t _max_bytes; // max container size in bytes

    inline bool _empty() { return _elem_count == 0; };
    inline int _size() { return this->_elem_count; };
    inline size_t _get_max_bytes() { return this->_max_bytes; }; 
    
    template <typename Container, typename Iterator>
    Iterator* _newIterator(Container* container) {
        if (this->_empty()) return nullptr;
    
        for (size_t i = 0; i < this->_data_array_size; i++) {
            bool has_non_empty_list = this->_data_array[i] != nullptr && !this->_data_array[i]->empty();
            if (has_non_empty_list) {
                return new Iterator(this->_data_array[i]->newIterator(), container);
            }
        }
        return nullptr;
    };
    
public:
    struct Node {
        void* key;
        size_t keySize; 
        void* elem;
        size_t elemSize; 
    };

    class IteratorUtils {
    public:
        static size_t get_elem_hash(GroupContainer* container, Iterator* list_iter) {        
            size_t elem_size;
            if (container->_type == ContainerType::set) {
                void* elem = list_iter->getElement(elem_size);
                if (!elem) {
                    throw Error("Elem in active iterator is null.");
                }
                return container->hash_function(elem, elem_size); 
            } else {
                Node* node = static_cast<Node*>(list_iter->getElement(elem_size));
                if (!node || !node->key || !node->elem) {
                    throw Error("Node in active iterator is null.");
                }
                return container->hash_function(node->key, node->keySize);  
            }
        };

        static void* getElement(size_t& size, GroupContainer* container, Iterator* list_iter);
        static bool hasNext(GroupContainer* container, Iterator* list_iter);
        static void goToNext(GroupContainer* container, LinkedList1::ListIterator*& list_iter);
        
        template <typename Iterator>
        static bool equals(Iterator* left, Iterator* right) {
            if (!left || !right) return false;
            return left->_list_iter->equals(right->_list_iter);
        };

        friend class GroupContainer;
    };

    GroupContainer(MemoryManager &mem, ContainerType type);

    size_t hash_function(void* key, size_t keySize);

    friend class IteratorUtils;
};
