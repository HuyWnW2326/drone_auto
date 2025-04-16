import rclpy
from rclpy.node import Node
from std_msgs.msg import String
import serial

class TestSerialNode(Node):
    def __init__(self):
        super().__init__('serial_node')
        
        # Mở cổng serial
        try:
            self.ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
            self.get_logger().info('Kết nối với serial thành công!')
        except Exception as e:
            self.get_logger().error(f"Không thể kết nối với serial: {e}")
            self.ser = None

        # Tạo subscriber để nhận dữ liệu kiểu char từ topic
        self.subscription = self.create_subscription(
            String,
            'input_topic',
            self.listener_callback,
            10
        )

    def listener_callback(self, msg):
        input_char = msg.data.strip()  # Lấy ký tự từ tin nhắn

        if len(input_char) != 1:
            self.get_logger().warn("Dữ liệu nhận được không phải là 1 ký tự.")
            return

        try:
            input_byte = input_char.encode('utf-8')  # Chuyển ký tự sang byte
            if self.ser and self.ser.is_open:
                self.ser.write(input_byte)  # Gửi byte qua serial
                self.get_logger().info(f"Đã gửi ký tự: '{input_char}' -> byte: {input_byte}")
            else:
                self.get_logger().warn("Cổng serial không mở.")
        except Exception as e:
            self.get_logger().error(f"Lỗi khi gửi dữ liệu serial: {e}")

    def __del__(self):
        if self.ser and self.ser.is_open:
            self.ser.close()
            self.get_logger().info("Đã đóng cổng serial.")

def main(args=None):
    rclpy.init(args=args)
    node = TestSerialNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
