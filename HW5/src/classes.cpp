#include "../include/classes.hpp"
#include <iostream>
#include <fstream>
#include <stack>
#include <sstream>


using namespace std;

Employee::Employee(){
    this->id = 0;
    this->salary = 0;
    this->department = 0;
}

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

EmployeeArray::EmployeeArray(){
    this->size = 0;
    this->ptr = nullptr;
}

EmployeeArray::~EmployeeArray(){
    delete[] this->ptr;
}

int EmployeeArray::id_index(int id){
    for (int i = 0; i < this->size; i++){
        if (this->ptr[i].get_id() == id){
            return i;
        }
    }
    return -1;
}

void EmployeeArray::add_employee(int new_id, int new_sal,int new_dep){
    int arrSize = this->size;
    Employee* extended = new Employee[arrSize + 1];
    for (int i = 0; i < arrSize; i++){
        int id = this->ptr[i].get_id();
        int salary = this->ptr[i].get_salary();
        int department = this->ptr[i].get_department();

        extended[i].set_id(id);
        extended[i].set_salary(salary);
        extended[i].set_department(department);
    }
    extended[arrSize].set_id(new_id);
    extended[arrSize].set_salary(new_sal);
    extended[arrSize].set_department(new_dep);

    delete[] this->ptr;
    this->ptr = extended;
    this->size++;
}

void EmployeeArray::update_employee(int id_to_update,int new_sal,int new_dep){
    int index = this->id_index(id_to_update);
    if (index != -1) {
        this->ptr[index].set_salary(new_sal);
        this->ptr[index].set_department(new_dep);
    } else {
        //cerr << "ERROR: An invalid ID to update" << endl;
    }
}

void EmployeeArray::delete_employee(int id_to_delete){

    int index = this->id_index(id_to_delete);

    if (index == -1){
        //cerr << "ERROR: An invalid ID to delete" << endl;
        return;
    }

    int arrSize = this->size;
    Employee* reduced = new Employee[arrSize - 1];
    
    int i = 0;
    for (; i < index; i++){
        reduced[i].set_id(this->ptr[i].get_id());
        reduced[i].set_salary(this->ptr[i].get_salary());
        reduced[i].set_department(this->ptr[i].get_department());
    }
    for (; i < arrSize - 1;i++){
        reduced[i].set_id(this->ptr[i+1].get_id());
        reduced[i].set_salary(this->ptr[i+1].get_salary());
        reduced[i].set_department(this->ptr[i+1].get_department());
    }
    
    delete[] this->ptr;
    this->ptr = reduced;
    this->size--;
}

EmployeeNode::EmployeeNode(int id, int salary, int department){
    this->data = new Employee(id,salary,department);
    this->next = nullptr;
}

EmployeeNode::~EmployeeNode(){
    delete this->data;
}

Employee* EmployeeNode::get_data(){
    return this->data;
}

EmployeeNode* EmployeeNode::get_next(){
    return this->next;
}

void EmployeeNode::set_next(EmployeeNode* next){
    this->next = next;
}

EmployeeLinkedList::EmployeeLinkedList(){
    this->head = nullptr;
    this->tail = nullptr;
}

EmployeeLinkedList::~EmployeeLinkedList(){
    while (head != nullptr){
        EmployeeNode* to_be_deleted = head;
        head = head->get_next();
        delete to_be_deleted;
    }
}

void EmployeeLinkedList::add_employee(int id, int salary, int department){
    EmployeeNode* new_node = new EmployeeNode(id,salary,department);
    if(head == nullptr){
        head = new_node;
        tail = new_node;
    } else {
        tail->set_next(new_node);
        tail = new_node;
    }
}

void EmployeeLinkedList::update_employee(int id, int salary, int department){
    EmployeeNode* ptr = this->head;
    while (ptr != nullptr){
        if(ptr->get_data()->get_id() == id){
            ptr->get_data()->set_salary(salary);
            ptr->get_data()->set_department(department);
            return;
        }
        ptr = ptr->get_next();
    }
    //cerr << "ERROR: An invalid ID to update" << endl;
}

void EmployeeLinkedList::delete_employee(int id){

    if (head == nullptr){
        return;
    }

    EmployeeNode* to_delete = nullptr;
    if (head->get_data()->get_id() == id){
        to_delete = head;
        head = head->get_next();
    } else { 
        EmployeeNode* ptr = head;
        while (ptr->get_next() != nullptr){
            if (ptr->get_next()->get_data()->get_id() == id){
                to_delete = ptr->get_next();
                if (ptr->get_next() == tail){
                    tail = ptr;
                }
                ptr->set_next(ptr->get_next()->get_next());
                break;
            }
            ptr = ptr->get_next();
        }
    }

    if (to_delete == nullptr){
        //cerr << "ERROR: An invalid ID to delete" << endl;
    } else {
        delete to_delete;  
    }
}

TreeNode::TreeNode(Employee* employee){
    this->employee = employee;
    this->left = nullptr;
    this->right = nullptr;
}

TreeNode::~TreeNode(){
    delete this->employee;
}

Employee* TreeNode::get_employee(){
    return this->employee;
}

TreeNode* TreeNode::get_left(){
    return this->left;
}

TreeNode* TreeNode::get_right(){
    return this->right;
}

TreeNode** TreeNode::get_left_address(){
    return &(this->left);
}

TreeNode** TreeNode::get_right_address(){
    return &(this->right);
}

void TreeNode::set_employee(Employee* employee){
    this->employee = employee;
}

void TreeNode::set_left(TreeNode* left){
    this->left = left;
}

void TreeNode::set_right(TreeNode* right){
    this->right = right;
}

BinaryTree::BinaryTree(){
    this->root = nullptr;
}

BinaryTree::~BinaryTree(){
    if (this->root == nullptr){
        return;
    }

    stack <TreeNode*> stack;
    stack.push(this->root);
    
    while (!stack.empty()){
        TreeNode* temp = stack.top();
        stack.pop();
        if (temp->get_right() != nullptr){
            stack.push(temp->get_right());
        }
        if (temp->get_left() != nullptr){
            stack.push(temp->get_left());
        }
        delete temp;
        temp = nullptr;
    }
}

void BinaryTree::add_employee(int id, int salary, int department){
    if (this->root == nullptr){
        TreeNode* new_employee = new TreeNode(new Employee(id,salary,department));
        this->root = new_employee;
        return;
    } 
    TreeNode* temp = this->root;
    while (temp != nullptr){
        if (id > temp->get_employee()->get_id()){
            if (temp->get_right() == nullptr){
                TreeNode* new_employee = new TreeNode(new Employee(id,salary,department));
                temp->set_right(new_employee);
                return;
            }
            temp = temp->get_right();
        } else if (id < temp->get_employee()->get_id()){
            if (temp->get_left() == nullptr){
                TreeNode* new_employee = new TreeNode(new Employee(id,salary,department));
                temp->set_left(new_employee);
                return;
            }
            temp = temp->get_left();
        } else {
            //cerr << "ERROR: ID already exists" << endl;
            return;
        }
    }
}

void BinaryTree::update_employee(int id, int salary, int department){
    Employee* to_be_updated = this->search(id);
    if (to_be_updated == nullptr){
        //cerr << "ERROR: An invalid ID to update" << endl;
        return;
    }
    to_be_updated->set_salary(salary);
    to_be_updated->set_department(department);
}

TreeNode** BinaryTree::search_inner(int id){ 
    TreeNode** temp = &(this->root);

    while (*temp != nullptr){
        if (id > (*temp)->get_employee()->get_id()){
            temp = (*temp)->get_right_address();
            if ((*temp != nullptr) && (*temp)->get_employee()->get_id() == id){
                return temp;
            }
        } else if (id < (*temp)->get_employee()->get_id()){
            temp = (*temp)->get_left_address();
            if ((*temp != nullptr) && (*temp)->get_employee()->get_id() == id){
                return temp;
            }
        } else {
            return temp; 
        }
    }
    return nullptr; 
}

Employee* BinaryTree::search(int id){
    TreeNode** ptr = this->search_inner(id);

    if (ptr == nullptr){
        return nullptr;
    } else {
        return (*ptr)->get_employee();
    }
}

void BinaryTree::delete_employee(int id){
    TreeNode** ptr = this->search_inner(id);

    if (ptr == nullptr){
        //cerr << "ERROR: An invalid ID to delete" << endl;
        return;
    } 

    TreeNode* to_delete = *ptr;
    if (to_delete->get_left() == nullptr){
        *ptr = to_delete->get_right();
        delete to_delete;
    } else if (to_delete->get_right() == nullptr){
        *ptr = to_delete->get_left();
        delete to_delete;
    } else {

        TreeNode** min = to_delete->get_right_address();
        while((*min)->get_left() != nullptr){
            min = (*min)->get_left_address();
        }
        TreeNode* to_copy = *min;
        Employee* employee_copy = to_copy->get_employee();
        to_copy->set_employee(to_delete->get_employee());
        to_delete->set_employee(employee_copy);
        *min = to_copy->get_right();
        delete to_copy;
    }  
}

DoublySkipList_Node::DoublySkipList_Node(Employee* data){
    this->data = data;
    this->next = nullptr;
    this->below = nullptr;
}

DoublySkipList_Node::~DoublySkipList_Node(){
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

DoublySkipList::DoublySkipList(int height){
    this->height = height;
    
    this->head = new DoublySkipList_Node(nullptr);
    this->head->set_next(new DoublySkipList_Node(nullptr));

    DoublySkipList_Node* bottom_head = this->head;
    for (int i = 1; i < height; i++){
        bottom_head->set_below(new DoublySkipList_Node(nullptr));
        bottom_head->get_below()->set_next(new DoublySkipList_Node(nullptr));
        bottom_head->get_next()->set_below(bottom_head->get_below()->get_next());
        bottom_head = bottom_head->get_below();
    }
}

DoublySkipList::~DoublySkipList(){

    DoublySkipList_Node* head_temp = this->head; 
    while (head_temp != nullptr){
        DoublySkipList_Node* head_to_delete = head_temp; 
        head_temp = head_temp->get_below();  
        DoublySkipList_Node* node_temp = head_to_delete;
        while (node_temp != nullptr){
            DoublySkipList_Node* node_to_delete = node_temp;
            node_temp = node_temp->get_next(); 
            delete node_to_delete; 
        }
    }  
}

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

void DoublySkipList::add_employee(int id, int salary, int department){

    int employee_height = 1; 
    while (employee_height < this->height && rand() % 2 == 1) {
        employee_height++;
    }
    int current_height = this->height;
    DoublySkipList_Node* search_ptr = this->head;

    while (current_height > employee_height){ 
        while (search_ptr->get_next()->get_next() != nullptr && 
               search_ptr->get_next()->get_data()->get_id() < id){
            search_ptr = search_ptr->get_next();
        }
        search_ptr = search_ptr->get_below();
        current_height -= 1;
    }

    DoublySkipList_Node* recently_inserted = nullptr;
    while (current_height > 0){
        while (search_ptr->get_next()->get_next() != nullptr && 
               search_ptr->get_next()->get_data()->get_id() < id){
            search_ptr = search_ptr->get_next();
        }

        DoublySkipList_Node* old_next = search_ptr->get_next(); 
        search_ptr->set_next(new DoublySkipList_Node(new Employee(id,salary,department)));
        search_ptr->get_next()->set_next(old_next);

        if (recently_inserted != nullptr){
            recently_inserted->set_below(search_ptr->get_next());
        }
        recently_inserted = search_ptr->get_next();
        search_ptr = search_ptr->get_below();
        current_height -= 1;
    }
}

void DoublySkipList::delete_employee(int id){

    DoublySkipList_Node* prev = this->search_from(this->head,id);
    if (prev != nullptr){
        while (prev != nullptr){
            DoublySkipList_Node* to_delete = prev->get_next();
            prev->set_next(to_delete->get_next());
            delete to_delete;
            prev = this->search_from(prev->get_below(),id); 
        }
    } else {
        //cerr << "ERROR: An invalid ID to delete" << endl;
    }
}

void DoublySkipList::update_employee(int id, int salary, int department){
    Employee* to_update = this->search(id);
    if (to_update != nullptr){
        to_update->set_salary(salary);
        to_update->set_department(department);
    } else {
        //cerr << "ERROR: An invalid ID to update" << endl;
    }
}

STLVector::~STLVector(){
    for (Employee* employee : this->vec) {
        delete employee;
    }
    this->vec.clear();
}

void STLVector::add_employee(int id, int salary, int department){
    this->vec.push_back(new Employee(id,salary,department));
}

void STLVector::update_employee(int id, int salary, int department){
    for (Employee* employee: this->vec){
        if (employee->get_id() == id){
            employee->set_salary(salary);
            employee->set_department(department);
            return;
        }
    }
    //cerr << "ERROR: An invalid ID to update" << endl;
}

void STLVector::delete_employee(int id){
    for (size_t index = 0; index < this->vec.size(); index++){
        if (this->vec[index]->get_id() == id){
            delete this->vec[index];
            this->vec.erase(vec.begin() + index);
            return;
        }
    }
    //cerr << "ERROR: An invalid ID to delete" << endl;
}

STLLinkedList::~STLLinkedList(){
    for (Employee* employee : this->employeeList) {
        delete employee;
    }
    this->employeeList.clear();
}

void STLLinkedList::add_employee(int id, int salary, int department){
    this->employeeList.push_back(new Employee(id,salary,department));
}

void STLLinkedList::update_employee(int id, int salary, int department){
    for (Employee* employee: this->employeeList){
        if (employee->get_id() == id){
            employee->set_salary(salary);
            employee->set_department(department);
            return;
        }
    }
    //cerr << "ERROR: An invalid ID to update" << endl;
}

void STLLinkedList::delete_employee(int id){
    for (auto it = this->employeeList.begin(); it != this->employeeList.end();) {
        if ((*it)->get_id() == id) {
            delete *it;
            it = this->employeeList.erase(it); 
            return;
        } else {
            ++it; 
        }
    }
    //cerr << "ERROR: An invalid ID to delete" << endl;
}

STLMap::~STLMap(){
    for (auto pair: this->employeeMap) {
        delete pair.second;
    }
    this->employeeMap.clear();
}

void STLMap::add_employee(int id, int salary, int department){
    this->employeeMap.insert(make_pair(id,new Employee(id,salary,department)));
}

void STLMap::update_employee(int id, int salary, int department){
    auto it = this->employeeMap.find(id);
    if (it != this->employeeMap.end()) {
        (*it).second->set_salary(salary);
        (*it).second->set_department(department);
    } else {
        //cerr << "ERROR: An invalid ID to update" << endl;
    }
}

void STLMap::delete_employee(int id){
    auto it = this->employeeMap.find(id);
    if (it != this->employeeMap.end()) {
        delete (*it).second;
        this->employeeMap.erase(it);
    } else {
        //cerr << "ERROR: An invalid ID to delete" << endl;
    }
}

FileIO::FileIO(string folderPath, string filename){
    this->folderPath = folderPath;
    this->filePath = folderPath + "/" + filename;

    ofstream file(this->filePath);
    if (file.is_open()) {
        file << "Employee_ID;Salary;Department\n";
        file.close();
    } else {
        throw runtime_error("Error opening the file: " + this->filePath);
    }
}

void FileIO::add_employee(int id, int salary, int department){
    ofstream file;
    file.open(this->filePath,ios::app);
    file << id << ';' << salary << ';' << department << '\n';
    file.close();
}

int FileIO::find_line_index(int id){
    ifstream file(this->filePath);
    string line;
    string line_id;
    getline(file,line);
    int line_index = 1;
    while (getline(file, line)){
        line_index++;
        istringstream line_stream(line);
        getline(line_stream,line_id,';');
        if (stoi(line_id) == id){
            file.close();
            return line_index;
        }
    }
    file.close();
    return -1;
}

void FileIO::update_employee(int id,int salary,int department){
    int line_index = this->find_line_index(id);
    if (line_index == -1){
        //cerr << "ERROR: An invalid ID to update" << endl;
        return;
    }
    
}

void FileIO::delete_employee(int id){

    int line_index = this->find_line_index(id);
    if (line_index == -1){
        //cerr << "ERROR: An invalid ID to delete" << endl;
        return;
    }
    string new_file_path = folderPath + "/new.csv";
    ifstream file(this->filePath);
    ofstream new_file(new_file_path);
    string line;
    for (int i = 1; i < line_index; i++){
        getline(file,line);
        new_file << line << '\n';
    }
    getline(file,line);
    while (getline(file,line)){
        new_file << line << '\n';
    }
    file.close();
    new_file.close();

    remove(this->filePath.c_str());
    rename(new_file_path.c_str(),this->filePath.c_str());
}