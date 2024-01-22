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
#include <time.h>

using namespace std;

bool is_empty_line(string& line){ 
    return (line[0] == '\r' || line[0] == '\n');  
}

int find_line_num(string& file_path){
    int line_num = 0;
    ifstream file;
    string line;
    file.open(file_path,ios::in);
    while (getline(file,line)){
        if (!is_empty_line(line)){
            line_num++;
        }
    }
    return line_num;
}

void add_employee(string& csv_file_path, int salary, int department){

    string line; 
    string last_line;
    string id;
    clock_t start = clock();
    ifstream icsv_file;
    icsv_file.open(csv_file_path,ios::in);
    while (getline(icsv_file,line)){
        last_line = line;
    }
    icsv_file.close();

    istringstream line_stream(last_line);
    getline(line_stream,id,';');

    ofstream ocsv_file;
    ocsv_file.open(csv_file_path,ios::app);
    ocsv_file << stoi(id) + 1 << ';' << salary << ';' << department << '\n';
    ocsv_file.close(); 

    clock_t end = clock();
    cout << "ADD:  " <<(double)(end - start) * 1000 / CLOCKS_PER_SEC << endl;
}

streampos employee_position(string& csv_file_path, int id){

    ifstream csv_file(csv_file_path);
    string line;
    string line_id;

    getline(csv_file,line);
    streampos position = csv_file.tellg();
    while (getline(csv_file,line)){
        istringstream line_stream(line);
        getline(line_stream, line_id,';');
        if (stoi(line_id) == id){
            csv_file.close();
            return position;
        }
        position = csv_file.tellg();
    }
    csv_file.close();
    return streampos(-1);
}

void update_employee(string& csv_file_path,int id,int salary,int department){
    clock_t start = clock();
    streampos position = employee_position(csv_file_path,id);
    if (position != streampos(-1)){
        fstream csv_file(csv_file_path, ios::in | ios::out);
        csv_file.seekp(position);
        csv_file << id << ';' << salary << ';' << department;
        csv_file.close();
    } else {
        cout << "ERROR: Invalid ID to update" << endl;
    }

    clock_t end = clock();
    cout << "UPDATE:  " <<(double)(end - start) * 1000 / CLOCKS_PER_SEC << endl;
}

int find_line_index(string& csv_file_path, int id){
    ifstream csv_file(csv_file_path);
    string line;
    string line_id;
    getline(csv_file,line);
    int line_index = 1;
    while (getline(csv_file, line)){
        line_index++;
        istringstream line_stream(line);
        getline(line_stream,line_id,';');
        if (stoi(line_id) == id){
            csv_file.close();
            return line_index;
        }
    }
    csv_file.close();
    return -1;
}

void delete_employee(string& csv_file_path,int id){

    int line_num = find_line_num(csv_file_path);
    if (line_num <= 1) {
        cout << "ERROR: There is no Employee" << endl;
        return;
    }

    clock_t start = clock();
    int line_index = find_line_index(csv_file_path, id);
    
    if (line_index != 1) {
        ifstream csv_file(csv_file_path);
        ofstream new_file("new.csv");
        string line;
        for (int i = 1; i < line_index; i++){
            getline(csv_file,line);
            new_file << line << '\n';
        }
        getline(csv_file,line);
        while (getline(csv_file,line)){
            new_file << line << '\n';
        }
        csv_file.close();
        new_file.close();

        remove(csv_file_path.c_str());
        rename("new.csv",csv_file_path.c_str());
    } 
    clock_t end = clock();
    cout << "DELETE:  " <<(double)(end - start) * 1000 / CLOCKS_PER_SEC << endl;
}


int main(int argc, char** argv) {

    string csv_file_path(argv[1]);
    string text_file_path(argv[2]);

    string operation;
    string line;

    string employee_id;
    string employee_salary;
    string employee_department;

    ifstream text_file;
    text_file.open(text_file_path,ios::in);
    while (getline(text_file,line)){
        if (!is_empty_line(line)){
            istringstream line_stream(line);
            getline(line_stream,operation,';');

            if (operation == "ADD"){
                getline(line_stream, employee_salary,';');
                getline(line_stream, employee_department,'\n');
                add_employee(csv_file_path,stoi(employee_salary),stoi(employee_department));
            } else if (operation == "UPDATE") {
                getline(line_stream, employee_id,';');
                getline(line_stream, employee_salary,';');
                getline(line_stream, employee_department,'\n');
                update_employee(csv_file_path,stoi(employee_id),stoi(employee_salary),stoi(employee_department));
            } else if (operation == "DELETE") {
                getline(line_stream, employee_id,'\n');
                delete_employee(csv_file_path,stoi(employee_id));
            } else {
                cerr << "ERROR: Invalid operation" << endl;
            }
        }
    }
    text_file.close();

    return EXIT_SUCCESS;
} 