/* @Author
StudentName: Eren Onaran
StudentID: 150210114
Date: 11.01.2024
*/

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <random>

using namespace std;

int main(int argc, char** argv) {

    random_device rd;
    mt19937 g(rd());
    uniform_int_distribution<int> salary_dis(1000,10000);
    uniform_int_distribution<int> department_dis(0,10);   
    string dataset_dir = "dataset/";

    for (int Employee_num = 10000; Employee_num <= 10000; Employee_num += 10000){
        string filename = dataset_dir + to_string(Employee_num / 1000) + "k_dataset.csv";

        ofstream dataset_file(filename);
        if (!dataset_file.is_open()) {
            cerr << "Error opening file: " << filename << std::endl;
            return EXIT_SUCCESS;
        }
        dataset_file << "Employee_ID;Salary;Department\n";

        vector<int> shuffledIDs(Employee_num);
        for (int i = 0; i < Employee_num; i++){
            shuffledIDs[i] = i + 1;
        }
        shuffle(shuffledIDs.begin(),shuffledIDs.end(),g);

        for (int i = 0; i < Employee_num; i++){
            int id = shuffledIDs[i];
            int salary = salary_dis(g);
            int department = department_dis(g);

            dataset_file << id << ";" << salary << ";" << department << "\n";
        }
        dataset_file.close();
        cout << filename << " generated successfully." << endl;
    }

    

    return EXIT_SUCCESS;
} 
