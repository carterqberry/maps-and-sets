#ifndef BST_HPP
#define BST_HPP

#include <string>
#include <memory>

#include "Node.hpp"

using namespace std;

template <class T>
class BST
{
private:
	shared_ptr<Node<T>> root;
	size_t _size;

public:
	BST() : root(nullptr), _size(0) {}

	BST(const BST& other) {
		root = other.root;
		_size = other._size;
	}

	BST& operator=(const BST& other) {
		root = other.root;
		_size = other._size;

		return *this;
	}

	bool add(T value)
	{
		if(root == nullptr)
		{
			root = shared_ptr<Node<T>>(new Node<T>(value));
			_size++;
			return true;
		}
		else
		{
			bool added = root->add(value);
			if(added) {
				_size++;
			}
			return added;
		}
	}

	bool remove(T value)
	{
		if(root == nullptr)
		{
			return false;
		}
		else
		{
			pair<shared_ptr<Node<T>>, bool> result = root->remove(value);
			root = result.first;
			if(result.second) _size--;
			return result.second;
		}
	}

	bool find(T value)
	{
		if(root == nullptr)
		{
			return false;
		}
		else
		{
			return root->find(value);
		}
	}

	size_t size() const { return _size; }

	void clear() { root = nullptr; _size = 0; }

	string toString() const {
		if(root != nullptr)
		{
			return root->toString();
		}
		else
		{
			return "empty";
		}
	}

	friend std::ostream& operator<<(ostream& os, const BST& bst)
	{
		os << bst.toString();
		return os;
	}
};

#endif // BST_HPP