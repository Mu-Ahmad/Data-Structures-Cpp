#include <iostream>
#include <list>
#include <utility>
#include <functional>

using namespace std;

template<class K, class V>
class HashTable_SeparateChaining{
public:
	int capacity, size;
	double load_factor;
	list< pair<K, V> >** table;
	hash<K> hash_func;

	// Constructor
	HashTable_SeparateChaining(int capacity, double load_factor = 0.5)
		: capacity(capacity), size(0), load_factor(load_factor)
		{
			table = new list< pair<K, V> >*[capacity];
			for(int i=0; i<capacity; i++) table[i] = NULL;
		}

	// Insert
	void insert(const K& key, const V& value){
		if(hasKey(key)) throw runtime_error("Key already exist!");

		int index = hash_func(key) % capacity;

		if(table[index] == NULL)
			table[index] = new list< pair<K, V> >;

		table[index]->push_back({key, value});
		size++;

		if (size/(double)capacity >= load_factor)
			this->resize();

	}

	// look up
	bool hasKey(const K& key){
		int index = hash_func(key) % capacity;

		if(table[index] == NULL) return false;

		for(auto& pi : *table[index])
			if(pi.first == key) return true;

		return false;
	}

	V get(const K& key){
		if(!hasKey(key)) throw runtime_error("Key does not exist!");

		int index = hash_func(key) % capacity;

		for(auto& pi : *table[index])
			if(pi.first == key) return pi.second;

		return K();
	}

	// deletion
	void remove(const K& key){
		V value = this->get(key);

		int index = hash_func(key) % capacity;

		table[index]->remove({key, value});

		size--;
	}

	void resize(){
		// reserve a new table
		list< pair<K, V> >** table1 = new list< pair<K, V> >*[capacity*2];

		for(int i=0; i<capacity*2; i++) table1[i] = NULL;

		for(int i=0; i<capacity; i++){
			if(table[i] == NULL)  continue;

			for(auto pi : *table[i]){
				int index = hash_func(pi.first) % (capacity*2);

				if(table1[index] == NULL)
					table1[index] = new list< pair<K, V> >;

				table1[index]->push_back(pi);
			}

			table[i]->clear();
			// delete table[i];
		}
		delete table;
		table = table1;
		this->capacity *= 2;
	}

	void printItOut() const{
		cout << "\n-------------------------------\n";
		for(int i=0; i<capacity; i++){
			cout << "| index " << i << "  | ";

			if(table[i] != NULL) 

			 for(auto pi : *table[i])
			 	cout << "{" << pi.first << ", " << pi.second << "} -----> ";

			 cout << "\n";
		}
		cout << "\n-------------------------------\n";
	}


};

int main(){
	HashTable_SeparateChaining<string, string> table(10);
	table.insert("abcadsa", "ABC");
	table.insert("xyasdasdz", "ABC");
	table.insert("dbasdasda", "ABC");
	table.insert("bdasdasdf", "ABC");
	table.printItOut();
	table.insert("aksjadsad", "ABC");
	table.insert("wejkrbewkjr", "ABC");
	table.insert("aksjadwerewrwe", "ABC");
	table.printItOut(); 
	cout << table.capacity << '\n';
	return 0;
}
