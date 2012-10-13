#!/usr/bin/env python2
# -*- coding: utf-8 -*-

'''
Programme de réception de données de la station au sol.

Date: 07/08/2012
Auteur: Zubair Iftikhar
Licence: Librement copiable et modifiable.
'''

from serial import *
import sys, threading, os, time

NUM_ETAPE = 2

if len(sys.argv)>2:
	PORT = sys.argv[1]
else:
	print "USAGE: python2 pycom.py PORT reception.bin"
	quit()



def RX(ser):
    buffer = ''

    while True:
        buffer += ser.read(ser.inWaiting())

        if '*[%4%]*' in buffer:
            print 'Meteo OK'
            buffer = ''

        if '*[%3%]*' in buffer:
            print 'Reset'
            file = open("meteo.dat",'a')
            file.write(buffer)
            file.close()
            buffer = ''

        if '*[%2%]*' in buffer:
            print 'Snap'
            buffer = ''

        if '*[%1%]*' in buffer:
            print 'Transmission'
            buffer = ''

        if '*[%0%]*' in buffer:
            print 'Fin'
            file = open(sys.argv[2],'wb')
            file.write(buffer)
            buffer = ''
            file.close()
            os.rename(sys.argv[2],"images/"+time.strftime("%H-%M-%S", time.gmtime())+'.jpg')



class myThread (threading.Thread):
	def __init__(self, name, serial):
		threading.Thread.__init__(self)
		self.name = name
		self.serial = serial
	def run(self):
		print "Starting " + self.name
		if self.name == 'RX':
			RX(self.serial)
		print "Exiting " + self.name


if __name__ ==  '__main__':

	print "Debut du programme"
	print "PORT choisi: "+PORT

	ser = Serial(
		port=PORT,
		baudrate=9600,
		bytesize=EIGHTBITS,
		parity=PARITY_NONE,
		stopbits=STOPBITS_ONE,
		timeout=1,
		xonxoff=0,
		rtscts=0,
		interCharTimeout=None
	)

	# Fabrication des threads
	thread_RX = myThread('RX',ser)

	# Lancement des threads
	thread_RX.start()

	#ser.close()
	quit()
