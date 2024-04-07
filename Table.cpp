#include <iostream>
#include "Table.h"



Table::Table(MemoryManager& mem) : AbstractTable(mem) {

	tableSize = 1000000;

	tableMemory = (List**)_memory.allocMem(tableSize * sizeof(List*));
	for (int i = 0; i < tableSize; i++) {
		tableMemory[i] = NULL;
	}
	obj_count = 0;
}

int Table::insertByKey(void* key, size_t keySize, void* elem, size_t elemSize) {
	size_t index = hash_function(key, keySize);

	if (tableMemory[index] == nullptr) {
		tableMemory[index] = new List(_memory);
	}
	Node* newNode = (Node*)_memory.allocMem(sizeof(Node));
	//Node newNode = {key, keySize, elem, elemSize};
	/*

	newNode->elem = _memory.allocMem(elemSize);
	memcpy(key, newNode->elem, elemSize);*/

	newNode->key = _memory.allocMem(keySize);
	memcpy(newNode->key, key, keySize);
	newNode->elem = _memory.allocMem(elemSize);
	memcpy(newNode->elem, elem, elemSize);

	//newNode->key = key;
	newNode->keySize = keySize;
	//newNode->elem = elem;
	newNode->elemSize = elemSize;

	std::cout << sizeof(Node);
	tableMemory[index]->push_front(newNode, sizeof(Node));
	//******************************************

	int* new_key = static_cast<int*>(newNode->key);
	int* new_elem = static_cast<int*>(newNode->elem);
	std::cout << "(" << *new_key << ", " << *new_elem << ") ";
	//*******************************************
	Table::obj_count++;
	int count_of_nodes = 0;
	count_of_nodes = this->tableMemory[index]->size();
	if (count_of_nodes >= 50) {

		size_t newSize = tableSize * 2;
		List** newTableMemory = (List**)_memory.allocMem(newSize * sizeof(List*));
		for (size_t i = 0; i < newSize; i++) {
			newTableMemory[i] = nullptr;
		}

		for (size_t i = 0; i < tableSize; i++) {
			if (tableMemory[i] != nullptr) {
				List::Iterator* iter = tableMemory[i]->newIterator();
				while (iter->hasNext()) {
					size_t elemSize;
					Node* node = static_cast<Node*>(iter->getElement(elemSize));

					Node* newNode = (Node*)_memory.allocMem(sizeof(Node));

					newNode->key = node->key;
					newNode->keySize = node->keySize;
					newNode->elem = node->elem;
					newNode->elemSize = node->elemSize;

					_memory.freeMem(node);
					//_memory.freeMem(node->elem);
					//_memory.freeMem(node->key);

					size_t newIndex = hash_function(newNode->key, newNode->keySize);

					if (newTableMemory[newIndex] == nullptr) {
						newTableMemory[newIndex] = new List(_memory);
					}

					newTableMemory[newIndex]->push_front(newNode, sizeof(Node));

					iter->goToNext();
				}
				
				tableMemory[i]->clear();
				delete iter;
			}
		}
		this->clear();
		_memory.freeMem(tableMemory);
		tableMemory = newTableMemory;
		tableSize = newSize;
	}

	return 0;
}

void Table::removeByKey(void* key, size_t keySize) {  // удаляем по ключу

	size_t index = hash_function(key, keySize);
	if (tableMemory[index] != nullptr && !tableMemory[index]->empty()) {

		List::Iterator* list_iter = tableMemory[index]->newIterator();
		Table::TableIterator* table_iter = dynamic_cast<Table::TableIterator*>(this->newIterator());
		while (list_iter->hasNext()) {
			size_t elemSize;
			Node* node = dynamic_cast<Node*>(list_iter);
			//Node* node = (Node*)(table_iter);
			void* elem = list_iter->getElement(elemSize);    // тк класс должен иметь хотя бы 1 виртуальный метод  ///!!!
			//******************************************
			int* new_key = static_cast<int*>(node->key);
			int new_keysize = static_cast<int>(node->keySize);
			int* new_elem = static_cast<int*>(node->elem);
			int new_elemsize = static_cast<int>(node->elemSize);

			int* new_key_1 = static_cast<int*>(key);
			std::cout << "key" << *new_key << " keysize" << new_keysize << "elem" << *new_elem << " elemsize" << new_elemsize;
			//******************************************
			size_t size_1;
			if (node->keySize == keySize && memcmp(node->key, key, keySize) == 0) {
				Node* node = (Node*)(table_iter->list_iterator->getElement(size_1));
				_memory.freeMem(node->key);
				_memory.freeMem (node->elem);
				this->tableMemory[index]->remove(table_iter->list_iterator);
				return;
			}

			table_iter->goToNext();
		}
	}
	//delete list_iter;

}


// remove insert find проверки 
Table::Iterator* Table::findByKey(void* key, size_t keySize) {
	size_t index = hash_function(key, keySize);
	if (tableMemory[index] != nullptr && !tableMemory[index]->empty()) {

		List::Iterator* list_iter = tableMemory[index]->newIterator();
		Table::TableIterator* table_iter = dynamic_cast<Table::TableIterator*>(this->newIterator());
		while (table_iter->hasNext()) {

			size_t elemSize;
			Node* node = (Node*)(list_iter);    // тк класс должен иметь хотя бы 1 виртуальный метод  ///!!!
			
			int* new_key = static_cast<int*>(node->key);
			int new_keysize = static_cast<int>(node->keySize);
			int* new_elem = static_cast<int*>(node->elem);
			int new_elemsize = static_cast<int>(node->elemSize);

			int* new_key_1 = static_cast<int*>(key);
			std::cout << "\nkey from arg: " << *new_key;
			std::cout << "\nkey: " << *new_key << " keysize: " << new_keysize << " elem: " << *new_elem << " elemsize: " << new_elemsize << "\n";
			// 
			if (node->keySize == keySize && memcmp(node->key, key, keySize) == 0) {
				return list_iter;
			}
			/*if (memcmp(node->key, key, keySize) == 0) {
				return new TableIterator(this, index, list_iter);
			}*/
			list_iter->goToNext();
		}

		//while (list_iter->hasNext() || !tableMemory[index]->empty()) {
		//	size_t elemSize;
		//	//void* currentElem = list_iter->getElement(elemSize);
		//	//Node* node = (Node*)(list_iter->getElement(elemSize));    // тк класс должен иметь хотя бы 1 виртуальный метод
		//	Node* node = dynamic_cast<Node*>(list_iter->getElement(elemSize));
		//	if (node->key==key) {
		//		return new TableIterator(this, index, list_iter);
		//	}
		//	/*if (memcmp(node->key, key, keySize) == 0) {
		//		return new TableIterator(this, index, list_iter);
		//	}*/
		//	list_iter->goToNext();
		//}

		delete list_iter;
	}

	return nullptr;
}

void* Table::at(void* key, size_t keySize, size_t& valueSize) {
	size_t index = hash_function(key, keySize);
	size_t size;
	if (tableMemory[index] != nullptr) {
		List::Iterator* list_iter = tableMemory[index]->newIterator();

		for (; list_iter->hasNext() || !tableMemory[index]->empty(); list_iter->goToNext()) {
			Node* node = (Node*)(list_iter->getElement(size));
			//Node* node = reinterpret_cast<Node*>(list_iter->getElement(size));
			if (node->keySize == keySize && memcmp(node->key, key, keySize) == 0) {
				valueSize = node->elemSize;
				return node->elem;
				//return node->elem;
			}

		}
	}
	return nullptr;
}


size_t Table::hash_function(void* key, size_t keySize) {
	/*
	const unsigned char* data = (const unsigned char*)key; int hash = 6;
	for (size_t i = 0; i < keySize; ++i) {
	hash = (hash << 5) ^ (hash >> 27) ^ data[i]; }

	*/
	unsigned char* data = (unsigned char*)key;
	size_t hash = 0;

	for (size_t i = 0; i < keySize; ++i) {
		hash += data[i];
	}

	return hash % this->arr_size;
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
	for (int i = 0; i < this->table->arr_size; i++) {
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

	for (int i = index; i < this->table->arr_size; i++) {
		if (this->table->tableMemory[i] != nullptr) {
			List::Iterator* list_iter_tmp = table->tableMemory[index]->newIterator();
			table->_memory.freeMem(list_iterator);
			this->list_iterator = dynamic_cast<List::ListIterator*>(list_iter_tmp);
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

	for (size_t i = 0; i < tableSize; ++i) {
		if (tableMemory[i] != nullptr && !tableMemory[i]->empty()) {

			//auto listIter = tableMemory[i]->newIterator();
			List::Iterator* listIter = tableMemory[i]->newIterator();
			while (listIter->hasNext() || !tableMemory[i]->empty()) {
				size_t elemSize;
				//void* currentElem = listIter->getElement(elemSize);
				Node* node = reinterpret_cast<Node*>(listIter->getElement(elemSize));
				if (/*elemSize == size &&*/ memcmp(node->elem, elem, size) == 0) {

					return new TableIterator(this, i, listIter);
				}
				listIter->goToNext();
			}
			delete listIter;
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
	//size_t index;

	Table::TableIterator* table_iter = dynamic_cast<Table::TableIterator*>(iter);
	size_t size_1;
	Node* node = (Node*)(table_iter->list_iterator->getElement(size_1));
	delete node->elem;
	delete node->key;
	tableMemory[table_iter->index]->remove(table_iter->list_iterator);
	if (!table_iter->list_iterator->hasNext()) { iter->goToNext(); }

	// добавить переход к следующему для iter
	_memory.freeMem(iter->getElement(size));



	//
	//size_t index = 0;
	//List::Iterator* list_iter = tableMemory[index]->newIterator();
	//Table::TableIterator* table_iter = dynamic_cast<Table::TableIterator*>(this->newIterator());
	//while (table_iter->hasNext()) {
	//	size_t elemSize;
	//	Node* node = (Node*)(table_iter->getElement(elemSize));    // тк класс должен иметь хотя бы 1 виртуальный метод  ///!!!

	//	int* new_key = static_cast<int*>(node->key);
	//	int new_keysize = static_cast<int>(node->keySize);
	//	int* new_elem = static_cast<int*>(node->elem);
	//	int new_elemsize = static_cast<int>(node->elemSize);


	//	std::cout << "key" << *new_key << " keysize" << new_keysize << "elem" << *new_elem << " elemsize" << new_elemsize;

	//	if (table_iter->equals(iter)) {

	//		this->tableMemory[index]->remove(table_iter->list_iterator);
	//		return;
	//	}

	//	table_iter->goToNext();
	//	index++;
	//	
	//}

	
}

void Table::clear() {
	if (obj_count == 0) return;

	std::cout << "CLEAR" << "\n";


	for (size_t i = 0; i < tableSize; ++i) {

		if (tableMemory[i] != nullptr) {
			List::Iterator* iter = tableMemory[i]->newIterator();
			while (iter->hasNext()) {
				size_t elemSize;
				Node* node = static_cast<Node*>(iter->getElement(elemSize));

				_memory.freeMem(node);

				//delete tableMemory[i];
				iter->goToNext();
			}
			tableMemory[i]->clear();
			tableMemory[i] = nullptr;
			delete iter;
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

Table::~Table() {
	this->clear();
	_memory.freeMem(tableMemory);
}

