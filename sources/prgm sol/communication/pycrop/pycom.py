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

NUM_ETAPE = 5
TX_PATH = 'tx.cmd'


if len(sys.argv)>2:
	PORT = sys.argv[1]
	file = open(sys.argv[2],'wb')
	file.write('Ouverture du fichier\n')
else:
	print "USAGE: python2 pycom.py PORT reception.bin"
	quit()




def RX(ser):
	etape = NUM_ETAPE
	buffer = ''

	while True:
		buffer += ser.read(ser.inWaiting())

		k = NUM_ETAPE
		while k >= 1 :
			if etape == k:
				if '*[%'+str(etape)+'%]*' in buffer:
					print 'ETAPE:\t'+str(etape)
					file.write(buffer)
					buffer = ''
					etape -= 1
					break
			k -=1

		if etape == 0:
			if '*[%0%]*' in buffer:
				file.write(buffer+'\nFermeture du fichier\n')
				file.close()
				break

def TX(ser):
	while True:
		time.sleep(1)
		if os.path.isfile(TX_PATH):
			tx = open(TX_PATH,'r')
			tx_cmd = tx.readline()
			tx.close()
			os.remove(TX_PATH)
			if '*[%END%]*' in tx_cmd:
				print 'END TX'
				if not thread_RX.isAlive():
					ser.close()
					break
			if '*[%JUMP%]*' in tx_cmd:
				print 'JUMP TO ETAPE:\t'+str(etape)
				file.write(buffer)
				etape -= 1


class myThread (threading.Thread):
	def __init__(self, name, serial):
		threading.Thread.__init__(self)
		self.name = name
		self.serial = serial
	def run(self):
		print "Starting " + self.name
		if self.name == 'RX':
			RX(self.serial)
		elif self.name == 'TX':
			TX(self.serial)
		print "Exiting " + self.name


if __name__ ==  '__main__':

	print "Debut du programme"
	print "PORT choisi: "+PORT

	etape = NUM_ETAPE

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


	#threading.Thread(target=RX, args=(ser,)).start()
	#threading.Thread(target=TX, args=(ser,)).start()

	# Fabrication des threads
	thread_RX = myThread('RX',ser)
	thread_TX = myThread('TX',ser)

	# Lancement des threads
	thread_TX.start()
	thread_RX.start()

	#ser.close()
	quit()
