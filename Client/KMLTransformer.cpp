#include "KMLTransformer.h"

void KML_Transformer::CreateKML(std::string filename_inp, double home_point_input[3])
{
	kml->set_feature(document);  // kml takes ownership.
								 //Serialize to XML
	xml = kmldom::SerializePretty(kml);
	printf("%s \n", xml.c_str());
	filename = filename_inp +".kml";
	for (int i = 0; i<3; i++)
		home_point[i] = home_point_input[i];
}

void KML_Transformer::KMLNewValue(int waypoint_num, float input[3])
{
	// Create <coordinates>.
	CoordinatesPtr coordinates = factory->CreateCoordinates();
	// Create <coordinates>-122.0816695,37.42052549<coordinates>

	// Create <Point> and give it <coordinates>.
	PointPtr point = factory->CreatePoint();

	// Create <Placemark> and give it a <name> and the <Point>.
	PlacemarkPtr placemark = factory->CreatePlacemark();

	double deg_lat = 111111;
	double deg_lon = 111111 * cos(home_point[0] * 180 / M_PI);

	coordinates->add_latlngalt(home_point[0] + input[0] / deg_lat, home_point[1] + input[1] / deg_lon, home_point[2] + input[2] * (-1));
	point->set_coordinates(coordinates);  // point takes ownership
	placemark->set_name("Waypoint" + std::to_string(waypoint_num));
	placemark->set_geometry(point);  // placemark takes ownership
	document->add_feature(placemark);

	xml = kmldom::SerializePretty(kml);

	write.open(filename.c_str(), std::ios::out);
	write.write(xml.c_str(), xml.size());
	write.close();
}