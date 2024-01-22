#pragma once
#include <string>

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