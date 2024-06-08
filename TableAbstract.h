#pragma once
#include "GroupContainer.h"

// ����������� �����: ������������ �������
class AbstractTable : public GroupContainer
{
public:
    // �����������
    AbstractTable(MemoryManager& mem) : GroupContainer(mem) {}
    // ������� ������ �� ���� � � �������� ��� ��� ���� �� ������ ����� ����� � ���� ��������  |  �������� � ������ 
    // ����������
    virtual ~AbstractTable() {}

    // ���������� �������� � ���������, � �������������� ������.
    // ���� ����� ���� ��� ����, ������� ������ �� ������ � ���������� 1.
    // � ������ ��������� ���������� ������� ���������� �������� 0, � ������ ������� 1.
    virtual int insertByKey(void * key, size_t keySize, void* elem, size_t elemSize) = 0;       // ����� �� �����������

    // �������� �������� � �������������� ������ �� ����������.
    virtual void removeByKey(void* key, size_t keySize) = 0;   // � ������ findByKey ����� remove

    // ������� ���������� ��������� �� ��������, ����������� �� ��������� � ���������� ������� � �������������� ������.
    // ���� ������� �� ������, ������������ ������� ���������.
    virtual Iterator* findByKey(void* key, size_t keySize) = 0;

    // ������ � �������� � ������ key
    virtual void* at(void* key, size_t keySize, size_t& valueSize) = 0;

    // ��� �������
    virtual size_t hash_function(void* key, size_t keySize) = 0;

};

