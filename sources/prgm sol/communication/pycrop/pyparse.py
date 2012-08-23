#!/usr/bin/env python2
# -*- coding: utf-8 -*-

'''
Programme de traitement du fichier récupéré pour fabriquer les fichiers de données.

Date: 23/08/2012
Auteur: Zubair Iftikhar
Licence: Librement copiable et modifiable.
'''

import sys,os
from collections import deque

if len(sys.argv)>1:
	rx_bin = sys.argv[1]
else:
	print "USAGE: python2 pyparse.py reception.bin"
	quit()


if __name__ == '__main__':
	with open(rx_bin, "rb") as f:
		byte = f.read(1)
		buffer = byte
		while byte:
			if byte == '*':
				if '*[%2%]*' in buffer:
					print "ETAPE 2"
					log = open("parsed/log.txt","w")
					log.write(buffer)
					buffer = ''
					log.close()
				if '*[%1%]*' in buffer:
					print "ETAPE 1"
					pth = open("parsed/PTH.TXT","w")
					pth.write(buffer)
					buffer = ''
					pth.close()
				if '*[%0%]*' in buffer:
					print "ETAPE 0"
					img = open("parsed/IMG01.JPG","w")
					img.write(buffer)
					buffer = ''
					img.close()
			byte = f.read(1)
			buffer += byte

		# ATTENTION! : CE QUI SUIT EST UNE USINE À GAZ

		# Parsage du fichier contenant pression, température et hygrométrie
		pth = open("parsed/PTH.TXT","r")
		tmp = open("tmp.txt","w")
		for line in pth:
			liste = line.split()
			if (len(liste) != 0 and not "*[%1%]*" in liste):
				tmp.write(line)
		pth.close()
		tmp.close()
		os.rename("tmp.txt","parsed/PTH.TXT")

		# Parsage de l'image IMG01.JPG
		with open("parsed/IMG01.JPG","r+b") as img:
			img.seek(img.read().rfind('*[%0%]*'))
			img.truncate()
		img.close()

		tmp = open("tmp.jpg","w")
		with open("parsed/IMG01.JPG","rb") as img:
			byte = img.read(1)
			byte2 = byte
			while (ord(byte)!=255 and ord(byte2)!=216):
				byte = byte2
				byte2 = img.read(1)
			img.seek(-1, os.SEEK_CUR)
			while byte:
				tmp.write(byte)
				byte = img.read(1)
		img.close()
		tmp.close()
		os.rename("tmp.jpg","parsed/IMG01.JPG")

		quit()



