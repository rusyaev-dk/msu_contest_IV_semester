#ifndef GROUPLIST_H
#define GROUPLIST_H
#include "./Container.h"

// Базовый класс для однонаправленного и двунаправленного списков
class GroupList: public Container
{
public:
    GroupList(MemoryManager &mem): Container(mem) {}

protected:

    class ListNode{
        private:
            void* data;
            ListNode* next;
            size_t elem_size;

        public:
            ListNode(void* new_data , ListNode* new_next , size_t new_size){
                this->data = new_data;
                this->next = new_next;
                this->elem_size = new_size;
            }
            ~ListNode() = default;

            void* get_data(){
                return this->data;
            }

            size_t get_size(){
                return this->elem_size;
            }

            ListNode* get_next(){ 
                return this->next;
            }

            void change_next(ListNode* new_next){
                this->next = new_next;
            }

    };

    public: 

    class ListIterator : public Container::Iterator{
        private:
            ListNode* cur_node;
            ListNode* prev_node;

        public:

            ListIterator(ListNode* new_cur , ListNode* new_prew){
                this->cur_node = new_cur;
                this->prev_node = new_prew;
            }

            ListIterator(ListNode* new_cur){
                this->cur_node = new_cur;
                this->prev_node = (ListNode*)NULL;
            }
            // Возврашает явно указатель на элемент, на который указывает итератор в данный момент.
            // Неявно возвращает размер данных.
            // Если итератор показывает за пределы контейнера (например, удален последний элемент), возвращает NULL.
            void* getElement(size_t &size){
                size = this->cur_node->get_size();
                return this->cur_node->get_data();
            }

            // Возвращает true, если есть следующий элемент, иначе false.
            bool hasNext(){
                return (bool)(this->cur_node->get_next());
            }

            // Переход к следующему элементу.
            void goToNext(){
                if(this->hasNext()){
                    this->prev_node = cur_node;
                    this->cur_node = this->cur_node->get_next();
                }
            }

            // проверка на равенство итераторов
            bool equals(Iterator *right){
                ListIterator* list_right = dynamic_cast<ListIterator*>(right);
                return (bool)(this==list_right);
            }

            friend class ListNode;
            friend class List;
    };


};
#endif
