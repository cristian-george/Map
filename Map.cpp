#include "Map.h"

template <class T1, class T2, class Compare>
Map<T1, T2, Compare>::Map()
	: nil(new Node)
{
	nil->color = Node::EColor::Black;
	root = nil;
	root->left = root->right = root->parent = nil;
}

template <class T1, class T2, class Compare>
Map<T1, T2, Compare>::Map(const Map& other) : nil(new Node)
{
	nil->color = Node::EColor::Black;
	root = nil;
	root = copyTree(other.root, other.nil);
}

template <class T1, class T2, class Compare>
Map<T1, T2, Compare>::Map(Map&& other) : nil(other.nil), root(other.root)
{
	other.nil = nullptr;
	other.root = nullptr;
}

template <class T1, class T2, class Compare>
Map<T1, T2, Compare>::~Map()
{
	clear(root);
	delete root;
}

template <class T1, class T2, class Compare>
void Map<T1, T2, Compare>::insert(const std::pair<T1, T2>& key_value)
{
	if (search(key_value.first) != nil)
		return;

	Node* x = root;
	Node* y = nil;
	Node* z = new Node(key_value.first, key_value.second);

	while (x != nil)
	{
		y = x;
		if (Compare()(z->key, x->key))
			x = x->left;
		else
			x = x->right;
	}

	z->parent = y;

	if (y == nil)
		root = z;
	else
	{
		if (Compare()(z->key, y->key))
			y->left = z;
		else
			y->right = z;
	}

	z->left = nil;
	z->right = nil;
	z->color = Node::EColor::Red;
	insertHelper(z);
}

template <class T1, class T2, class Compare>
void Map<T1, T2, Compare>::erase(const T1& key)
{
	Node* x = new Node;
	Node* y = new Node;
	Node* z = search(key);
	if (z != nil)
	{
		typename Node::EColor color = z->color;
		if (z->left == nil)
		{
			x = z->right;
			transplant(z, x);
		}
		else
			if (z->right == nil)
			{
				x = z->left;
				transplant(z, x);
			}
			else
			{
				y = successor(z);
				color = y->color;
				x = y->right;
				if (y->parent == z)
					x->parent = y;
				else
				{
					transplant(y, x);
					y->right = z->right;
					y->right->parent = y;
				}
				transplant(z, y);
				y->left = z->left;
				y->left->parent = y;
				y->color = z->color;
			}

		delete z;
		if (color == Node::EColor::Black)
			deleteHelper(x);
	}
}

template <class T1, class T2, class Compare>
void Map<T1, T2, Compare>::clear(Node*& x)
{
	if (x == nil)
		return;

	clear(x->left);
	clear(x->right);

	delete x;
	x = nil;
}

template <class T1, class T2, class Compare>
void Map<T1, T2, Compare>::print()
{
	printTree(root, 0);
	std::cout << std::endl << std::endl;
}

template <class T1, class T2, class Compare>
bool Map<T1, T2, Compare>::empty()
{
	return (root == nil);
}

template <class T1, class T2, class Compare>
Map<T1, T2, Compare>& Map<T1, T2, Compare>::operator=(const Map& other) noexcept
{
	this->~Map();

	root = nil = new Node(0, 0, Node::EColor::Black);
	root->left = root->right = root->parent = nil;

	root = copyTree(other.root, other.nil);

	return *this;
}

template <class T1, class T2, class Compare>
Map<T1, T2, Compare>& Map<T1, T2, Compare>::operator=(Map&& other) noexcept
{
	std::swap(nil, other.nil);
	std::swap(root, other.root);

	return *this;
}

template <class T1, class T2, class Compare>
T2& Map<T1, T2, Compare>::operator[](const T1& key)
{
	if (search(key) == nil)
		insert(std::make_pair(key, 0));

	return search(key)->value;
}

template <class T1, class T2, class Compare>
void Map<T1, T2, Compare>::insertHelper(Node* z)
{
	while (z->parent->color == Node::EColor::Red)
	{
		if (z->parent == z->parent->parent->left)
		{
			Node* U = z->parent->parent->right;
			if (U->color == Node::EColor::Red)
			{
				z->parent->color = Node::EColor::Black;
				U->color = Node::EColor::Black;
				z->parent->parent->color = Node::EColor::Red;
				z = z->parent->parent;
			}
			else
			{
				if (z == z->parent->right)
				{
					z = z->parent;
					rotationLeft(z);
				}
				z->parent->color = Node::EColor::Black;
				z->parent->parent->color = Node::EColor::Red;
				rotationRight(z->parent->parent);
			}
		}
		else
			if (z->parent == z->parent->parent->right)
			{
				Node* U = z->parent->parent->left;
				if (U->color == Node::EColor::Red)
				{
					z->parent->color = Node::EColor::Black;
					U->color = Node::EColor::Black;
					z->parent->parent->color = Node::EColor::Red;
					z = z->parent->parent;
				}
				else
				{
					if (z == z->parent->left)
					{
						z = z->parent;
						rotationRight(z);
					}
					z->parent->color = Node::EColor::Black;
					z->parent->parent->color = Node::EColor::Red;
					rotationLeft(z->parent->parent);
				}
			}
	}

	root->color = Node::EColor::Black;
}

template <class T1, class T2, class Compare>
void Map<T1, T2, Compare>::transplant(Node* x, Node* y)
{
	if (x->parent == nil)
		root = y;
	else
	{
		if (x == x->parent->left)
			x->parent->left = y;
		else
			x->parent->right = y;
	}
	y->parent = x->parent;
}

template <class T1, class T2, class Compare>
void Map<T1, T2, Compare>::deleteHelper(Node*& x)
{
	Node* F;
	while (x != root && x->color == Node::EColor::Black)
	{
		if (x == x->parent->left)
		{
			F = x->parent->right;
			if (F->color == Node::EColor::Red)
			{
				F->color = Node::EColor::Black;
				x->parent->color = Node::EColor::Red;
				rotationLeft(x->parent);
				F = x->parent->right;
			}

			if (F->left->color == Node::EColor::Black && F->right->color == Node::EColor::Black)
			{
				F->color = Node::EColor::Red;
				x = x->parent;
			}
			else
			{
				if (F->right->color == Node::EColor::Black)
				{
					F->left->color = Node::EColor::Black;
					F->color = Node::EColor::Red;
					rotationRight(F);
					F = x->parent->right;
				}

				F->color = x->parent->color;
				x->parent->color = Node::EColor::Black;
				F->right->color = Node::EColor::Black;
				rotationLeft(x->parent);
				x = root;
			}
		}
		else
			if (x == x->parent->right)
			{
				F = x->parent->left;
				if (F->color == Node::EColor::Red)
				{
					F->color = Node::EColor::Black;
					x->parent->color = Node::EColor::Red;
					rotationRight(x->parent);
					F = x->parent->left;
				}

				if (F->left->color == Node::EColor::Black && F->right->color == Node::EColor::Black)
				{
					F->color = Node::EColor::Red;
					x = x->parent;
				}
				else
				{
					if (F->left->color == Node::EColor::Black)
					{
						F->right->color = Node::EColor::Black;
						F->color = Node::EColor::Red;
						rotationLeft(F);
						F = x->parent->left;
					}

					F->color = x->parent->color;
					x->parent->color = Node::EColor::Black;
					F->left->color = Node::EColor::Black;
					rotationRight(x->parent);
					x = root;
				}
			}
	}
	x->color = Node::EColor::Black;
}

template <class T1, class T2, class Compare>
void Map<T1, T2, Compare>::rotationLeft(Node* x)
{
	Node* y = x->right;
	x->right = y->left;
	if (y->left != nil)
		y->left->parent = x;

	y->parent = x->parent;
	if (x->parent == nil)
		root = y;
	else
	{
		if (x == x->parent->left)
			x->parent->left = y;
		else
			x->parent->right = y;
	}

	y->left = x;
	x->parent = y;
}

template <class T1, class T2, class Compare>
void Map<T1, T2, Compare>::rotationRight(Node* x)
{
	Node* y = x->left;
	x->left = y->right;
	if (y->right != nil)
		y->right->parent = x;

	y->parent = x->parent;
	if (x->parent == nil)
		root = y;
	else
	{
		if (x == x->parent->right)
			x->parent->right = y;
		else
			x->parent->left = y;
	}

	y->right = x;
	x->parent = y;
}

template <class T1, class T2, class Compare>
Map<T1, T2, Compare>::Node* Map<T1, T2, Compare>::search(const T1& key) const
{
	Node* x = root;

	while (x != nil && x->key != key)
		if (Compare()(key, x->key))
			x = x->left;
		else
			x = x->right;

	return x;
}

template <class T1, class T2, class Compare>
Map<T1, T2, Compare>::Node* Map<T1, T2, Compare>::begin(Node* x) const
{
	Node* y = x;
	if (y != nil)
		while (y->left != nil)
			y = y->left;
	return y;
}

template <class T1, class T2, class Compare>
Map<T1, T2, Compare>::Node* Map<T1, T2, Compare>::end(Node* x) const
{
	Node* y = x;
	if (y != nil)
		while (y->right != nil)
			y = y->right;
	return y;
}

template <class T1, class T2, class Compare>
Map<T1, T2, Compare>::Node* Map<T1, T2, Compare>::predecessor(const Node* x) const
{
	Node* y = nil;

	if (x->left != nil)
		y = end(x->left);
	else
	{
		y = x->parent;
		while (y != nil && x == y->left)
		{
			x = y;
			y = y->parent;
		}
	}

	return y;
}

template <class T1, class T2, class Compare>
Map<T1, T2, Compare>::Node* Map<T1, T2, Compare>::successor(const Node* x) const
{
	Node* y = nil;

	if (x->right != nil)
		y = begin(x->right);
	else
	{
		y = x->parent;
		while (y != nil && x == y->right)
		{
			x = y;
			y = y->parent;
		}
	}

	return y;
}

template <class T1, class T2, class Compare>
void Map<T1, T2, Compare>::printTree(Node* x, int space)
{
	if (x == nil)
		return;
	space += 10;

	printTree(x->right, space);
	std::cout << std::endl;

	for (int index = 10; index < space; ++index)
		std::cout << " ";

	std::cout << *x;
	printTree(x->left, space);
}

template <class T1, class T2, class Compare>
Map<T1, T2, Compare>::Node* Map<T1, T2, Compare>::copyTree(Node* root, Node* nil) const
{
	if (root == nil)
		return this->nil;

	Node* node = new Node(root->key, root->value, root->color);
	node->left = node->right = node->parent = this->nil;

	node->left = copyTree(root->left, nil);
	node->left->parent = node;

	node->right = copyTree(root->right, nil);
	node->right->parent = node;

	return node;
}