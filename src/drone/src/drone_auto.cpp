#include <px4_msgs/msg/offboard_control_mode.hpp>
#include <px4_msgs/msg/trajectory_setpoint.hpp>
#include <px4_msgs/msg/vehicle_command.hpp>
#include <px4_msgs/msg/vehicle_control_mode.hpp>
#include <px4_msgs/msg/vehicle_attitude_setpoint.hpp>
#include <px4_msgs/msg/distance_sensor.hpp>
#include <px4_msgs/msg/vehicle_optical_flow.hpp>
#include <px4_msgs/msg/vehicle_optical_flow_vel.hpp>
#include <std_msgs/msg/string.hpp>

#include <px4_msgs/msg/vehicle_local_position.hpp>
#include <px4_msgs/msg/vehicle_global_position.hpp>
#include <px4_msgs/msg/vehicle_attitude.hpp>
#include <px4_msgs/msg/input_rc.hpp>
#include <px4_msgs/msg/vehicle_status.hpp>

#include <rclcpp/rclcpp.hpp>
#include <stdint.h>
#include <math.h>

#include <chrono>
#include <iostream>

using namespace std::chrono;
using namespace std::chrono_literals;
using namespace px4_msgs::msg;

#define MANUAL_MODE      0          
#define POSITION_MODE    2
#define LOITER_MODE      4
#define OFFBOARD_MODE    14
#define TAKEOFF_MODE     17
#define LAND_MODE        18
#define ARMED            101
#define READY            100

class OffboardControl : public rclcpp::Node
{
public:
    OffboardControl() : Node("offboard_control")
    {
        offboard_control_mode_publisher_ = this->create_publisher<OffboardControlMode>("/fmu/in/offboard_control_mode", 10);
        trajectory_setpoint_publisher_ = this->create_publisher<TrajectorySetpoint>("/fmu/in/trajectory_setpoint", 10);
        vehicle_command_publisher_ = this->create_publisher<VehicleCommand>("/fmu/in/vehicle_command", 10);
        vehicle_attitude_setpoint_publisher_= this->create_publisher<VehicleAttitudeSetpoint>("/fmu/in/vehicle_attitude_setpoint", 10);
        custom_msg_publisher_= this->create_publisher<std_msgs::msg::String>("input_topic", 10);

        rmw_qos_profile_t qos_profile = rmw_qos_profile_sensor_data;
        auto qos = rclcpp::QoS(rclcpp::QoSInitialization(qos_profile.history, 10), qos_profile);
        
        vehicle_local_position_subscription_ = this->create_subscription<px4_msgs::msg::VehicleLocalPosition>("/fmu/out/vehicle_local_position", qos,
        [this](const px4_msgs::msg::VehicleLocalPosition::UniquePtr msg) {
            x_current  = msg->x;
            vx_current = msg->vx;

            y_current  = msg->y;
            vy_current = msg->vy;

            z_current  = msg->z;
            vz_current = msg->vz;
        });     

        vehicle_attitude_subscription_ = this->create_subscription<px4_msgs::msg::VehicleAttitude>("/fmu/out/vehicle_attitude", qos,
        [this](const px4_msgs::msg::VehicleAttitude::UniquePtr msg) {
            double w = msg->q[0];
            double x = msg->q[1];
            double y = msg->q[2];
            double z = msg->q[3];

            Roll_current = atan2(2.0 * (w * x + y * z), 1.0 - 2.0 * (x * x + y * y)) ;
            Pitch_current = asin(std::clamp(2.0 * (w * y - z * x), -1.0, 1.0));
            Yaw_current = atan2(2.0 * (w * z + x * y), 1.0 - 2.0 * (y * y + z * z)) ;
        });

        input_rc_subscription_ = this->create_subscription<px4_msgs::msg::InputRc>("/fmu/out/input_rc", qos,
        [this](const px4_msgs::msg::InputRc::UniquePtr msg) {
            Rc_CH6 = msg->values[5];
        });

        // distance_sensor_subscription_ = this->create_subscription<px4_msgs::msg::DistanceSensor>("/fmu/out/distance_sensor", qos,
        // [this](const px4_msgs::msg::DistanceSensor::UniquePtr msg) {
        //     // z_current = -(msg->current_distance)*(cos(Roll_current) * cos(Pitch_current));
        // });

        vehicle_optical_flow_vel_subscription_ = this->create_subscription<px4_msgs::msg::VehicleOpticalFlowVel>("/fmu/out/vehicle_optical_flow_vel", qos,
        [this](const px4_msgs::msg::VehicleOpticalFlowVel::UniquePtr msg) {
            vx_optical = msg->vel_ne[0];
            vy_optical = msg->vel_ne[1];
        });

        vehicle_status_subscription_ = this->create_subscription<px4_msgs::msg::VehicleStatus>("/fmu/out/vehicle_status", qos,
        [this](const px4_msgs::msg::VehicleStatus::UniquePtr msg) {
            nav_state_ = msg->nav_state;
            arming_state = msg->arming_state == 2;
        });
        
        auto timer_callback = [this]() -> void {
            timer_count ++;
            offboard_setpoint_counter_ ++;
            if(offboard_setpoint_counter_ > 50)
            {
                RCLCPP_INFO(this->get_logger(), "x_current   = %.3f ", x_current);
                RCLCPP_INFO(this->get_logger(), "y_current   = %.3f ", y_current);
                RCLCPP_INFO(this->get_logger(), "z_current   = %.3f ", z_current);
                RCLCPP_INFO(this->get_logger(), "Yaw_desired   = %.3f ", Yaw_desired);
            }
            if(Rc_CH6 > 1500)
            {
                switch(STATE)
                {    
                    case READY:
                        if(nav_state_ != MANUAL_MODE)
                            this->manual();
                        else
                        {
                            STATE = MANUAL_MODE;
                            timer_count = 0;
                        }
                    break;
                    case MANUAL_MODE:
                        if(!arming_state)
                            this->arm();
                        else 
                        {
                            STATE = ARMED;
                            Yaw_desired = Yaw_current;
                            timer_count = 0;
                        }
                    break;
                    case ARMED:
                        if(nav_state_ != TAKEOFF_MODE)
                            this->takeoff();
                        else 
                        {
                            STATE = TAKEOFF_MODE;
                            timer_count = 0;
                        }
                    break;
                    case TAKEOFF_MODE:
                        publish_offboard_control_mode();
                        if((nav_state_ != OFFBOARD_MODE)&&(timer_count >= takeoff_timer))
                            this->offboard();
                        else if((nav_state_ == OFFBOARD_MODE)&&(timer_count <= timer_1))
                        {
                            x_body = 3.0;
                            y_body = 0.0;
                            position_setpoint(x_body, y_body, -2.0, Yaw_desired);
                            if(timer_count >= (timer_1 - 20))
                                servo("1");
                        }
                        else if((nav_state_ == OFFBOARD_MODE)&&(timer_count <= timer_2))
                        {
                            x_body = 5.0;
                            y_body = 0.0;
                            position_setpoint(x_body, y_body, -2.0, Yaw_desired);
                            if(timer_count >= (timer_2 - 20))
                                servo("2");
                        }
                        else if((nav_state_ == OFFBOARD_MODE)&&(timer_count <= timer_3))
                        {
                            x_body = 5.0;
                            y_body = 2.0;
                            position_setpoint(x_body, y_body, -2.0, Yaw_desired);
                            if(timer_count >= (timer_3 - 20))
                                servo("3");
                        }
                        else if((nav_state_ == OFFBOARD_MODE)&&(timer_count <= timer_4))
                        {
                            x_body = 5.0;
                            y_body = -2.0;
                            position_setpoint(x_body, y_body, -2.0, Yaw_desired);
                            if(timer_count >= (timer_4 - 20))
                                servo("4");
                        }
                        else if((nav_state_ == OFFBOARD_MODE)&&(timer_count <= timer_5))
                        {
                            x_body = 7.0;
                            y_body = 0.0;
                            position_setpoint(x_body, y_body, -2.0, Yaw_desired);
                            if(timer_count >= (timer_5 - 20))
                                servo("5");
                        }
                        else if((nav_state_ == OFFBOARD_MODE)&&(timer_count <= timer_return))
                        {
                            x_body = 0.0;
                            y_body = 0.0;
                            position_setpoint(x_body, y_body, -2.0, Yaw_desired);
                        }
                        else if((nav_state_ == OFFBOARD_MODE)&&(timer_count >= timer_return))
                        {
                            this->landing();
                            STATE = LAND_MODE;
                        }
                    break;
                }
            }
            else 
                publish_vehicle_command(VehicleCommand::VEHICLE_CMD_DO_SET_MODE, 1, 3);  
        };
        timer_ = this->create_wall_timer(10ms, timer_callback);
    }

    void arm();
	void disarm();
    float PID_inner(float DesiredValue, float CurrentValue, float Kp);
    float PID_outter(float DesiredValue, float CurrentValue, float Kp);
    std::array<float, 4> RPY_to_Quaternion(float Roll, float Pitch, float Yaw);
    float Controller_z(float DesiredValue);
    void Controller_xyz(float DesiredValueX, float DesiredValueY, float DesiredValueZ);
    void xyz_setpoint(float x_desired, float y_desired, float z_desired);
    void position_setpoint(float x, float y, float z, float Yaw_);
    void takeoff();
    void manual();
    void landing();
    void offboard();
    void servo(std::string data_input);

private:
    rclcpp::TimerBase::SharedPtr timer_;

    rclcpp::Publisher<OffboardControlMode>::SharedPtr offboard_control_mode_publisher_;
    rclcpp::Publisher<TrajectorySetpoint>::SharedPtr trajectory_setpoint_publisher_;
    rclcpp::Publisher<VehicleCommand>::SharedPtr vehicle_command_publisher_;
    rclcpp::Publisher<VehicleAttitudeSetpoint>::SharedPtr vehicle_attitude_setpoint_publisher_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr custom_msg_publisher_;

    rclcpp::Subscription<VehicleLocalPosition>::SharedPtr vehicle_local_position_subscription_;
    rclcpp::Subscription<VehicleAttitude>::SharedPtr vehicle_attitude_subscription_;
    rclcpp::Subscription<InputRc>::SharedPtr input_rc_subscription_;
    rclcpp::Subscription<DistanceSensor>::SharedPtr distance_sensor_subscription_;
    rclcpp::Subscription<VehicleOpticalFlowVel>::SharedPtr vehicle_optical_flow_vel_subscription_;
    rclcpp::Subscription<VehicleStatus>::SharedPtr vehicle_status_subscription_;

    std::atomic<uint64_t> timestamp_;   //!< common synced timestamped
    uint64_t timer_count = 0;
    uint64_t offboard_setpoint_counter_ = 1;   //!< counter for the number of setpoints sent

    // Parameter of Quadcopter
    float fz = 0.0;     
    float g = 9.8;
    float m = 1.545;
    float b = 4.6 * pow(10,-6);
    float omg_max = 1100;
    const double f_max = 4 * b * pow(omg_max,2); 

    float x_d = 0.0;
    float y_d = 0.0;
    float z_d = 0.0;
    float Yaw_hover = 0.0;

    float x_current = 0.0;
    float y_current = 0.0;
    float z_current = 0.0;

    float vx_current = 0.0;
    float vy_current = 0.0;
    float vz_current = 0.0;

    float vx_optical= 0.0;
    float vy_optical = 0.0;

    float Roll_current  = 0.0;
    float Pitch_current = 0.0;
    float Yaw_current   = 0.0;

    float Rc_CH6 = 0.0;
    uint16_t nav_state_ = 97;
    int16_t STATE = READY;
    uint8_t arming_state;

    float timer_target_1 = 400;
    float timer_target_2 = 350;
    float timer_target_3 = 350;
    float timer_target_4 = 600;
    float timer_target_5 = 500;
    float takeoff_timer  = 900;

    float timer_1 = takeoff_timer + timer_target_1;
    float timer_2 = timer_1 + timer_target_2;
    float timer_3 = timer_2 + timer_target_3;
    float timer_4 = timer_3 + timer_target_4;
    float timer_5 = timer_4 + timer_target_5;
    float timer_return = timer_5 + 500;
    float timer_landing = timer_return + 150;

    float Yaw_desired = 0.0;

    float x_body = 0.0;
    float y_body = 0.0;

    uint8_t state_offboard = 0;

    

    void publish_offboard_control_mode();
    void publish_vehicle_command(uint16_t command, float param1 = 0.0, float param2 = 0.0);
    void publish_vehicle_attitude_setpoint(float thrust_z, std::array<float, 4> quaternion);
};

void OffboardControl::arm()
{
    publish_vehicle_command(VehicleCommand::VEHICLE_CMD_COMPONENT_ARM_DISARM, 1.0);
    RCLCPP_INFO(this->get_logger(), "Arm command send");
}

void OffboardControl::disarm()
{
    publish_vehicle_command(VehicleCommand::VEHICLE_CMD_COMPONENT_ARM_DISARM, 0.0);
    RCLCPP_INFO(this->get_logger(), "Disarm command send");
}

void OffboardControl::offboard()
{
    publish_vehicle_command(VehicleCommand::VEHICLE_CMD_DO_SET_MODE, 1, 6);
    RCLCPP_INFO(this->get_logger(), "Offboard command send");
}

void OffboardControl::servo(std::string data_input)
{
    std_msgs::msg::String msg;
    msg.data = data_input;
    custom_msg_publisher_->publish(msg);
}

void OffboardControl::takeoff()
{
	auto msg = px4_msgs::msg::VehicleCommand();
    msg.timestamp = this->get_clock()->now().nanoseconds() / 1000;
    msg.param1 = 0.0f;         // pitch
    msg.param2 = 0.0f;
    msg.param3 = 0.0f;
    msg.param4 = 0.0f;         // yaw angle
    msg.param5 = NAN;          // latitude
    msg.param6 = NAN;          // longitude
    msg.param7 = -3.0f;        // altitude

    msg.command = px4_msgs::msg::VehicleCommand::VEHICLE_CMD_NAV_TAKEOFF;
    msg.target_system = 1;
    msg.target_component = 1;
    msg.source_system = 1;
    msg.source_component = 1;
    msg.from_external = true;

    RCLCPP_INFO(this->get_logger(), "Sending TAKEOFF command");
    vehicle_command_publisher_->publish(msg);
}

void OffboardControl::manual()
{
    publish_vehicle_command(VehicleCommand::VEHICLE_CMD_DO_SET_MODE, 1.0, 1.0);
    RCLCPP_INFO(this->get_logger(), "Manual command send");
}

void OffboardControl::landing()
{
    VehicleCommand msg{};
    msg.command = VehicleCommand::VEHICLE_CMD_NAV_LAND;  // MAV_CMD_NAV_LAND
    msg.target_system = 1;
    msg.target_component = 1;
    msg.source_system = 1;
    msg.source_component = 1;
    msg.from_external = true;
    msg.timestamp = this->get_clock()->now().nanoseconds() / 1000;
    vehicle_command_publisher_->publish(msg);
    RCLCPP_INFO(this->get_logger(), "Landing command send");
    RCLCPP_INFO(this->get_logger(), "Mode  = %d", nav_state_);
}

void OffboardControl::position_setpoint(float x, float y, float z, float Yaw_){
	TrajectorySetpoint msg{};
	msg.timestamp = this->get_clock()->now().nanoseconds() / 1000;
    msg.position = {x * cos(Yaw_) - y * sin(Yaw_), x * sin(Yaw_) + y * cos(Yaw_), z};
	msg.velocity = {NAN, NAN, NAN};
    msg.yaw = Yaw_;
    trajectory_setpoint_publisher_->publish(msg);
    RCLCPP_INFO(this->get_logger(), "Velocity command send");
}

void OffboardControl::publish_offboard_control_mode()
{
    OffboardControlMode msg{};
    msg.position = true;
    msg.velocity = false;
    msg.acceleration = false;
    msg.attitude = false;
    msg.actuator = false;
    msg.body_rate = false;
    msg.timestamp = this->get_clock()->now().nanoseconds() / 1000;
    offboard_control_mode_publisher_->publish(msg);
}

void OffboardControl::publish_vehicle_command(uint16_t command, float param1, float param2)
{
    VehicleCommand msg{};
    msg.param1 = param1;
    msg.param2 = param2;
    msg.command = command;
    msg.target_system = 1;
    msg.target_component = 1;
    msg.source_system = 1;
    msg.source_component = 1;
    msg.from_external = true;
    msg.timestamp = this->get_clock()->now().nanoseconds() / 1000;
    vehicle_command_publisher_->publish(msg);
}

void OffboardControl::publish_vehicle_attitude_setpoint(float thrust_z, std::array<float, 4> quaternion)
{
    VehicleAttitudeSetpoint msg{};
    msg.thrust_body[0] = 0.0;
    msg.thrust_body[1] = 0.0;
    msg.thrust_body[2] = thrust_z;
    msg.q_d[0] = quaternion[0];
    msg.q_d[1] = quaternion[1];
    msg.q_d[2] = quaternion[2];
    msg.q_d[3] = quaternion[3];
    msg.timestamp = this->get_clock()->now().nanoseconds() / 1000;
    vehicle_attitude_setpoint_publisher_->publish(msg);
}

std::array<float, 4> OffboardControl::RPY_to_Quaternion(float Roll, float Pitch, float Yaw)
{
    float cy = cos(Yaw/2);
    float sy = sin(Yaw/2);
    float cp = cos(Pitch/2);
    float sp = sin(Pitch/2);
    float cr = cos(Roll/2);
    float sr = sin(Roll/2);
    std::array<float, 4> quaternion;

    quaternion[0] = cr * cp * cy + sr * sp * sy;
    quaternion[1] = sr * cp * cy - cr * sp * sy;
    quaternion[2] = cr * sp * cy + sr * cp * sy;
    quaternion[3] = cr * cp * sy - sr * sp * cy;

    return quaternion;
}

float OffboardControl::PID_inner(float DesiredValue, float CurrentValue, float Kp)
{
    float err;
    float PID;

    err = DesiredValue - CurrentValue;
    PID = Kp * err;
    return PID;
}

float OffboardControl::PID_outter(float DesiredValue, float CurrentValue, float Kp)
{
    float err;
    float PID;

    err = DesiredValue - CurrentValue;
    if(err > 0.5) err = 0.5;
    else if (err < -0.5)  err = -0.5;

    PID = Kp * err;
    return PID;
}

float OffboardControl::Controller_z(float DesiredValue)
{
    float fz_out = 0.0;
    float PID = 0.0;

    PID = PID_inner(PID_outter(DesiredValue, z_current, 2.5), vz_current, 6.4);

    fz = ((g - PID) * m) / (cos(Roll_current) * cos(Pitch_current));
    fz_out = -fz/f_max;

    return fz_out;
}

void OffboardControl::Controller_xyz(float DesiredValueX, float DesiredValueY, float DesiredValueZ)
{
    float ux = 0.0;
    float uy = 0.0;
    float ux1 = 0.0;
    float uy1 = 0.0;
    float Roll_d = 0.0;
    float Pitch_d = 0.0;
    float Yaw_d = Yaw_current - M_PI/2;

    ux1 = PID_inner(PID_outter(DesiredValueX, x_current, 1.25), vx_current, 3.2);
    uy1 = PID_inner(PID_outter(DesiredValueY, y_current, 1.25), vy_current, 3.2);
    
    ux = -(ux1 * m) / fz;
    uy = -(uy1 * m) / fz;

    if((ux * sin(Yaw_d) - uy * cos(Yaw_d)) >= 1)         Roll_d = M_PI/2;
    else if((ux * sin(Yaw_d) - uy * cos(Yaw_d)) <= -1)   Roll_d = -M_PI/2;
    else                                                 Roll_d = asin(ux * sin(Yaw_d) - uy * cos(Yaw_d));
    
    if(((ux * cos(Yaw_d) + uy * sin(Yaw_d)) / cos(Roll_d)) >= 1)         Pitch_d = M_PI/2;
    else if(((ux * cos(Yaw_d) + uy * sin(Yaw_d)) / cos(Roll_d)) <= -1)   Pitch_d = -M_PI/2;
    else                                                                 Pitch_d = asin((ux * cos(Yaw_d) + uy * sin(Yaw_d)) / cos(Roll_d));

    if(Roll_d < -M_PI/6)     Roll_d = -M_PI/6;
    else if(Roll_d > M_PI/6) Roll_d = M_PI/6;

    if(Pitch_d < -M_PI/6)     Pitch_d = -M_PI/6;
    else if(Pitch_d > M_PI/6) Pitch_d = M_PI/6;

    publish_vehicle_attitude_setpoint(Controller_z(DesiredValueZ), RPY_to_Quaternion(Pitch_d, -Roll_d, Yaw_hover + M_PI/2));
}

void OffboardControl::xyz_setpoint(float x_desired, float y_desired, float z_desired)
{
    x_d = x_current + x_desired;
    y_d = y_current + y_desired;
    z_d = z_current + z_desired;
}
    
int main(int argc, char *argv[])
{
    std::cout << "Starting offboard control node..." << std::endl;
    setvbuf(stdout, NULL, _IONBF, BUFSIZ);
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<OffboardControl>());

    rclcpp::shutdown();
    return 0;
}