import paramiko
import socket
import time
import rospy
from geometry_msgs.msg import Twist

def waitStreams(chan):
    time.sleep(1)
    outdata = errdata = ""
    while chan.recv_ready():
        outdata += chan.recv(1000).decode('utf-8')
    while chan.recv_stderr_ready():
        errdata += chan.recv_stderr(1000).decod('utf-8')
    return outdata, errdata

def pub_stop_motor():
    rospy.init_node('stop', anonymous=True, disable_rosout=True)
    pub = rospy.Publisher("/cmd_vel", Twist, True, queue_size=100)
    pub.publish(Twist())
    timeout_t = time.time() + 3
    while not rospy.is_shutdown() and time.time() < timeout_t:
        rospy.sleep(0.2)

def main():
    try:
        remote_ip = socket.gethostbyname(socket.gethostname())
        client_ip = f"{remote_ip[:-3]}223"
        ssh = paramiko.SSHClient()
        ssh.set_missing_host_key_policy(paramiko.MissingHostKeyPolicy())
        ssh.connect(client_ip, username="odroid", password="odroid")
        channel = ssh.invoke_shell()
        outdata, errdata = waitStreams(channel)
        print(outdata)
        channel.send(f"export ROS_MASTER_URI=http://{remote_ip}:11311\n")
        outdata, errdata = waitStreams(channel)
        channel.send(f"export ROS_HOSTNAME={client_ip}\n")
        outdata, errdata = waitStreams(channel)
        print(outdata)
        channel.send("roslaunch stella_bringup stella_robot.launch\n")
        while True:
            outdata, errdata = waitStreams(channel)
            if outdata:
                print(outdata)
            if errdata:
                print(errdata)

    except KeyboardInterrupt:
        print('Interrupted')
        pub_stop_motor()
        channel.send(chr(3))

    except Exception as e:
        print(e)

    finally:
        if ssh is not None:
            ssh.close()

if __name__ == "__main__":
    main()
    