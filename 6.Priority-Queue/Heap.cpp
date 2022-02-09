#include <iostream>
#include <vector>

template<class T>
class Heap {
	size_t _size;
	std::vector<T> _data;
public:
	Heap()
		: _size(0), _data(1)
	{}

	void insert(const T& ELEMENT) {
		// insert the new element at the end of array

		if (_size + 1 == _data.size())
			_data.push_back(ELEMENT);
		else
			_data[_size + 1] = ELEMENT;


		// Restore the heap property
		// _swim(++_size);
		_swimItr(++_size);
	}

	const T pop() {
		if (isEmpty()) throw std::runtime_error("No element in heap!");

		// Swap with Last Value
		T minValue = _data[1];
		std::swap(_data[1], _data[_size--]);

		// Restore Heap Property
		// _sink(1);
		_sinkItr(1);

		// Return the min value
		return minValue;
	}

	const T& peak() const {
		if (isEmpty()) throw std::runtime_error("No element in heap!");
		return _data[0];
	}
	size_t size() const {return _size;}

	bool isEmpty() const {
		return _size == 0;
	}


protected:
	void _swim(size_t index) {

		if (index != 1) {
			size_t parentIndex = index / 2;
			if (_data[index] < _data[parentIndex]) {
				std::swap(_data[index], _data[parentIndex]);
				_swim(parentIndex);
			}
		}
		return;
	}

	void _swimItr(size_t index) {

		while (index != 1) {
			size_t parentIndex = index / 2;
			if (_data[index] < _data[parentIndex]) {
				std::swap(_data[index], _data[parentIndex]);
				index = parentIndex;
			} else break;
		}
		return;
	}

	void _sink(size_t index) {

		if (!_isLeaf(index)) {
			size_t minChildIndex = _minChildIndex(index);
			if (_data[index] > _data[minChildIndex]) {
				std::swap(_data[index], _data[minChildIndex]);
				_sink(minChildIndex);
			}
		}
		return;
	}

	void _sinkItr(size_t index) {

		while (!_isLeaf(index)) {
			size_t minChildIndex = _minChildIndex(index);
			if (_data[index] > _data[minChildIndex]) {
				std::swap(_data[index], _data[minChildIndex]);
				index = minChildIndex;
			} else break;
		}

		return;
	}

	bool _isLeaf(size_t index) {
		// A leaf node has no children, so it is a leaf
		// if the left child (index*2) is larger than the size
		// of the heap
		return index * 2 > _size;
	}

	size_t _minChildIndex(size_t index) {
		size_t left = index * 2;
		size_t right = left + 1;

		if (right > _size) return left;

		if (_data[left] < _data[right]) return left;

		return right;
	}
};

template<class T>
class MaxHeap: public Heap<T> {
public:
	void insert(const T& ELEMENT) {
		Heap<T>::insert(ELEMENT * -1);
	}

	const T pop() {
		return Heap<T>::pop() * -1;
	}
};

int main() {

	Heap<int> heap;
	// MaxHeap<int> heap;

	std::cout << " === 10 calls to heap.insert() === " << '\n';
	heap.insert(4);
	heap.insert(10);
	heap.insert(2);
	heap.insert(22);
	heap.insert(45);
	heap.insert(18);
	heap.insert(-8);
	heap.insert(95);
	heap.insert(-69);
	heap.insert(42);

	std::cout << heap.size() << " <-- size\n";

	std::cout << " === 10 calls to heap.pop() === " << '\n';
	std::cout << heap.pop() << std::endl;
	std::cout << heap.pop() << std::endl;
	std::cout << heap.pop() << std::endl;
	std::cout << heap.pop() << std::endl;
	std::cout << heap.pop() << std::endl;
	std::cout << heap.pop() << std::endl;
	std::cout << heap.pop() << std::endl;
	std::cout << heap.pop() << std::endl;
	std::cout << heap.pop() << std::endl;
	std::cout << heap.pop() << std::endl;

	return 0;

	return 0;
}