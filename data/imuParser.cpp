#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "imuRecord.hpp"

using namespace std;

void imuReader(vector<ImuRecord> *imuD, vector<int> *accDigits, vector<int> *gyroDigits){
    int digitsAfterFloatingPoins = 0;
    int digitsAfterFloatingPoins_acc = 0;
    ifstream file;
    file.open("./data/imu.csv");
    string line, word;
    getline(file, line);
    while (getline(file, line)) {
        ImuRecord imu;
        imu.Initialize();
        stringstream ss(line);
        int index=0;
        while (getline(ss, word, ',')) {
            switch (index)
            {
            case 0 :
                imu.timestamp = stold(word);
                break;
            case 1 :
                digitsAfterFloatingPoins = word.substr(word.find(".")).length();
                gyroDigits->push_back(digitsAfterFloatingPoins);
                imu.gyro_x = stold(word);
                break;
            case 2 :
                imu.gyro_y = stold(word);
                break;
            case 3 :
                imu.gyro_z = stold(word);
                break;
            case 4 :
                digitsAfterFloatingPoins_acc = word.substr(word.find(".")).length();
                accDigits->push_back(digitsAfterFloatingPoins_acc);
                imu.acc_x = stold(word);
                break;
            case 5 :
                imu.acc_y = stold(word);
                break;
            case 6 :
                imu.acc_z = stold(word);
                break;
            case 7 :
                imu.conf_gyro = stold(word);
                break;
            case 8 :
                imu.conf_acc = stold(word);
                break;
            default:
                break;
            }
            index++;
        }
        imu.count = index;
        imuD->push_back(imu);
    }
    file.close();
}