#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "imuRecord.hpp"

using namespace std;

int main () {

    vector<ImuRecord> imuData;
    
    
    ifstream file;
    file.open("imu.csv");
    string line, word;
    getline(file, line);
    while (getline(file, line)) {
        //vector<string> row;
        ImuRecord imu;
        imu.Initialize();
        stringstream ss(line);
        int index=0;
        while (getline(ss, word, ',')) {
            //row.push_back(word);
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
        imuData.push_back(imu);
    }
    file.close();
    
    for(int i=0; i<imuData.size(); i++){
        printf("%Lf,%Lf,%Lf,%Lf,%Lf,%Lf,%Lf,%Lf,%.15Lf\n", imuData[i].timestamp, imuData[i].gyro_x, imuData[i].gyro_y, imuData[i].gyro_z, imuData[i].acc_x, imuData[i].acc_y, imuData[i].acc_z, imuData[i].conf_gyro, imuData[i].conf_acc);
    }
    
    return 0;
}