#!/usr/bin/env python2
# -*- coding: utf-8 -*-

'''
Programme de traitement des données réceptionnées par la station au sol.

Date: 17/08/2012
Auteur: Zubair Iftikhar
Licence: Librement copiable et modifiable.
'''

'''
Je suppose que rx.bin a donné les fichiers PTHXX.TXT etc.
'''

import sys, os
import matplotlib.pyplot as plt

if len(sys.argv)>1:
	os.chdir('parsed')
	PTH_file = open(sys.argv[1],'r')
	print 'Ouverture du fichier ' + sys.argv[1]
else:
	print "USAGE: ./pycook PTHXX.TXT"
	sys.exit(0)


if __name__ ==  '__main__':

	temps = []
	P_bmp = []
	T_bmp = []
	H_rht = []
	T_rht = []

	for line in PTH_file:
		liste = line.split()
		if len(liste) == 1:
			temps.append((float(liste[0])/1000000))
		else:
			if liste[0] == 'B':
				T_bmp.append(liste[1])
				P_bmp.append(liste[2])
			else:
				H_rht.append(liste[1])
				T_rht.append(liste[2])


	print 'Fin de la lecture du fichier ' + sys.argv[1]
	print str(temps[0])
	print 'Tracage des graphs'
	'''
	plt.subplot(211)
	plt.plot(temps,T_bmp,'bo')
	plt.plot(temps,T_bmp,'b')
	plt.ylabel(u'température (°C)')
	plt.grid(True)
	plt.subplot(212)
	'''
	plt.plot(temps,T_rht,'ro')
	plt.plot(temps,T_rht,'r')
	plt.xlabel(u'time since drop (s)')
	plt.ylabel(u'temperature (°C)')
	plt.grid(True)
	plt.title(u'Graph of Temperature with respect to Time')
	plt.savefig('cooked/Temp.pdf', format='pdf')
	plt.savefig('cooked/Temp.png', format='png')
	plt.close()

	plt.plot(temps,H_rht,'ro')
	plt.plot(temps,H_rht,'r')
	plt.xlabel(u'time since drop (s)')
	plt.ylabel(u'humidity (%)')
	plt.title(u'Graph of Humidity with respect to Time')
	plt.grid(True)
	plt.savefig('cooked/Hygro.pdf', format='pdf')
	plt.savefig('cooked/Hygro.png', format='png')
	plt.close()

	'''
	plt.plot(temps,P_bmp,'bo')
	plt.plot(temps,P_bmp,'b')
	plt.xlabel(u'temps (µs)')
	plt.ylabel(u'pression (Pa)') # à convertir en atm?
	plt.title(u'Évolution de la pression en fonction du temps')
	plt.grid(True)
	plt.savefig('cooked/Press.pdf', format='pdf')
	plt.close()
	'''

	PTH_file.close()
	print 'FIN'
	quit()
