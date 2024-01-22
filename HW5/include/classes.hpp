#pragma once
#include <string>
#include <vector>
#include <list>
#include <map>

using namespace std;

class Employee{
    private:
        int id;
        int salary;
        int department;
    public:
        Employee();
        Employee(int,int,int);
        int get_id();
        int get_salary();
        int get_department();
        void set_id(int);
        void set_salary(int);
        void set_department(int);
};

class EmployeeArray{
    private:
        int size;
        Employee* ptr;
        int id_index(int);
    public:
        EmployeeArray();
        ~EmployeeArray();
        void add_employee(int,int,int);
        void update_employee(int,int,int);
        void delete_employee(int);
};

class EmployeeNode{
    private:
        Employee* data;
        EmployeeNode* next;
    public:
        EmployeeNode(int,int,int);
        ~EmployeeNode();
        Employee* get_data();
        EmployeeNode* get_next();
        void set_next(EmployeeNode*);
};

class EmployeeLinkedList{
    private:
        EmployeeNode* head;
        EmployeeNode* tail;
    public:
        EmployeeLinkedList();
        ~EmployeeLinkedList();
        void add_employee(int,int,int);
        void update_employee(int,int,int);
        void delete_employee(int);
};

class TreeNode{
    private:
        Employee* employee;
        TreeNode* left;
        TreeNode* right;
    public:
        TreeNode(Employee*);
        ~TreeNode();
        Employee* get_employee();
        TreeNode* get_left();
        TreeNode* get_right();
        TreeNode** get_left_address();
        TreeNode** get_right_address();
        void set_employee(Employee*);
        void set_left(TreeNode*);
        void set_right(TreeNode*);
};

class BinaryTree{
    private:
        TreeNode* root;
        TreeNode** search_inner(int);
    public:
        BinaryTree();
        ~BinaryTree();
        void add_employee(int,int,int); 
        void update_employee(int,int,int);
        void delete_employee(int);
        Employee *search(int);
};

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

class DoublySkipList{
    private:
        DoublySkipList_Node* head;
        int height;
        DoublySkipList_Node* search_from(DoublySkipList_Node*,int);
    public:
        DoublySkipList(int);
        ~DoublySkipList();
        void add_employee(int,int,int);
        void delete_employee(int);
        Employee* search(int);
        void update_employee(int,int,int);
};

class STLVector{
    private:
        vector<Employee*> vec;
    public:
        ~STLVector();
        void add_employee(int,int,int);
        void update_employee(int,int,int);
        void delete_employee(int);
};

class STLLinkedList{
    private:
        list<Employee*> employeeList;
    public:
        ~STLLinkedList();
        void add_employee(int,int,int);
        void update_employee(int,int,int);
        void delete_employee(int);
};

class STLMap{
    private:
        map<int,Employee*> employeeMap;
    public:
        ~STLMap();
        void add_employee(int,int,int);
        void update_employee(int,int,int);
        void delete_employee(int);
};

class FileIO{
    private:
        string filePath;
        string folderPath;
        int find_line_index(int);
    public:
        FileIO(string, string);
        void add_employee(int,int,int);
        void update_employee(int,int,int);
        void delete_employee(int);
};