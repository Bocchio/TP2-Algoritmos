#ifndef NMEA__H
#define NMEA__H

#include <stdio.h>
#include "types.h"
#include "adt.h"
#include "vector.h"


#define FILE_FORMAT_CSV		"csv"
#define FILE_FORMAT_KML		"kml"

#define GPGA_HEADER	"$GPGGA"

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
		</Placemark>\n\
	</Document>\n\
</kml>"

#define NMEA_GGA_LABEL	"coordinates"

typedef enum {
	FORMAT_KML,
	FORMAT_CSV
} file_format_t;

typedef struct{
	double latitude;
	double longitude;
	double altitude;
} ADT_NMEA_GGA_t;

status_t parse_NMEA_from_csv(FILE *fi, ADT_Vector_t *gga, string delimiter);
status_t parse_file_format(const string, file_format_t *);
status_t export_NMEA(const ADT_Vector_t *vector, file_format_t format);

status_t ADT_NMEA_GGA_new(ADT_NMEA_GGA_t **nmea_gga_node, string *fields);
status_t ADT_NMEA_GGA_delete(ADT_NMEA_GGA_t **nmea_gga_node);
status_t ADT_NMEA_GGA_export_as_kml(ADT_NMEA_GGA_t *nmea_gga_node, void *tabs, FILE *fo);

#endif