#include <iostream>
using namespace std;
struct Node
{
	string name;
	int age;
	float height;
	Node *next; // Pointer to next node
	Node() {
		next = NULL;
	}
};
Node *start_ptr = NULL; // Start Pointer (root)
void push(const string &name, const int &age, const float &height) {
	Node* temp = new Node;
	temp->name = name;
	temp->age = age;
	temp->height = height;
	temp->next = start_ptr;
	start_ptr = temp;
}
void pop() {
	if(!start_ptr)
		return;
	Node* temp = start_ptr;
	start_ptr = start_ptr->next;
	delete temp;
}
int main()
{
	string name;
	int age;
	float height;
	while(cin >> name >> age >> height) 
		push(name, age, height);
	Node* head = start_ptr;
	while(start_ptr) {
		cout << start_ptr->name << ' ';
		cout << start_ptr->age << ' ';
		cout << start_ptr->height << '\n';
		pop();//pop函數裡包含start_ptr = start_ptr->next的功能 
	}
	return 0;
}

