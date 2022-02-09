#include <iostream>

// Timer Class for benchmarking
class Timer {
	std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
	long long elapsed_time;
	const char* str;
public:
	Timer(const char* _str = ""): str(_str) {
		start = std::chrono::high_resolution_clock::now();

	}
	~Timer() {
		end = std::chrono::high_resolution_clock::now();
		elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		std::cout << "\n" << str << " Elapsed Time: " << elapsed_time << "ms\n";
	}

};


template<class T>
class BST {
public:
	class TreeNode {
	public:
		TreeNode *left, *right;
		T data;
		TreeNode(T data)
			: data(data), left(NULL), right(NULL)
		{}
	};
	TreeNode *root;

	BST(): root(NULL) {}

	void insert(T data) {
		root = insert(data, root);
	}

	TreeNode* insert(T data, TreeNode* curr) {
		if (curr == NULL) return new TreeNode(data);

		if (curr->data > data)
			curr->left = insert(data, curr->left);
		else
			curr->right = insert(data, curr->right);

		return curr;
	}

	void inOrder() const {
		return inOrder(root);
	}

	void inOrder(TreeNode *curr) const {
		if (curr == NULL) return;

		inOrder(curr->left);
		std::cout << curr->data << ' ';
		inOrder(curr->right);
		return;
	}

	int height() const {
		return height(root);
	}

	int height(TreeNode* curr) const {
		if (curr == NULL) return -1;

		return std::max(height(curr->left), height(curr->right)) + 1;
	}

	TreeNode* findRightMost(TreeNode* curr) {
		if (curr->right == NULL) return curr;
		return findRightMost(curr->right);
	}

	void remove(T data) {
		root = remove(data, root);
	}

	TreeNode* remove(T data, TreeNode* curr) {
		if (curr == NULL) return NULL;

		if (curr->data == data) {
			// No child case
			if (curr->left == NULL and curr->right == NULL) {
				delete curr;
				return NULL;
			}
			// two child only
			if (curr->left != NULL and curr->left == NULL) {
				// Find IOP (In order Predecessor)
				TreeNode* temp = findRightMost(curr->left);
				// Swap with IOP
				std::swap(temp->data, curr->data);
				// Call remove on left subtree
				curr->left = remove(data, curr->left);
				return curr;
			}
			// Single Child Case
			TreeNode* temp = curr->left ? curr->left : curr->right;
			delete curr;
			return temp;
		}	else {
			if (curr->data > data)
				curr->left = remove(data, curr->left);
			else
				curr->right = remove(data, curr->right);
			return curr;
		}
	}

	bool exist(T data) const {
		return exist(data, root);
	}

	bool exist(T data, TreeNode* curr) const {
		if (curr == NULL) return false;
		if (curr->data == data) return true;

		if (curr->data > data) return exist(data, curr->left);
		return exist(data, curr->right);
	}
};

template<class T>
class AVL {
public:
	class TreeNode {
	public:
		T data;
		size_t n_height;
		TreeNode *left, *right;
		TreeNode(T data)
			: data(data), n_height(0), left(NULL), right(NULL)
		{}
	};
private:
	int _get_height(TreeNode* curr) const {
		if (!curr)
			// A non-existent node has a height of 1
			return -1;
		else
			// We assume that an existing node already has an updated height
			return curr->n_height;
	}

	void _updateHeight(TreeNode*& curr) {
		// If the node is nullptr, then do nothing and return.
		if (curr == nullptr) return;
		// Otherwise update the height to be one more than the greater of the
		// two children's heights. The get_height function safely handles the
		// case where a child is just nullptr (no node exists), in which case
		// its implicit height is -1.
		curr->n_height = 1 + std::max(_get_height(curr->left), _get_height(curr->right));
		return;
	}

	int _getBalanceFactor(TreeNode* curr) {
		if (curr == NULL) return 0;
		// return height(curr->left) - height(curr->right);
		return _get_height(curr->left) - _get_height(curr->right);
	}

	TreeNode* _ensureBalance(TreeNode* curr) {
		int initial_balance = _getBalanceFactor(curr);

		// Tree is left heavy
		if (initial_balance >= 2) {
			int l_balance = _getBalanceFactor(curr->left);
			if (l_balance >= 0)
				return _rotateRight(curr); // repairing a stick
			else {
				curr->left = _rotateLeft(curr->left);
				return  _rotateRight(curr); // repairing an elbow
			}
		} else if (initial_balance <= -2) { // Tree is right heavy
			int r_balance = _getBalanceFactor(curr->right);
			if (r_balance <= 0)
				return _rotateLeft(curr); // repairing a stick
			else {
				curr->right = _rotateRight(curr->right);
				return  _rotateLeft(curr); // repairing an elbow
			}
		}
		_updateHeight(curr);
		return curr;
	}

	TreeNode* _rotateRight(TreeNode* curr) {
		if (!curr) {
			throw std::runtime_error("ERROR: _rotateRight called on nullptr");
		}
		if (!curr->left) {
			throw std::runtime_error("ERROR: _rotateRight: left child is nullptr");
		}
		TreeNode* next = curr->left;
		// Moving the subtree
		curr->left = next->right;
		// Breaking the stick
		next->right = curr;
		// Updating height
		_updateHeight(curr);
		_updateHeight(next);
		// throwing up the node
		return next;
	}

	// Its the mirror image of _rotateRight
	TreeNode* _rotateLeft(TreeNode* curr) {
		// If these error conditions arise, something already went wrong before
		// this function was called.
		if (!curr) {
			throw std::runtime_error("ERROR: _rotateLeft called on nullptr");
		}
		if (!curr->right) {
			throw std::runtime_error("ERROR: _rotateLeft: right child is nullptr");
		}
		TreeNode* next = curr->right;
		// Moving the subtree
		curr->right = next->left;
		// Breaking the stick
		next->left = curr;
		// Updating height
		_updateHeight(curr);
		_updateHeight(next);
		// throwing up the node
		return next;
	}
public:
	TreeNode *root;

	AVL(): root(NULL) {}

	void insert(T data) {
		root = _ensureBalance(insert(data, root));
	}

	TreeNode* insert(T data, TreeNode* curr) {
		if (curr == NULL) return new TreeNode(data);

		if (curr->data > data)
			curr->left = insert(data, curr->left);
		else
			curr->right = insert(data, curr->right);

		return _ensureBalance(curr);
	}

	void inOrder() const {
		return inOrder(root);
	}

	void inOrder(TreeNode *curr) const {
		if (curr == NULL) return;

		inOrder(curr->left);
		std::cout << curr->data << ' ';
		inOrder(curr->right);
		return;
	}

	void preOrder() const {
		return preOrder(root);
	}

	void preOrder(TreeNode *curr) const {
		if (curr == NULL) return;

		std::cout << curr->data << ' ';
		preOrder(curr->left);
		preOrder(curr->right);
		return;
	}

	int height() const {
		return height(root);
	}

	int height(TreeNode* curr) const {
		if (curr == NULL) return -1;

		return std::max(height(curr->left), height(curr->right)) + 1;
	}

	TreeNode* findRightMost(TreeNode* curr) {
		if (curr->right == NULL) return curr;
		return findRightMost(curr->right);
	}

	void remove(T data) {
		root = _ensureBalance(remove(data, root));
	}

	TreeNode* remove(T data, TreeNode* curr) {
		if (curr == NULL) return NULL;

		if (curr->data == data) {
			// No child case
			if (curr->left == NULL and curr->right == NULL) {
				delete curr;
				return NULL;
			}
			// two child only
			if (curr->left != NULL and curr->left != NULL) {
				// Find IOP (In order Predecessor)
				TreeNode* IOP = findRightMost(curr->left);
				// Swap with IOP
				curr->data = IOP->data;
				// Call remove on left subtree
				curr->left = remove(IOP->data, curr->left);
				return _ensureBalance(curr);
			}
			// Single Child Case
			TreeNode* temp = curr->left ? curr->left : curr->right;
			delete curr;
			return temp;

		}	else {
			if (curr->data > data)
				curr->left = remove(data, curr->left);
			else
				curr->right = remove(data, curr->right);
			return _ensureBalance(curr);
		}
	}

	bool exist(T data) const {
		return exist(data, root);
	}

	bool exist(T data, TreeNode* curr) const {
		if (curr == NULL) return false;
		if (curr->data == data) return true;

		if (curr->data > data) return exist(data, curr->left);
		return exist(data, curr->right);
	}
};

int main() {
	BST<int> tree;
	AVL<int> balanced_tree;
	int temp;
	srand(time(0));
	int n = 5000;
	{
		Timer t("BST Insertion");
		for (int i = 0; i <= n; i++) {
			temp = i + 1/*rand() % 500*/;
			tree.insert(temp);
		}
	}

	{
		Timer t("AVL Insertion");
		for (int i = 0; i <= n; i++) {
			temp = i + 1/*rand() % 500*/;
			balanced_tree.insert(temp);
		}
	}
	// tree.inOrder();
	std::cout << "\nTree Height (BST)" << tree.height() << "\n--------------------------\n";
	// balanced_tree.inOrder();
	std::cout << "Tree Height (AVL)" << balanced_tree.height() << "\n--------------------------\n";

	int query[2] = {0};
	{
		Timer T1("BST");
		for (int i = n / 3; i < 12 * n; i++)
			query[tree.exist(i)]++;
	}
	std::cout << query[1] << " Found and " << query[0] << " Not Found!";

	{
		query[0] = query[1] = 0;
		Timer T1("AVL");
		for (int i = n / 3; i < 12 * n; i++)
			query[balanced_tree.exist(i)]++;
	}
	std::cout << query[1] << " Found and " << query[0] << " Not Found!\n";



	/*Draw on paper along side and observe*/
	/*AVL<int> balanced_tree1;
	balanced_tree1.insert(50);
	balanced_tree1.insert(35);
	balanced_tree1.insert(92);
	balanced_tree1.insert(80);
	balanced_tree1.insert(100);
	balanced_tree1.preOrder();
	std::cout << "Tree Height (AVL) " << balanced_tree1.height() << "\n--------------------------\n";
	balanced_tree1.remove(35);
	balanced_tree1.preOrder();
	std::cout << "Tree Height (AVL) " << balanced_tree1.height() << "\n--------------------------\n";
	balanced_tree1.remove(92);
	balanced_tree1.preOrder();
	std::cout << "Tree Height (AVL) " << balanced_tree1.height() << "\n--------------------------\n";*/
	return 0;
}
