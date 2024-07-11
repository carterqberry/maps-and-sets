#ifndef NODE_HPP
#define NODE_HPP

#include <memory>

using namespace std;

template <typename T>
struct Node
{
	T value;
	shared_ptr<Node<T>> left;
	shared_ptr<Node<T>> right;

	Node(T v, shared_ptr<Node<T>> left = nullptr, shared_ptr<Node<T>> right = nullptr) : value(v)
	{
		this->left = left;
		this->right = right;
	}

	Node(const Node<T>& other) {
		value = other.value;
		left = other.left;
		right = other.right;
	}

	Node& operator=(const Node<T>& other) {
		value = other.value;
		left = other.left;
		right = other.right;

		return *this;
	}

	bool add(T value)
	{
		if(value == this->value)
		{
			return false;
		}
		else if(value < this->value)
		{
			if(this->left == nullptr)
			{
				this->left = shared_ptr<Node<T>>(new Node<T>(value));
				return true;
			}
			else
			{
				return this->left->add(value);
			}
		}
		else
		{
			if(this->right == nullptr)
			{
				this->right = shared_ptr<Node<T>>(new Node<T>(value));
				return true;
			}
			else
			{
				return this->right->add(value);
			}
		}
	}

	pair<shared_ptr<Node<T>>, bool> remove(T targetValue)
	{
		if(targetValue == value)
		{
			if(left != nullptr)
			{
				pair<shared_ptr<Node<T>>, T> result = left->getPredecessor();
				T newValue = result.second;
				shared_ptr<Node<T>> newLeft = result.first;
				return make_pair(
					shared_ptr<Node<T>>(
						new Node<T>(
							newValue,
							newLeft,
							right
						)
					),
					true
				);
			}
			else
			{
				return make_pair(right, true);
			}
		}
		else if(targetValue < value)
		{
			if(left != nullptr)
			{
				auto result = left->remove(targetValue);
				auto newLeft = result.first;
				auto removed = result.second;
				return make_pair(
					shared_ptr<Node<T>>(
						new Node<T>(value, newLeft, right)
					),
					removed
				);
			}
			else
			{
				return make_pair(shared_ptr<Node<T>>(new Node<T>(value, left, right)), false);
			}
		}
		else
		{
			if(right != nullptr)
			{
				auto result = right->remove(targetValue);
				auto newRight = result.first;
				auto removed = result.second;
				return make_pair(
					shared_ptr<Node<T>>(
						new Node<T>(value, left, newRight)
					),
					removed
				);
			}
			else
			{
				return make_pair(shared_ptr<Node<T>>(new Node<T>(value, left, right)), false);
			}
		}
	}

	pair<shared_ptr<Node<T>>, T> getPredecessor()
	{
		if(right != nullptr)
		{
			pair<shared_ptr<Node<T>>, T> result = right->getPredecessor();
			shared_ptr<Node<T>> newRight = result.first;
			T predValue = result.second;
			return make_pair(shared_ptr<Node<T>>(new Node<T>(value, left, newRight)), predValue);
		}
		else
		{
			return make_pair(left, value);
		}
	}

	bool find(T searchValue)
	{
		if(searchValue == value)
		{
			return true;
		}
		else if(searchValue < value)
		{
			if(left != nullptr)
			{
				return left->find(searchValue);
			}
			else
			{
				return false;
			}
		}
		else
		{
			if(right != nullptr)
			{
				return right->find(searchValue);
			}
			else
			{
				return false;
			}
		}
	}

	size_t getDepth() const
	{
		if(left == nullptr && right == nullptr)
		{
			return 1;
		}
		else if(left != nullptr && right == nullptr)
		{
			return left->getDepth() + 1;
		}
		else if(left == nullptr && right != nullptr)
		{
			return right->getDepth() + 1;
		}
		else
		{
			size_t leftDepth = left->getDepth();
			size_t rightDepth = right->getDepth();

			return (leftDepth > rightDepth ? leftDepth : rightDepth) + 1;
		}
	}

	void printDepth(ostream& out, int depth) const
	{
		if(depth == 0) {
			out << " " << value;
			return;
		}

		if(left != nullptr)
			left->printDepth(out, depth - 1);
		else if(depth == 1 && right != nullptr)
			out << " _";

		if(right != nullptr)
			right->printDepth(out, depth - 1);
		else if(depth == 1 && left != nullptr)
			out << " _";

	}

	string toString() const
	{
		ostringstream out;

		if(left != nullptr)
			out << left->toString() << ",";
		
		out << value;

		if(right != nullptr)
			out << "," << right->toString();

		return out.str();
	}

	friend std::ostream& operator<<(ostream& os, const Node& node)
	{
		os << node.toString();
		return os;
	}
};

#endif // NODE_HPP