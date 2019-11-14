//template<class Ctype>
///empty()
///top()
///pop()
///push()
#include <iostream>

using namespace std;


template <class T>
struct stackNode
{
	T data;
	stackNode * next;
};
template <class T>
class stack
{
public:
	stack();
	~stack();
	void push(T data);
	void pop();
	T top();
	bool empty();
private:
	stackNode<T> * Top;

};
template <class T>
stack<T>::stack(){
	Top = NULL;
}
template <class T>
stack<T>::~stack(){
	stackNode<T> * node = NULL;
	while (Top != NULL)
	{
		node = Top;
		Top = Top->next;
		delete node;
	}
}
template <class T>
void stack<T>::push(T data){
	stackNode<T>* node = new stackNode<T>;
	node->data = data;
	node->next = Top;
	Top = node;
}
template <class T>
void stack<T>::pop(){
	stackNode<T>* node = Top;
	Top = Top->next;
	delete node;
}
template <class T>
bool stack<T>::empty(){
	return Top == NULL;
}
template <class T>
T stack<T>::top(){
	return Top->data;
}
