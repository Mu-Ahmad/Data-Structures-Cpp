#include <iostream>

template <typename K, typename D>
class AVL {
private:
	class TreeNode	{
	public:
		K key;
		D data;
		TreeNode *left, *right;
		size_t height;
		TreeNode(const K& key, const D& data)
			: key(key), data(data), left(nullptr), right(nullptr)
		{}
	};

	TreeNode * m_head;
public:
	AVL(): m_head(nullptr) {}

	D& find(const K& key) const {
		TreeNode*& node = _find(key, m_head);

		if (node == nullptr) throw std::runtime_error("error: trying to access key that does not exist");

		return node->data;
	}

	void insert(const K& key, const D& data) {
		return _find_and_insert(key, data, m_head);
	}

	void remove(const K& key) {
		return _find_and_remove(key, m_head);
	}

	bool contains(const K& key) const {
		// This is just like "find" but when the item is not found, we just return
		// false instead of throwing an exception. When found, return true.
		TreeNode*& node = _find(key, m_head);

		return node != nullptr;
	}

	bool isEmpty() const {return m_head == nullptr;}

	void printInOrder() const {return _printInOrder(m_head);}

	void tracePath(const K& key) {
		return _tracePath(key, m_head);
	}

	~AVL() {
		while (m_head) {
			// As long as the head pointer isn't null, we can just look at
			// what the key is and call remove based on that.
			remove(m_head->key);
		}
	}
private:

	void _tracePath(const K& key, TreeNode*& cur) {
		if (cur == nullptr) {
			std::cout << "Unable to find: " << key << '\n';
			return;
		}
		if (cur->key == key) {
			std::cout << "Key Has been found " << key << '\n';
			return;
		}
		if (key > cur->key) {
			std::cout << cur->key << "----> ";
			return _tracePath(key, cur->right);
		} else {
			std::cout << cur->key << "----> ";
			return _tracePath(key, cur->left);
		}
	}

	TreeNode*& _find(const K& key, TreeNode*& cur) const {
		// [Base case 1: when the key is not found]
		if (cur == nullptr) return cur;

		// [Base case 2: when the key is found]
		if (cur->key == key) return cur;

		//[Recursive Cases]
		if (key > cur->key) return _find(key, cur->right);
		else return _find(key, cur->left);
	}

	void _iop_remove(TreeNode*& cur, TreeNode*& iop) {
		if (iop->right != nullptr) {
			//Search Right and remove
			_iop_remove(cur, iop->right);
			// Ensure balance and update height of this ancestor
			// on the way back up the call stack:
			_ensureBalance(iop);
			return;
		}

		//Base Case
		K temp_key = iop->key;
		D temp_data = iop->data;

		iop->key = cur->key;
		iop->data = cur->data;


		cur->key = temp_key;
		cur->data = temp_data;

		_remove(iop);
		return;
	}

	void _remove(TreeNode*& node) {
		// Zero child remove:
		if (node->left == nullptr and node->right == nullptr) {
			delete node;
			node = nullptr;
			return;
		}
		// One-child (left) remove
		else if (node->left != nullptr and node->right == nullptr) {
			// Similar to the previous case, except that we need to remap the "node"
			// pointer to point to the node's child, so that the parent of the node
			// being deleted will retain its connection to the rest of the tree
			// below this point.
			TreeNode* temp = node;
			node = node->left;
			delete temp;
			return;
		}
		// One-child (right) remove
		else if (node->left == nullptr and node->right != nullptr) {
			// Similar to last case
			TreeNode* temp = node;
			node = node->right;
			delete temp;
			return;
		}
		// Two-child remove
		// We need to be little more carefull in this case
		else {
			_iop_remove(node, node->left);
			return;
		}
	}

	void _find_and_remove(const K& key, TreeNode*& cur) {
		// [Base case 1: Key not found]
		if (cur == nullptr) throw std::runtime_error("error: trying to delete a non-existent key");

		// [Base case 2: Key is found(remove it)]
		if (key == cur->key) return _remove(cur);

		// [Recursive Cases]
		if ( key > cur->key) {
			//Search Right and remove
			_find_and_remove(key, cur->right);
			// Ensure balance and update height of this ancestor
			// on the way back up the call stack:
			_ensureBalance(cur);
			return;
		} else {
			//Search left and remove
			_find_and_remove(key, cur->left);
			// Ensure balance and update height of this ancestor
			// on the way back up the call stack:
			_ensureBalance(cur);
		}
	}

	void _find_and_insert(const K& key, const D& data, TreeNode*& cur) {
		// [Base Case 1: Found the empty space to insert the key value pair]
		if (cur == nullptr) {
			cur = new TreeNode(key, data);
			return;
		}

		// [Base Case 2: Key Already Present]
		if (cur->key == key) throw std::runtime_error("error: key already present!");

		// [Recursive Cases]
		if (key > cur->key) {
			// Search right and insert
			_find_and_insert(key, data, cur->right);
			// On the way back up, ensure the balance of this node
			_ensureBalance(cur);
			return;
		} else {
			// Search left and insert
			_find_and_insert(key, data, cur->left);
			// On the way back up, ensure the balance of this node
			_ensureBalance(cur);
			return;
		}
	}

	// A wrapper for checking the height of a subtree.
	int _get_height(TreeNode*& node) const {
		if (!node) {
			// A non-existent node has a height of 1
			return -1;
		}
		else {
			// We assume that an existing node already has an updated height
			return node->height;
		}
	}

	// Update the height of the specified node, based on the existing
	// heights of its children, which we assume are correct. (This function
	// is not recursive. It can do a fast update of the specified node but
	// this relies on the node's children already having correct, updated
	// heights.)
	void _updateHeight(TreeNode*& cur) {
		// If the node is nullptr, then do nothing and return.
		if (cur == nullptr) return;
		// Otherwise update the height to be one more than the greater of the
		// two children's heights. The get_height function safely handles the
		// case where a child is just nullptr (no node exists), in which case
		// its implicit height is -1.
		cur->height = 1 + std::max(_get_height(cur->left), _get_height(cur->right));
		return;
	}

	// _get_balance_factor: A helper function for safely calculating the balance
	// factor of the node that is passed as the argument.
	int _get_balance_factor(TreeNode*& node) const {
		if (!node) {
			// A non-existent node has a balance factor of 0
			return 0;
		}
		else {
			// Calculate the balance factor safely and return it.
			return _get_height(node->right) - _get_height(node->left);
		}
	}

	// This is the single most important function in this AVL implementation.
	// Ensure that the balance factor of specified node has magnitude
	// no greater than 1. This calls rotation functions as necessary to
	// rebalance the subtree rooted here. It also records the updated height
	// in the node. These changes need to cascade upward, so after we call
	// this function, we need to make sure that it also gets called on those
	// nodes on the path of ancestry up towards the root node.
	void _ensureBalance(TreeNode*& cur) {
		// Base case for safety: do nothing if cur is nullptr.
		if (!cur) return;

		const int initial_balance = _get_balance_factor(cur);

		// Check if the current node is not in balance. If it is not, then we check
		// the balance in the direction of the imbalance, and use that information
		// to choose the correct rotation to apply

		if (initial_balance == -2) { //our tree is left heavy
			// To find out if we have stick or elbow at hand
			const int l_balance = _get_balance_factor(cur->left);
			if (l_balance == -1 || l_balance == 0) {
				// Repairing stick
				_rotateRight(cur);
			} else if (l_balance == 1) {
				// Repairing elbow
				_rotateLeftRight(cur);
			}
		}	else if (initial_balance == 2) { //our tree is left heavy
			const int r_balance = _get_balance_factor(cur->right);
			// To find out if we have stick or elbow at hand
			if (r_balance == 1 || r_balance == 0) {
				// Repairing stick
				_rotateLeft(cur);
			} else if (r_balance == -1) {
				// Repairing elbow
				_rotateRightLeft(cur);
			}
		}

		// If nodes rotated anywhere underneath, their heights were updated.
		// Then we need to update this node's height too.
		// (Note that we do this regardless of whether the above conditional
		// section did a rotation here or not. That's because another function
		// may have called "_ensureBalance" as part of a recursive process,
		// assuming that at the very least, this function would update the height
		// of cur. That would be necessary if nodes were rotated further beneath
		// this point in the tree. So to fulfill these assumptions, we make sure
		// to update the height of cur here.)
		_updateHeight(cur);
	}

	// These functions perform the specified balancing rotation on the
	// subtree that is rooted at the specified node.
	// Refer to the implementations of _rotateLeft and _rotateRightLeft
	// for more details. The other functions are similar.
	void _rotateLeft(TreeNode*& cur) {
		// If these error conditions arise, something already went wrong before
		// this function was called.
		if (!cur) {
			throw std::runtime_error("ERROR: _rotateLeft called on nullptr");
		}
		if (!cur->right) {
			throw std::runtime_error("ERROR: _rotateLeft: right child is nullptr");
		}

		// x points to the node that was originally the subtree root.
		TreeNode* x = cur;
		// y points to the node that was originally the right child.
		TreeNode* y = cur->right;
		// z points to the node that was originally the left child of the right child.
		//  if present, is simply the root of that subtree.)
		TreeNode* z = cur->right->left;

		// Let node x's new right child be the old left child of node y.
		x->right = z;
		// Node y's new left child is x. This puts node y on top.
		y->left = x;
		// Since cur is the original tree node pointer that points to the root
		// of this subtree, we need it to now point to the new root of the
		// subtree, which is node y.
		cur = y;

		// Nodes x and y have changed heights, but z should not have changed height.
		// When we update x and y, it's important to update x first, since it is
		// now the lower of the two, and y's update will depend upon it.
		_updateHeight(x);
		_updateHeight(y);
	}

	void _rotateRightLeft(TreeNode*& cur) {
		// We have to be careful about the distinction between making extra
		// temporary copies of pointers to node addresses here vs. operating
		// directly on the captive child pointers that are in the nodes we're
		// dealing with. We need to be sure to actually rewire the node
		// connections that are in the tree by changing the actual pointers,
		// not simply making copies of pointers and doing things to the copies.
		// In this case, all it really means is we must call _rotateLeft directly
		// on the cur pointer that was passed by reference.

		if (!cur) {
			throw std::runtime_error("ERROR: _rotateRightLeft called on nullptr");
		}

		// Perform a right rotation on the right subtree under cur.
		_rotateRight(cur->right);

		// Perform a left rotation on cur.
		_rotateLeft(cur);

	}

	void _rotateRight(TreeNode*& cur) {
		// This implementation is a mirror image of _rotateLeft.

		if (!cur) {
			throw std::runtime_error("ERROR: _rotateRight called on nullptr");
		}
		if (!cur->left) {
			throw std::runtime_error("ERROR: _rotateRight: left child is nullptr");
		}

		TreeNode* x = cur;
		TreeNode* y = cur->left;
		TreeNode* z = cur->left->right;

		x->left = z;
		y->right = x;
		cur = y;

		_updateHeight(x);
		_updateHeight(y);
	}

	void _rotateLeftRight(TreeNode*& cur) {
		// Similar to _rotateRightLeft

		if (!cur) {
			throw std::runtime_error("ERROR: _rotateLeftRight called on nullptr");
		}

		_rotateLeft(cur->left);

		_rotateRight(cur);
	}

	void _printInOrder(TreeNode* node) const {
		// Base case:
		if (node == nullptr) {
			std::cout << " ";
			return;
		}

		// Recurse left:
		_printInOrder(node->left);
		// Print this node:
		std::cout << "[" << _get_balance_factor(node) << " ; " << node->key << " : " << node->data << "]";
		// Recurse right:
		_printInOrder(node->right);
		return;
	}
};

int main() {
	AVL<int, int> avlDictionary;
	avlDictionary.insert(1, 1);
	avlDictionary.insert(2, 2);
	avlDictionary.insert(3, 3);
	avlDictionary.insert(4, 4);
	avlDictionary.insert(5, 5);
	avlDictionary.insert(6, 6);
	avlDictionary.insert(7, 7);
	avlDictionary.insert(8, 8);
	avlDictionary.insert(9, 9);
	avlDictionary.insert(10, 10);
	avlDictionary.insert(11, 11);
	avlDictionary.insert(12, 12);
	avlDictionary.insert(13, 13);
	avlDictionary.insert(14, 14);
	avlDictionary.insert(15, 15);
	avlDictionary.insert(16, 16);
	avlDictionary.insert(17, 17);
	avlDictionary.insert(18, 18);
	avlDictionary.insert(19, 19);
	avlDictionary.insert(20, 20);
	avlDictionary.printInOrder();
	std::cout << '\n';
	avlDictionary.tracePath(21);
	avlDictionary.tracePath(-1);
	avlDictionary.remove(7);
	avlDictionary.remove(8);
	avlDictionary.tracePath(-1);
	avlDictionary.tracePath(21);
	avlDictionary.printInOrder();
	avlDictionary.remove(9);
	avlDictionary.remove(18);
	std::cout << '\n';
	avlDictionary.printInOrder();
	return 0;
}