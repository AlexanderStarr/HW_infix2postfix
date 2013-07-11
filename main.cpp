#include <iostream>
#include <string>
#include <math.h>
using namespace std;

string getNextNum(string equation, int i);
string infixToPostfix(string infix);
float evalPostfix(string postfix);
float doOperation(float x, float y, char op);
int getOpPriority(char op, char type);
const int DEFAULT_SIZE = 20;

// These are the Stack class definition and methods.
// Uses a class template to allow the stack to hold any data type.
// All class methods must be preceeded by a template declaration.
template <class T>
class Stack {
	T * arr;
	int tos, capacity;
public:
	//Constructors
	Stack();
	Stack(int size);
	
	//Destructor
	~Stack();
	
	//Methods
	void push(T a);
	T pop();
	T top();
	int get_size();
	bool empty();
	bool full();
};

template <class T>
Stack<T>::Stack(){
	arr = new T[DEFAULT_SIZE];
	tos = 0;
	capacity = DEFAULT_SIZE;
}

template <class T>
Stack<T>::Stack(int size){
	arr = new T[size];
	tos = 0;
	capacity = size;
}

template <class T>
Stack<T>::~Stack(){
	delete[] arr;
}

template <class T>
void Stack<T>::push(T a){
	if (!full()) {
		arr[tos++] = a;
	}
	else {
		cout << "Error, stack is full" << endl;
	}
}

template <class T>
T Stack<T>::pop(){
	if (!empty()) {
		return arr[--tos];
	}
	else {
		cout << "Error, stack is empty" << endl;
		return -1;
	}
}

template <class T>
int Stack<T>::get_size(){
	return tos;
}

template <class T>
bool Stack<T>::empty(){
	if (tos == 0) {
		return true;
	}
	else {
		return false;
	}
}

template <class T>
bool Stack<T>::full(){
	if (tos == capacity) {
		return true;
	}
	else {
		return false;
	}
}

template <class T>
T Stack<T>::top(){
	if (!empty()) {
		return arr[tos - 1];
	}
	else {
		cout << "Error, stack is empty";
		return -1;
	}

}

// ----------------------- End of class methods ------------------------- //

// Takes an operator and type (whether it in the 's'tack or 'i'ncoming).
// From that, returns the precedence of the operator.
int getOpPriority(char op, char type)
{
	int isp;
	int icp;
	switch (op) {
		case '(':
			isp = 0;
			icp = 4;
			break;
		case '+':
			isp = 1;
			icp = 1;
			break;
		case '-':
			isp = 1;
			icp = 1;
			break;
		case '*':
			isp = 2;
			icp = 2;
			break;
		case '/':
			isp = 2;
			icp = 2;
			break;
		case '^':
			isp = 3;
			icp = 4;
			break;
		default:
			isp = -1;
			icp = -1;
			break;
	}
	if (type == 's') {
		return isp;
	}
	else {
		return icp;
	}

}

// Does the given operation based on the operator character.
float doOperation(float x, float y, char op)
{
	switch (op) {
		case '+':
			return x + y;
		case '-':
			return x - y;
		case '*':
			return x * y;
		case '/':
			return x / y;
		case '^':
			return pow(x, y);
		default:
			return 0.0;
	}
}

// Takes a string which is assumed to be an equation (in infix or postfix notation).
// Also takes an integer i, which is the current index of the string.
// Returns a string consisting of all consecutive characters from index i to
// the next delimiter.
string getNextNum(string equation, int i)
{
	string num = "";
	const string DELIM = "+-*/%^ ()";
	while (i<equation.length()) {
		// Iterate through equation until you hit a delimiter.
		if (DELIM.find(equation[i]) != -1) {
			break;
		}
		num += equation[i];
		i++;
	}
	return num;
}

// Takes a string of an infix notation expression, and returns the postfix equivalent.
string infixToPostfix(string infix)
{
	const string OPS = "+-*/%^(";
	const string NUMS = "0123456789";
	string postfix = "";
	Stack<char> opStack(infix.length());
	opStack.push('#');
	int i = 0;
	// Scans through infix, performing appropriate operations on each character.
	while (i<infix.length()) {
		if (NUMS.find(infix[i]) != -1) {
			// If a digit is found, then the entire number is determined.
			// The number is appended to postfix, and i is incremented appropriately.
			string sNextNum = getNextNum(infix, i);
			i += (sNextNum.length() - 1);
			sNextNum += ' ';
			postfix += sNextNum;
		}
		else if (OPS.find(infix[i]) != -1) {
			// If an operator is found, then the priority is found of both it
			// and the operator on the top of the stack.  As long as the in-stack
			// priority is higher, that operator is popped and appended to postfix.
			// Once the incoming priority is higher, the loop terminates it is pushed to the stack.
			int isp = getOpPriority(opStack.top(), 's');
			int icp = getOpPriority(infix[i], 'i');
			while (isp >= icp) {
				postfix += opStack.pop();
				postfix += ' ';
				isp = getOpPriority(opStack.top(), 's');
			}
			opStack.push(infix[i]);
		}
		else if (infix[i] == ')') {
			// If a closing brace is found, then you pop and append operators
			// from the stack until you reach the left brace.  Then pop the left brace.
			while ((opStack.top() != '(') && (not opStack.empty())) {
				postfix += opStack.pop();
				postfix += ' ';
			}
			opStack.pop();
		}
		i++;
	}
	while (opStack.top() != '#') {
		// Once the string is scanned, pop and append all operators from the stack.
		postfix += opStack.top();
		postfix += ' ';
		opStack.pop();
	}
	return postfix;
}

// Returns the floating point value that the postfix expression evaluates to.
float evalPostfix(string postfix)
{
	const string OPS = "+-*/%^";
	const string NUMS = "0123456789";
	Stack<float> numStack(postfix.length());
	int i = 0;
	while (i<postfix.length()) {
		if (NUMS.find(postfix[i]) != -1) {
			// If a character is a digit, then you should get the 
			// value and push it to the stack (could be multiple characters long).
			string sNextNum = getNextNum(postfix, i);
			float fNextNum = atof(sNextNum.c_str());
			
			numStack.push(fNextNum);
			i += (sNextNum.length() - 1);
		}
		else if (OPS.find(postfix[i]) != -1) {
			// Otherwise, pop the top two elements of the stack, perform the
			// operation, then push the result back to the stack.
			char op = postfix[i];
			
			float y = numStack.pop();
			
			float x = numStack.pop();
			
			float z = doOperation(x, y, op);
			numStack.push(z);
		}
		i++;
	}
	// Once the entire string has been scanned through, there should be a float
	// left in the stack, simply return that.
	return numStack.top();
}

// ---------------------------- Main program --------------------------- //

int main ()
{
	cout << "Enter an infix expression: ";
	string infix;
	getline(cin, infix);
	
	string postfix = infixToPostfix(infix);
	cout << "Postfix: " << postfix << endl;
	
	float eval = evalPostfix(postfix);
	cout << "Evaluation: " << eval << endl;
	return 0;
}
