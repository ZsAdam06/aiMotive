#include "imuRecord.hpp"

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