#pragma once
#include <iostream>
#include "ListAbstract.h"
#include <list>

typedef struct Pair {
    Pair(size_t elemSize, void* elem) : elemSize(elemSize), elem(elem) {}
    size_t elemSize;
    void* elem;

} Pair;

using TList = list<Pair*>;

class List : public AbstractList
{
private:
    TList* m_list;
    int m_count;
public:

    List(MemoryManager& mem) : AbstractList(mem), m_list(new TList()) {}

    ~List() {}

    int push_front(void* elem, size_t elemSize)
    {
        void* newElem = _memory.allocMem(elemSize);
        memcpy(newElem, elem, elemSize);

        m_list->push_front(new Pair(elemSize, newElem));
        m_count++;
        return 0;
    }

    void pop_front() {
        m_list->pop_front();
        m_count--;
    }

    void* front(size_t& size) {
        TList::iterator it = m_list->begin();
        size = (*it)->elemSize;
        return (*it)->elem;
    }

    class ListIterator : public List::Iterator {
    public:
        TList* l;
        TList::iterator it;

        ListIterator(TList* l) : l(l), it(l->begin()) {}

        void* getElement(size_t& size) {
            size = (*it)->elemSize;
            return (*it)->elem;
        }

        bool hasNext() {
            if (l->size() <= 1) return false;

            TList::iterator prev, cur = l->begin();

            while (cur != l->end()) {
                prev = cur;
                cur++;
            }
            if (it == prev) return false;

            return true;
        }

        void goToNext() { it++; }

        bool equals(Iterator* right) {
            ListIterator* r = dynamic_cast<ListIterator*>(right);
            return r->it == it;
        }
    };

    int insert(Iterator* iter, void* elem, size_t elemSize) {
        m_count++;
        return 0;
    }

    int size() { return m_list->size(); }

    size_t max_bytes() { return 0; }

    Iterator* find(void* elem, size_t size) {
        ListIterator* it = new ListIterator(m_list);

        size_t _size;
        void* _elem;
        while (it->it != m_list->end()) {
            _elem = it->getElement(_size);

            if (_size == size && !memcmp(_elem, elem, _size)) return it;

            it->goToNext();
        }
        return nullptr;
    }

    Iterator* newIterator() { return new ListIterator(m_list); }

    void remove(Iterator* iter) {
        size_t size1, size2;
        void* toFind = iter->getElement(size1);

        if (m_list->size() == 0) return;

        ListIterator* it = dynamic_cast<ListIterator*>(newIterator());
        void* cur;

        while (it) {
            cur = it->getElement(size2);
            if (!cur) break;

            if (size1 != size2) continue;

            int flag = memcmp(toFind, cur, size2);


            if (flag == 0) {

                //!!!
                m_list->remove(*(it->it));
                m_count--;
                return;
            }
            it->goToNext();
        }
    }

    void clear() {
        //!!!
        m_list->clear();
        m_count = 0;

    }

    bool empty() { return m_list->size() == 0; }
    int getCount() const {
        return m_count;
    }
};




//#pragma once
//#include <iostream>
//#include "ListAbstract.h"
//#include <list>
//
//const size_t ELEMSIZE = sizeof(int);
//
//using TList = list<void*>;
//
//class List : public AbstractList {
//private:
//	TList* m_list;
//	int m_count;
//
//public:
//	List(MemoryManager& mem) : AbstractList(mem), m_list(new TList()), m_count(0) {}
//
//	~List() {}
//
//	int push_front(void* elem, size_t elemSize) {
//		void* newElem = _memory.allocMem(elemSize);
//		memcpy(newElem, elem, ELEMSIZE);
//
//		m_list->push_front(newElem);
//		m_count++;
//		return 0;
//	}
//
//	void pop_front() {
//		m_list->pop_front();
//		m_count--;
//	}
//
//	void* front(size_t& size) {
//		size = ELEMSIZE;
//		TList::iterator it = m_list->begin();
//		return *it;
//	}
//
//	class ListIterator : public List::Iterator {
//	public:
//		TList* l;
//		TList::iterator it;
//
//		ListIterator(TList* l) : l(l), it(l->begin()) {}
//
//		void* getElement(size_t& size) { size = ELEMSIZE; return *it; }
//
//		bool hasNext() {
//			it++;
//
//			if (it == l->end()) {
//				--it;
//				return false;
//			}
//			else {
//				--it;
//				return true;
//			}
//		}
//
//		void goToNext() { it++; }
//
//		bool equals(Iterator* right) {
//			ListIterator* r = dynamic_cast<ListIterator*>(right);
//			return r->it == it;
//		}
//	};
//
//	int insert(Iterator* iter, void* elem, size_t elemSize) {
//		List::ListIterator* it = dynamic_cast<List::ListIterator*>(iter);
//
//		m_list->insert(it->it, elem);
//		m_count++;
//		return 0;
//	}
//	int size() { return m_list->size(); }
//
//	size_t max_bytes() { return 0; }
//
//	Iterator* find(void* elem, size_t size) {
//		ListIterator* it = new ListIterator(m_list);
//		size_t _size = ELEMSIZE;
//
//
//		while (it->it != m_list->end()) {
//			int t = *((int*)it->getElement(_size));
//			int t2 = *((int*)elem);
//
//			// printf("now: %d ?= %d\n", t, t2);
//
//			if (memcmp(it->getElement(_size), elem, _size / 2) == 0) {
//				return it;
//			}
//
//			it->goToNext();
//		}
//		return nullptr;
//	}
//
//	Iterator* newIterator() { return new ListIterator(m_list); }
//
//	void remove(Iterator* iter) {}
//
//	void clear() {
//		m_list->clear();
//		m_count = 0;
//	}
//
//	bool empty() { return m_list->size() == 0; }
//	int getCount() const {
//		return m_count;
//	}
//};
//
//
//
//#pragma once
//#include <iostream>
//#include <cstring>
//#include "ListAbstract.h"
//#include <list>
//
//using TList = std::list<std::pair<void*, size_t>>; // Хранение пары: указатель и размер
//
//class List : public AbstractList {
//private:
//    TList* m_list;
//    int m_count;
//
//public:
//    List(MemoryManager& mem) : AbstractList(mem), m_list(new TList()), m_count(0) {}
//
//    ~List() {
//        clear(); // Освобождение памяти
//        delete m_list;
//    }
//
//    int push_front(void* elem, size_t elemSize) {
//        void* newElem = _memory.allocMem(elemSize);
//        std::memcpy(newElem, elem, elemSize);
//
//        m_list->push_front(std::make_pair(newElem, elemSize));
//        m_count++;
//        return 0;
//    }
//
//    void pop_front() {
//        if (!m_list->empty()) { 
//            auto& front = m_list->front();
//            _memory.freeMem(front.first); // Освобождение памяти
//            m_list->pop_front();
//            m_count--;
//        }
//    }
//
//    void* front(size_t& size) {
//        if (!m_list->empty()) {
//            auto& front = m_list->front();
//            size = front.second;
//            return front.first;
//        }
//        return nullptr; // В случае пустого списка
//    }
//
//    class ListIterator : public List::Iterator {
//    public:
//        TList* l;
//        TList::iterator it;
//
//        ListIterator(TList* l) : l(l), it(l->begin()) {}
//
//        void* getElement(size_t& size) {
//            if (it != l->end()) {
//                size = it->second;
//                return it->first;
//            }
//            return nullptr;
//        }
//
//        bool hasNext() {
//            auto next = it;
//            return (++next != l->end());
//        }
//
//        void goToNext() {
//            if (it != l->end()) it++;
//        }
//
//        bool equals(Iterator* right) {
//            ListIterator* r = dynamic_cast<ListIterator*>(right);
//            return r && (r->it == it);
//        }
//    };
//
//    int insert(Iterator* iter, void* elem, size_t elemSize) {
//        List::ListIterator* it = dynamic_cast<List::ListIterator*>(iter);
//        if (it != nullptr && it->it != m_list->end()) {
//            void* newElem = _memory.allocMem(elemSize);
//            std::memcpy(newElem, elem, elemSize);
//            m_list->insert(it->it, std::make_pair(newElem, elemSize));
//            m_count++;
//            return 0;
//        }
//        return -1; // В случае ошибки
//    }
//
//    int size() { return m_list->size(); }
//
//    size_t max_bytes() { return 0; }
//
//    Iterator* find(void* elem, size_t size) {
//        ListIterator* it = new ListIterator(m_list);
//        while (it->hasNext()) {
//            size_t elemSize;
//            void* currentElem = it->getElement(elemSize);
//            if (elemSize == size && std::memcmp(currentElem, elem, size) == 0) {
//                return it;
//            }
//            it->goToNext();
//        }
//        delete it; // Если элемент не найден, удаляем итератор, чтобы избежать утечек памяти
//        return nullptr;
//    }
//
//
//    Iterator* newIterator() { return new ListIterator(m_list); }
//
//    void remove(Iterator* iter)
//
//    {
//        if (ListIterator* it = dynamic_cast<ListIterator*>(iter)) {
//            if (it->it != m_list->end()) {
//                _memory.freeMem(it->it->first); // Освобождение памяти
//                m_list->erase(it->it);
//                m_count--;
//            }
//        }
//    }
//    void clear() {
//        for (auto& elem : *m_list) {
//            _memory.freeMem(elem.first); // Освобождение памяти для каждого элемента
//        }
//        m_list->clear();
//        m_count = 0;
//    }
//
//    bool empty() { return m_list->empty(); }
//    int getCount() const { return m_count; }
//};


//новый мурода
//#pragma once
//#include <iostream>
//#include "ListAbstract.h"
//#include <list>
//
//typedef struct Pair {
//    Pair(size_t elemSize, void* elem) : elemSize(elemSize), elem(elem) {}
//    size_t elemSize;
//    void* elem;
//} Pair;
//
//using TList = list<Pair*>;
//
//class List : public AbstractList
//{
//private:
//    TList* m_list;
//    int m_count;
//
//public:
//    List(MemoryManager& mem) : AbstractList(mem), m_list(new TList()) {}
//
//    ~List() {}
//
//    int push_front(void* elem, size_t elemSize)
//    {
//        void* newElem = _memory.allocMem(elemSize);
//        memcpy(newElem, elem, elemSize);
//
//        m_list->push_front(new Pair(elemSize, newElem));
//        m_count++;
//        return 0;
//    }
//
//    void pop_front() { 
//        m_list->pop_front();
//        m_count--;
//    }
//
//    void* front(size_t& size) {
//        TList::iterator it = m_list->begin();
//        size = (*it)->elemSize;
//        return (*it)->elem;
//    }
//
//    class ListIterator : public List::Iterator {
//    public:
//        TList* l;
//        TList::iterator it;
//
//        ListIterator(TList* l) : l(l), it(l->begin()) {}
//
//        void* getElement(size_t& size) {
//            size = (*it)->elemSize;
//            return (*it)->elem;
//        }
//
//        bool hasNext() {
//            if (l->size() <= 1) return false;
//
//            TList::iterator prev, cur = l->begin();
//
//            while (cur != l->end()) {
//                prev = cur;
//                cur++;
//            }
//            if (it == prev) return false;
//
//            return true;
//        }
//
//        void goToNext() { it++; }
//
//        bool equals(Iterator* right) {
//            ListIterator* r = dynamic_cast<ListIterator*>(right);
//            return r->it == it;
//        }
//    };
//
//    int insert(Iterator* iter, void* elem, size_t elemSize) {
//        m_count++;
//        return 0; 
//    }
//
//    int size() { return m_list->size(); }
//
//    size_t max_bytes() { return 0; }
//
//    Iterator* find(void* elem, size_t size) {
//        ListIterator* it = new ListIterator(m_list);
//
//        size_t _size;
//        void* _elem;
//        while (it->it != m_list->end()) {
//            _elem = it->getElement(_size);
//
//            if (_size == size && !memcmp(_elem, elem, _size)) return it;
//
//            it->goToNext();
//        }
//        return nullptr;
//    }
//
//    Iterator* newIterator() { return new ListIterator(m_list); }
//
//    void remove(Iterator* iter) {
//        if (m_list->size() == 0) return;
//        size_t size1, size2;
//
//        ListIterator* it = dynamic_cast<ListIterator*>(newIterator());
//
//        while (it) {
//            void* elem = it->getElement(size1);
//            int flag = memcmp(elem, iter->getElement(size2), min(size1, size2));
//
//            if (it->hasNext()) it->goToNext(); else it = nullptr;
//
//            if (!flag && size1 == size2) {
//                m_list->remove(*(it->it));
//                m_count--;
//                return;
//            }
//        }
//    }
//
//    void clear() { m_list->clear(); }
//
//    bool empty() { return m_list->size() == 0; }
//
//    int getCount() const { return m_count; }
//};

//тот который от гпт 
//#pragma once
//#include <iostream>
//#include <cstring>
//#include "ListAbstract.h"
//#include <list>
//
//using TList = std::list<std::pair<void*, size_t>>; // Хранение пары: указатель и размер
//
//class List : public AbstractList {
//private:
//    TList* m_list;
//    int m_count;
//
//public:
//    List(MemoryManager& mem) : AbstractList(mem), m_list(new TList()), m_count(0) {}
//
//    ~List() {
//        clear(); // Освобождение памяти
//        delete m_list;
//    }
//
//    int push_front(void* elem, size_t elemSize) {
//        void* newElem = _memory.allocMem(elemSize);
//        std::memcpy(newElem, elem, elemSize);
//
//        m_list->push_front(std::make_pair(newElem, elemSize));
//        m_count++;
//        return 0;
//    }
//
//    void pop_front() {
//        if (!m_list->empty()) {
//            auto& front = m_list->front();
//            _memory.freeMem(front.first); // Освобождение памяти
//            m_list->pop_front();
//            m_count--;
//        }
//    }
//
//    void* front(size_t& size) {
//        if (!m_list->empty()) {
//            auto& front = m_list->front();
//            size = front.second;
//            return front.first;
//        }
//        return nullptr; // В случае пустого списка
//    }
//
//    class ListIterator : public List::Iterator {
//    public:
//        TList* l;
//        TList::iterator it;
//
//        ListIterator(TList* l) : l(l), it(l->begin()) {}
//
//        void* getElement(size_t& size) {
//            if (it != l->end()) {
//                size = it->second;
//                return it->first;
//            }
//            return nullptr;
//        }
//
//        bool hasNext() {
//            auto next = it;
//            return (++next != l->end());
//        }
//
//        void goToNext() {
//            if (it != l->end()) it++;
//        }
//
//        bool equals(Iterator* right) {
//            ListIterator* r = dynamic_cast<ListIterator*>(right);
//            return r && (r->it == it);
//        }
//    };
//
//    int insert(Iterator* iter, void* elem, size_t elemSize) {
//        List::ListIterator* it = dynamic_cast<List::ListIterator*>(iter);
//        if (it != nullptr && it->it != m_list->end()) {
//            void* newElem = _memory.allocMem(elemSize);
//            std::memcpy(newElem, elem, elemSize);
//            m_list->insert(it->it, std::make_pair(newElem, elemSize));
//            m_count++;
//            return 0;
//        }
//        return -1; // В случае ошибки
//    }
//
//    int size() { return m_list->size(); }
//
//    size_t max_bytes() { return 0; }
//
//    Iterator* find(void* elem, size_t size) {
//        ListIterator* it = new ListIterator(m_list);
//        while (it->hasNext()) {
//            size_t elemSize;
//            void* currentElem = it->getElement(elemSize);
//            if (elemSize == size && std::memcmp(currentElem, elem, size) == 0) {
//                return it;
//            }
//            it->goToNext();
//        }
//        delete it; // Если элемент не найден, удаляем итератор, чтобы избежать утечек памяти
//        return nullptr;
//    }
//
//
//    Iterator* newIterator() { return new ListIterator(m_list); }
//
//    void remove(Iterator* iter)
//
//    {
//        if (ListIterator* it = dynamic_cast<ListIterator*>(iter)) {
//            if (it->it != m_list->end()) {
//                _memory.freeMem(it->it->first); // Освобождение памяти
//                m_list->erase(it->it);
//                m_count--;
//            }
//        }
//    }
//    void clear() {
//        for (auto& elem : *m_list) {
//            _memory.freeMem(elem.first); // Освобождение памяти для каждого элемента
//        }
//        m_list->clear();
//        m_count = 0;
//    }
//
//    bool empty() { return m_list->empty(); }
//    int getCount() const { return m_count; }
//};
