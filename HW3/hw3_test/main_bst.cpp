/* @Author
StudentName: Eren Onaran
StudentID: 150210114
Date: 
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stack>
#include <queue>
#include <stdio.h>
#include <time.h>

using namespace std;

class Employee{
    private:
        int id;
        int salary;
        int department;
    public:
        Employee(int,int,int);
        int get_id();
        int get_salary();
        int get_department();
        void set_id(int);
        void set_salary(int);
        void set_department(int);
};

Employee::Employee(int id, int salary, int department){
    this->id = id;
    this->salary = salary;
    this->department = department;
}

int Employee::get_id(){
    return this->id;
}

int Employee::get_salary(){
    return this->salary;
}

int Employee::get_department(){
    return this->department;
}

void Employee::set_id(int id){
    this->id = id;
}

void Employee::set_salary(int salary){
    this->salary = salary;
}

void Employee::set_department(int department){
    this->department = department;
}

class Node{
    private:
        Employee *employee;
        Node *left;
        Node *right;
    public:
        Node(Employee*);
        ~Node();
        Employee* get_employee();
        Node* get_left();
        Node* get_right();
        Node** get_left_address();
        Node** get_right_address();
        void set_employee(Employee*);
        void set_left(Node*);
        void set_right(Node*);
};

Node::Node(Employee* employee){
    this->employee = employee;
    this->left = nullptr;
    this->right = nullptr;
}

Node::~Node(){
    delete this->employee;
}

Employee* Node::get_employee(){
    return this->employee;
}

Node* Node::get_left(){
    return this->left;
}

Node* Node::get_right(){
    return this->right;
}

Node** Node::get_left_address(){
    return &(this->left);
}

Node** Node::get_right_address(){
    return &(this->right);
}

void Node::set_employee(Employee* employee){
    this->employee = employee;
}

void Node::set_left(Node* left){
    this->left = left;
}

void Node::set_right(Node* right){
    this->right = right;
}

class BinaryTree{
    private:
        Node* root;
        Node** search_inner(int);
    public:
        BinaryTree();
        ~BinaryTree();
        void insert(Employee*);
        void insert(int,int); 
        void update(int,int,int);
        void remove(int);
        Employee *search(int);
        int getHeight();
        void printToFile(ofstream&);
        void printToConsole();
};

BinaryTree::BinaryTree(){
    this->root = nullptr;
}

BinaryTree::~BinaryTree(){
    if (this->root == nullptr){
        return;
    }

    stack <Node*> stack;
    stack.push(this->root);
    
    while (!stack.empty()){
        Node* temp = stack.top();
        stack.pop();
        if (temp->get_right() != nullptr){
            stack.push(temp->get_right());
        }
        if (temp->get_left() != nullptr){
            stack.push(temp->get_left());
        }
        delete temp;
        temp = nullptr;
    }
}

void BinaryTree::insert(Employee* employee){
    if (this->root == nullptr){
        Node* new_employee = new Node(employee);
        this->root = new_employee;
        return;
    } 
    Node* temp = this->root;
    while (temp != nullptr){
        if (employee->get_id() > temp->get_employee()->get_id()){
            if (temp->get_right() == nullptr){
                Node* new_employee = new Node(employee);
                temp->set_right(new_employee);
                return;
            }
            temp = temp->get_right();
        } else if (employee->get_id() < temp->get_employee()->get_id()){
            if (temp->get_left() == nullptr){
                Node* new_employee = new Node(employee);
                temp->set_left(new_employee);
                return;
            }
            temp = temp->get_left();
        } else {
            cerr << "ERROR: ID already exists" << endl;
            return;
        }
    }
}

void BinaryTree::insert(int salary, int department){

    if (this->root == nullptr){
        this->root = new Node(new Employee(1,salary,department));
        return;
    }

    Node* temp = this->root;
    while (temp->get_right() != nullptr){
        temp = temp->get_right(); 
    }
    int id = temp->get_employee()->get_id();
    temp->set_right(new Node(new Employee(id+1,salary,department)));
}


void BinaryTree::update(int id, int salary, int department){
    Employee* to_be_updated = this->search(id);

    if (to_be_updated == nullptr){
        cerr << "ERROR: An invalid ID to update" << endl;
        return;
    }
    to_be_updated->set_salary(salary);
    to_be_updated->set_department(department);
}

Node** BinaryTree::search_inner(int id){ 
    Node** temp = &(this->root);

    while (*temp != nullptr){
        if (id > (*temp)->get_employee()->get_id()){
            temp = (*temp)->get_right_address();
            if ((*temp != nullptr) && (*temp)->get_employee()->get_id() == id){
                return temp;
            }
        } else if (id < (*temp)->get_employee()->get_id()){
            temp = (*temp)->get_left_address();
            if ((*temp != nullptr) && (*temp)->get_employee()->get_id() == id){
                return temp;
            }
        } else {
            return temp; 
        }
    }
    return nullptr; 
}

Employee* BinaryTree::search(int id){
    Node** ptr = this->search_inner(id);

    if (ptr == nullptr){
        return nullptr;
    } else {
        return (*ptr)->get_employee();
    }
}

void BinaryTree::remove(int id){
    Node** ptr = this->search_inner(id);

    if (ptr == nullptr){
        cerr << "ERROR: An invalid ID to delete" << endl;
        return;
    } 

    Node* to_delete = *ptr;
    if (to_delete->get_left() == nullptr){
        *ptr = to_delete->get_right();
        delete to_delete;
    } else if (to_delete->get_right() == nullptr){
        *ptr = to_delete->get_left();
        delete to_delete;
    } else {

        Node** min = to_delete->get_right_address();
        while((*min)->get_left() != nullptr){
            min = (*min)->get_left_address();
        }
        Node* to_copy = *min;
        Employee* employee_copy = to_copy->get_employee();
        to_copy->set_employee(to_delete->get_employee());
        to_delete->set_employee(employee_copy);
        *min = to_copy->get_right();
        delete to_copy;
    }  
}

int BinaryTree::getHeight(){ 

    if (this->root == nullptr) {
        return 0;
    }

    int height = 0;
    queue<Node*> q;
    q.push(this->root);

    while (!q.empty()){
        height++;
        int node_at_level = q.size();
        for (int i = 0; i < node_at_level; i++){
            Node* current = q.front();
            q.pop();
            if (current->get_left() != nullptr){
                q.push(current->get_left());
            }
            if (current->get_right() != nullptr){
                q.push(current->get_right());
            }
        }  
    }
    return height;
}

void BinaryTree::printToFile(ofstream& output_file){
    if (!output_file.is_open()){
        output_file.open("output.csv",ios::out);
    }
    output_file << "Employee_ID;Salary;Department" << endl;

     if (this->root == nullptr){
        output_file.close();
        return;
    }

    struct NodeWithFlag{
        Node* node;
        bool flag;

        NodeWithFlag(Node* n,bool f) : node(n),flag(f) {}
    };

    stack<NodeWithFlag> stack;
    stack.push(NodeWithFlag(this->root,false));

    while (!stack.empty()){
        NodeWithFlag& current = stack.top();
        if (!current.flag){
            current.flag = true;
            if (current.node->get_left() != nullptr){
                stack.push(NodeWithFlag(current.node->get_left(),false));
            }
        } else {
            stack.pop();
            int id = current.node->get_employee()->get_id();
            int salary = current.node->get_employee()->get_salary();
            int department = current.node->get_employee()->get_department();
            output_file << id << ';' << salary << ';' << department << "\n";
            if (current.node->get_right() != nullptr){
                stack.push(NodeWithFlag(current.node->get_right(),false));
            }
        }
    }
    output_file.close();
}

void BinaryTree::printToConsole(){

    cout << "P" << endl;

    if (this->root == nullptr){
        return;
    }

    struct NodeWithFlag{
        Node* node;
        bool flag;

        NodeWithFlag(Node* n,bool f) : node(n),flag(f) {}
    };

    stack<NodeWithFlag> stack;
    stack.push(NodeWithFlag(this->root,false));

    while (!stack.empty()){
        NodeWithFlag& current = stack.top();
        if (!current.flag){
            Employee* employee = current.node->get_employee();
            int id = employee->get_id();
            int salary = employee->get_salary();
            int department = employee->get_department();
            cout << id << ';' << salary << ';' << department << endl;
            current.flag = true;
            if (current.node->get_left() != nullptr){
                stack.push(NodeWithFlag(current.node->get_left(),false));
            }
        } else {
            stack.pop();
            if (current.node->get_right() != nullptr){
                stack.push(NodeWithFlag(current.node->get_right(),false));
            }
        }
    }
}

bool is_empty_line(string& line){
    return (line[0] == '\r' || line[0] == '\n');
}

int main(int argc, char** argv) {

    BinaryTree EmployeeTree;

    ifstream dataset_file;
    dataset_file.open(argv[1],ios::in);
    if (!dataset_file.is_open()) {
        cerr << "Unable to open dataset file!" << endl;
        return EXIT_FAILURE;
    }

    string line;
    getline(dataset_file,line);
    while(getline(dataset_file,line)){
        if(!is_empty_line(line)){
            istringstream line_stream(line);
            string id_string, salary_string, department_string;
            getline(line_stream,id_string,';');
            getline(line_stream,salary_string,';');
            getline(line_stream,department_string,'\n');
            int id = stoi(id_string), salary = stoi(salary_string), department = stoi(department_string);
            Employee* employee = new Employee(id,salary,department);
            EmployeeTree.insert(employee);
        }
    }
    dataset_file.close();


    ifstream op_file;
    op_file.open(argv[2],ios::in);
    if (!op_file.is_open()) {
        cerr << "Unable to open operations file!" << endl;
        return EXIT_FAILURE;
    }

    while (getline(op_file,line)){
        if (!is_empty_line(line)){
            if (line.back() == '\r'){
                line.pop_back();
            }
            istringstream line_stream(line);
            string operation;
            getline(line_stream,operation,';');
    
            if (operation == "ADD"){
                string salary_string, department_string;
                getline(line_stream,salary_string,';');
                getline(line_stream,department_string,'\n');
                //clock_t start = clock();
                EmployeeTree.insert(stoi(salary_string),stoi(department_string));
                //clock_t end = clock();
                //cout << "ADD: "<< (double)(end - start) * 1000 / CLOCKS_PER_SEC << endl;
            } else if (operation == "UPDATE"){
                string id_string, salary_string, department_string;
                getline(line_stream,id_string,';');
                getline(line_stream,salary_string,';');
                getline(line_stream,department_string,'\n');
                //clock_t start = clock();
                EmployeeTree.update(stoi(id_string),stoi(salary_string),stoi(department_string));
                //clock_t end = clock();
                //cout << "UPDATE: "<< (double)(end - start) * 1000 / CLOCKS_PER_SEC << endl;
            } else if (operation == "DELETE"){
                string id_string;
                getline(line_stream,id_string,'\n');
                //clock_t start = clock();
                EmployeeTree.remove(stoi(id_string));
                //clock_t end = clock();
                //cout << "DELETE: "<< (double)(end - start) * 1000 / CLOCKS_PER_SEC << endl;
            } else if (operation == "PRINT"){
                EmployeeTree.printToConsole();
            } else if (operation == "HEIGHT"){
                cout << "H "<< EmployeeTree.getHeight() << endl;
            } else {
                cerr << "ERROR: Invalid operation\n";
            }
        }
    }
    op_file.close();

    ofstream output_file;
    output_file.open("output.csv",ios::out);
    if (!output_file.is_open()) {
        cerr << "Unable to open output file!" << endl;
        return EXIT_FAILURE;
    }
    EmployeeTree.printToFile(output_file);

    return EXIT_SUCCESS;
} 