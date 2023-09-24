#pragma once

#include <string>

class ImuRecord{
    public:
        float timestamp;
        float gyro_x;
        float gyro_y;
        float gyro_z;
        float acc_x;
        float acc_y;
        float acc_z;
        float conf_gyro;
        float conf_acc;
        void convertData(std::string ts, std::string gx, 
                        std::string gy, std::string gz,
                        std::string ax, std::string ay,
                        std::string az, std::string cg,
                        std::string ca){};
};