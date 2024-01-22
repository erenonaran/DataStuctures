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
#include <vector>

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

void Employee::set_id(int id){
    this->id = id;
}

void Employee::set_salary(int salary){
    this->salary = salary;
}

void Employee::set_department(int department){
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

bool is_empty_line(string& line){
    return (line[0] == '\r' || line[0] == '\n');
}

int main(int argc, char** argv) {

    vector<Employee> employee_vector;
    vector<Employee> deleted_employee_vector;
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
            employee_vector.push_back(Employee(stoi(employee_id),stoi(employee_salary),stoi(employee_department)));
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
                for (int i = 0; i < employee_vector.size();i++){
                    if (employee_vector[i].get_id() > max_id){
                        max_id = employee_vector[i].get_id();
                    }
                }
                int id = max_id + 1;
                for (int i = 0; i < deleted_employee_vector.size(); i++){
                    for (int j = 0; j < deleted_employee_vector.size(); j++){
                        if (deleted_employee_vector[j].get_id() == id){
                            id++;
                        }
                    }
                }
                employee_vector.push_back(Employee(id,stoi(employee_salary),stoi(employee_department)));
                
            } else if (operation == "UPDATE"){
                getline(line_stream, employee_id,';');
                getline(line_stream, employee_salary,';');
                getline(line_stream, employee_department,'\n');
                bool contains_id = false;
                int id = stoi(employee_id);
                for (int i = 0; i < employee_vector.size(); i++){
                    if (employee_vector[i].get_id() == id){
                        employee_vector[i].set_salary(stoi(employee_salary));
                        employee_vector[i].set_department(stoi(employee_department));
                        contains_id = true;
                        break;
                    }
                } 
                if (!contains_id){
                    cerr << "ERROR: An invalid ID to update" << endl;
                }
                
            } else if (operation == "DELETE"){
                getline(line_stream, employee_id,'\n');
                if (employee_vector.empty()){
                    cerr << "ERROR: There is no Employee" << endl;
                } else {
                    bool contains_id = false;
                    int id = stoi(employee_id);
                    for (int i = 0; i < employee_vector.size(); i++){
                        if (employee_vector[i].get_id() == id){
                            Employee deleted(id,employee_vector[i].get_salary(),employee_vector[i].get_department());
                            deleted_employee_vector.push_back(deleted);
                            employee_vector.erase(employee_vector.begin() + i);
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
    output_file.open("vector_solution.csv",ios::out);
    output_file << "Employee_ID;Salary;Department" << endl;
    for (int i = 0; i < employee_vector.size(); i++){  
        output_file << employee_vector[i].get_id() << ';' << employee_vector[i].get_salary() << ';' << employee_vector[i].get_department() << "\n";
    }
    output_file.close();


    return EXIT_SUCCESS;
} 