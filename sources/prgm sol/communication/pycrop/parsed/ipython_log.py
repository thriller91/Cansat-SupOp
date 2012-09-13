# IPython log file

get_ipython().magic(u'logstart')
PTH_File = open("PTH.TXT","r")
temps = []
H = []
T = []
for line in PTH_File:
    liste = line.split()
    if len(liste) == 1:
        temps.append(liste)
        
for line in PTH_File:
    liste = line.split()
    if len(liste) != 1:
        H.append(liste[1])
T.append(liste[2])
for line in PTH_File:
    liste = line.split()
    if len(liste) != 1:
        H.append(liste[1])
        T.append(liste[2])
        
import matplotlib.pyplot as plt
plt.plot(temps,T,'bo')
temps
T
H
H = []
for line in PTH_File:
    liste = line.split()
    if len(liste) == 3:
        H.append(liste[1])
        T.append(liste[2])
        
plt.plot(temps,T,'bo')
H = []
T
PTH_File.close()
PTH_File = open("PTH.TXT","r")
for line in PTH_File:
    liste = line.split()
    if len(liste) == 3:
        H.append(liste[1])
        T.append(liste[2])
        
T
plt.plot(temps,T,'bo')
plt.plot(temps,T,'b')
plt.figure
plt.figure(1)
plt.plot(temps,T,'b')
plt.plot(temps,T,'bo')
plt.figure(2)
plt.plot(temps,H,'bo')
plt.figure(2)
plt.plot(temps,H,'ro')
plt.plot(temps,H,'r')
