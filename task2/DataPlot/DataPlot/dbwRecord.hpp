#ifndef DBWRECORD_H
#define DBWRECORD_H
 

class DbwRecord{
    public:
        long double timestamp; 
        long double speed;
        long double yaw_rate;
        long double v_front_left;
        long double v_front_right;
        long double v_rear_left;
        long double v_rear_right;
        void Initialize();
};

#endif