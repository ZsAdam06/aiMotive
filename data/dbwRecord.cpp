#include "dbwRecord.hpp"

void DbwRecord::Initialize(){
    this->timestamp = 0;
    this->speed = 0;
    this->yaw_rate = 0;
    this->v_front_left = 0;
    this->v_front_right = 0;
    this->v_rear_left = 0;
    this->v_rear_right = 0;
}