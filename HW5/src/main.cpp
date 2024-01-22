/* @Author
StudentName: Eren Onaran
StudentID: 150210114
Date: 11.01.2024
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include "../include/classes.hpp"
#include <vector>
#include <time.h>
#include <random>


using namespace std;

bool is_empty_line(string&);
int operation_rep = 0;
random_device rd;  
mt19937 gen(rd()); 
vector<double> insert_vec;
vector<double> remove_vec;
vector<double> search_vec;

template <typename T>
void insert(ifstream& file, T& collection){
    clock_t start = clock();
    string line;
    getline(file,line);
    while(getline(file,line)){
        if(!is_empty_line(line)){
            istringstream line_stream(line);
            string id_string, salary_string, department_string;
            getline(line_stream,id_string,';');
            getline(line_stream,salary_string,';');
            getline(line_stream,department_string,'\n');
            int id = stoi(id_string), salary = stoi(salary_string), department = stoi(department_string);
            collection.add_employee(id,salary,department);
        }
    }
    clock_t end = clock();
    double time = (double)(end - start) * 1000 / CLOCKS_PER_SEC;
    //cout << "INSERT:  " << time << endl;
    insert_vec.push_back(time);
}

template <typename T>
void search(T& collection){

    uniform_int_distribution<> distribution(1, operation_rep);

    clock_t start = clock();
    for (int i = 0; i < operation_rep; i++){
        collection.update_employee(distribution(gen),0,0);
    }
    clock_t end = clock();
    double time = (double)(end - start) * 1000 / CLOCKS_PER_SEC;
    //cout << "SEARCH:  " << time << endl;
    search_vec.push_back(time);
}

template <typename T>
void remove(T& collection){

    uniform_int_distribution<> distribution(1, operation_rep);

    clock_t start = clock();
    for (int i = 0; i < operation_rep; i++){
        collection.delete_employee(distribution(gen));
    }
    clock_t end = clock();
    double time = (double)(end - start) * 1000 / CLOCKS_PER_SEC;
    //cout << "REMOVE:  " << time << endl;
    remove_vec.push_back(time);
}

bool is_empty_line(string& line){
    return (line[0] == '\r' || line[0] == '\n');
}
 
int main(int argc, char** argv) {

    string folderPath = "dataset";
    for (int size = 1; size <= 5; size += 1){

        operation_rep = size * 1000;
        string filePath = folderPath + "/" + to_string(size) + "k_dataset.csv";

        ifstream file(filePath);
        if (file.is_open()){
            cout << filePath << " opened successfully." << endl;
        } else {
            cerr << "Unable to open " << filePath << endl;
            return EXIT_FAILURE; 
        }

        /*try{
            FileIO EmployeeFile("dataset_copy", to_string(size) + "k_dataset_copy.csv");
            insert(file,EmployeeFile);
            file.close();
            search(EmployeeFile);
            remove(EmployeeFile);
        } catch (const exception& e) {
            cerr << e.what() << endl;
            return EXIT_FAILURE;
        }*/
        
        EmployeeArray arr;
        insert(file,arr);
        file.close();
        search(arr);
        remove(arr);
    }

    cout << "INSERT:" << endl;
    for (double time: insert_vec){
        cout << time << endl;
    }
    cout << "SEARCH:" << endl;
    for (double time: search_vec){
        cout << time << endl;
    }
    cout << "REMOVE:" << endl;
    for (double time: remove_vec){
        cout << time << endl;
    }

    return EXIT_SUCCESS;
} 