#ifndef SET_HPP
#define SET_HPP

#include "BST.hpp"
#include "SetInterface.hpp"

template <class T>
class Set : SetInterface<T>
{
	private:
		BST<T> _set;
	
	public:
		Set() {}

		Set(const Set& other) {
			_set = other._set;
		}

		Set& operator=(const Set& other) {
			_set = other._set;

			return *this;
		}

		/** Inserts item into the set, if the container doesn't
			already contain an element with an equivalent value. */
		bool insert(const T& item)
		{
			return _set.add(item);
		}

		/** Removes all items from the set. */
		void clear()
		{
			_set.clear();
		}

		/** @return: the number of elements contained by the Set. */
		size_t size() const
		{
			return _set.size();
		}

		/** @return: return 1 if contains element equivalent to item, else 0. */
		size_t count(const T& item)
		{
			return _set.find(item) ? 1 : 0;
		}

		bool find(const T& item)
		{
			return _set.find(item);
		}

		/** @return: string representation of items in Set. */
		string toString() const
		{
			return _set.toString();
		}

		friend std::ostream& operator<<(ostream& os, const Set& set)
		{
			os << set.toString();
			return os;
		}
};

#endif // SET_HPP