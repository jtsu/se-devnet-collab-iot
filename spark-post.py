#!/usr/bin/env python

# This code is originally from the Cisco DevNet Express Learning Labs
# and has been modified for the Cisco SoCal SE Mini DevNet Lab.

from myspark import spark_get_room_id, spark_send_message
import requests

# We need to know our token and other information.
# Make sure you either create your own room
# or provide the name of the common event room.
SPARK_TOKN = "" # Your Spark Token
SPARK_ROOM = "" # Your Spark Room

# this is the variable of the room id we need to figure out:
room_id = None
# you can replace room ID from output of script here
# this will speed up things a bit as the room search
# can be omitted.
# room_id = 'your-room-id-output-from-the-script'


if not room_id:
    room_id = spark_get_room_id(SPARK_TOKN, SPARK_ROOM)
    print('Your room ID "{id}".'.format(id=room_id))

if room_id:
    spark_send_message(SPARK_TOKN, room_id,
                       'Hello room! My script verified that I can '
                       'post messages to Spark using REST API calls.')

    print('Please check room ' + SPARK_ROOM +
          ', there are messages posted on your behalf.')
