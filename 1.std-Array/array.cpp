#include <iostream>
#include <string>

template<class Array>
class ArrayIterator {
public:
	using ValueType = typename Array::ValueType;
	ArrayIterator(ValueType* ptr)
		: m_ptr(ptr) {}
private:
	ValueType* m_ptr;
public:
	ArrayIterator operator++() {
		m_ptr++;
		return *this;
	}
	ArrayIterator operator++(int) {
		ArrayIterator temp = *this;
		m_ptr++;
		return temp;
	}
	ArrayIterator operator--() {
		m_ptr--;
		return *this;
	}
	ArrayIterator operator--(int) {
		ArrayIterator temp = *this;
		m_ptr--;
		return temp;
	}
	ValueType* operator->() {
		return m_ptr;
	}
	ValueType& operator[] (size_t index) {
		return *(m_ptr + index);
	}
	ValueType& operator*() {
		return *m_ptr;
	}
	bool operator==(const ArrayIterator& other) {
		return m_ptr == other.m_ptr;
	}
	bool operator!=(const ArrayIterator& other) {
		return m_ptr != other.m_ptr;
	}
};

template<class T, size_t SIZE>
class Array {
public:
	using ValueType = T;
	using iterator = ArrayIterator<Array<T, SIZE > >;

	constexpr size_t Size() const {return SIZE;}
	T& operator[](size_t index) { return data[index];}
	const T& operator[](size_t index) const { return data[index];}
private:
	T data[SIZE];
public:
	iterator begin() {
		return iterator(data);
	}

	iterator end() {
		return iterator(data + Size());
	}
};


int main() {
	Array<int, 5> arr; //////
	arr[0] = 1;
	arr[1] = 2;
	arr[2] = 3;
	arr[3] = 4;
	arr[4] = 5;
	for (int i = 0; i < arr.Size(); i++)
		std::cout << arr[i] << ' ';
	std::cout << '\n';

	for (int x : arr)
		std::cout << x << ' ';
	std::cout << '\n';

	for (auto it = arr.begin(); it != arr.end(); it++)
		std::cout << *it << ' ';
	std::cout << '\n';

	Array<std::string, 3> name;
	name[0] = "FTH";
	name[1] = "Malik Prince";
	name[2] = "CH Rafay";
	for (std::string n : name)
		std::cout << n << '\n';
}