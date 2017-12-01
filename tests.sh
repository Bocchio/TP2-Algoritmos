#!/usr/bin/bash

./gpsviewer -fmt kml -out trayectoria-A.kml trayectoria-A.txt
./gpsviewer -fmt kml -out trayectoria-B.kml trayectoria-B.txt
./gpsviewer -fmt kml -out 9dejulio.kml 9dejulio.txt
./gpsviewer -fmt kml -out parquecolon.kml parquecolon.txt

./gpsviewer -fmt csv -out trayectoria-A.csv trayectoria-A.txt
./gpsviewer -fmt csv -out trayectoria-B.csv trayectoria-B.txt
