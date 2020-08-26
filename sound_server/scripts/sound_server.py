#!/usr/bin/env python
# ROS Node that will read the battery level every n seconds and play or say
# the level using the sound_play node
# Dave Kush 
# Aug 22, 2020

import rospy, os, sys
import roslib
from std_msgs.msg import String
from sensor_msgs.msg import BatteryState
from sound_play.msg import SoundRequest
from sound_play.libsoundplay import SoundClient
#from sound_yak.msg import yak_cmd
from sensor_msgs.msg import Joy

battery_percentage = 0

pub = rospy.Publisher('/sound_play', String, queue_size=10)
throttle = 10 * 60  # duration of publishing the sound in seconds

def play_sound(data):
  #print data
  global allow_sound
  if rospy.Time.now() <= allow_sound: # Throttles yak to avoid SoundClient segfault
    #print("sound throttled")
    return

  # Now allow yak to run
  allow_sound = rospy.Time.now() + rospy.Duration.from_sec(throttle)
  soundhandle.say("battery at: " + str(data) + "%") 

def battery_callback(msg):
  battery_state_msgs = msg
  battery_percentage = round(msg.percentage, 1)
  rospy.loginfo(battery_percentage)
  play_sound(battery_percentage)

def joy_callback(msg):
  button = msg.buttons[0]
  rospy.loginfo(button)
  if (msg.buttons[0]):
    soundhandle.say("Please move over, I'm not that intelligent") 
  elif (msg.buttons[1]):
    soundhandle.say("My scanners are detecting a biological entity") 
  elif (msg.buttons[2]):
    soundhandle.say("Reactor is only running at 87% efficiency") 
  elif (msg.buttons[3]):
    soundhandle.say("Calculating approximate path with decision matrix") 


def listener():
  #rospy.init_node('sound_server', log_level=rospy.INFO)
  rospy.init_node('sound_server')
  rospy.Subscriber('/battery_state', BatteryState, callback, queue_size=1)
  #pub.publish(String(battery_percentage))

  global allow_sound
  allow_sound = rospy.Time.now()
  #soundhandle.say(str(battery_percentage)) 
  rospy.spin()


if __name__== '__main__':
  soundhandle = SoundClient()
  rospy.sleep(1)

  rospy.init_node('sound_server')

  rospy.Subscriber('/battery_state', BatteryState, battery_callback, queue_size=1)

  rospy.Subscriber('/joy', Joy, joy_callback, queue_size=1)

  global allow_sound
  allow_sound = rospy.Time.now()

  rospy.spin()

#  try:
#    listener()
#  except rospy.ROSInterruptException:
#    pass
