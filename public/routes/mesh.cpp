#include <iostream>
#include <fstream>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/IO/OFF_reader.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>
#include <CGAL/Polygon_mesh_processing/orientation.h>
#include <CGAL/Polygon_mesh_processing/orient_polygon_soup_extension.h>
#include <CGAL/Surface_mesh.h>
//#include <CGAL/HalfedgeDS_default.h>
//#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <CGAL/Inverse_index.h>
#include <CGAL/Arrangement_2.h>

struct Traits { typedef int Point_2; };
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_3 Point_3;
typedef CGAL::Polyhedron_3<Kernel> Polyhedron;
typedef CGAL::Surface_mesh<Point_3> Mesh;
typedef CGAL::HalfedgeDS_default<Traits> HDS;
typedef HDS::Halfedge_iterator Halfedge_iterator;
typedef Polyhedron::Halfedge_handle Halfedge_handle;
typedef Polyhedron::Vertex_handle Vertex_handle;
typedef CGAL::HalfedgeDS_default<Traits> HDS;
typedef Polyhedron::Vertex_const_iterator VCI;
typedef Polyhedron::Facet_const_iterator FCI;
typedef Polyhedron::Halfedge_around_facet_const_circulator HFCC;
typedef CGAL::Inverse_index<VCI> Index;


int main()
{
	/*const char* filename = "bunny.off";
	std::ifstream input(filename);
	if (!input)
	{
		std::cerr << "Cannot open file " << std::endl;
		return EXIT_FAILURE;
	}
	std::vector<Kernel::Point_3> points;
	std::vector< std::vector<std::size_t> > polygons;
	CGAL::File_scanner_OFF scanner(input);
	polygons.resize(scanner.size_of_facets());
	points.resize(scanner.size_of_vertices());

	for (std::size_t i = 0; i < scanner.size_of_vertices(); ++i)
	{
		double x, y, z, w;
		scanner.scan_vertex(x, y, z, w);
		points[i] = Point_3(x, y, z, w);
		scanner.skip_to_next_vertex(i);
	}

	for (std::size_t i = 0; i < scanner.size_of_facets(); ++i) {
		std::size_t no;
		scanner.scan_facet(no, i);
		polygons[i].resize(no);

		for (std::size_t j = 0; j < no; ++j) {
			std::size_t id;
			scanner.scan_facet_vertex_index(id, i);
			if (id < scanner.size_of_vertices()) {
				polygons[i][j] = id;
			}
			else {
				std::cerr << "Error: Input file is not valid.\n";
				return 1;
			}
		}
	}
	input.close();
	std::cout << "Is the soup a polygon mesh ? : "
		<< CGAL::Polygon_mesh_processing::is_polygon_soup_a_polygon_mesh(polygons)
		<< std::endl;
	CGAL::Polygon_mesh_processing::orient_polygon_soup(points, polygons);
	std::cout << "And now ? : "
		<< CGAL::Polygon_mesh_processing::is_polygon_soup_a_polygon_mesh(polygons)
		<< std::endl;

	Mesh poly;
	CGAL::Polygon_mesh_processing::polygon_soup_to_polygon_mesh(points, polygons, poly);

	Polyhedron mesh;
	CGAL::copy_face_graph(poly, mesh);*/

	const char* filename_ply = "bun_zipper.ply";
	std::ifstream ply_input(filename_ply);
	if (!ply_input)
	{
		std::cerr << "Cannot open file " << std::endl;
		return EXIT_FAILURE;
	}
	Mesh mesh_ply;
	read_ply(ply_input, mesh_ply);
	ply_input.close();

	std::cout << "before conversion: " << mesh_ply.number_of_vertices() << std::endl;

	Polyhedron mesh;
	CGAL::copy_face_graph(mesh_ply, mesh);

	std::cout << "after conversion: " << mesh.size_of_vertices() << std::endl;
	std::vector<Point_3> border_points;
	//Index v_index(mesh.vertices_begin(), mesh.vertices_end());

	for (auto h = mesh.halfedges_begin(); h != mesh.halfedges_end(); h++) {

		Halfedge_handle edge = h;
		//std::cout << h->is_border() << std::endl;
		//border_edges.push_back(edge);
		if (h->is_border()) {
			//Point_3 point = h->vertex()->point();
			border_points.push_back(h->vertex()->point());
			//std::cout << v->point() << std::endl;
			//border_points.push_back(v_index[VCI(h->vertex())]);
		}
	}

	//for (auto v = mesh_ply.vertices_begin(); v != mesh_ply.vertices_end(); h++) {
	//	if (Mesh::is_border(v->idx()) {
	//	}
	//}
	std::cout << border_points.size() << std::endl;
	//std::cout << border_points[1] << std::endl;

	int i = 0, j = 0;
	std::filebuf fb;
	fb.open("edited_bunny.ply", std::ios::out);
	std::ostream os(&fb);

	os << "ply\n"
		<< "format ascii 1.0\n"
		<< "element vertex " << mesh.size_of_vertices() << "\n"
		<< "property float x\n"
		<< "property float y\n"
		<< "property float z\n"
		<< "property uchar red\n"
		<< "property uchar green\n"
		<< "property uchar blue\n"
		<< "element face " << mesh.size_of_facets() << "\n"
		<< "property list uchar int vertex_index\n"
		<< "end_header\n";

	for (auto it = mesh.vertices_begin(); it != mesh.vertices_end(); it++) {
		os << it->point().x() << " " << it->point().y() << " " << it->point().z() << " " << 255 << " " << 0 << " " << 0 << std::endl;
	}

	/*for (auto it = mesh.vertices_begin(); it != mesh.vertices_end(); it++) {
		//Point_3 p = it->point();
		for (int n = 0; n < border_points.size(); n++) {
			if (it->point() == border_points[n]) {
				//std::cout << i << std::endl;
				os << it->point().x() << " " << it->point().y() << " " << it->point().z() << " " << 255 << " " << 0 << " " << 0 << std::endl;
				//i++;
			}
			//else {
			//}
		}
		os << it->point().x() << " " << it->point().y() << " " << it->point().z() << " " << 255 << " " << 255 << " " << 255 << std::endl;
	}*/
	
	Index index(mesh.vertices_begin(), mesh.vertices_end());

	for (FCI fi = mesh.facets_begin(); fi != mesh.facets_end(); ++fi) {
		HFCC hc = fi->facet_begin();
		HFCC hc_end = hc;

		os << circulator_size(hc) << " ";
		do {
			os << index[VCI(hc->vertex())] << " ";
			++hc;
		} while (hc != hc_end);

		os << "\n";
	}

	fb.close();

    return 0;
}