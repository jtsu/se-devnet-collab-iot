#!/usr/bin/env python

from myspark import spark_get_room_id, spark_send_message
import requests

# Disable all warning messages since we're dealing with a
# self-signed certificate on APIC-EM
requests.packages.urllib3.disable_warnings()

# We need to know our token and other information.
# Make sure you either create your own room
# or provide the name of the common event room.
# Replace None with the value
SPARK_TOKN = 'REPLACE_WITH_YOUR_SPARK_TOKEN'
SPARK_ROOM = 'REPLACE_WITH_YOUR_SPARK_ROOM'

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
