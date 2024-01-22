/* @Author
StudentName: Eren Onaran
StudentID: 150210114
Date: 28.10.2023
*/

#include "employee.hpp"
#include <string>

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