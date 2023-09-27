#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "../data/imuRecord.hpp"
#include "../data/dbwRecord.hpp"

using namespace std;

void imuReader(vector<ImuRecord>*);
void dbwReader(vector<DbwRecord>*);

int main () {

    vector<ImuRecord> *imuData = new vector<ImuRecord>();
    vector<DbwRecord> *dbwData = new vector<DbwRecord>();
    
    imuReader(imuData);
    dbwReader(dbwData);

    //imu-data-check
    for(int i=0; i<imuData->size(); i++){
        printf("%Lf,%Lf,%Lf,%Lf,%Lf,%Lf,%Lf,%Lf,%.15Lf\n", imuData->at(i).timestamp, imuData->at(i).gyro_x, imuData->at(i).gyro_y, imuData->at(i).gyro_z, imuData->at(i).acc_x, imuData->at(i).acc_y, imuData->at(i).acc_z, imuData->at(i).conf_gyro, imuData->at(i).conf_acc); 
    }
    
    //dbw-data-check
    for(int i=0; i<dbwData->size(); i++){
        printf("%Lf,%Lf,%Lf,%Lf,%Lf,%Lf,%Lf\n", dbwData->at(i).timestamp, dbwData->at(i).speed, dbwData->at(i).yaw_rate, dbwData->at(i).v_front_left, dbwData->at(i).v_front_right, dbwData->at(i).v_rear_left, dbwData->at(i).v_rear_right); 
    }
    return 0;
}