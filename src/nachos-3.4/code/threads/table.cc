#include "table.h"
#include <stdlib.h>

Table::Table(int size) : m_size(size)
{
	int i;
	m_entries = new void*[size];
	for (i = 0; i < size; ++i)
		m_entries[i] = NULL;
}

Table::Table() : m_size(0), m_entries(NULL)
{
	// Do nothing
}

Table::Table(const Table &table)
{
	// Do nothing
	// This constructor is forbbiden
}

Table::~Table()
{
	// Do nothing
	// Let the host destroy entries through Relase method
}

Table& Table::operator = (const Table &table)
{
	// Do nothing
	// This operator is forbbiden
}

int Table::Alloc(void *object)
{
	int i;
	for (i = 0; i < m_size; ++i) {
		if (m_entries[i] == NULL) {
			m_entries[i] = object;
			return i;
		}
	}
	return -1;
}

int Table::Set(void *object, int index)
{
	if (index < 0 || index >= m_size)
		return -1;
	if (m_entries[index] != NULL)
		return -1;
	m_entries[index] = object;
	return index;
}

void *Table::Get(int index)
{
	if (index < 0 || index >= m_size)
		return NULL;
	return m_entries[index];
}

int Table::Release(int index)
{
	if (index < 0 || index >= m_size)
		return -1;
	if (m_entries[index] == NULL)
		return -1;
	delete[]m_entries[index];
	m_entries[index] = NULL;
	return 0;
}
