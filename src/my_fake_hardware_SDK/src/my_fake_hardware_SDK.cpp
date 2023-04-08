# include "my_fake_hardware_SDK/my_fake_hardware_SDK.hpp"

bool MyFakeHardware::init(std::string usb_port, int baud_rate, int n_joints, const char ** Log)
{
    if (!(usb_port == usb_port_)){
        std::string s = "Fail to connect with port";
        (*Log) = s.c_str();
        return false;
    }
    if (!(baud_rate == baud_rate_)){
        std::string s = "Baud rate is not corret.";
        (*Log) = s.c_str();
        return false;
    }
    if (n_joints < 1){
        std::string s = "Number of Joints must be more than 0.";
        (*Log) = s.c_str();
        return false;
    }
    n_joints_ = n_joints;
    position_value_.resize(n_joints_, 0.0);
    velocity_value_.resize(n_joints_, 0.0);
    effort_value_.resize(n_joints_, 0.0);
    return true;
};

bool MyFakeHardware::ping(int component_id, const char ** Log)
{
    if ((component_id < 0) || (component_id >= n_joints_)){
        std::string s = "Pin fail with invalid ID.";
        (*Log) = s.c_str();
        return false;
    }
    return true;
};

bool MyFakeHardware::write_position_data(int component_id, double data, const char ** Log)
{
    if ((component_id < 0) || (component_id >= n_joints_)){
        std::string s = "Writing position data fail with invalid ID.";
        (*Log) = s.c_str();
        return false;
    }
    position_value_[component_id] = data * 0.5; // To test whether the function work properly, we set writing is alway half of original value
                                                // Therefore, we can check the value from ros2, if it shows half of out input, then the function works properly.
                                                // Update: after post checking, it work perfectly!!!
    return true;
};

bool MyFakeHardware::write_velocity_data(int component_id, double data, const char ** Log)
{
    if ((component_id < 0) || (component_id >= n_joints_)){
        std::string s = "Writing velocity data fail with invalid ID.";
        (*Log) = s.c_str();
        return false;
    }
    velocity_value_[component_id] = data * 0.5;
    return true;
};

bool MyFakeHardware::write_effort_data(int component_id, double data, const char ** Log)
{
    if ((component_id < 0) || (component_id >= n_joints_)){
        std::string s = "Writing velocity data fail with invalid ID.";
        (*Log) = s.c_str();
        return false;
    }
    effort_value_[component_id] = data * 0.5;
    return true;
};

double MyFakeHardware::read_position_data(int component_id)
{
    return position_value_[component_id];
};

double MyFakeHardware::read_velocity_data(int component_id)
{
    return velocity_value_[component_id];
};

double MyFakeHardware::read_effort_data(int component_id)
{
    return effort_value_[component_id];
};
