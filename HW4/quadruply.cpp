/* @Author
StudentName: Eren Onaran
StudentID: 150210114
Date: 08.01.2024
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <cstdlib>
#include <ctime>

using namespace std;

class Employee{
    private:
        int id;
        int salary;
        int department;
        static int max_id;
    public:
        Employee(int,int,int);
        Employee(int,int);
        int get_id();
        int get_salary();
        int get_department();
        void set_id(int);
        void set_salary(int);
        void set_department(int);
};

int Employee::max_id = -1;

Employee::Employee(int id, int salary, int department){
    this->id = id;
    this->salary = salary;
    this->department = department;
    if (id > max_id){
        max_id = id;
    }
}

Employee::Employee(int salary,int department){
    this->id = max_id + 1;
    this->salary = salary;
    this->department = department;
    max_id++;
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

class QuadruplySkipList_Node{
    private:
        Employee* data;
        QuadruplySkipList_Node* next;
        QuadruplySkipList_Node* prev;
        QuadruplySkipList_Node* below;
        QuadruplySkipList_Node* above;
    public:
        QuadruplySkipList_Node(Employee*);
        ~QuadruplySkipList_Node();
        Employee* get_data();
        QuadruplySkipList_Node* get_next();
        QuadruplySkipList_Node* get_prev();
        QuadruplySkipList_Node* get_below();
        QuadruplySkipList_Node* get_above();
        void set_data(Employee*);
        void set_next(QuadruplySkipList_Node*);
        void set_prev(QuadruplySkipList_Node*);
        void set_below(QuadruplySkipList_Node*);
        void set_above(QuadruplySkipList_Node*);    
};

QuadruplySkipList_Node::QuadruplySkipList_Node(Employee* employee){
    this->data = employee;
    this->next = nullptr; 
    this->prev = nullptr; 
    this->below = nullptr; 
    this->above = nullptr; 
}

QuadruplySkipList_Node::~QuadruplySkipList_Node(){
    //Deletes Employee object when the node is at bottom
    if (this->get_below() == nullptr){
        delete this->data;
    }
}

Employee* QuadruplySkipList_Node::get_data(){
    return this->data;
}

QuadruplySkipList_Node* QuadruplySkipList_Node::get_next(){
    return this->next;
}

QuadruplySkipList_Node* QuadruplySkipList_Node::get_prev(){
    return this->prev;
}

QuadruplySkipList_Node* QuadruplySkipList_Node::get_below(){
    return this->below;
}

QuadruplySkipList_Node* QuadruplySkipList_Node::get_above(){
    return this->above;
}

void QuadruplySkipList_Node::set_data(Employee* employee){
    this->data = employee;
}

void QuadruplySkipList_Node::set_next(QuadruplySkipList_Node* next){
    this->next = next;
}

void QuadruplySkipList_Node::set_prev(QuadruplySkipList_Node* prev){
    this->prev = prev;
}

void QuadruplySkipList_Node::set_above(QuadruplySkipList_Node* above){
    this->above = above;
}

void QuadruplySkipList_Node::set_below(QuadruplySkipList_Node* below){
    this->below = below;
}

class QuadruplySkipList{
    private:
        QuadruplySkipList_Node* head;
        int height;
        QuadruplySkipList_Node* search_top_node(int);
    public:
        QuadruplySkipList(int);  
        ~QuadruplySkipList();   
        void insert(Employee*);  
        void remove(int); 
        Employee* search(int);  
        void dumpToFile(ofstream&);
        void update(int,int,int);
};

QuadruplySkipList::QuadruplySkipList(int height){
    this->height = height;
    
    //Creates the start and end sentinels of the uppermost line
    this->head = new QuadruplySkipList_Node(nullptr);  
    this->head->set_next(new QuadruplySkipList_Node(nullptr));
    this->head->get_next()->set_prev(this->head);

    QuadruplySkipList_Node* bottomleft = this->head;
    for (int i = 1; i < height; i++){
        //Using the pointer to the bottomleft node creates the line below (height - 1) times 
        bottomleft->set_below(new QuadruplySkipList_Node(nullptr));
        bottomleft->get_below()->set_above(bottomleft);
        bottomleft->get_below()->set_next(new QuadruplySkipList_Node(nullptr));
        bottomleft->get_below()->get_next()->set_prev(bottomleft->get_below());
        bottomleft->get_next()->set_below(bottomleft->get_below()->get_next());
        bottomleft->get_next()->get_below()->set_above(bottomleft->get_next());
        bottomleft = bottomleft->get_below();
    }
}

QuadruplySkipList::~QuadruplySkipList(){

    //Starting from the uppermost line, deletes the nodes in the line 
    QuadruplySkipList_Node* head_temp = this->head; 
    while (head_temp != nullptr){
        QuadruplySkipList_Node* head_to_delete = head_temp; 
        head_temp = head_temp->get_below();  //Assigns the address of the below sentinel
        QuadruplySkipList_Node* node_temp = head_to_delete;
        while (node_temp != nullptr){
            QuadruplySkipList_Node* node_to_delete = node_temp;
            node_temp = node_temp->get_next(); //Assigns the address of the next node
            delete node_to_delete; 
        }
    }
}

void QuadruplySkipList::insert(Employee* employee){
    //Generates a random height between 1 and max_height, similar to a geometric RV
    int employee_height = 1; 
    while (employee_height < this->height && rand() % 2 == 1) {
        employee_height++;
    }
    int current_height = this->height;
    QuadruplySkipList_Node* search_ptr = this->head;

    //Goes as right as possible in the line and switches to the below line
    while (current_height > employee_height){ 
        while (search_ptr->get_next()->get_next() != nullptr && 
               search_ptr->get_next()->get_data()->get_id() < employee->get_id()){
            search_ptr = search_ptr->get_next();
        }
        search_ptr = search_ptr->get_below();
        current_height -= 1;
    }
    
    //Finds the previous node, adjusts the pointer attributes of the neighbor nodes
    QuadruplySkipList_Node* recently_inserted = nullptr;
    while (current_height > 0){
        while (search_ptr->get_next()->get_next() != nullptr && 
               search_ptr->get_next()->get_data()->get_id() < employee->get_id()){
            search_ptr = search_ptr->get_next();
        }

        QuadruplySkipList_Node* old_next = search_ptr->get_next(); 
        search_ptr->set_next(new QuadruplySkipList_Node(employee));
        search_ptr->get_next()->set_next(old_next);
        old_next->set_prev(search_ptr->get_next());
        search_ptr->get_next()->set_prev(search_ptr);

        search_ptr->get_next()->set_above(recently_inserted);
        if (recently_inserted != nullptr){
            recently_inserted->set_below(search_ptr->get_next());
        }

        recently_inserted = search_ptr->get_next();
        search_ptr = search_ptr->get_below();
        current_height -= 1;
    }
}

//Common operation in Employee* search and removal
//Returns the address of the top node with given id
QuadruplySkipList_Node* QuadruplySkipList::search_top_node(int id){

    QuadruplySkipList_Node* search_ptr = this->head;
    int current_height = this->height;
    while (current_height > 0){
        if (search_ptr->get_next()->get_next() != nullptr){
            // Goes towards right as much as possible then switches to the below line
            if (search_ptr->get_next()->get_data()->get_id() < id){
                search_ptr = search_ptr->get_next();
            } else if (search_ptr->get_next()->get_data()->get_id() == id){
                return search_ptr->get_next();
            } else {
                search_ptr = search_ptr->get_below();
                current_height -= 1;
            }
        } else {
            search_ptr = search_ptr->get_below();
            current_height -= 1;
        }
    }
    return nullptr; //Returns nullptr when the Employee with given id is not found
}

Employee* QuadruplySkipList::search(int id){
    QuadruplySkipList_Node* top_node = this->search_top_node(id);
    if (top_node != nullptr){
        return top_node->get_data();
    } else {
        return nullptr;
    }
}

void QuadruplySkipList::remove(int id){

    QuadruplySkipList_Node* top_node = this->search_top_node(id);
    if (top_node == nullptr){
        cerr << "ERROR: An invalid ID to delete" << endl;
        return;
    } 
    while (top_node != nullptr){
        //With the help of previous node pointer one search is sufficient
        QuadruplySkipList_Node* to_delete = top_node;
        top_node = top_node->get_below();
        to_delete->get_prev()->set_next(to_delete->get_next());
        to_delete->get_next()->set_prev(to_delete->get_prev());
        delete to_delete;
    }
}

void QuadruplySkipList::dumpToFile(ofstream& output_file){

    if (!output_file.is_open()){
        output_file.open("quadruply_output.csv",ios::out);
    }
    output_file << "Employee_ID;Salary;Department" << endl;

    QuadruplySkipList_Node* ptr = this->head;
    while (ptr->get_below() != nullptr){
        ptr = ptr->get_below(); 
    }
    //ptr currently points to the head sentinel of the bottom row
    while (ptr != nullptr){
        if (ptr->get_data() != nullptr){ //sentinel nodes have no Employee data
            int id = ptr->get_data()->get_id();
            int salary = ptr->get_data()->get_salary();
            int department = ptr->get_data()->get_department();
            output_file << id << ';' << salary << ';' << department << "\n";
        }
        ptr = ptr->get_next();
    }
    output_file.close();
}

void QuadruplySkipList::update(int id, int salary, int department){
    Employee* to_update = this->search(id);
    if (to_update != nullptr){
        to_update->set_salary(salary);
        to_update->set_department(department);
    } else {
        cerr << "ERROR: An invalid ID to update" << endl;
    }
}

bool is_empty_line(string& line){
    return (line[0] == '\r' || line[0] == '\n');
}

int main(int argc, char** argv) {

    srand(static_cast<unsigned int>(time(nullptr)));
    QuadruplySkipList EmployeeSkipList(20);

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
            EmployeeSkipList.insert(employee);
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
            istringstream line_stream(line);
            string operation;
            getline(line_stream,operation,';');
    
            if (operation == "ADD"){
                string salary_string, department_string;
                getline(line_stream,salary_string,';');
                getline(line_stream,department_string,'\n');
                //clock_t start = clock();
                Employee* new_emp = new Employee(stoi(salary_string),stoi(department_string));
                EmployeeSkipList.insert(new_emp);
                //clock_t end = clock();
                //cout << "ADD: "<< (double)(end - start) * 1000 / CLOCKS_PER_SEC << endl;
            } else if (operation == "UPDATE"){
                string id_string, salary_string, department_string;
                getline(line_stream,id_string,';');
                getline(line_stream,salary_string,';');
                getline(line_stream,department_string,'\n');
                //clock_t start = clock();
                EmployeeSkipList.update(stoi(id_string),stoi(salary_string),stoi(department_string));
                //clock_t end = clock();
                //cout << "UPDATE: "<< (double)(end - start) * 1000 / CLOCKS_PER_SEC << endl;
            } else if (operation == "DELETE"){
                string id_string;
                getline(line_stream,id_string,'\n');
                //clock_t start = clock();
                EmployeeSkipList.remove(stoi(id_string));
                //clock_t end = clock();
                //cout << "DELETE: "<< (double)(end - start) * 1000 / CLOCKS_PER_SEC << endl;
            } else {
                cerr << "ERROR: Invalid operation\n";
            }
        }
    }
    op_file.close();
    

    ofstream output_file;
    output_file.open("quadruply_output.csv",ios::out);
    if (!output_file.is_open()) {
        cerr << "Unable to open output file!" << endl;
        return EXIT_FAILURE;
    }
    EmployeeSkipList.dumpToFile(output_file);

    return EXIT_SUCCESS;
} 