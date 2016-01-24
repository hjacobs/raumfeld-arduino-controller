#!/usr/bin/env python3

import raumfeld
import serial
import time
from pysimplesoap.simplexml import SimpleXMLElement

devices = raumfeld.discover()
dev = devices[0]

ser = serial.Serial('/dev/ttyACM0')
while True:
    pos = dev.av_transport.GetPositionInfo(InstanceID=0)
    metadata = SimpleXMLElement(str(pos.TrackMetaData))
    track = {}
    for elem in metadata.item.children():
        track[elem.get_name()] = str(elem)

    text = '{} - {}'.format(track.get('upnp:artist'), track.get('dc:title'))
    ser.write(text.encode('utf-8'))
    val = int(ser.readline().strip())
    print('Setting volume to {}..'.format(val))
    dev.volume = val
    time.sleep(0.1)


