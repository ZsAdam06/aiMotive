#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "imuRecord.hpp"

using namespace std;

void imuReader(vector<ImuRecord> *imuD){
    ifstream file;
    file.open("imu.csv");
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
                imu.gyro_x = stold(word);
                break;
            case 2 :
                imu.gyro_y = stold(word);
                break;
            case 3 :
                imu.gyro_z = stold(word);
                break;
            case 4 :
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

int main () {

    vector<ImuRecord> *imuData = new vector<ImuRecord>();
    
    imuReader(imuData);
    
    //imu-data-check
    for(int i=0; i<imuData->size(); i++){
        printf("%Lf,%Lf,%Lf,%Lf,%Lf,%Lf,%Lf,%Lf,%.15Lf\n", imuData->at(i).timestamp, imuData->at(i).gyro_x, imuData->at(i).gyro_y, imuData->at(i).gyro_z, imuData->at(i).acc_x, imuData->at(i).acc_y, imuData->at(i).acc_z, imuData->at(i).conf_gyro, imuData->at(i).conf_acc); 
    }
    
    return 0;
}