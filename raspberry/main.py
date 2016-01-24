#!/usr/bin/env python3

import raumfeld
import serial
import sys
import time
from pysimplesoap.simplexml import SimpleXMLElement

devices = raumfeld.discover()
dev = devices[0]

ser = serial.Serial(sys.argv[1])
while True:
    pos = dev.av_transport.GetPositionInfo()
    try:
        metadata = SimpleXMLElement(str(pos.TrackMetaData))
    except:
        metadata = None
    if metadata:
        track = {}
        for elem in metadata.item.children():
            track[elem.get_name()] = str(elem)
        text = '{} - {}'.format(track.get('upnp:artist'), track.get('dc:title'))
        ser.write(text.encode('utf-8'))
    val = ser.readline().strip()
    print(val)
    key, sep, val = val.partition(b'=')
    if key == b'volume':
        val = int(val)
        print('Setting volume to {}..'.format(val))
        dev.volume = val
    elif key == b'prev':
        print('Previous track..')
        dev.av_transport.Previous()
    elif key == b'next':
        print('Next track..')
        dev.av_transport.Next()
    time.sleep(0.1)


