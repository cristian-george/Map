#include "Map.h"

template <class KeyType, class ValueType, class Compare>
Map<KeyType, ValueType, Compare>::Map()
	: m_nil(new Node)
{
	m_nil->Color = Node::EColor::Black;
	m_root = m_nil;
	m_root->Left = m_root->Right = m_root->Parent = m_nil;
}

template <class KeyType, class ValueType, class Compare>
Map<KeyType, ValueType, Compare>::Map(const Map& other) : m_nil(new Node)
{
	m_nil->Color = Node::EColor::Black;
	m_root = m_nil;
	m_root = copyTree(other.m_root, other.m_nil);
}

template <class KeyType, class ValueType, class Compare>
Map<KeyType, ValueType, Compare>::Map(Map&& other) : m_nil(other.m_nil), m_root(other.m_root)
{
	other->~Map();
}

template <class KeyType, class ValueType, class Compare>
Map<KeyType, ValueType, Compare>::~Map()
{
	clearTree(m_root);
	delete m_root;
}

template <class KeyType, class ValueType, class Compare>
void Map<KeyType, ValueType, Compare>::insert(const std::pair<KeyType, ValueType>& pair)
{
	if (search(pair.first) != m_nil)
		return;

	Node* x = m_root;
	Node* y = m_nil;
	Node* z = new Node(pair.first, pair.second);

	while (x != m_nil)
	{
		y = x;
		if (Compare()(z->Key, x->Key))
			x = x->Left;
		else
			x = x->Right;
	}

	z->Parent = y;

	if (y == m_nil)
		m_root = z;
	else
	{
		if (Compare()(z->Key, y->Key))
			y->Left = z;
		else
			y->Right = z;
	}

	z->Left = m_nil;
	z->Right = m_nil;
	z->Color = Node::EColor::Red;
	insertHelper(z);
}

template <class KeyType, class ValueType, class Compare>
void Map<KeyType, ValueType, Compare>::erase(const KeyType& key)
{
	Node* x;
	Node* y;
	Node* z = search(key);
	if (z != m_nil)
	{
		typename Node::EColor color = z->Color;
		if (z->Left == m_nil)
		{
			x = z->Right;
			transplant(z, x);
		}
		else
			if (z->Right == m_nil)
			{
				x = z->Left;
				transplant(z, x);
			}
			else
			{
				y = successor(z);
				color = y->Color;
				x = y->Right;
				if (y->Parent == z)
					x->Parent = y;
				else
				{
					transplant(y, x);
					y->Right = z->Right;
					y->Right->Parent = y;
				}
				transplant(z, y);
				y->Left = z->Left;
				y->Left->Parent = y;
				y->Color = z->Color;
			}

		delete z;
		if (color == Node::EColor::Black)
			deleteHelper(x);
	}
}

template <class KeyType, class ValueType, class Compare /*= std::less<T1>*/>
void Map<KeyType, ValueType, Compare>::clear()
{
	clearTree(m_root);
}

template <class KeyType, class ValueType, class Compare>
void Map<KeyType, ValueType, Compare>::print()
{
	printTree(m_root, 0);
	std::cout << std::endl << std::endl;
}

template <class KeyType, class ValueType, class Compare>
bool Map<KeyType, ValueType, Compare>::empty() const
{
	return (m_root == m_nil);
}

template <class KeyType, class ValueType, class Compare>
Map<KeyType, ValueType, Compare>& Map<KeyType, ValueType, Compare>::operator=(const Map& other) noexcept
{
	if (this != &other)
	{
		this->~Map();

		m_root = m_nil = new Node(0, 0, Node::EColor::Black);
		m_root->Left = m_root->Right = m_root->Parent = m_nil;

		m_root = copyTree(other.m_root, other.m_nil);
	}

	return *this;
}

template <class KeyType, class ValueType, class Compare>
Map<KeyType, ValueType, Compare>& Map<KeyType, ValueType, Compare>::operator=(Map&& other) noexcept
{
	std::swap(m_nil, other.m_nil);
	std::swap(m_root, other.m_root);

	return *this;
}

template <class KeyType, class ValueType, class Compare>
ValueType& Map<KeyType, ValueType, Compare>::operator[](const KeyType& key)
{
	insert(std::make_pair(key, ValueType()));
	return search(key)->Value;
}

template <class KeyType, class ValueType, class Compare>
void Map<KeyType, ValueType, Compare>::insertHelper(Node* z)
{
	while (z->Parent->Color == Node::EColor::Red)
	{
		if (z->Parent == z->Parent->Parent->Left)
		{
			Node* U = z->Parent->Parent->Right;
			if (U->Color == Node::EColor::Red)
			{
				z->Parent->Color = Node::EColor::Black;
				U->Color = Node::EColor::Black;
				z->Parent->Parent->Color = Node::EColor::Red;
				z = z->Parent->Parent;
			}
			else
			{
				if (z == z->Parent->Right)
				{
					z = z->Parent;
					rotationLeft(z);
				}
				z->Parent->Color = Node::EColor::Black;
				z->Parent->Parent->Color = Node::EColor::Red;
				rotationRight(z->Parent->Parent);
			}
		}
		else
			if (z->Parent == z->Parent->Parent->Right)
			{
				Node* U = z->Parent->Parent->Left;
				if (U->Color == Node::EColor::Red)
				{
					z->Parent->Color = Node::EColor::Black;
					U->Color = Node::EColor::Black;
					z->Parent->Parent->Color = Node::EColor::Red;
					z = z->Parent->Parent;
				}
				else
				{
					if (z == z->Parent->Left)
					{
						z = z->Parent;
						rotationRight(z);
					}
					z->Parent->Color = Node::EColor::Black;
					z->Parent->Parent->Color = Node::EColor::Red;
					rotationLeft(z->Parent->Parent);
				}
			}
	}

	m_root->Color = Node::EColor::Black;
}

template <class KeyType, class ValueType, class Compare>
void Map<KeyType, ValueType, Compare>::transplant(Node* x, Node* y)
{
	if (x->Parent == m_nil)
		m_root = y;
	else
	{
		if (x == x->Parent->Left)
			x->Parent->Left = y;
		else
			x->Parent->Right = y;
	}
	y->Parent = x->Parent;
}

template <class KeyType, class ValueType, class Compare>
void Map<KeyType, ValueType, Compare>::deleteHelper(Node*& x)
{
	Node* F;
	while (x != m_root && x->Color == Node::EColor::Black)
	{
		if (x == x->Parent->Left)
		{
			F = x->Parent->Right;
			if (F->Color == Node::EColor::Red)
			{
				F->Color = Node::EColor::Black;
				x->Parent->Color = Node::EColor::Red;
				rotationLeft(x->Parent);
				F = x->Parent->Right;
			}

			if (F->Left->Color == Node::EColor::Black && F->Right->Color == Node::EColor::Black)
			{
				F->Color = Node::EColor::Red;
				x = x->Parent;
			}
			else
			{
				if (F->Right->Color == Node::EColor::Black)
				{
					F->Left->Color = Node::EColor::Black;
					F->Color = Node::EColor::Red;
					rotationRight(F);
					F = x->Parent->Right;
				}

				F->Color = x->Parent->Color;
				x->Parent->Color = Node::EColor::Black;
				F->Right->Color = Node::EColor::Black;
				rotationLeft(x->Parent);
				x = m_root;
			}
		}
		else
			if (x == x->Parent->Right)
			{
				F = x->Parent->Left;
				if (F->Color == Node::EColor::Red)
				{
					F->Color = Node::EColor::Black;
					x->Parent->Color = Node::EColor::Red;
					rotationRight(x->Parent);
					F = x->Parent->Left;
				}

				if (F->Left->Color == Node::EColor::Black && F->Right->Color == Node::EColor::Black)
				{
					F->Color = Node::EColor::Red;
					x = x->Parent;
				}
				else
				{
					if (F->Left->Color == Node::EColor::Black)
					{
						F->Right->Color = Node::EColor::Black;
						F->Color = Node::EColor::Red;
						rotationLeft(F);
						F = x->Parent->Left;
					}

					F->Color = x->Parent->Color;
					x->Parent->Color = Node::EColor::Black;
					F->Left->Color = Node::EColor::Black;
					rotationRight(x->Parent);
					x = m_root;
				}
			}
	}
	x->Color = Node::EColor::Black;
}

template <class KeyType, class ValueType, class Compare>
void Map<KeyType, ValueType, Compare>::rotationLeft(Node* x)
{
	Node* y = x->Right;
	x->Right = y->Left;
	if (y->Left != m_nil)
		y->Left->Parent = x;

	y->Parent = x->Parent;
	if (x->Parent == m_nil)
		m_root = y;
	else
	{
		if (x == x->Parent->Left)
			x->Parent->Left = y;
		else
			x->Parent->Right = y;
	}

	y->Left = x;
	x->Parent = y;
}

template <class KeyType, class ValueType, class Compare>
void Map<KeyType, ValueType, Compare>::rotationRight(Node* x)
{
	Node* y = x->Left;
	x->Left = y->Right;
	if (y->Right != m_nil)
		y->Right->Parent = x;

	y->Parent = x->Parent;
	if (x->Parent == m_nil)
		m_root = y;
	else
	{
		if (x == x->Parent->Right)
			x->Parent->Right = y;
		else
			x->Parent->Left = y;
	}

	y->Right = x;
	x->Parent = y;
}

template <class KeyType, class ValueType, class Compare>
Map<KeyType, ValueType, Compare>::Node* Map<KeyType, ValueType, Compare>::search(const KeyType& key) const
{
	Node* x = m_root;

	while (x != m_nil && x->Key != key)
		if (Compare()(key, x->Key))
			x = x->Left;
		else
			x = x->Right;

	return x;
}

template <class KeyType, class ValueType, class Compare>
Map<KeyType, ValueType, Compare>::Node* Map<KeyType, ValueType, Compare>::begin(Node* x) const
{
	Node* y = x;
	if (y != m_nil)
		while (y->Left != m_nil)
			y = y->Left;
	return y;
}

template <class KeyType, class ValueType, class Compare>
Map<KeyType, ValueType, Compare>::Node* Map<KeyType, ValueType, Compare>::end(Node* x) const
{
	Node* y = x;
	if (y != m_nil)
		while (y->Right != m_nil)
			y = y->Right;
	return y;
}

template <class KeyType, class ValueType, class Compare>
Map<KeyType, ValueType, Compare>::Node* Map<KeyType, ValueType, Compare>::predecessor(const Node* x) const
{
	if (x->Left != m_nil)
		return end(x->Left);

	Node* y = x->Parent;
	while (y != m_nil && x == y->Left)
	{
		x = y;
		y = y->Parent;
	}

	return y;
}

template <class KeyType, class ValueType, class Compare>
Map<KeyType, ValueType, Compare>::Node* Map<KeyType, ValueType, Compare>::successor(const Node* x) const
{
	if (x->Right != m_nil)
		return begin(x->Right);

	Node* y = x->Parent;
	while (y != m_nil && x == y->Right)
	{
		x = y;
		y = y->Parent;
	}

	return y;
}

template <class KeyType, class ValueType, class Compare>
void Map<KeyType, ValueType, Compare>::printTree(Node* x, int space)
{
	if (x == m_nil)
		return;
	space += 10;

	printTree(x->Right, space);
	std::cout << std::endl;

	for (int index = 10; index < space; ++index)
		std::cout << " ";

	std::cout << *x;
	printTree(x->Left, space);
}

template <class KeyType, class ValueType, class Compare>
Map<KeyType, ValueType, Compare>::Node* Map<KeyType, ValueType, Compare>::copyTree(Node* root, Node* nil) const
{
	if (root == nil)
		return this->m_nil;

	Node* node = new Node(root->Key, root->Value, root->Color);
	node->Left = node->Right = node->Parent = this->m_nil;

	node->Left = copyTree(root->Left, nil);
	node->Left->Parent = node;

	node->Right = copyTree(root->Right, nil);
	node->Right->Parent = node;

	return node;
}

template <class KeyType, class ValueType, class Compare>
void Map<KeyType, ValueType, Compare>::clearTree(Node*& x)
{
	if (x == m_nil)
		return;

	clearTree(x->Left);
	clearTree(x->Right);

	delete x;
	x = m_nil;
}