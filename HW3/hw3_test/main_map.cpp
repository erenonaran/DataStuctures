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
#include <map>

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

bool is_empty_line(string& line){
    return (line[0] == '\r' || line[0] == '\n');
}

void insert(map<int, Employee*>& EmployeeMap, int salary, int department){
    int next_id = 0;
    if (!EmployeeMap.empty()){
        auto it = EmployeeMap.rbegin();
        next_id = it->first + 1;
    }
    Employee* employee = new Employee(next_id,salary,department);
    EmployeeMap[employee->get_id()] = employee;
}

void update(map<int, Employee*>& EmployeeMap,int id,int salary, int department){
    auto it = EmployeeMap.find(id);
    if (it != EmployeeMap.end()) {
        Employee* employee = it->second;
        employee->set_salary(salary);
        employee->set_department(department);
    } else {
        cerr << "ERROR: An invalid ID to update" << endl;
    }
}

void remove(map<int, Employee*>& EmployeeMap,int id){
    auto it = EmployeeMap.find(id);
    if (it != EmployeeMap.end()) {
        delete it->second;
        EmployeeMap.erase(it);
    } else {
        cerr << "ERROR: An invalid ID to delete" << endl;
    }
}

void printToFile(map<int, Employee*>& EmployeeMap, ofstream& output_file){
    if (!output_file.is_open()){
        output_file.open("output.csv",ios::out);
    }
    output_file << "Employee_ID;Salary;Department" << endl;

    for (auto it = EmployeeMap.begin(); it != EmployeeMap.end(); ++it) {
        Employee* employee = it->second;
        int id = employee->get_id();
        int salary = employee->get_salary();
        int department = employee->get_department();
        output_file << id << ';' << salary << ';' << department << "\n";
    }
    output_file.close();
}

void dealloc(map<int, Employee*>& EmployeeMap){
    for (auto it = EmployeeMap.begin(); it != EmployeeMap.end(); ++it) {
        Employee* employee = it->second;
        delete employee;
    }
}

int main(int argc, char** argv) {

    map<int, Employee*> EmployeeMap;

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
            EmployeeMap[employee->get_id()] = employee;
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
                insert(EmployeeMap,stoi(salary_string),stoi(department_string));
                //clock_t end = clock();
                //cout << "ADD: "<< (double)(end - start) * 1000 / CLOCKS_PER_SEC << endl;
            } else if (operation == "UPDATE"){
                string id_string, salary_string, department_string;
                getline(line_stream,id_string,';');
                getline(line_stream,salary_string,';');
                getline(line_stream,department_string,'\n');
                //clock_t start = clock();
                update(EmployeeMap,stoi(id_string),stoi(salary_string),stoi(department_string));
                //clock_t end = clock();
                //cout << "UPDATE: "<< (double)(end - start) * 1000 / CLOCKS_PER_SEC << endl;
            } else if (operation == "DELETE"){
                string id_string;
                getline(line_stream,id_string,'\n');
                //clock_t start = clock();
                remove(EmployeeMap,stoi(id_string));
                //clock_t end = clock();
                //cout << "DELETE: "<< (double)(end - start) * 1000 / CLOCKS_PER_SEC << endl;
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
    printToFile(EmployeeMap,output_file);
    dealloc(EmployeeMap);

    return EXIT_SUCCESS;
} 