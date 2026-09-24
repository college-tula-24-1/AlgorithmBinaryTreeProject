#pragma once
#include <stack>

// type Color for Node
enum class Color
{
	Red,
	Black
};

// Node of Tree
template<typename T>
struct Node
{
	T value;
	Color color{ Color::Red };
	
	Node<T>* parent{};
	Node<T>* left{};
	Node<T>* right{};
};

// Interface for compare value
template<typename T>
class IComparator
{
public:
	virtual int Compare(T left, T right) = 0;
	virtual int operator()(T left, T right) = 0;
};

// Declaration Iterator
template<typename T>
class Iterator;


// BinaryTree Api
template<typename T>
class BinaryTree
{
	Node<T>* root{};
	size_t size{};

	IComparator<T> comparator;

public:
	BinaryTree(IComparator<T>* comporator = nullptr)
		: root{}, size{}
	{
		if (comporator)
			this->comparator = comparator;
	}
	~BinaryTree();

	Node<T>* CreateNode(Node<T>* parent, T value);

	void InsertLoop(T value);
	void InsertReq(T value, Node<T>* parent = nullptr);

	void RemoveNode(Node<T>* node);
	void RemoveBranch(Node<T>* node);

	Node<T>* Find(T value);

	Node<T>* Min(Node<T>* parent = nullptr);
	Node<T>* Max(Node<T>* parent = nullptr);

	void RotateRight(Node<T>* node);
	void RotateLeft(Node<T>* node);
	void SwapCopor(Node<T>* parent);

	size_t Size() const;
	Node<T>* Root() const;

	Iterator<T> GetIterator();

	bool Contains(T value);

	void Print(Node<T>* node = nullptr);
};

// Iterator Api
template<typename T>
class Iterator
{
	BinaryTree<T>* tree;
	Node<T>* current;
	std::stack<Node<T>*> nodes;
	
	Node<T>* Min(Node<T>* parent = nullptr);

public:
	Iterator(BinaryTree<T>* tree) : tree{ tree }
	{
		Reset();
	}

	void Reset();
	void Next();
	Node<T>* Current();
	bool IsEnd();
};



// BinaryTree Implementation
template<typename T>
inline BinaryTree<T>::~BinaryTree()
{
	RemoveBranch(root);
}

template<typename T>
inline Node<T>* BinaryTree<T>::CreateNode(Node<T>* parent, T value)
{
	Node<T>* node = new Node<T>;
	node->value = value;
	node->parent = parent;
	return node;
}

template<typename T>
inline void BinaryTree<T>::InsertLoop(T value)
{
	if (!root)
	{
		root = CreateNode(nullptr, value);
		root->color = Color::Black;
		size++;
		return;
	}

	Node<T>* nodeCurrent{ root };
	Node<T>* nodeParent{};

	while (nodeCurrent)
	{
		nodeParent = nodeCurrent;
		if (comparator(value, nodeCurrent->value) < 0)
			nodeCurrent = nodeCurrent->left;
		else
			nodeCurrent = nodeCurrent->right;
	}

	nodeCurrent = CreateNode(nodeParent, value);
	if (comparator->Compare(nodeCurrent->value, nodeParent->value) < 0)
		nodeParent->left = nodeCurrent;
	else
		nodeParent->right = nodeCurrent;

	size++;
}

template<typename T>
inline void BinaryTree<T>::InsertReq(T value, Node<T>* parent)
{
	if (!root)
	{
		root = CreateNode(nullptr, value);
		root->color = Color::Black;
		size++;
		return;
	}

	if (!parent)
		parent = root;

	if (comparator->Compare(value, parent->value) < 0)
	{
		if (parent->left)
			InsertReq(value, parent->left);
		else
			parent->left = CreateNode(parent, value);
	}
	else
	{
		if (parent->right)
			InsertReq(value, parent->right);
		else
			parent->right = CreateNode(parent, value);
	}
	size++
		

}

template<typename T>
inline void BinaryTree<T>::RemoveNode(Node<T>* node)
{
	// remove leaf
	if (!node->left && !node->right)
	{
		if (node == root)
			root = nullptr;
		else
		{
			if (node->parent->left == node)
				node->parent->left = nullptr;
			else
				node->parent->right = nullptr;
		}
		delete node;
		size--;
		return;
	}

	// one child
	if ((bool)node->left ^ (bool)node->right)
	{
		Node<T>* child{ node->left ? node->left : node->right };

		if (node == root)
			root = child;
		else
		{
			if (node == node->parent->left)
				node->parent->left = child;
			else
				node->parent->right = child;
		}
		delete node;
		size--;
		return;
	}

	Node<T>* minRight{ Min(node->right) };
	node->value = minRight->value;
	RemoveNode(minRight);
}

template<typename T>
inline void BinaryTree<T>::RemoveBranch(Node<T>* node)
{
	if (node->left)
		RemoveBranch(node->left);

	if (node->right)
		RemoveBranch(node->right);

	if (node != root)
	{
		if (node->parent->left == node)
			node->parent->left = nullptr;
		else
			node->parent->right = nullptr;
	}

	delete node;
}

template<typename T>
inline Node<T>* BinaryTree<T>::Find(T value)
{
	Node<T>* node{ root };
	while (node)
	{
		if (node->value == value)
			break;
		
		if (node->value > value)
			node = node->left;
		else
			node = node->right;
	}

	return node;
}

template<typename T>
inline Node<T>* BinaryTree<T>::Min(Node<T>* parent)
{
	if (!parent)
		parent = root;

	while (parent->left)
		parent = parent->left;

	return parent;
}

template<typename T>
inline Node<T>* BinaryTree<T>::Max(Node<T>* parent)
{
	if (!parent)
		parent = root;

	while (parent->right)
		parent = parent->right;

	return parent;
}

template<typename T>
inline void BinaryTree<T>::RotateLeft(Node<T>* node)
{
	Node<T>* B{ node };
	Node<T>* A{ node->parent };

	B->parent = A->parent;

	if (A != root)
	{
		if (A->parent->left == A)
			A->parent->left = B;
		else
			A->parent->right = B;
	}
	else
		root = B;

	A->parent = B;
	A->right = B->left;

	if (A->right)
		A->right->parent = A;
	
	B->left = A;

	A->color = Color::Red;
	B->color = Color::Black;
}

template<typename T>
inline void BinaryTree<T>::SwapCopor(Node<T>* parent)
{
	parent->left->color = parent->color;
	parent->right->color = parent->color;
	parent->color = (Color)(!(bool)parent->color);
}

template<typename T>
inline size_t BinaryTree<T>::Size() const
{
	return size;
}

template<typename T>
inline Node<T>* BinaryTree<T>::Root() const
{
	return root;
}

template<typename T>
inline bool BinaryTree<T>::Contains(T value)
{
	return Find(value);
}
