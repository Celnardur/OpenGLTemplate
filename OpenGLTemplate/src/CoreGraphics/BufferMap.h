#ifndef BUFFER_MAP
#define BUFFER_MAP

#include <vector>
#include <cstdint>

class BufferMap
{
private:
	struct Entry
	{
		uint64_t hash;
		unsigned int uidBuffer;
		Entry * pNext;

		Entry() : hash(0), uidBuffer(0), pNext(nullptr) {};
		Entry(uint64_t hash, unsigned int uidBuffer, Entry * pNext)
			: hash(hash), uidBuffer(uidBuffer), pNext(pNext) {};
		Entry(const Entry & toCopy)
		{
			this->hash = toCopy.hash;
			this->uidBuffer = toCopy.uidBuffer;
			if (toCopy.pNext != nullptr)
				this->pNext = new Entry(*toCopy.pNext);
		}

		void swap(Entry & first, Entry & second) const
		{
			std::swap(first.hash, second.hash);
			std::swap(first.uidBuffer, second.uidBuffer);
			std::swap(first.pNext, second.pNext);
		}

		Entry & operator=(Entry toCopy)
		{
			swap(*this, toCopy);
			return *this;
		}

		Entry(Entry && toCopy) : Entry()
		{
			swap(*this, toCopy);
		}

		~Entry()
		{
			delete pNext;
		}
	};

public:
	BufferMap(double dLoadFactor = 0.9, size_t initialCapacity = 16);
	BufferMap(const BufferMap & toCopy) = default;
	BufferMap(BufferMap && toCopy) = default;
	~BufferMap() = default;

	bool check(uint64_t hash);
	void insert(uint64_t hash, unsigned int uidBuffer);
	unsigned int operator[](uint64_t hash);
	bool destroy(uint64_t hash);
private:
	std::vector<Entry> m_buckets;
	size_t m_size;
	double m_dLoadFactor;

	void resize(size_t newCapacity);
	static void vecInsert(std::vector<Entry> & buckets, uint64_t hash, unsigned int uidBuffer);
};

#endif
