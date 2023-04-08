#ifndef MY_FAKE_HARDWARE_SDK_HPP_
#define MY_FAKE_HARDWARE_SDK_HPP_

#include <vector>
#include <string>

class MyFakeHardware
{
public:
    bool init(std::string usb_port, int baud_rate, int n_joints, const char ** Log);
    bool ping(int component_id, const char ** Log);
    bool write_position_data(int component_id, double data, const char ** Log);
    bool write_velocity_data(int component_id, double data, const char ** Log);
    bool write_effort_data(int component_id, double data, const char ** Log);
    double read_position_data(int component_id);
    double read_velocity_data(int component_id);
    double read_effort_data(int component_id);

private:
    std::string usb_port_ = "usb_port";
    int baud_rate_ = 1234;
    int n_joints_;
    std::vector<double> position_value_;
    std::vector<double> velocity_value_;
    std::vector<double> effort_value_;
};

#endif /*MY_FAKE_HARDWARE_SDK_HPP_*/