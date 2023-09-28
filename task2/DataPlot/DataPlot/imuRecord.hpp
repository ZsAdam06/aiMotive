#ifndef IMURECORD_H
#define IMURECORD_H
 

#include <string>

class ImuRecord{
    public:
        long double timestamp; 
        long double gyro_x;
        long double gyro_y;
        long double gyro_z;
        long double acc_x;
        long double acc_y;
        long double acc_z;
        long double conf_gyro;
        long double conf_acc;
        int count = 0;
        void Initialize();
        void convertData(std::string ts, std::string gx, 
                        std::string gy, std::string gz,
                        std::string ax, std::string ay,
                        std::string az, std::string cg,
                        std::string ca);
};

#endif