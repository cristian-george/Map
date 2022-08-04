#pragma once

#ifndef _MAP_H_
#define _MAP_H_

#include <iostream>

template <class T1, class T2, class Compare = std::less<T1>>
class Map
{
private:
	template <class T1, class T2>
	class NodeTree
	{
	public:
		enum class EColor
		{
			Red = 0,
			Black
		};

	public:
		NodeTree(T1 key = T1(), T2 value = T2(), EColor color = EColor::Red)
			: key(key)
			, value(value)
			, color(color)
			, left(nullptr)
			, right(nullptr)
			, parent(nullptr)
		{

		}

		friend std::ostream& operator<<(std::ostream& out, const NodeTree& nodeTree)
		{
			out << nodeTree.key << "|" << nodeTree.value;
			(nodeTree.color == NodeTree::EColor::Red) ? out << "(r)" : out << "(b)";

			return out;
		}

	public:
		T1 key;
		T2 value;

		EColor color;
		NodeTree* left, * right, * parent;
	};

	using Node = NodeTree<T1, T2>;

public:
	Map();
	Map(const Map& other);
	Map(Map&& other);

	~Map();

	void insert(const std::pair<T1, T2>& key_value);
	void erase(const T1& key);
	bool empty();
	void clear(Node*& x);
	void print();

	Map& operator=(const Map& other) noexcept;
	Map& operator=(Map&& other) noexcept;

	T2& operator[](const T1& key);

public:
	class MapIterator
	{
	public:
		MapIterator() : nodePtr(nullptr), tree(nullptr) {}

		bool operator==(const MapIterator& other) const
		{
			return tree = other.tree && nodePtr == other.nodePtr;
		}
		bool operator!=(const MapIterator& other) const
		{
			return tree != other.tree or nodePtr != other.nodePtr;
		}

		const std::pair<T1, T2> operator*() const
		{
			return std::make_pair(nodePtr->key, nodePtr->value);
		}
		const std::pair<T1, T2>* operator->() const
		{
			return new std::pair<T1, T2>(nodePtr->key, nodePtr->value);
		}

		MapIterator& operator++()
		{
			nodePtr = tree->successor(nodePtr);
			return *this;
		}
		MapIterator operator++(int)
		{
			auto it = *this;
			operator++();
			return it;
		}
		MapIterator& operator--()
		{
			nodePtr = tree->predecessor(nodePtr);
			return *this;
		}
		MapIterator operator--(int)
		{
			auto it = *this;
			operator--();
			return it;
		}

	private:
		friend class Map<T1, T2, Compare>;

		const Node* nodePtr;
		const Map<T1, T2, Compare>* tree;

		MapIterator(const Node* parent, const Map<T1, T2, Compare>* tree)
			: nodePtr(parent)
			, tree(tree)
		{

		}
	};

	MapIterator find(const T1& key) const
	{
		return MapIterator(search(key), this);
	}
	MapIterator begin() const
	{
		return MapIterator(begin(root), this);
	}
	MapIterator end() const
	{
		return MapIterator(nil, this);
	}

private:
	void insertHelper(Node* z);
	void transplant(Node* x, Node* y);
	void deleteHelper(Node*& x);

	void rotationLeft(Node* x);
	void rotationRight(Node* x);

	Node* search(const T1& key) const;
	Node* begin(Node* x) const;
	Node* end(Node* x) const;

	Node* predecessor(const Node* x) const;
	Node* successor(const Node* x) const;

	void printTree(Node* x, int space);
	Node* copyTree(Node* root, Node* nil) const;

private:
	Node* root;
	Node* nil;
};

#include "Map.cpp"

#endif