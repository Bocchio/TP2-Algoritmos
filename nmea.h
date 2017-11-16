#ifndef NMEA__H
#define NMEA__H

#include <stdio.h>
#include "types.h"
#include "adt.h"
#include "vector.h"


#define KML_HEADER "\
<kml xmlns=\"http://www.opengis.net/kml/2.2\">\n\
	<Document>\n\
		<name>Rutas</name>\n\
		<description>Ejemplos de rutas</description>\n\
		<Style id=\"yellowLineGreenPoly\">\n\
			<LineStyle>\n\
				<color>7f00ffff</color>\n\
				<width>4</width>\n\
			</LineStyle>\n\
			<PolyStyle>\n\
				<color>7f00ff00</color>\n\
			</PolyStyle>\n\
		</Style>\n\
		<Placemark>\n\
			<name>Relieve absoluto</name>\n\
			<description>Pared verde transparente con contornos amarillos</description>\n\
			<styleUrl>#yellowLineGreenPoly</styleUrl>\n\
			<LineString>\n\
				<extrude>1</extrude>\n\
				<tessellate>1</tessellate>\n\
				<altitudeMode>absolute</altitudeMode>\n"

#define KML_FOOTER "\
			</LineString>\n\
		</Placemark>\n\
	</Document>\n\
</kml>"

#define GPGGA_HEADER	"$GPGGA"
#define NMEA_GGA_LABEL	"coordinates"
#define NMEA_FIELD_DELIMITER	","
#define OUTPUT_CSV_DELIMITER	"|"

#define GPGGA_NORTH_TOKEN 	"N"
#define GPGGA_SOUTH_TOKEN 	"S"
#define GPGGA_EAST_TOKEN	"E"
#define GPGGA_WEST_TOKEN	"W"

#define GPGGA_EW_INDICATOR_POS	5
#define GPGGA_NS_INDICATOR_POS	3

#define GPGGA_LATITUDE_POS		2
#define GPGGA_LONGITUDE_POS		4
#define GPGGA_ALTITUDE_POS		9

typedef struct{
	double latitude;
	double longitude;
	double altitude;
	bool_t has_coord;
} ADT_NMEA_GGA_t;

status_t parse_NMEA(FILE *fi, ADT_Vector_t **gga);
status_t export_NMEA(const ADT_Vector_t *vector, file_format_t format, FILE *fo);
status_t parse_NMEA_latlon(string coord, double *degrees);

status_t ADT_NMEA_GGA_new(ADT_NMEA_GGA_t **gga_node, string *fields);
status_t ADT_NMEA_GGA_delete(ADT_NMEA_GGA_t **gga_node);
status_t ADT_NMEA_GGA_export_as_kml(const ADT_NMEA_GGA_t *gga_node, void *ctx, FILE *fo);
status_t ADT_NMEA_GGA_export_as_csv(const ADT_NMEA_GGA_t *gga_node, void *ctx, FILE *fo);


#endif