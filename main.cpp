
#include <iostream>
#include "TableAbstract.h"
#include "Table.h"
#include "Mem.h"
#include "TableTest.h"
#include "List.h"
#include <chrono>
#include "ContainerTesterException.h"

#define __CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define DEBUG_NEW new_1(_NORMAL_BLOCK, FILE, LINE)
#define new_1 DEBUG_NEW


int main()
{
	setlocale(LC_ALL, "Russian");
	size_t bytesSize = 10000000;
	int numElements = 1000000;
	Mem memoryManager(bytesSize);
	Table myTable(memoryManager);

	/*int a = 1;
	int b = 2;
	myTable.insertByKey(&a, sizeof(int), &b, sizeof(int));*/


	/*вставляем один элем
	int a= 1;
	int b = 2;
	myTable.insertByKey(&a, sizeof(int), &b, sizeof(int));

	вставка нескольких объектов
	for (int i = 0; i < 100; ++i) {
		int key = i;
		int value = i * 2;
		
		myTable.insertByKey(&key, sizeof(int), &value, sizeof(int));
	}

	myTable.removeByKey(&a, sizeof(int));

	auto res = myTable.findByKey(&a, sizeof(int));
	delete res;

	size_t valueSize;
	void* value = myTable.at(&a, sizeof(int), valueSize);

	auto iter = myTable.find(&b, sizeof(b));
	delete iter;*/
	

	TableTest tester(bytesSize, numElements);
	
	try{

	tester.test_insert( numElements);
	tester.testRemoveByKey();
	tester.testFindByKey();
	tester.testAt();
	tester.testSize();
	tester.test_find(numElements);
	tester.test_remove(numElements);
	tester.test_clear(numElements);
	tester.testEmpty();
	}
	catch (ContainerTesterException& e) {
		cout << e.what() << endl;
	}

	 //delete myTable;
	
	
	return 0;

}


