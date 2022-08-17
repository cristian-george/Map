#pragma once

#ifndef _MAP_H_
#define _MAP_H_

#include <iostream>

template <class KeyType, class ValueType, class Compare = std::less<KeyType>>
class Map
{
private:
	struct Node
	{
		enum class EColor
		{
			Red = 0,
			Black
		};

		Node(const KeyType& key = KeyType(), const ValueType& value = ValueType(), EColor color = EColor::Red)
			: Key(key)
			, Value(value)
			, Color(color)
			, Left(nullptr)
			, Right(nullptr)
			, Parent(nullptr)
		{

		}

		friend std::ostream& operator<<(std::ostream& out, const Node& node)
		{
			out << node.Key << "|" << node.Value;
			(node.Color == EColor::Red) ? out << "(r)" : out << "(b)";

			return out;
		}

		KeyType Key;
		ValueType Value;

		EColor Color;
		Node* Left, * Right, * Parent;
	};

public:
	Map();
	Map(const Map& other);
	Map(Map&& other);

	~Map();

	void insert(const std::pair<KeyType, ValueType>& key_value);
	void erase(const KeyType& key);
	bool empty() const;
	void clear();
	void print();

	Map& operator=(const Map& other) noexcept;
	Map& operator=(Map&& other) noexcept;

	ValueType& operator[](const KeyType& key);

public:
	class Iterator
	{
	public:
		Iterator()
			: m_nodePtr(nullptr)
			, m_treePtr(nullptr)
		{

		}

		bool operator==(const Iterator& other) const
		{
			return m_treePtr = other.m_treePtr && m_nodePtr == other.m_nodePtr;
		}
		bool operator!=(const Iterator& other) const
		{
			return m_treePtr != other.m_treePtr or m_nodePtr != other.m_nodePtr;
		}

		const std::pair<KeyType, ValueType> operator*() const
		{
			return std::make_pair(m_nodePtr->Key, m_nodePtr->Value);
		}
		const std::pair<KeyType, ValueType>* operator->() const
		{
			return new std::pair<KeyType, ValueType>(m_nodePtr->Key, m_nodePtr->Value);
		}

		Iterator& operator++()
		{
			m_nodePtr = m_treePtr->successor(m_nodePtr);
			return *this;
		}
		Iterator operator++(int)
		{
			auto it = *this;
			operator++();
			return it;
		}
		Iterator& operator--()
		{
			m_nodePtr = m_treePtr->predecessor(m_nodePtr);
			return *this;
		}
		Iterator operator--(int)
		{
			auto it = *this;
			operator--();
			return it;
		}

	private:
		friend class Map<KeyType, ValueType, Compare>;

		const Node* m_nodePtr;
		const Map<KeyType, ValueType, Compare>* m_treePtr;

		Iterator(const Node* nodePtr, const Map<KeyType, ValueType, Compare>* treePtr)
			: m_nodePtr(nodePtr)
			, m_treePtr(treePtr)
		{

		}
	};

	Iterator find(const KeyType& key) const
	{
		return Iterator(search(key), this);
	}
	Iterator begin() const
	{
		return Iterator(begin(m_root), this);
	}
	Iterator end() const
	{
		return Iterator(m_nil, this);
	}

private:
	void insertHelper(Node* z);
	void transplant(Node* x, Node* y);
	void deleteHelper(Node*& x);

	void rotationLeft(Node* x);
	void rotationRight(Node* x);

	Node* search(const KeyType& key) const;
	Node* begin(Node* x) const;
	Node* end(Node* x) const;

	Node* predecessor(const Node* x) const;
	Node* successor(const Node* x) const;

	void printTree(Node* x, int space);
	Node* copyTree(Node* root, Node* nil) const;

	void clearTree(Node*& x);

private:
	Node* m_root;
	Node* m_nil;
};

#include "Map.cpp"

#endif