/* @Author
StudentName: Eren Onaran
StudentID: 150210114
Date: 
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>

using namespace std;

class EmployeeNode{
    private:
        int id;
        int salary;
        int department;
        EmployeeNode* next;
    public:
        EmployeeNode(int,int,int);
        // ~EmployeeNode();
        void set_id(int);
        void set_salary(int);
        void set_department(int);
        void set_next(EmployeeNode*);
        int get_id();
        int get_salary();
        int get_department();
        EmployeeNode* get_next();
};

EmployeeNode::EmployeeNode(int id,int salary,int department){
    this->id = id;
    this->salary = salary;
    this->department = department;
    this->next = NULL;
}

void EmployeeNode::set_id(int id){
    this->id = id;
}

void EmployeeNode::set_salary(int salary){
    this->salary = salary;
}

void EmployeeNode::set_department(int department){
    this->department = department;
}

void EmployeeNode::set_next(EmployeeNode* next){
    this->next = next;
}

int EmployeeNode::get_id(){
    return this->id;
}

int EmployeeNode::get_salary(){
    return this->salary;
}

int EmployeeNode::get_department(){
    return this->department;
}

EmployeeNode* EmployeeNode::get_next(){
    return this->next;
}

class EmployeeLinkedList{
    private:
        EmployeeNode* head;
        EmployeeNode* tail;
    public:
        EmployeeLinkedList();
        ~EmployeeLinkedList();
        EmployeeNode* get_head();
        void add_tail(int,int,int);
        void update(int,int,int);
        EmployeeNode* remove(int);
        void display_list();
        int find_max_id();
        int find_employee_num();
};

EmployeeLinkedList::EmployeeLinkedList(){
    this->head = NULL;
    this->tail = NULL;
}

EmployeeLinkedList::~EmployeeLinkedList(){
    while (head != tail){
        EmployeeNode* to_be_deleted = head;
        head = head->get_next();
        delete to_be_deleted;
    }
    delete head;
}

EmployeeNode* EmployeeLinkedList::get_head(){
    return this->head;
}

void EmployeeLinkedList::add_tail(int id,int salary,int department){
    EmployeeNode* new_node = new EmployeeNode(id,salary,department);
    if(head == NULL){
        head = new_node;
        tail = new_node;
    } else {
        tail->set_next(new_node);
        tail = new_node;
    }
}

void EmployeeLinkedList::update(int id, int salary,int department){
    EmployeeNode* traverser = this->head;
    while (traverser != NULL){
        if(traverser->get_id() == id){
            traverser->set_salary(salary);
            traverser->set_department(department);
            return;
        }
        traverser = traverser->get_next();
    }
    cerr << "ERROR: An invalid ID to update" << endl;
}

EmployeeNode* EmployeeLinkedList::remove(int id){

    if (head == NULL){
        cerr << "ERROR: There is no Employee" << endl;
        return NULL;
    } 
    EmployeeNode* to_be_deleted = head;
    while(to_be_deleted != NULL){
        if (to_be_deleted->get_id() == id){
            break;
        }
        to_be_deleted = to_be_deleted->get_next();
    }
    if (to_be_deleted == NULL){
        cerr << "ERROR: An invalid ID to delete" << endl;
        return NULL;
    } else {
        if (head == tail){
            head = NULL;
            tail = NULL;
        } else if (to_be_deleted == head) {
            head = head->get_next();
        } else {
            EmployeeNode* prev = head;
            while(prev != NULL){
                if (prev->get_next() == to_be_deleted){
                    break;
                }
                prev = prev->get_next();
            }
            prev->set_next(to_be_deleted->get_next());
            if (to_be_deleted == tail){
                tail = prev;
            }
        }
        return to_be_deleted;
    }
}

void EmployeeLinkedList::display_list(){
    EmployeeNode* traverser = head;
    while (traverser != NULL){
        cout << traverser->get_id() << "/" << traverser->get_salary() << "/" << traverser->get_department() << endl;
        traverser = traverser->get_next();
    }
}

int EmployeeLinkedList::find_max_id(){
    EmployeeNode* traverser = head;
    int max_id = 0;
    while (traverser != NULL){
        if (traverser->get_id() > max_id){
            max_id = traverser->get_id();
        }
        traverser = traverser->get_next();
    }
    return max_id;
}

int EmployeeLinkedList::find_employee_num(){
    EmployeeNode* traverser = head;
    int employee_num = 0;
    while(traverser != NULL){
        employee_num++; 
        traverser = traverser->get_next();
    }
    return employee_num;
}

bool is_empty_line(string& line){
    return (line[0] == '\r' || line[0] == '\n');
}

int main(int argc, char** argv) {

    EmployeeLinkedList employees;
    EmployeeLinkedList deleted_employees;
    string employee_id;
    string employee_salary;
    string employee_department;
    
    ifstream csv_file;
    csv_file.open(argv[1],ios::in);
    string line;
    getline(csv_file,line);
    while(getline(csv_file,line)){
        if(!is_empty_line(line)){
            istringstream line_stream(line);
            getline(line_stream,employee_id,';');
            getline(line_stream,employee_salary,';');
            getline(line_stream,employee_department,'\n');
            employees.add_tail(stoi(employee_id),stoi(employee_salary),stoi(employee_department));
        }
    }
    csv_file.close();

    string operation;
    ifstream text_file;
    text_file.open(argv[2],ios::in);
    while (getline(text_file,line)){
        if (!is_empty_line(line)){
            istringstream line_stream(line);
            getline(line_stream,operation,';');
            
            if (operation == "ADD"){
                getline(line_stream, employee_salary,';');
                getline(line_stream, employee_department,'\n');
                int id = employees.find_max_id() + 1;
                int deleted_employee_num = deleted_employees.find_employee_num();
                for (int i = 0; i < deleted_employee_num; i++){
                    EmployeeNode* traverser = deleted_employees.get_head();
                    for (int j = 0; j < deleted_employee_num; j++){
                        if(traverser->get_id() == id){
                            id++;
                        }
                        traverser = traverser->get_next();
                    }
                }
                employees.add_tail(id,stoi(employee_salary),stoi(employee_department));
                
            } else if (operation == "UPDATE"){
                getline(line_stream, employee_id,';');
                getline(line_stream, employee_salary,';');
                getline(line_stream, employee_department,'\n');
                employees.update(stoi(employee_id),stoi(employee_salary),stoi(employee_department));

            } else if (operation == "DELETE"){
                getline(line_stream, employee_id,'\n');
                EmployeeNode* deleted_employee = employees.remove(stoi(employee_id));
                if (deleted_employee != NULL){
                    deleted_employees.add_tail(deleted_employee->get_id(),deleted_employee->get_salary(),deleted_employee->get_department());
                    delete deleted_employee;
                }
                
            } else {
                cerr << "ERROR: Invalid operation\n";
            }
        }
    }
    text_file.close();
    
    ofstream output_file;
    output_file.open("linked_list_solution.csv",ios::out);
    output_file << "Employee_ID;Salary;Department" << endl;
    EmployeeNode* traverser = employees.get_head();
    while (traverser != NULL){
        output_file << traverser->get_id() << ';' << traverser->get_salary() << ';' << traverser->get_department() << "\n";
        traverser = traverser->get_next();
    }       
    output_file.close();

    return EXIT_SUCCESS;
} 