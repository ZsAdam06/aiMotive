#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "dbwRecord.hpp"

using namespace std;

void dbwReader(vector<DbwRecord> *dbwD){
    ifstream file;
    file.open("./dbw.csv");
    string line, word;
    getline(file, line);
    while (getline(file, line)) {
        DbwRecord dbw;
        dbw.Initialize();
        stringstream ss(line);
        int index=0;
        while (getline(ss, word, ',')) {
            if (word != "")
            {
                switch (index)
                {
                case 0 :
                    dbw.timestamp = stold(word);
                    break;
                case 1 :
                    dbw.speed = stold(word);
                    break;
                case 2 :
                    dbw.yaw_rate = stold(word);
                    break;
                case 3 :
                    dbw.v_front_left = stold(word);
                    break;
                case 4 :
                    dbw.v_front_right = stold(word);
                    break;
                case 5 :
                    dbw.v_rear_left = stold(word);
                    break;
                case 6 :
                    dbw.v_rear_right = stold(word);
                    break;
                default:
                    break;
                }
            }
            index++;
        }
        dbwD->push_back(dbw);
    }
    file.close();
}