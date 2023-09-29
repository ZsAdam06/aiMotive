#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <math.h>
#include "../data/imuRecord.hpp"
#include "../data/dbwRecord.hpp"

using namespace std;

void imuReader(vector<ImuRecord>*, vector<int>*, vector<int>*);
void dbwReader(vector<DbwRecord>*);

int incompleteDataTracker(vector<DbwRecord> *dbwData){
    int missingTimestamps = 0;
    for(int i=0; i<dbwData->size(); i++){
        if (dbwData->at(i).timestamp == 0)
        {
            missingTimestamps++;
        }
    }
    return missingTimestamps;
}

bool sameTime(vector<ImuRecord> *imuData, vector<DbwRecord> *dbwData){
    bool same = true;
    int index = 0;
    int length = (imuData->size() < dbwData->size()) ? imuData->size() : dbwData->size();
    while(same && index<length){
        if(roundf(imuData->at(index).gyro_z) == roundf(dbwData->at(index).yaw_rate)){
            index++;
        }else{
            same = false;
        }
    }
    return same;
}

void qualityCheck(vector<int>* accDigits, vector<int>* gyroDigits, int* accDMax, int* accDMin, int* gyroDMax, int* gyroDMin){
    int n=0;
    int length = (gyroDigits->size() < accDigits->size()) ? gyroDigits->size() : accDigits->size();
    for(int i=0; i<length; i++){
        if(i==0){
            *accDMax = accDigits->at(i);
            *accDMin = accDigits->at(i);
            *gyroDMax = gyroDigits->at(i);
            *gyroDMin = gyroDigits->at(i);
        }
        if(accDigits->at(i)>*accDMax){
            *accDMax = accDigits->at(i);
        }else if(accDigits->at(i)<*accDMin){
            *accDMin = accDigits->at(i);            
        }
        if(gyroDigits->at(i)>*gyroDMax){
            *gyroDMax = gyroDigits->at(i);
        }else if(gyroDigits->at(i)<*gyroDMin){
            *gyroDMin = gyroDigits->at(i);
        }
        //cout<<"G: "<< gyroDigits->at(i) << " A: "<<accDigits->at(i)<<"\n";
    }
}

int main () {

    vector<ImuRecord> *imuData = new vector<ImuRecord>();
    vector<DbwRecord> *dbwData = new vector<DbwRecord>();
    
    vector<int> *accDigits = new vector<int>();
    vector<int> *gyroDigits = new vector<int>();

    int* accDMax = new int();
    int* accDMin = new int();
    int* gyroDMax = new int();
    int* gyroDMin = new int();

    imuReader(imuData, accDigits, gyroDigits);
    dbwReader(dbwData);

    //imu-data-check
    
    /*
    for(int i=0; i<imuData->size(); i++){
        printf("%Lf,%Lf,%Lf,%Lf,%Lf,%Lf,%Lf,%Lf,%.15Lf\n", imuData->at(i).timestamp, imuData->at(i).gyro_x, imuData->at(i).gyro_y, imuData->at(i).gyro_z, imuData->at(i).acc_x, imuData->at(i).acc_y, imuData->at(i).acc_z, imuData->at(i).conf_gyro, imuData->at(i).conf_acc); 
    }
    */
    
    //dbw-data-check
    /*
    for(int i=0; i<dbwData->size(); i++){
        printf("%Lf,%Lf,%Lf,%Lf,%Lf,%Lf,%Lf\n", dbwData->at(i).timestamp, dbwData->at(i).speed, dbwData->at(i).yaw_rate, dbwData->at(i).v_front_left, dbwData->at(i).v_front_right, dbwData->at(i).v_rear_left, dbwData->at(i).v_rear_right); 
    }
    */

    //a
    cout<<"Task 1:\na,\n";
    cout<<"We have "<<incompleteDataTracker(dbwData)<<" incomplete(missing timestamp) record in our database!\n";

    //b
    cout<<"b,\n";
    if(sameTime(imuData, dbwData)){
        cout<<"The two databases likely contain data measured at the same time.\n";
    }else{
        cout<<"The two databases do not contain data measured at the same time.\n";
    }

    //c
    cout<<"c,\n";
    qualityCheck(accDigits, gyroDigits, accDMax, accDMin, gyroDMax, gyroDMin);
    cout<<"They are not always of the same quality.\n";
    cout<<"Maximum digits after floating point for acc signals: "<<*accDMax<<"\n";
    cout<<"Minimum digits after floating point for acc signals: "<<*accDMin<<"\n";
    cout<<"Maximum digits after floating point for gyro signals: "<<*gyroDMax<<"\n";
    cout<<"Minimum digits after floating point for gyro signals: "<<*gyroDMin<<"\n";
    return 0;
}