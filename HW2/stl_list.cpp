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
#include <list>


using namespace std;

class Employee{
    private:
        int id;
        int salary;
        int department;
    public:
        Employee(int,int,int);
        bool operator==(const Employee& other) const{
            return this->get_id() == other.get_id();
        }
        int get_id() const;
        int get_salary() const;
        int get_department() const;
        void set_id(int);
        void set_salary(int);
        void set_department(int);
};

Employee::Employee(int id, int salary, int department){
    this->id = id;
    this->salary = salary;
    this->department = department;
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

int Employee::get_id() const{
    return this->id;
}

int Employee::get_salary() const{
    return this->salary;
}

int Employee::get_department() const{
    return this->department;
}

bool is_empty_line(string& line){
    return (line[0] == '\r' || line[0] == '\n');
}

int main(int argc, char** argv) {

    list<Employee> employee_list;
    list<Employee> deleted_employee_list;
    string line;
    string employee_id;
    string employee_salary;
    string employee_department;

    ifstream csv_file;
    csv_file.open(argv[1],ios::in);
    getline(csv_file,line);
    while (getline(csv_file,line)){
        if (!is_empty_line(line)){
            istringstream line_stream(line);
            getline(line_stream,employee_id,';');
            getline(line_stream,employee_salary,';');
            getline(line_stream,employee_department,'\n');
            employee_list.push_back(Employee(stoi(employee_id),stoi(employee_salary),stoi(employee_department)));
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
                int max_id = 0;
                for (const auto &employee : employee_list) {
                    if (employee.get_id() > max_id){
                        max_id = employee.get_id();
                    }
                }
                int id = max_id + 1;
                for (int i = 0; i < deleted_employee_list.size(); i++){
                    for (const auto &deleted_employee : deleted_employee_list) {
                        if (deleted_employee.get_id() == id){
                            id++;
                        }
                    }
                }
                employee_list.push_back(Employee(id,stoi(employee_salary),stoi(employee_department)));
                
            } else if (operation == "UPDATE"){
                getline(line_stream, employee_id,';');
                getline(line_stream, employee_salary,';');
                getline(line_stream, employee_department,'\n');
                bool contains_id = false;
                int id = stoi(employee_id);
                for (auto &employee : employee_list){
                    if (employee.get_id() == id){
                        employee.set_salary(stoi(employee_salary));
                        employee.set_department(stoi(employee_department));
                        contains_id = true;
                        break;
                    }
                }
                if (!contains_id){
                    cerr << "ERROR: An invalid ID to update" << endl;
                }
                
            } else if (operation == "DELETE"){
                getline(line_stream, employee_id,'\n');
                if (employee_list.empty()){
                    cerr << "ERROR: There is no Employee" << endl;
                } else {
                    bool contains_id = false;
                    int id = stoi(employee_id);
                    for (auto &employee : employee_list){
                        if (employee.get_id() == id){
                            Employee to_be_deleted(id,employee.get_salary(),employee.get_department());
                            deleted_employee_list.push_back(to_be_deleted);
                            employee_list.remove(to_be_deleted);
                            contains_id = true;
                            break;
                        }
                    }
                    if (!contains_id){
                        cerr << "ERROR: An invalid ID to delete" << endl;
                    }
                }
                
            } else {
                cerr << "ERROR: Invalid operation\n";
            }
        }
    }
    text_file.close();

    ofstream output_file;
    output_file.open("list_solution.csv",ios::out);
    output_file << "Employee_ID;Salary;Department" << endl;
    for (const auto &employee : employee_list){  
        output_file << employee.get_id() << ';' << employee.get_salary() << ';' << employee.get_department() << "\n";
    }
    output_file.close();


    return EXIT_SUCCESS;
} 