#include <iostream>
#include <fstream>
#include <CGAL/boost/graph/IO/PLY.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Kernel/global_functions.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polygon_mesh_processing/border.h>
#include <CGAL/Polygon_mesh_processing/triangulate_hole.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>
#include <string>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_3 Point_3;
typedef CGAL::Surface_mesh<Point_3> Mesh;

int main(int argc, char* argv[])
{
	const char* filename = argv[1];
	const char* convert_to = argv[2];

	//Getting the input and loading the mesh acc to filetype
	std::string fn = filename;
	std::string output_filetype = convert_to;
	std::string filetype = fn.substr(fn.length() - 3);
	std::cout << "Given mesh is of type ." << filetype << std::endl;
	std::cout << "File needs to be converted to ." << convert_to << std::endl;
	Mesh mesh;
	std::vector<Point_3> points;
	std::vector<std::vector<std::size_t> > polygons;

	std::ifstream mesh_input(filename);
	if (!mesh_input)
	{
		std::cerr << "Cannot open file " << std::endl;
		return EXIT_FAILURE;
	}
	if (filetype == "ply") {
		read_PLY(mesh_input, points, polygons);
	}
	else if (filetype == "obj") {
		read_OBJ(mesh_input, points, polygons);
	}
	else if (filetype == "off") {
		read_OFF(mesh_input, points, polygons);
	}
	else {
		std::cout << "Provided file format is not supported by this application" << std::endl;
		return EXIT_FAILURE;
	}
	mesh_input.close();

	//Construct mesh from input
	CGAL::Polygon_mesh_processing::polygon_soup_to_polygon_mesh(points, polygons, mesh);

	//Open output stream and write the output file according to specified type
	std::string output_fn = fn.erase(fn.length() - 3) + convert_to;
	std::cout << "Output filename will be: " << output_fn << std::endl;
	std::filebuf fb;
	fb.open(output_fn, std::ios::out);
	std::ostream os(&fb);
	//std::cout << convert_to == "obj" << std::endl;
	if (output_filetype == "ply") {
		write_PLY(os, points, polygons);
	}
	else if (output_filetype == "obj") {
		write_OBJ(os, points, polygons);
	}
	else if (output_filetype == "off") {
		write_OFF(os, points, polygons);
	}
	else {
		std::cout << "Provided file format is not supported by this application" << std::endl;
		return EXIT_FAILURE;
	}
	fb.close();
	
    return 0;
}