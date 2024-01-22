/* @Author
StudentName: Eren Onaran
StudentID: 150210114
Date: 28.10.2023
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>

using namespace std;

class Employee{
    private:
        int id;
        int salary;
        int department;
    public:
        Employee();
        void set_salary(int);
        void set_id(int);
        void set_department(int);
        void set_all(int,int,int);
        int get_salary();
        int get_id();
        int get_department();
};

Employee::Employee(){
    id = 0;
    salary = 0;
    department = 0;
}

void Employee::set_salary(int salary){
    this->salary = salary;
}

void Employee::set_id(int id){
    this->id = id;
}

void Employee::set_department(int department){
    this->department = department;
}

void Employee::set_all(int id, int salary, int department){
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

bool is_empty_line(string& line){
    return (line[0] == '\r' || line[0] == '\n');
}

Employee* add_employee(Employee* employees,int& employee_num, int salary, int department,Employee* deleted_employees,int deleted_employee_num){
    //clock_t start = clock();
    Employee* employees_extended = new Employee[employee_num + 1];
    
    int max_id = 0;
    
    for (int i = 0; i < employee_num; i++){
        if (employees[i].get_id() > max_id){
            max_id = employees[i].get_id();
        }
        employees_extended[i].set_all(employees[i].get_id(),employees[i].get_salary(),employees[i].get_department());
    }
    max_id++;
    for (int j = 0; j < employee_num; j++){
        for (int i = 0; i < deleted_employee_num; i++){
            if (max_id == deleted_employees[i].get_id()){
                max_id++;
            }
        }
    }
    employees_extended[employee_num].set_all(max_id,salary,department);
    employee_num++;
    delete[] employees;
    //clock_t end = clock();
    //cout << "ADD:  " <<(double)(end - start) * 1000 / CLOCKS_PER_SEC << endl;
    return employees_extended;
}
int id_index(Employee* employees,int employee_num, int id){

    for (int i = 0; i < employee_num; i++){
        if (employees[i].get_id() == id){
            return i;
        }
    }
    return -1;
}

Employee* delete_employee(Employee* employees,int& employee_num, int id,Employee* deleted_employees, int& deleted_employee_num){
    
    if (employee_num == 0){
        cerr << "ERROR: There is no Employee" << endl;
        return employees;
    }
    //clock_t start = clock();
    int index = id_index(employees, employee_num, id);
    if (index != -1){
        deleted_employees[deleted_employee_num].set_all(employees[index].get_id(),employees[index].get_salary(),employees[index].get_department());
        deleted_employee_num++;

        Employee* employees_reduced = new Employee[employee_num - 1];
        int i = 0;
        for (; i < index; i++){
            employees_reduced[i].set_all(employees[i].get_id(),employees[i].get_salary(),employees[i].get_department());
        }
        for (; i < employee_num - 1;i++){
            employees_reduced[i].set_all(employees[i+1].get_id(),employees[i+1].get_salary(),employees[i+1].get_department());
        }
        employee_num--;
        delete[] employees;
        //clock_t end = clock();
        //cout << "DELETE:  " <<(double)(end - start) * 1000 / CLOCKS_PER_SEC << endl;
        return employees_reduced;
    } else {
        cerr << "ERROR: An invalid ID to delete" << endl;
        //clock_t end = clock();
        //cout << "DELETE:  " <<(double)(end - start) * 1000 / CLOCKS_PER_SEC << endl;
        return employees;
    }
}

void update_employee(Employee* employees, int employee_num, int id, int salary, int department){
    //clock_t start = clock();    
    int index = id_index(employees,employee_num,id);
    if (index != -1) {
        employees[index].set_salary(salary);
        employees[index].set_department(department);
    } else {
        cerr << "ERROR: An invalid ID to update" << endl;
    }
    //clock_t end = clock();
    //cout << "UPDATE:  " <<(double)(end - start) * 1000 / CLOCKS_PER_SEC << endl;
}

int main(int argc, char** argv) {

    ifstream csv_file;
    csv_file.open(argv[1],ios::in);
    
    string line;
    int line_num = 0;
    while (getline(csv_file, line)){
       if (!is_empty_line(line)){
            line_num++;
       }  
    }
    csv_file.close();

    int employee_num = line_num - 1;
    int deleted_employee_num = 0;
    Employee* employees = new Employee[employee_num];
    Employee deleted_employees[10];

    string employee_id;
    string employee_salary;
    string employee_department;

    int i = 0;
    csv_file.open(argv[1],ios::in);
    getline(csv_file,line);
    while (getline(csv_file,line)){
        if (!is_empty_line(line)){
            istringstream line_stream(line);
            getline(line_stream,employee_id,';');
            getline(line_stream,employee_salary,';');
            getline(line_stream,employee_department,'\n');

            employees[i].set_all(stoi(employee_id),stoi(employee_salary),stoi(employee_department));
            i++;
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
                employees = add_employee(employees,employee_num,stoi(employee_salary),stoi(employee_department),deleted_employees,deleted_employee_num);
            } else if (operation == "UPDATE"){
                getline(line_stream, employee_id,';');
                getline(line_stream, employee_salary,';');
                getline(line_stream, employee_department,'\n');
                update_employee(employees,employee_num,stoi(employee_id),stoi(employee_salary),stoi(employee_department));
            } else if (operation == "DELETE"){
                getline(line_stream, employee_id,'\n');
                employees = delete_employee(employees,employee_num,stoi(employee_id),deleted_employees,deleted_employee_num);
            } else {
                cerr << "ERROR: Invalid operation\n";
            }
        }
    }
    text_file.close();

    //clock_t start = clock();
    ofstream output_file;
    output_file.open("array_solution.csv",ios::out);
    output_file << "Employee_ID;Salary;Department" << endl;
    for (int i = 0; i < employee_num; i++){  
        output_file << employees[i].get_id() << ';' << employees[i].get_salary() << ';' << employees[i].get_department() << "\n";
    }
    output_file.close();
    //clock_t end = clock();
    //cout << "WRITE TO A FILE:  " <<(double)(end - start) * 1000 / CLOCKS_PER_SEC << endl;

    delete[] employees;
    
    return EXIT_SUCCESS;
} 