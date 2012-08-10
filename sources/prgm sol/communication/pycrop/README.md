#Station au sol

##Généralités
La communication entre le τετραφάρμακος et la station au sol se fait avec des modules XBee Pro.

##Pré-requis
L'ordinateur de la station au sol doit être connecté à une XBee Pro *via* un dongle USB. Le script fournit dans ce dossier utilise Python (la version 2.7.3 sur ma machine), il faut en outre que le module **pySerial** soit installé.
Ce script doit fonctionner sous la plupart des système d'exploitation, mais il a uniquement été testé sous GNU/Linux, et le paragraphe suivant donne un mode d'emploi avec cet OS.

##Utilisation
Pour lancer le script, il faut connaître le port utilisé par le dongle USB. Pour cela, il faut lancer `ls /dev`, et repérer un **/dev/ttyUSB0** (il peut eventuellement s'agir de ttyUSB1 ou un autre).
Puis il faut lancer `python2 pycom.py /dev/ttyUSB0 rx.bin`


