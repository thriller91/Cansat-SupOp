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

NUM_ETAPE = 1
TX_PATH = 'tx.cmd'


if len(sys.argv)>2:
		PORT = sys.argv[1]
		file = open(sys.argv[2],'wb')
		file.write('Ouverture du fichier\n')
else:
		print "USAGE: ./pycom PORT reception.dat"
		sys.exit(0)




def RX(ser):
		buffer = ''
		etape = NUM_ETAPE

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
						if '*[%FIN%]*' in buffer:
								file.write(buffer+'\nFermeture du fichier\n')
								file.close()
								break

def TX(ser):
		ser.write('plop\n')
		while True:
				time.sleep(1)
				if os.path.isfile(TX_PATH):
						tx = open(TX_PATH,'r')
						tx_cmd = tx.readline()
						tx.close()
						os.remove(TX_PATH)
						if '*[%FIN%]*' in tx_cmd:
								break
						else:
								ser.write(tx_cmd)
								print 'TX:\t'+tx_cmd

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
				self.serial.close()
				print "Exiting " + self.name


if __name__ ==  '__main__':

		print "Debut du programme"
		print "PORT choisi: "+PORT

		ser = Serial(
				port=PORT,
				baudrate=115200,
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

		quit()
