#ifndef HASHMAP_HPP
#define HASHMAP_HPP

#include <memory>
#include <optional>

#include "HashMapInterface.hpp"

using namespace std;

template <class V>
class HashMap : HashMapInterface<string, V>
{
	private:
		struct Box {
			string key;
			V value;

			Box(string key) : key(key), value(V()) {}
			Box(string key, V value) : key(key), value(value) {}
		};
		
		unique_ptr<optional<Box>[]> items;
		size_t _size;
		size_t _capacity;

		long long hashString(string s) const {
			long long hash = 0;
			for (size_t i = 0; i < s.length(); i++)
			{
				hash = (hash * HASH_CONSTANT) + (long long) s[i];
			}
			return hash;
		}

		pair<size_t, bool> getIndex(const string& key, size_t index, size_t iter, const unique_ptr<optional<Box>[]>& items) const
		{
			if(items[index] == nullopt)
				return make_pair(index, false);
			else if(items[index]->key == key)
				return make_pair(index, true);
			else
				return getIndex(key, (index + iter) % _capacity, iter + 2, items);
		}

		void rehash()
		{
			size_t newCapacity = _capacity * 2;
			unique_ptr<optional<Box>[]> newItems = unique_ptr<optional<Box>[]>(new optional<Box>[newCapacity]);

			for(size_t i = 0; i < newCapacity; ++i)
				newItems[i] = nullopt;
			
			for (size_t i = 0; i < _capacity; i++)
			{
				if(items[i].has_value()) {
					string& key = items[i]->key;
					size_t hash = (size_t) (hashString(key) % (newCapacity));
					pair<size_t, bool> find = getIndex(key, hash, 1, newItems);

					size_t idx = find.first;
					newItems[idx] = Box(key);
					newItems[idx]->value = items[i]->value;
				}
			}
			
			items = move(newItems);
			_capacity = newCapacity;
		}

	public:
		HashMap() : _size(0), _capacity(DEFAULT_MAP_HASH_TABLE_SIZE)
		{
			items = unique_ptr<optional<Box>[]>(new optional<Box>[DEFAULT_MAP_HASH_TABLE_SIZE]);
			for(size_t i = 0; i < _capacity; i++)
				items[i] = nullopt;
		}

		/** Read/write index access operator.
		If the key is not found, an entry is made for it.
		@return: Read and write access to the value mapped to the provided key. */
		V& operator[](const string& key)
		{
			if(_size * 100 / _capacity > LOAD_THRESHOLD)
				rehash();
			
			size_t hash = (size_t) (hashString(key) % _capacity);
			pair<size_t, bool> find = getIndex(key, hash, 1, items);
			size_t idx = find.first;
			bool found = find.second;
			if(!found)
			{

				_size++;
				items[idx] = Box(key);
			}

			return items[idx]->value;
		}

		/** @return: the number of elements that match the key in the Map. */
		size_t count(const string& key)
		{
			size_t hash = (size_t) (hashString(key) % _capacity);
			return getIndex(key, hash, 1, items).second ? 1 : 0;
		}

		/** Removes all items from the Map. */
		void clear()
		{
			items = unique_ptr<optional<Box>[]>(new optional<Box>[DEFAULT_MAP_HASH_TABLE_SIZE]);
			_size = 0;
			_capacity = DEFAULT_MAP_HASH_TABLE_SIZE;
		}

		/** @return: number of Key-Value pairs stored in the Map. */
		size_t size() const
		{
			return _size;
		}

		/** @return: maximum number of Key-Value pairs that the Map can hold. */
		size_t max_size() const
		{
			return _capacity;
		}

		/** @return: string representation of Key-Value pairs in Map. */
		std::string toString() const
		{
			ostringstream out;

			for(size_t i = 0; i < _capacity; ++i) {
				auto item = items[i];
				if(item.has_value()) {
					out << "  [" << i << ":" << item->key << "->" << item->value << "]" << endl;
				}
			}

			return out.str();
		}

		friend std::ostream& operator<<(ostream& os, const HashMap& map)
		{
			os << map.toString();
			return os;
		}
};

#endif // HASHMAP_HPP