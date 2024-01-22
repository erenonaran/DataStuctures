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

class DoublySkipList_Node{
    private:
        Employee* data;
        DoublySkipList_Node* next;
        DoublySkipList_Node* below;
    public:
        DoublySkipList_Node(Employee*);
        ~DoublySkipList_Node();
        Employee* get_data();
        DoublySkipList_Node* get_next();
        DoublySkipList_Node* get_below();
        void set_data(Employee*);
        void set_next(DoublySkipList_Node*);
        void set_below(DoublySkipList_Node*);
};

DoublySkipList_Node::DoublySkipList_Node(Employee* data){
    this->data = data;
    this->next = nullptr;
    this->below = nullptr;
}

DoublySkipList_Node::~DoublySkipList_Node(){
    //Deletes Employee object when the node is at bottom
    if (this->get_below() == nullptr){
        delete this->data;
    }
}

Employee* DoublySkipList_Node::get_data(){
    return this->data;
}

DoublySkipList_Node* DoublySkipList_Node::get_next(){
    return this->next;
}

DoublySkipList_Node* DoublySkipList_Node::get_below(){
    return this->below;
}

void DoublySkipList_Node::set_data(Employee* data){
    this->data = data;
}

void DoublySkipList_Node::set_next(DoublySkipList_Node* next){
    this->next = next;
}

void DoublySkipList_Node::set_below(DoublySkipList_Node* below){
    this->below = below;
}

class DoublySkipList{
    private:
        DoublySkipList_Node* head;
        int height;
        DoublySkipList_Node* search_from(DoublySkipList_Node*,int);
    public:
        DoublySkipList(int);
        ~DoublySkipList();
        void insert(Employee*);
        void remove(int);
        Employee* search(int);
        void dumpToFile(ofstream&);
        void update(int,int,int);
};

DoublySkipList::DoublySkipList(int height){
    this->height = height;
    //First line is initialized
    this->head = new DoublySkipList_Node(nullptr);
    this->head->set_next(new DoublySkipList_Node(nullptr));
    //New line is added to the bottom (height-1) times
    DoublySkipList_Node* bottom_head = this->head;
    for (int i = 1; i < height; i++){
        bottom_head->set_below(new DoublySkipList_Node(nullptr));
        bottom_head->get_below()->set_next(new DoublySkipList_Node(nullptr));
        bottom_head->get_next()->set_below(bottom_head->get_below()->get_next());
        bottom_head = bottom_head->get_below();
    }
}

DoublySkipList::~DoublySkipList(){

    //Starting from the uppermost line, deletes the nodes in the line 
    DoublySkipList_Node* head_temp = this->head; 
    while (head_temp != nullptr){
        DoublySkipList_Node* head_to_delete = head_temp; 
        head_temp = head_temp->get_below();  //Assigns the address of the below sentinel
        DoublySkipList_Node* node_temp = head_to_delete;
        while (node_temp != nullptr){
            DoublySkipList_Node* node_to_delete = node_temp;
            node_temp = node_temp->get_next(); //Assigns the address of the next node
            delete node_to_delete; //Employee objects are deleted only during the deletion of bottom line
        }
    }  
}

void DoublySkipList::insert(Employee* employee){

    //Generates a random height between 1 and max_height, similar to a geometric RV
    int employee_height = 1; 
    while (employee_height < this->height && rand() % 2 == 1) {
        employee_height++;
    }
    int current_height = this->height;
    DoublySkipList_Node* search_ptr = this->head;

    //Goes as right as possible in the line and switches to the below line
    while (current_height > employee_height){ 
        while (search_ptr->get_next()->get_next() != nullptr && 
               search_ptr->get_next()->get_data()->get_id() < employee->get_id()){
            search_ptr = search_ptr->get_next();
        }
        search_ptr = search_ptr->get_below();
        current_height -= 1;
    }

    //When current_height <= employee_height, must insert node to the current line
    //Before insertion, goes right and finds the previous node
    DoublySkipList_Node* recently_inserted = nullptr;
    while (current_height > 0){
        while (search_ptr->get_next()->get_next() != nullptr && 
               search_ptr->get_next()->get_data()->get_id() < employee->get_id()){
            search_ptr = search_ptr->get_next();
        }

        DoublySkipList_Node* old_next = search_ptr->get_next(); 
        search_ptr->set_next(new DoublySkipList_Node(employee));
        search_ptr->get_next()->set_next(old_next);

        if (recently_inserted != nullptr){
            recently_inserted->set_below(search_ptr->get_next());
        }
        recently_inserted = search_ptr->get_next();
        search_ptr = search_ptr->get_below();
        current_height -= 1;
    }
}

//Due to the absence of previous node pointer, when removing the nodes with the given id
//Common operation to search the node specifying the starting node
//Can also be utilized in search function passing this->head node as starting node
DoublySkipList_Node* DoublySkipList::search_from(DoublySkipList_Node* starting_node, int id){

    DoublySkipList_Node* search_ptr = starting_node;
    while (search_ptr != nullptr){
        if (search_ptr->get_next()->get_next() != nullptr){
            int next_id = search_ptr->get_next()->get_data()->get_id();
            if (id > next_id){
                search_ptr = search_ptr->get_next();
            } else if (next_id == id){
                return search_ptr;
            } else {
                search_ptr = search_ptr->get_below();
            }
        } else {
            search_ptr = search_ptr->get_below();
        }
    }
    return nullptr;
}

Employee* DoublySkipList::search(int id){
    DoublySkipList_Node* prev = this->search_from(this->head,id);
    if (prev != nullptr){
        return prev->get_next()->get_data();
    } else {
        return nullptr;
    }
}

void DoublySkipList::remove(int id){

    DoublySkipList_Node* prev = this->search_from(this->head,id);
    if (prev != nullptr){
        while (prev != nullptr){
            DoublySkipList_Node* to_delete = prev->get_next();
            prev->set_next(to_delete->get_next());
            delete to_delete;
            //Continues searching in the line below
            prev = this->search_from(prev->get_below(),id); 
        }
    } else {
        cerr << "ERROR: An invalid ID to delete" << endl;
    }
}

void DoublySkipList::dumpToFile(ofstream& output_file){

    if (!output_file.is_open()){
        output_file.open("doubly_output.csv",ios::out);
    }
    output_file << "Employee_ID;Salary;Department" << endl;

    DoublySkipList_Node* ptr = this->head;
    while (ptr->get_below() != nullptr){
        ptr = ptr->get_below(); 
    }
    //ptr currently points to the head sentinel of the bottom row
    while (ptr != nullptr){
        if (ptr->get_data() != nullptr){
            int id = ptr->get_data()->get_id();
            int salary = ptr->get_data()->get_salary();
            int department = ptr->get_data()->get_department();
            output_file << id << ';' << salary << ';' << department << "\n";
        }
        ptr = ptr->get_next();
    }
    output_file.close();
}

void DoublySkipList::update(int id, int salary, int department){
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
    DoublySkipList EmployeeSkipList(10);

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
    output_file.open("doubly_output.csv",ios::out);
    if (!output_file.is_open()) {
        cerr << "Unable to open output file!" << endl;
        return EXIT_FAILURE;
    }
    EmployeeSkipList.dumpToFile(output_file);

    return EXIT_SUCCESS;
} 