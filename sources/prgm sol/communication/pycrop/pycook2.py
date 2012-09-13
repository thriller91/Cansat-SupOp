#!/usr/bin/env python2
# -*- coding: utf-8 -*-

'''
Programme de traitement des données réceptionnées par la station au sol.

Date: 29/08/2012
Auteur: Zubair Iftikhar
Licence: Librement copiable et modifiable.
'''

import sys, os
import matplotlib.pyplot as plt

if len(sys.argv)>1:
	#os.chdir('parsed')
	POS_file = open(sys.argv[1],'r')
	print 'Ouverture du fichier ' + sys.argv[1]
else:
	print "USAGE: ./pycook PTHXX.TXT"
	sys.exit(0)


if __name__ ==  '__main__':

	temps = []
	a_x = []
	a_y = []
	a_z = []

	for line in POS_file:
		liste = line.split()
		a_x.append(liste[0])
		a_y.append(liste[1])
		a_z.append(liste[2])
		temps.append(float(liste[3])/1000000)


	print 'Fin de la lecture du fichier ' + sys.argv[1]
	print 'Tracage des graphs'

	plt.subplot(311)
	plt.title("3-Axis Acceleration plotting with respect to Time")
	plt.plot(temps,a_x,'r')
	plt.ylabel('axis "X" (g)')
	plt.ylim([-1.5,1.5])
	plt.xlim([0,15])
	plt.grid(True)

	plt.subplot(312)
	plt.plot(temps,a_y,'g')
	plt.ylabel('axis "Y" (g)')
	plt.ylim([-1.5,1.5])
	plt.xlim([0,15])
	plt.grid(True)

	plt.subplot(313)
	plt.plot(temps,a_z,'b')
	plt.ylabel('axis "Z" (g)')
	plt.ylim([-1.5,1.5])
	plt.xlim([0,15])
	plt.grid(True)

	plt.xlabel('time since drop (s)')

	plt.savefig('parsed/cooked/Acc.pdf', format='pdf')
	plt.savefig('parsed/cooked/Acc.png', format='png')

	POS_file.close()
	print 'FIN'
	quit()
