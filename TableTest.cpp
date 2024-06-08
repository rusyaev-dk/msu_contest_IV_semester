#include "tableTest.h"
#include <iostream>
#include <chrono>
using ErrorCode = ContainerTesterException::ErrorCode;

TableTest::TableTest(size_t bytesSize, int numElements) : BaseContainerTester(bytesSize), numElements(numElements) {
	table = nullptr;
}


void TableTest::test_insert(size_t elem_count) {
	_create_container();

	std::cout << "Testing insertByKey" << std::endl;
	auto start = std::chrono::high_resolution_clock::now();
	int flag = 1;
	int failCount = 0;
	this->_fill_container_with_size_t(numElements);
	auto finish = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < numElements; i++) {
		auto iter = table->findByKey(&i, sizeof(i));
		size_t elemSize;
		int* res = static_cast<int*>(iter->getElement(elemSize));
		if (*res != 2*i) {
			delete iter;
			flag = 0;
			throw ContainerTesterException(ErrorCode::INSERT_ERROR, "yyy");
		}
	}
	if(flag==1) cout << "Успешно вставлено " << numElements << " элементов" << std::endl;

	std::chrono::duration<double, std::milli> elapsed = finish - start;
	std::cout << "Время выполнения вставки: " << elapsed.count() << " мс.\n";
	flag = 1;
	for (int i = 0; i < numElements; i++) {

		auto iter = table->findByKey(&i, sizeof(i));
		size_t elemSize;
		int* res = static_cast<int*>(iter->getElement(elemSize));
		
		if (*res != i*2) {
			delete iter;
			flag = 0;
			throw ContainerTesterException(ErrorCode::ELEM_NOT_FOUND_ERROR, "ya shlapay");
		}
		
		
	}
	if(flag==1) cout << "Вставка прошла успешно, все элементы найдены " << endl;
	
	int key = 1;
	int value = 2;
	std::cout << "Попытка добавить элемент с уже существующим key " << endl;
	int insertRes = table->insertByKey(&key, sizeof(key), &value, sizeof(value));


	cout << "Проверка вставкии double" << endl;
	double test_number = 50.625;
	double test_number1 = 100.256;
	int testResult = table->insertByKey(&test_number, sizeof(double), &test_number1, sizeof(double));
	size_t size_1;
	auto it = table->find(&test_number1, sizeof(double));
	double* found_value = static_cast<double*>(it->getElement(size_1));

	if (*found_value == test_number1) {
		std::cout << "Значение найдено и соответствует: " << *found_value << std::endl;
	}
	else { std::wcout << "Ошибка при работе с double" << endl; }



	cout << "Проверка вставки struct " << endl;
	struct Coordinates {
		double x;
		double y;
	};
	string test_msg = "Привет мир)";

	Coordinates coord = { 10.125, 20.625 };
	int testResult1 = table->insertByKey(&coord, sizeof(Coordinates), &test_msg, sizeof(string));
	size_t size_2;
	auto it1 = table->findByKey(&coord, sizeof(Coordinates));
	string* msg1 = static_cast<string*>(it1->getElement(size_2));

	if (*msg1 == test_msg && size_2 == sizeof(string)) {
		std::cout << "Значения string соответствуют: " << *msg1 << std::endl;
	}
	else {
		std::cout << "Ошибка при работе с string" << endl;
	}





	int insertResult = table->insertByKey(nullptr, sizeof(key), nullptr, sizeof(value));
	if (insertResult == 1) {
		std::cout << "Вставка с nullptr не выполнена." << std::endl;
	}
	else {
		std::cout << "Ошибка при вставке нуллптр" << std::endl;
	}


	this->_destroy_container();
	//table->clear();
}

// Тестирование функции removeByKey
void TableTest::testRemoveByKey() {
	std::cout << "\n" << std::endl;
	std::cout << "Testing removeByKey" << std::endl;

	this->_create_container();


	this->_fill_container_with_size_t(numElements);
	std::cout << "Все элементы добавлены." << std::endl;




	int failCount = 0;
	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0, j=0; i < numElements; i += 2) {
		table->removeByKey(&i, sizeof(i));
		auto even = table->findByKey(&i, sizeof(i));
		if (even != nullptr) {
			delete even;
			failCount = 1;
			throw ContainerTesterException(ErrorCode::REMOVE_ERROR, "!!!");
		}
		//size_t elemSize1;
		/*int* even_res = static_cast<int*>(even->getElement(elemSize1));*/

		 j=i+1;
		auto odd = table->findByKey(&j, sizeof(j));
		size_t elemSize2;
		int* odd_res = static_cast<int*>(odd->getElement(elemSize2));


		if (*odd_res!=j*2) {
			delete odd;
			failCount = 1;
			throw ContainerTesterException(ErrorCode::REMOVE_ERROR, "!!!");
		}

	}
	//подготовить ф ию которая пробегает по контейнеру и ставит метки (виде невидел)

	if (failCount == 0) {
		std::cout << "Все четные элементы успешно удалены, нечетные элементы на месте." << std::endl;
	}
	




	int nonExistingKey = 10000000;
	std::cout << "Попытка удалить несуществующий " << std::endl;
	table->removeByKey(&nonExistingKey, sizeof(nonExistingKey));



	int key = 5, value = 10;
	table->insertByKey(&key, sizeof(key), &value, sizeof(value));
	std::cout << "Попытка удалить 2 раза:" << std::endl;
	table->removeByKey(&key, sizeof(key));


	table->removeByKey(&key, sizeof(key));

	cout << "Попытка удалить элемент по ключу после очистки " << endl;
	// Удаление при пустой таблице
	table->clear();
	table->removeByKey(&key, sizeof(key));



	this->_destroy_container();

}






void TableTest::testFindByKey() {
	std::cout << "\n" << std::endl;
	std::cout << "Testing findByKey" << std::endl;

	this->_create_container();

	int nonExistingKey = 10000000;
	auto result = table->findByKey(&nonExistingKey, sizeof(nonExistingKey));
	if (result == nullptr) {
		std::cout << "Несуществующий ключ " << nonExistingKey << " не найден." << std::endl;
	}
	else {
		throw ContainerTesterException(ErrorCode::UNKNOWN_ERROR, "Ошибка! несуществующий ключ  найден ");
	}

	int key = 5, value = 10;
	table->insertByKey(&key, sizeof(key), &value, sizeof(value));
	table->removeByKey(&key, sizeof(key));
	result = table->findByKey(&key, sizeof(key));
	if (result == nullptr) {
		std::cout << "Ключ " << key << " не найден после удаления." << std::endl;
	}
	else {
		throw ContainerTesterException(ErrorCode::UNKNOWN_ERROR, "Ошибка! Удаленный ключ  найден ");
	}


	this->_fill_container_with_size_t(numElements);

	int searchKey = 500;

	result = table->findByKey(&searchKey, sizeof(searchKey));
	size_t elemSize;
	int* res = static_cast<int*>(result->getElement(elemSize));

	if (*res==searchKey*2) {
		std::cout << "Существующий ключ " << searchKey << " найден успешно." << std::endl;
		delete result;
	}
	else {
		throw ContainerTesterException(ErrorCode::REMOVE_ERROR, "!!!");
	}


	table->clear();
	this->_destroy_container();
}





void TableTest::testAt() {
	std::cout << "\n" << std::endl;
	std::cout << "Testing at" << std::endl;
	this->_create_container();


	int nonExistingKey = 10000000;
	size_t valueSize;
	void* value = table->at(&nonExistingKey, sizeof(nonExistingKey), valueSize);     //как чистить?
	if (value == nullptr) {
		std::cout << "Доступ к несуществующему ключу " << nonExistingKey << " не получен" << std::endl;
	}
	else {
		throw ContainerTesterException(ErrorCode::UNKNOWN_ERROR, "Ошибка: получен доступ по несуществующему ключу. ");
		
	}


	int key = 5, actualValue = 10;
	table->insertByKey(&key, sizeof(key), &actualValue, sizeof(actualValue));
	table->removeByKey(&key, sizeof(key));
	cout << "Попытка получить доступ полсе удаления." << '\n';
	value = table->at(&key, sizeof(key), valueSize);
	if (value == nullptr) {
		std::cout << "Доступ к ключу " << key << " невозможен, тк удалили" << std::endl;
	}
	else {
		throw ContainerTesterException(ErrorCode::UNKNOWN_ERROR, "Ошибка: получен доступ по удаленному ключу. ");
		
	}


	this->_fill_container_with_size_t(numElements);
	std::cout << "Все элементы добавлены." << std::endl;
	value = nullptr;
	int failCount = 0;
	for (int i = 0; i < 100; i++) {
		value = table->at(&i, sizeof(i), valueSize);
		int* new_key = static_cast<int*>(value);
		if (*new_key != i*2 || valueSize != sizeof(i)) {
			failCount++;
			
			throw ContainerTesterException(ErrorCode::UNKNOWN_ERROR, "Ошибка: при попытке получить доступ к элементам. ");
		}

	}
	if (failCount == 0) { cout << "Успешно получен доступ к первым 100 элементам " << endl; }

	table->clear();
	this->_destroy_container();
}
//
//
//

//
void TableTest::testSize() {
	std::cout << "\n" << std::endl;
	std::cout << "Testing size" << std::endl;
	this->_create_container();

	this->_fill_container_with_size_t(numElements);
	std::cout << "Размер таблицы после вставки 1кк элементов: " << table->size() << std::endl;

	int keyToRemove = 5;
	table->removeByKey(&keyToRemove, sizeof(keyToRemove));
	std::cout << "Размер таблицы после удаления одного элемента: " << table->size() << std::endl;

	table->clear();
	std::cout << "Размер таблицы после очистки: " << table->size() << std::endl;

	this->_destroy_container();
}
//
void TableTest::test_find(size_t elem_count) {
	std::cout << "\n" << std::endl;
	std::cout << "Testing Find" << std::endl;
	this->_create_container();

	int key = 1, value = 100;
	table->insertByKey(&key, sizeof(key), &value, sizeof(value));
	Table::Iterator* iter = table->find(&value, sizeof(value));

	size_t elemSize;
	int* res = static_cast<int*>(iter->getElement(elemSize));
	
	if (*res == value) {
		std::cout << "Элемент со значением " << value << " найден." << std::endl;
	}
	else {
		throw ContainerTesterException(ErrorCode::ELEM_NOT_FOUND_ERROR, "Ошибка: элемент не найден ");
	}


	table->removeByKey(&key, sizeof(key));
	iter = table->find(&value, sizeof(value));
	if (iter == nullptr) {
		std::cout << "Элемент со значением " << value << " не найден после его удаления" << std::endl;
	}
	else {
		throw ContainerTesterException(ErrorCode::UNKNOWN_ERROR, "Ошибка: удаленный элемент найден. ");
		
	}

	table->clear();
	int Value = 200;
	iter = table->find(&Value, sizeof(Value));
	if (iter == nullptr) {
		std::cout << "Поиск в пустой таблице не нашел элемента" << std::endl;
	}
	else {
		
		delete iter;
		throw ContainerTesterException(ErrorCode::UNKNOWN_ERROR, "Ошибка: найден элемент в пустой таблице. ");
	}
	this->_destroy_container();
}


void TableTest::test_remove(size_t elem_count) {
	std::cout << "\n" << std::endl;
	std::cout << "Testing Remove" << std::endl;
	this->_create_container();

	this->_fill_container_with_size_t(numElements);

	std::cout << "Все элементы добавлены." << std::endl;

	int searchelem = 2;

	auto iter = table->find(&searchelem, sizeof(searchelem));
	
	if (iter != nullptr) {
		size_t elemSize;
		int* res = static_cast<int*>(iter->getElement(elemSize));
		std::cout << "Найден элемент: " <<*res<< std::endl;

		std::cout << "Тестирование удаления элемента:" << std::endl;
		
			table->remove(iter);
			
			iter = table->find(&searchelem, sizeof(searchelem));
			if (iter == nullptr) {
				std::cout << "После удаления элемент не найден." << std::endl;
			}
			else {
				
				delete iter;
				throw ContainerTesterException(ErrorCode::REMOVE_ERROR, "Ошибка в удалении!. ");
			}
		
	}
	else {
		throw ContainerTesterException(ErrorCode::ELEM_NOT_FOUND_ERROR , "Элемент не найден. ");
		
	}


	

	
	
	this->_destroy_container();
	//**********************************
	/*iter = table->newIterator();

	table->remove(iter);
	table->remove(iter);*/


	//+ещще 1 тест из тг
	/*auto iter_1 = table->newIterator();
	auto iter_2 = table->newIterator();

	table->remove(iter_1);
	table->remove(iter_2);*/
	//**************************************
}




void TableTest::test_clear(size_t elem_count) {
	std::cout << "\n" << std::endl;
	std::cout << "Testing Clear" << std::endl;
	this->_create_container();
	std::cout << "Пробуем очистить пустую таблицу." << std::endl;
	table->clear();

	std::cout << "Очистка выполнена." << std::endl;

	this->_fill_container_with_size_t(numElements);

	//*******
	//List::Iterator* iter = table->tableMemory[0]->newIterator();
	//delete iter;

	std::cout << "Пробуем очистить заполненную таблицу." << std::endl;
	table->clear();
	if (table->empty()) {
		std::cout << "Очистка выполнена." << std::endl;
	}
	else {
		throw ContainerTesterException(ErrorCode::UNKNOWN_ERROR, "В таблице остались элементы. ");
	}

	this->_destroy_container();
	/*std::cout << "Проверяем занулился ли итератор" << endl;

	if (!iter) {
		std::cout << "Итератор занулился успешно" << endl;

	}else
		std::cout << "Итератор не nullptr" << endl;*/
		//**************
}

void TableTest::testEmpty() {
	std::cout << "\n" << std::endl;
	std::cout << "Testing Empty" << std::endl;
	this->_create_container();

	if (table->empty()) {
		std::cout << "Таблица пустая" << std::endl;
	}
	else {
		throw ContainerTesterException(ErrorCode::UNKNOWN_ERROR, "Ошибка: новая таблица не пустая. ");
		
	}

	this->_fill_container_with_size_t(numElements);

	if (!table->empty()) {
		std::cout << "Таблица непустая после добавления элементов." << std::endl;
	}
	else {
		throw ContainerTesterException(ErrorCode::UNKNOWN_ERROR, "Ошибка: таблица пустая после добавления элементов. ");
	}
	for (int i = 0; i < numElements; i++) {
		table->removeByKey(&i, sizeof(i));
	}
	if (table->empty()) {
		std::cout << "Таблица пустая после удаления всех элем." << std::endl;
	}
	else {
		throw ContainerTesterException(ErrorCode::UNKNOWN_ERROR, "Ошибка: таблица не пустая после удаления всех элементов. ");
	}
	this->_destroy_container();

}

void TableTest::_create_container() {
	if (this->table) delete this->table;
	this->table = new Table(*this->_mem_manager);
}

void TableTest::_destroy_container() {
	if (!this->table) return;
	delete this->table;
	this->table = nullptr;
}

void TableTest::_fill_container_with_size_t(size_t numElements) {
	for (int i = 0; i < numElements; ++i) {
		int key = i;
		int value = i * 2;

		int err_code = table->insertByKey(&key, sizeof(int), &value, sizeof(int));
		this->_validate_insertion_code(err_code);
		
	}
}





TableTest::~TableTest() {
	cout << " Table pochistilas" << endl;

	//delete table;
}

