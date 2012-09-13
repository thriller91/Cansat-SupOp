#!/usr/bin/env python2
# -*- coding: utf-8 -*-

'''
Programme supprime le retour à la ligne!

Date: 28/08/2012
Auteur: Zubair Iftikhar
Licence: Librement copiable et modifiable.
'''

import sys, os
import matplotlib.pyplot as plt

if len(sys.argv)>1:
	os.chdir('parsed')
	POS_file = open(sys.argv[1],'r')
	print 'Ouverture du fichier ' + sys.argv[1]
else:
	print "USAGE: ./pycook POS.TXT"
	sys.exit(0)


if __name__ ==  '__main__':

	time = ""
	ligne = ""
	num = 0

	POS_cooked = open("cooked/POS.TXT","w")

	for line in POS_file:
		liste = line.split()
		if len(liste) == 1:
			ligne += str(liste[0]) + "\n"
			#if (num%4) == 1:
			POS_cooked.write(ligne)
			ligne = ""
		else:
			ligne = str(liste[0]) + "\t"
			ligne += str(liste[1]) + "\t"
			ligne += str(liste[2]) + "\t"
		num += 1

	POS_cooked.close()
	POS_file.close()
	print 'FIN'
	quit()
