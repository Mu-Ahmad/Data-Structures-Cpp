#include <iostream>
#include <string>


//Test Class
class point {
	int x;
	double y;
	int *trash;
public:
	point(int x = 9, double y = 0)
		: x(x), y(y) {
		trash = new int[5];
	}
	point(const point& other)
		: x(other.x), y(other.y) {
		trash = new int[5];
	}

	friend std::ostream& operator<<(std::ostream& out, const point& other) {
		out << "X: " << other.x << ' '
		    << "Y: " << other.y << '\n';
	}
	~point() {
		delete[] trash;
	}
};


//Iterator Class
template<class Vector>
class VectorIterator {
public:
	using ValueType = typename Vector::ValueType;
	VectorIterator(ValueType* ptr)
		: m_ptr(ptr) {}
private:
	ValueType* m_ptr;
public:
	VectorIterator operator++() {
		m_ptr++;
		return *this;
	}
	VectorIterator operator++(int) {
		VectorIterator temp = *this;
		m_ptr++;
		return temp;
	}
	VectorIterator operator--() {
		m_ptr--;
		return *this;
	}
	VectorIterator operator--(int) {
		VectorIterator temp = *this;
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
	bool operator==(const VectorIterator& other) {
		return m_ptr == other.m_ptr;
	}
	bool operator!=(const VectorIterator& other) {
		return m_ptr != other.m_ptr;
	}
};

//Vector Class
template<class T>
class Vector {
	T* m_data;
	size_t m_size, m_capacity;

	void resize(size_t new_size) {
		if (new_size == 0) new_size++;

		T* temp_block = (T*) ::operator new(new_size * sizeof(T));

		m_size = std::min(m_size, new_size);
		for (size_t i = 0; i < m_size; i++)
			new(&temp_block[i]) T(m_data[i]);

		clear(m_size);
		::operator delete(m_data, m_capacity * sizeof(T));
		m_data = temp_block;
		m_capacity = new_size;
	}

public:
	using ValueType = T;
	using iterator = VectorIterator<Vector<T>>;

	Vector(size_t capacity = 0)
		: m_size(capacity), m_capacity(capacity) {
		m_data = new T[m_capacity];
	}
	void reserve(size_t capacity) {
		resize(capacity);
	}
	Vector(const Vector& other) {
		delete[] m_data;
		m_data = new T[other.capacity];
		for (size_t i = 0; i < other.m_size; i++)
			m_data[i] = other.m_data[i];
		m_size = other.m_size;
		m_capacity = other.m_capacity;
	}
	~Vector() {
		clear();
		::operator delete(m_data, m_capacity * sizeof(T));
		// delete[] m_data;
	}

	T& push_back(const T& ELEMENT) {
		if (m_size == m_capacity)
			resize(2 * m_capacity);
		new(&m_data[m_size]) T(ELEMENT);
		// m_data[m_size] = ELEMENT;
		return m_data[m_size++];
	}

	template<class... ARGS>
	T& emplace_back(ARGS&&... args) {
		if (m_size == m_capacity)
			resize(2 * m_capacity);
		new(&m_data[m_size]) T(std::forward<ARGS>(args)...);
		return m_data[m_size++];
	}

	void insert(size_t index, const T& other) {
		if (m_size == m_capacity)
			resize(2 * m_capacity);
		new(&m_data[m_size]) T(m_data[m_size - 1]);
		for (int i = m_size - 2; i >= index; i--)
			m_data[i + 1] = m_data[i];
		m_data[index] = other;
		m_size++;
	}


	T& operator[](size_t index) {
		return *(m_data + index);
	}

	const T& operator[](size_t index) const {
		return *(m_data + index);
	}

	void clear(size_t n = 0) {
		for (int i = 0; i < m_size; i++)
			m_data[i].~T();
		m_size = n;
	}

	void shrink_to_size() {
		if (m_capacity - m_size)
			resize(m_size);
	}

	void pop_back() {
		if (m_size)
			m_data[m_size--].~T();
	}
	size_t size() const {return m_size;}
	size_t capacity() const {return m_capacity;}
	iterator begin() {
		return iterator(m_data);
	}
	iterator end() {
		return iterator(m_data + m_size);
	}
};

int main() {
#if 1
	Vector<point> pointVector;
	pointVector.reserve(15);
	pointVector.emplace_back(4, 5);
	pointVector.emplace_back(1, 5);
	pointVector.emplace_back(3, 9);
	pointVector.emplace_back(4, 6);
	pointVector.emplace_back();

	for (int i = 0; i < pointVector.size(); i++) {
		std::cout << pointVector[i];
	}
#endif
#if 1
	Vector<std::string> stringVector;
	stringVector.push_back("Ahmad");
	stringVector.pop_back();
	stringVector.push_back("Butt");
	stringVector.push_back("Irtaza");
	stringVector.push_back("Malik");
	stringVector.push_back("Ahmad");
	stringVector.push_back("Hala-Madrid");
	for (int i = 0; i < stringVector.size(); i++)
		std::cout << stringVector[i] << '\n';

	std::cout << stringVector.capacity() << '\n';
	stringVector.shrink_to_size();
	std::cout << stringVector.capacity() << '\n';
#endif
#if 1
	Vector<int> arr;
	arr.clear();
	arr.push_back(7);
	arr.push_back(8);
	arr.push_back(3);
	arr.insert(1, 69);
	for (auto x : arr)
		std::cout << x << '\n';
#endif

	return 0;

}

