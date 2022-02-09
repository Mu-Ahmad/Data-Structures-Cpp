#include <iostream>
#include <utility>


template <typename K, typename D>
class Dictionary {
private:
	class TreeNode {
	public:
		K key;
		D data;

		TreeNode *left, *right;
		TreeNode(const K& key, const D& data)
			: key(key), data(data), left(nullptr), right(nullptr) { }
	};

	TreeNode *m_head;

	TreeNode*& _find(const K& key, TreeNode*& cur) const {

		// [Base case 1: When the key is not found]
		// cur will be nullptr if the tree is empty, or if we descend below the
		// lowest level (leaves) without finding the key. Then we return nullptr and the
		// outer "find" function (which calls "_find") will report that as an
		// error. Or, if we were calling insert, then the pointer returned is the
		// position where the item should be placed.
		//   Note: The "cur" we return in this case is equal to nullptr, but
		// it's important to write "return cur;" and not "return nullptr;" since
		// this function returns by reference. We specifically want to return the
		// pointer at this position we found. This is true whether we want to
		// replace it, as when we're doing an insertion, or if this is a failed
		// "find" operation that should report an error. We should not return a
		// reference to the "nullptr" literal, and we should avoid making
		// references to temporary constants like numerical literals in any case.
		if (cur == nullptr)  return cur;

		// [Base case 2: When the key is found]
		// If we find a key that matches by value, then return the current TreeNode*
		else if (key == cur->key)  return cur;

		// [When we need to search left]
		// If the key we're looking for is smaller than the current node's key,
		// then we should look to the left next.
		else if (key < cur->key)  return _find(key, cur->left);

		// [When we need to search right]
		// Otherwise, implicitly, the key we're looking for is larger than the
		// current node's key. (We know this because it's not equal and not less.)
		// So we should search to the right next.
		else  return _find(key, cur->right);

	}

	TreeNode*& _iop(TreeNode*& cur) const {
		if (cur->right) return _iop(cur->right);
		return cur;
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
		std::cout << "[" << node->key << " : " << node->data << "]";
		// Recurse right:
		_printInOrder(node->right);
		return;
	}
public:
	Dictionary() : m_head(nullptr) { }

	bool empty() const {
		return !m_head;
	}

	void printInOrder() const {
		_printInOrder(m_head);
	}

	const D& find(const K& key) {
		// Find the key in the tree starting at the head.
		// If found, we receive the tree's actual stored pointer to that node
		//   through return-by-reference (We are doing this to modify tree in place).
		// If not found, then the node returned has a value of nullptr.
		TreeNode*& node = _find(key, m_head);
		if (node == nullptr) { throw std::runtime_error("error: key not found"); }
		// We found the node, so return the actual data there, by reference.
		return node->data;
	}

	void insert(const K& key, const D& data) {
		// Find the place where the item should go.
		TreeNode *& node = _find(key, m_head);

		// If key already exists throw an error
		if (node) { throw std::runtime_error("error: insert() used on an existing key"); }

		// else add a node of key value pair their
		node = new TreeNode(key, data);
	}

	void _remove(TreeNode*& node) {
		// Zero child remove:
		if (node->left == nullptr && node->right == nullptr) {
			delete node;
			node = nullptr;
			return;
		}
		// One-child (left) remove
		else if (node->left != nullptr && node->right == nullptr) {
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
		else if (node->left == nullptr && node->right != nullptr) {
			// Similar to last case
			TreeNode* temp = node;
			node = node->right;
			delete temp;
			return;
		}
		// Two-child remove
		else {
			// When the node being deleted has two children, we have to be very
			// careful. The lecture discusses this case in detail. (The versions
			// of the helper functions being used here are slightly different
			// compared to lecture, as noted in other comments here.)

			// Find the IOP (in-order predecessor) of the current node.
			TreeNode*& iop = _iop(node->left);


			K temp_key = iop->key;
			D temp_data = iop->data;

			iop->key = node->key;
			iop->data = node->data;


			node->key = temp_key;
			node->data = temp_data;

			// This will turn the two child remove case into either no child case
			// or single left child case and call the remove function again which will
			// repair the link accordingly
			_remove(iop);
		}
	}

	void remove(const K& key) {

		// First, find the actual pointer to the node containing this key.
		// If not found, then the pointer returned will be equal to nullptr.
		TreeNode*& node = _find(key, m_head);
		// If the node we are trying to remove is a nullptr, then it's an error,
		if (!node) {
			throw std::runtime_error("error: _remove() used on non-existent key");
		}

		return _remove(node);


	}

	~Dictionary() {
		while (m_head) remove(m_head->key);
	}
};

int main() {
	Dictionary<int, std::string> digits;
	digits.insert(5, "five");
	digits.insert(6, "six");
	digits.insert(4, "four");
	digits.insert(8, "eight");
	digits.insert(9, "nine");
	digits.insert(2, "two");
	digits.insert(7, "seven");
	digits.insert(1, "one");
	digits.insert(3, "three");
	digits.printInOrder();
	digits.remove(5);
	std::cout << '\n';
	digits.printInOrder();
	digits.remove(6);
	std::cout << '\n';
	digits.printInOrder();
	digits.remove(1);
	std::cout << '\n';
	digits.printInOrder();
	digits.remove(9);
	std::cout << '\n';
	digits.printInOrder();
	std::cout << "\n---------------\n" << digits.find(3) << '\n';
	digits.remove(3);
	std::cout << "\n---------------\n" << digits.find(3) << '\n';
	return 0;
}