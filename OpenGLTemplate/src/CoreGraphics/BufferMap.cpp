#include "BufferMap.h"
#include <iostream>

BufferMap::BufferMap(double dLoadFactor, size_t initialCapacity)
{
	m_buckets.reserve(initialCapacity);
	m_buckets.resize(initialCapacity);
	m_buckets.shrink_to_fit();
	m_size = 0;
	m_dLoadFactor = dLoadFactor;
}

// returns true if hash is in the data structure
bool BufferMap::check(uint64_t hash)
{
	unsigned int index = hash % m_buckets.size();
	Entry * pIter = m_buckets[index].pNext;
	while(pIter != nullptr)
	{
		if (pIter->hash == hash)
			return true;
		pIter = pIter->pNext;
	}
	return false;
}

void BufferMap::vecInsert(std::vector<Entry> & buckets, uint64_t hash, unsigned int uidBuffer)
{
	unsigned int index = hash % buckets.size();
	Entry * toAdd = new Entry(hash, uidBuffer, buckets[index].pNext);
	buckets[index].pNext = toAdd;
}

void BufferMap::insert(uint64_t hash, unsigned int uidBuffer)
{
	if ((double(m_size) / m_buckets.size()) > m_dLoadFactor)
		this->resize(m_buckets.size() * 2);

	vecInsert(m_buckets, hash, uidBuffer);
	++m_size;
}

// returns 0 if element not found
unsigned int BufferMap::operator[](uint64_t hash)
{
	unsigned int index = hash % m_buckets.size();
	Entry * pIter = m_buckets[index].pNext;
	while(pIter != nullptr)
	{
		if (pIter->hash == hash)
			return pIter->uidBuffer;
		pIter = pIter->pNext;
	}
	return 0;
}


bool BufferMap::destroy(uint64_t hash)
{
	unsigned int index = hash % m_buckets.size();
	Entry * pIter = &(m_buckets[index]);
	while(pIter->pNext != nullptr && pIter->pNext->hash != hash)
	{
		pIter = pIter->pNext;
	}

	if (pIter->pNext == nullptr)
		return false;

	Entry * toDelete = pIter->pNext;
	pIter->pNext = pIter->pNext->pNext;
	toDelete->pNext = nullptr;
	delete toDelete;
	toDelete = nullptr;

	--m_size;
	return true;
}

void BufferMap::resize(size_t newCapacity)
{
	std::vector<Entry> newBuckets;
	newBuckets.reserve(newCapacity);
	newBuckets.resize(newCapacity);
	newBuckets.shrink_to_fit();

	for (int i = 0; i < m_buckets.size(); ++i)
	{
		Entry * pIter = m_buckets[i].pNext;
		while (pIter != nullptr)
		{
			vecInsert(newBuckets, pIter->hash, pIter->uidBuffer);
			pIter = pIter->pNext;
		}
		delete m_buckets[i].pNext;
		m_buckets[i].pNext = nullptr;
	}

	m_buckets = std::move(newBuckets);

	std::cout << "Resized to: " << newCapacity << std::endl;
}
