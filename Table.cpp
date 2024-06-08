#include <iostream>
#include "Table.h"
#include "ContainerTesterException.h"
using ErrorCode = ContainerTesterException::ErrorCode; 



Table::Table(MemoryManager& mem) : AbstractTable(mem) {

	tableSize = arr_size;
	tableMemory = (List**)_memory.allocMem(tableSize * sizeof(List*));
	if(!tableMemory){ throw(Error("Error: memory  for tableMemory is not allocated")); }
	for (int i = 0; i < tableSize; i++) {
		tableMemory[i] = nullptr;
	}
	obj_count = 0;
}

int Table::insertByKey(void* key, size_t keySize, void* elem, size_t elemSize) {
	if (key == nullptr || elem == nullptr || keySize <= 0 || elemSize <= 0) return 1;
	size_t index = hash_function(key, keySize);

	/*cout << index << endl;

	int* new_key_1 = static_cast<int*>(key);
	int* new_elem_1 = static_cast<int*>(elem);
	cout << "key: " << *new_key_1 << " elem: " << *new_elem_1 << endl;*/

	if (tableMemory[index] == nullptr) {
		tableMemory[index] = new List(_memory);
		if (!tableMemory[index]){ throw(Error("Error: memory  for tableMemory[i] is not allocated")); }
	}
	Node* newNode = (Node*)_memory.allocMem(sizeof(Node));
	
	if (!newNode){ throw(Error("Error: memory  for node is not allocated")); }

	newNode->key = _memory.allocMem(keySize);
	memcpy(newNode->key, key, keySize);
	newNode->elem = _memory.allocMem(elemSize);
	memcpy(newNode->elem, elem, elemSize);

	if (!newNode->key) { throw(Error("Error: memory  for node is not allocated")); }
	if (!newNode->elem) { throw(Error("Error: memory  for node is not allocated")); }

	newNode->keySize = keySize;
	newNode->elemSize = elemSize;
	size_t nodeSize;
	//проверка на то что хот€т добавить ноду с уже существующим key
	if (!tableMemory[index]->empty()) {
		List::Iterator* list_iter = tableMemory[index]->newIterator();
		do {
			Node* node = static_cast<Node*>(list_iter->getElement(nodeSize));
			if (memcmp(node->key, key, keySize) == 0) {
				std::cout << "Ёлемент с уже существующим ключем не добавлен" << endl;
				return 1;
			}
			if (!list_iter->hasNext()) {
				break;
			}
			list_iter->goToNext();
		} while (true);
		
		_memory.freeMem( list_iter);
	}

	
	if(tableMemory[index]->push_front(newNode, sizeof(Node))!=0){
		return 2;
		//throw(Error("Error: Wrong pushing front"));
	}
	//******************************************

	/*int* new_key = static_cast<int*>(newNode->key);
	int* new_elem = static_cast<int*>(newNode->elem);*/
	//std::cout << "(" << *new_key << ", " << *new_elem << ") ";

	
	obj_count++;




	int count_of_nodes = 0;
	count_of_nodes = this->tableMemory[index]->size();
	if (count_of_nodes == 50) {
		std::cout << "ѕерехеширование\n";
		rehashing();
	}

	return 0;
}

void Table::rehashing() {
	size_t newSize = tableSize * 2;
	this->arr_size *= 2;
	List** newTableMemory = (List**)_memory.allocMem(newSize * sizeof(List*));
	if (!newTableMemory) { throw(Error("Error: memory for newTableMemory is not allocated")); }
	for (size_t i = 0; i < newSize; i++) {
		newTableMemory[i] = nullptr;
	}

	for (size_t i = 0; i < tableSize; i++) {
		if (tableMemory[i] != nullptr) {
			List::Iterator* iter = tableMemory[i]->newIterator();   

			do {
				size_t elemSize;
				Node* node = static_cast<Node*>(iter->getElement(elemSize));

				size_t newIndex = hash_function(node->key, node->keySize);
				if (newTableMemory[newIndex] == nullptr) {
					newTableMemory[newIndex] = new List(_memory);
					if (!newTableMemory[newIndex]) { throw(Error("Error: memory  for newTableMemory[newIndex] is not allocated")); }
				}

				newTableMemory[newIndex]->push_front(node, sizeof(Node));
				if (!iter->hasNext()) {
					break;
				}
				iter->goToNext();
			} while (true);

			tableMemory[i]->clear();

			_memory.freeMem(iter);
		}
	}
	//this->clear();
	_memory.freeMem(tableMemory);
	tableMemory = newTableMemory;
	tableSize = newSize;	
}

void Table::removeByKey(void* key, size_t keySize) {
	if (key == nullptr || keySize <= 0) return;

	size_t index = hash_function(key, keySize);
	/*cout << index << endl;

	int* new_key = static_cast<int*>(key);
	
	cout << "key: " << *new_key << endl;*/

	if (tableMemory[index] != nullptr && !tableMemory[index]->empty()) {

	

		Table::TableIterator* table_iter = dynamic_cast<Table::TableIterator*>(findByKey(key, keySize));
		if (!table_iter) {
			cout << "€ не удалил !" << '\n';
			return;
		}

		List::Iterator* list_iter = table_iter->list_iterator;
		
		size_t size_1;
		Node* node = static_cast<Node*>(list_iter->getElement(size_1));

		_memory.freeMem(node->key);
		_memory.freeMem(node->elem);
		
		node->key = nullptr;
		node->elem = nullptr;

		this->tableMemory[index]->remove(list_iter);
		obj_count--;
		//delete list_iter;
		_memory.freeMem(table_iter);
		
		
		return;

	}
	else cout << "€ не удалил !" << endl;

}


// remove insert find проверки 
Table::Iterator* Table::findByKey(void* key, size_t keySize) {
	if (key == nullptr || keySize <= 0) return nullptr;

	size_t index = hash_function(key, keySize);
	//std::cout << "\hash " << index;
	if (tableMemory[index] != nullptr && !tableMemory[index]->empty()) {

		List::Iterator* list_iter = tableMemory[index]->newIterator();
		Table::TableIterator* table_iter = dynamic_cast<Table::TableIterator*>(this->newIterator());


		do {
			size_t elemSize;

			size_t size;
			Node* node = static_cast<Node*>(list_iter->getElement(size));
			/*int* new_key = static_cast<int*>(node->key);
			int new_keysize = static_cast<int>(node->keySize);
			int* new_elem = static_cast<int*>(node->elem);
			int new_elemsize = static_cast<int>(node->elemSize);

			int* new_key_1 = static_cast<int*>(key);
			std::cout << "\nkey from arg: " << *new_key_1;
			std::cout << "\nkey: " << *new_key << " keysize: " << new_keysize << " elem: " << *new_elem << " elemsize: " << new_elemsize << "\n";*/
			if (node->keySize == keySize && memcmp(node->key, key, keySize) == 0) {
				return new TableIterator(this, index, list_iter);
			}
			if (!list_iter->hasNext()) {
				break;
			}
			list_iter->goToNext();
		} while (true);
		_memory.freeMem(list_iter);
		_memory.freeMem(table_iter);
	}

	return nullptr;
}

void* Table::at(void* key, size_t keySize, size_t& valueSize) {
	if (key == nullptr || keySize <= 0) return nullptr;
	size_t index = hash_function(key, keySize);
	//std::cout << "\nhash " << index;
	size_t size;
	if (tableMemory[index] != nullptr && !tableMemory[index]->empty()) {

		List::Iterator* list_iter = tableMemory[index]->newIterator();
		Table::TableIterator* table_iter = dynamic_cast<Table::TableIterator*>(this->newIterator());
		do {
			size_t elemSize;

			size_t size;
			Node* node = static_cast<Node*>(list_iter->getElement(size));
			//****************************************
			/*int* new_key = static_cast<int*>(node->key);
			int new_keysize = static_cast<int>(node->keySize);
			int* new_elem = static_cast<int*>(node->elem);
			int new_elemsize = static_cast<int>(node->elemSize);
			int* new_key_1 = static_cast<int*>(key);
			std::cout << "\nkey from arg: " << *new_key;
			std::cout << "\nkey: " << *new_key << " keysize: " << new_keysize << " elem: " << *new_elem << " elemsize: " << new_elemsize << "\n";*/
			//*****************************************************
			if (node->keySize == keySize && memcmp(node->key, key, keySize) == 0) {
				
				valueSize = node->elemSize;
				return node->elem;
			}
			if (!list_iter->hasNext()) {
				break;
			}
			list_iter->goToNext();
		} while (true);
		_memory.freeMem(list_iter);
		_memory.freeMem(table_iter);
	}
	return nullptr;
}



size_t Table::hash_function(void* key, size_t keySize) {
	return GroupContainer::hash_function(key, keySize);
}


void* Table::TableIterator::getElement(size_t& size) {

	size_t nodeSize;
	Node* node = static_cast<Node*>(this->list_iterator->getElement(nodeSize));

	if (node) {
		size = node->elemSize;
		return node->elem;
	}
	else {
		size = 0;
		return nullptr;
	}
}

bool Table::TableIterator::hasNext() {
	if (list_iterator->hasNext()) {
		return true;
	}
	for (int i = index + 1; i < this->table->arr_size; i++) {
		if (this->table->tableMemory[i] != nullptr)
			return true;
	}
	return false;
}

void Table::TableIterator::goToNext() {
	if (list_iterator->hasNext()) {
		this->list_iterator->goToNext();
		return;
	}


	for (int i = index + 1; i < this->table->arr_size; i++) {
		if (this->table->tableMemory[i] != nullptr && !table->tableMemory[i]->empty()) {
			this->list_iterator = dynamic_cast<List::ListIterator*>(this->table->tableMemory[i]->newIterator());
			this->index = i;
			return;
		}
	}

}

bool Table::TableIterator::equals(Iterator* right) {
	TableIterator* rightIter = dynamic_cast<TableIterator*>(right);

	return this->list_iterator->equals(rightIter->list_iterator);
}


size_t Table::max_bytes() {
	return _memory.size();
}

int Table::size() {
	return Table::obj_count;
}

Table::Iterator* Table::find(void* elem, size_t size) {

	int flag = 0;
	int i = 0;
	for (i = 0; i < tableSize && flag == 0; i++) {
		if (tableMemory[i] != nullptr && !tableMemory[i]->empty() && obj_count != 0) {

			List::Iterator* list_iter = tableMemory[i]->newIterator();
			Table::TableIterator* table_iter = dynamic_cast<Table::TableIterator*>(this->newIterator());

			do {
				size_t elemSize;
				int* new_elem = static_cast<int*>(elem);
				int* element = static_cast<int*>(table_iter->getElement(elemSize));
				//cout << "элемент из аргуметов: " << *new_elem << " полученный элемент: " << *element << '\n';

				if (elemSize == size && memcmp(element, elem, size) == 0) {

					return table_iter;
					//return new TableIterator(this, i, table_iter->list_iterator);
				}
				if (table_iter->hasNext() == false) {
					flag = 1;
					break;
				}
				table_iter->goToNext();
			} while (true);

		}
	}

	return nullptr;
}

Table::TableIterator::TableIterator(Table* table, int index, Table::Iterator* iter) {
	this->index = index;
	this->table = table;
	this->list_iterator = dynamic_cast<List::ListIterator*>(iter);
}



Table::Iterator* Table::newIterator() {
	if (Table::obj_count == 0)  return nullptr;
	int index = 0; // index of non emty list
	while (tableMemory[index] == nullptr) {
		index++;
	}
	return new TableIterator(this, index, tableMemory[index]->newIterator());
}

void Table::remove(Iterator* iter) {
	size_t size;

	Table::TableIterator* table_iter = dynamic_cast<Table::TableIterator*>(iter);
	size_t size_1;
	Node* node = (Node*)(table_iter->list_iterator->getElement(size_1));
	if (!node) return;
		
	_memory.freeMem(node->elem);
	_memory.freeMem(node->key);
	
	
	tableMemory[table_iter->index]->remove(table_iter->list_iterator);
	
	
	/*int i = 0;
	for (i = table_iter->index + 1; i < arr_size; i++) {
		if (tableMemory[i] != nullptr) break;
	}

	if (!table_iter->list_iterator->hasNext()) { tableMemory[i]->newIterator(); }*/
	

}

void Table::clear() {
	if (obj_count == 0) return;

	//std::cout << "CLEAR" << "\n";

	for (size_t i = 0; i < tableSize; ++i) {

		if (tableMemory[i] != nullptr && !tableMemory[i]->empty()) {
			List::Iterator* iter = tableMemory[i]->newIterator();
			do {
				size_t elemSize;
				Node* node = static_cast<Node*>(iter->getElement(elemSize));

				_memory.freeMem(node->key);
				_memory.freeMem(node->elem);
				

				if (!iter->hasNext()) {
					break;
				}
				iter->goToNext();
			} while (true);
			
			
			tableMemory[i]->clear();
			_memory.freeMem(tableMemory[i]);
			tableMemory[i] = nullptr;
			/*dynamic_cast<Table::TableIterator*>(iter)->~TableIterator();
			delete(iter);*/
			_memory.freeMem(iter);
		}

	}

	
	obj_count = 0;

}

bool Table::empty() {
	if (Table::obj_count == 0) {
		return true;
	}
	return false;
}

size_t Table::TableIterator::getIndex(){
	size_t size;
	Node* node = static_cast<Node*>(this->list_iterator->getElement(size));
	
	size_t index = table->hash_function(node->key, node->keySize);
	return index;
}






Table::~Table() {
	this->clear();
	_memory.freeMem(tableMemory);
	std::cout << "YA POCHISTIL\n";
	

}
