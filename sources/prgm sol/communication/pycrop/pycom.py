#!/usr/bin/env python2
# -*- coding: utf-8 -*-

'''
Programme de réception de données de la station au sol.

Date: 07/08/2012
Auteur: Zubair Iftikhar
Licence: Librement copiable et modifiable.
'''

from serial import *
import sys, threading, re


if len(sys.argv)>2:
		PORT = sys.argv[1]
		file = open(sys.argv[2],'w')
		file.write('Ouverture du fichier\n')
else:
		print "USAGE: ./pycom PORT reception.dat"
		sys.exit(0)




def RX(ser):
		buffer = ''
		last_match = 0

		while True:
				buffer += ser.read(ser.inWaiting())

				if '*[%END%]*' in buffer:
						file.write(buffer+'\nFermeture du fichier\n')
						file.close()
						print "\n\n\t***Fin de la reception, tapper sur Q pour quitter***"
						break

				m=p.match(buffer)

				if m:
						matching=p.findall(buffer)
						if len(matching) > last_match:
								print 'RX:\t'+matching[-1]
								last_match = len(matching)
								file.write(buffer)
								buffer = ''
								last_match = 0


def TX(ser):
		ser.write('TX:plop\n')



if __name__ ==  '__main__':

		p = re.compile('\*\[%[A-Z0-9_]*%\]\*')
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


		threading.Thread(target=RX, args=(ser,)).start()
		threading.Thread(target=TX, args=(ser,)).start()

		while True:
				commande = raw_input()

				if commande == 'Q':
						sys.exit(0)
