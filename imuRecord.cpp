#include "imuRecord.hpp"

void ImuRecord::Initialize(){
    this->timestamp = 0;
    this->gyro_x = 0;
    this->gyro_y = 0;
    this->gyro_z = 0;
    this->acc_x = 0;
    this->acc_y = 0;
    this->acc_z = 0;
    this->conf_gyro = 0;
    this->conf_acc = 0;
}

void ImuRecord::convertData(std::string ts, std::string gx, 
                        std::string gy, std::string gz,
                        std::string ax, std::string ay,
                        std::string az, std::string cg,
                        std::string ca){
    this->timestamp = std::stof(ts);
    this->gyro_x = std::stof(gx);
    this->gyro_y = std::stof(gy);
    this->gyro_z = std::stof(gz);
    this->acc_x = std::stof(ax);
    this->acc_y = std::stof(ay);
    this->acc_z = std::stof(az);
    this->conf_gyro = std::stof(cg);
    this->conf_acc = std::stof(ca);
}