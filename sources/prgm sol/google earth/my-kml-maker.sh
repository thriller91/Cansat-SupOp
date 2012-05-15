#! /bin/sh
#
# Script shell permettant de générer un fichier KML contenant
# les coordonées GPS du vol. Ce fichier sera alors lu avec
# Google Earth (C) pour tracer la trajectoire du cansat 
# pendant son vol.
# Ce script prend en argument les coordonnées au format texte
# suivant: 
# par exemple "-112.2549277039738,36.08117083492122,2357"
#
# Usage : my-kml-maker.sh coords.txt > flight.kml
# Version : 0.1

#test sur le nombre de paramètres.
if [ $# = 0 ] 
then 
	echo "Donner un fichier de coordonnées"
	exit 1
fi

echo '<?xml version="1.0" encoding="UTF-8"?>
<kml xmlns="http://www.opengis.net/kml/2.2">
  <Document>
    <name>Vol Cansat</name>
    <description>Trajectoire de vol du cansat.</description>
    <Style id="yellowLineGreenPoly">
      <LineStyle>
        <color>7f00ffff</color>
        <width>4</width>
      </LineStyle>
      <PolyStyle>
        <color>7f00ff00</color>
      </PolyStyle>
    </Style>
    <Placemark>
      <name>Biscarosse?</name>
      <description>Transparent green wall with yellow outlines</description>
      <styleUrl>#yellowLineGreenPoly</styleUrl>
      <LineString>
        <extrude>1</extrude>
        <tessellate>1</tessellate>
        <altitudeMode>absolute</altitudeMode>
        <coordinates>'

cat $1

echo '</coordinates>
      </LineString>
    </Placemark>
  </Document>
</kml>'

exit 0

