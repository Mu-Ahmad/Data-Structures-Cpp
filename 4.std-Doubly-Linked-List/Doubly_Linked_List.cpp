#include <iostream>
#include <sstream>
#include <set>

//Doubly-linked-list
template<typename T>
class Linked_List {
private:
	class Node	{
		T m_data;
		Node *m_next, *m_previous;
		friend class Linked_List<T>;
	public:
		Node()
			: m_next(nullptr), m_previous(nullptr)
		{}

		Node(const T& data, Node* next, Node* previous)
			: m_data(data), m_next(next), m_previous(previous)
		{}
	};

	class Iterator {
	private:
		Node* m_ptr;
	public:
		Iterator(Node* ptr)
			: m_ptr(ptr)
		{}
		Iterator operator++() {
			m_ptr = m_ptr->m_next;
			return *this;
		}
		Iterator operator++(int) {
			Iterator temp = *this;
			m_ptr = m_ptr->m_next;
			return temp;
		}
		Iterator operator--() {
			m_ptr = m_ptr->m_previous;
			return *this;
		}
		Iterator operator--(int) {
			Iterator temp = *this;
			m_ptr = m_ptr->m_previous;
			return temp;
		}
		T* operator->() const {
			return &(m_ptr->m_data);
		}
		T& operator*() const {
			return m_ptr->m_data;
		}
		bool operator==(const Iterator& other) const {
			return m_ptr == other.m_ptr;
		}
		bool operator!=(const Iterator& other) const {
			return m_ptr != other.m_ptr;
		}
	};

private:
	size_t m_size;
	Node *m_head, *m_tail;

	T _remove(Node* other) { //Remove an arbritary node from the linked list O(1)
		// checks if its m_head
		if (other->m_previous == nullptr) return pop_front();
		// checks if its m_tail
		if (other->m_next == nullptr) return pop_back();

		//Make pointers of adjacent nodes to skip over current node
		other->m_next->m_previous = other->m_previous;
		other->m_previous->m_next = other->m_next;

		T temp_data = other->m_data;
		delete other;
		m_size--;
		return temp_data;
	}

	Node* _merge(Node *first, Node *second) {
		Node dummy;
		Node *last = &dummy;
		while (first != nullptr and second != nullptr) {
			if (first->m_data < second->m_data) {
				last->m_next = first;
				first->m_previous = last;
				last = first;
				first = first->m_next;
			}	else	{
				last->m_next = second;
				second->m_previous = last;
				last = second;
				second = second->m_next;
			}
		}
		while (first != nullptr) {
			last->m_next = first;
			first->m_previous = last;
			last = first;
			first = first->m_next;
		}
		while (second != nullptr) {
			last->m_next = second;
			second->m_previous = last;
			last = second;
			second = second->m_next;
		}

		m_tail = last;
		return dummy.m_next;
	}

	Node* _split(Node* head) {
		Node *fast = head, *slow = head;

		while (fast->m_next != nullptr and fast->m_next->m_next != nullptr) {
			fast = fast->m_next->m_next;
			slow = slow->m_next;
		}

		fast = slow->m_next;
		slow->m_next = nullptr;
		return fast;
	}

	Node* _merge_sort(Node* head) {
		//Base Case
		// if the end is reached
		if (head == nullptr or head->m_next == nullptr)
			return head;

		// Recursive Case
		// split the linked list in two halves
		Node*  second = _split(head);

		//call the sort on both halves
		head = _merge_sort(head);
		second = _merge_sort(second);

		// Merge the sorted halves
		return _merge(head, second);
	}
public:
	Linked_List()
		: m_size(0) {}

	bool isEmpty() const {
		return m_size == 0;
	}

	void push_back(const T& ELEMENT) {
		if (isEmpty()) {
			m_head = new Node(ELEMENT, nullptr, nullptr);
			m_tail = m_head;
		}	else	{
			m_tail->m_next = new Node(ELEMENT, nullptr, m_tail);
			m_tail = m_tail->m_next;
		}
		m_size++;
	}

	void push_front(const T& ELEMENT) {
		if (isEmpty()) {
			m_head = new Node(ELEMENT, nullptr, nullptr);
			m_tail = m_head;
		}	else  {
			m_head->m_previous = new Node(ELEMENT, m_head, nullptr);
			m_head = m_head->m_previous;
		}
		m_size++;
	}

	T pop_back() {
		if (isEmpty()) throw std::runtime_error("Empty List");
		T temp_data = m_tail->m_data;
		m_tail = m_tail->m_previous;

		delete m_tail->m_next;

		m_tail->m_next = nullptr;
		m_size--;
		if (isEmpty()) m_head = nullptr;

		return temp_data;
	}

	T pop_front() {
		if (isEmpty()) throw std::runtime_error("Empty List");
		T temp_data = m_head->m_data;
		m_head = m_head->m_next;

		delete m_head->m_previous;
		m_head->m_previous = nullptr;
		m_size--;
		// if list had only one element
		if (isEmpty()) m_tail = nullptr;

		return temp_data;
	}

	int remove(const T& ELEMENT) {
		int count = 0;
		Node* temp = m_head, *next;
		while (temp != nullptr) {
			next = temp->m_next;
			if (temp->m_data == ELEMENT) {
				_remove(temp);
				count++;
			}
			temp = next;
		}
		return count;
	}

	T remove_at(size_t index) {
		if (index >= m_size) throw std::invalid_argument("Invalid Argument");

		Node* temp;
		size_t i = 0;
		if (index < m_size / 2)
			for (i = 0, temp = m_head; i < index; i++)
				temp = temp->m_next;
		else
			for (i = m_size - 1, temp = m_tail; i > index; i--)
				temp = temp->m_previous;

		return _remove(temp);
	}

	int remove_duplicate() {
		int count = 0;
		std::set<T> lookup;
		Node* temp = m_head, *next;
		while (temp != nullptr) {
			next = temp->m_next;
			if (lookup.count(temp->m_data) == 0)
				lookup.insert(temp->m_data);
			else {
				_remove(temp);
				count++;
			}
			temp = next;
		}
		return count;
	}

	T peak_first() const {
		if (isEmpty()) throw std::runtime_error("Empty List");
		return m_head->m_data;
	}

	T peak_last() const {
		if (isEmpty()) throw std::runtime_error("Empty List");
		return m_tail->m_data;
	}

	int indexOf(const T& ELEMENT) const {
		int index = 0;
		Node* temp = m_head;
		while (temp != nullptr) {
			if (temp->m_data == ELEMENT)
				return index;
			index++;
			temp = temp->m_next;
		}
		return -1;
	}

	bool contains(const T& ELEMENT) const {
		return indexOf(ELEMENT) != -1;
	}

	void reverse() {
		Node *current = m_head, *next = nullptr, *previous = nullptr;
		while (current != nullptr) {
			//update next;
			next = current->m_next;
			//Reverse the forward reference pointer of current node
			current->m_next = previous;
			//update current and previous
			previous = current;
			current = next;
		}
		m_head = previous; // which is m_tail

		current = m_tail;
		next = previous = nullptr;
		while (current != nullptr) {
			//store previous
			previous = current->m_previous;
			//Reverse the backward reference pointer of current node
			current->m_previous = next;
			//update current and next
			next = current;
			current = previous;
		}
		m_tail = next;
	}

	void sort() {
		m_head = _merge_sort(m_head);
		m_head->m_previous = nullptr;
	}

	void clear() {
		Node* next;
		while (m_head != nullptr) {
			next = m_head->m_next;
			delete m_head;
			m_head = next;
		}
		m_tail = nullptr;
		m_size = 0;
	}

	T* to_array() const {
		int* arr = new T[m_size];
		Node* temp = m_head;
		size_t i = 0;
		while (temp != nullptr) {
			arr[i++] = temp->m_data;
			temp = temp->m_next;
		}
		return arr;
	}

	Iterator begin() {
		return Iterator(m_head);
	}

	Iterator end() {
		return Iterator(nullptr);
	}

	~Linked_List() {
		clear();
	}

	std::string to_string() const {
		std::stringstream out;
		out << "{ ";
		if (isEmpty()) out << " }";
		else {
			Node* temp = m_head;
			while (temp != nullptr) {
				out << temp->m_data;
				temp = temp->m_next;
				if (temp != nullptr) out << " <----> ";
			}
			out << " }";
		}
		return out.str();
	}

	friend std::ostream& operator <<(std::ostream& out, const Linked_List<T>& other) {
		out << other.to_string();
		return out;
	}
};

int main() {
	Linked_List<int> intList;
	intList.push_back(69);
	intList.push_back(69);
	intList.push_front(9);
	intList.push_front(9);
	intList.push_back(98);
	intList.push_back(98);
	std::cout << intList << '\n';
	// std::cout << intList.remove(69) << ' ' << intList.remove(77) << '\n';
	// intList.remove_at(1);
	std::cout << intList.remove_duplicate() << '\n';
	std::cout << intList << '\n';
	intList.reverse();
	std::cout << intList << '\n';
	intList.clear();
	std::cout << intList << '\n';
	intList.push_back(56);
	intList.push_back(98);
	intList.push_back(9832);
	intList.push_back(532);
	intList.push_back(69);
	intList.push_back(-9869);
	std::cout << intList << '\n';
	std::cout << intList.peak_first() << '\n';
	std::cout << intList.peak_last() << '\n';
	intList.sort();
	std::cout << intList << '\n';
	std::cout << intList.peak_first() << '\n';
	std::cout << intList.peak_last() << '\n';

	std::cout << "\n=========================\n";

	Linked_List<std::string> strList;
	strList.push_back("Irtaza");
	strList.push_back("Butt");
	strList.push_front("Ahmad");
	strList.push_back("Malik");
	std::cout << strList << "\n";
	std::cout << strList.peak_first() << "\n";
	std::cout << strList.peak_last() << "\n";
	strList.reverse();
	std::cout << strList << "\n";
	std::cout << strList.peak_first() << "\n";
	std::cout << strList.peak_last() << "\n";
	std::cout << "Hello\n";
	for (auto x : strList)
		std::cout << x << '\n';
}