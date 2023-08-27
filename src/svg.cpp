#include "svg.h"
#include "line.h"
#include "bezier.h"
#include "fill.h"
#include <array>
#include <fstream>
#include <regex>
#include <sstream>
#include <stdexcept>

const std::regex path_regex("^<path .* d=\"(.*)\"/>$");
const std::regex path_cmd_regex("(?:[A-Za-z](?: ?\\d+ ?)*)");

std::vector<int> get_path_coords(const std::string& coords_str)
{
	std::vector<int> coords;
	std::stringstream ss(coords_str);
	std::string token;
	while (getline(ss, token, ' ')) {
		int coord = std::stoi(token);
		coords.push_back(coord);
	}
	return coords;
}

void draw_path(std::vector<std::uint32_t>& pixels, const unsigned int width,
	const std::uint32_t color, const std::string& path)
{
	int sx = 0;
	int sy = 0;
	int cx = 0;
	int cy = 0;

	auto cmds_begin = std::sregex_iterator(path.begin(), path.end(), path_cmd_regex);
	auto cmds_end = std::sregex_iterator();

	int n = 0;
	for (std::sregex_iterator i = cmds_begin; i != cmds_end; i++) {
		std::smatch match = *i;
		std::string match_str = match.str();
		const char type = match_str.at(0);

		if (type == 'Z') {
			draw_line_bresenham(pixels, width, color, cx, cy, sx, sy);
			n++;
			continue;
		}

		std::vector<int> coords = get_path_coords(match_str.substr(1));

		switch (type) {
		case 'M':
			cx = coords.at(0);
			cy = coords.at(1);
			if (n == 0) {
				sx = cx;
				sy = cy;	
			}
			break;
		case 'C':
			draw_bezier_cubic(pixels, width, color, cx, cy, coords.at(0), coords.at(1), coords.at(2), coords.at(3), coords.at(4), coords.at(5));
			cx = coords.at(4);
			cy = coords.at(5);
			break;
		case 'Q':
			draw_bezier_quad(pixels, width, color, cx, cy, coords.at(0), coords.at(1), coords.at(2), coords.at(3));
			cx = coords.at(2);
			cy = coords.at(3);
			break;
		}
		n++;
	}
}

std::vector<std::string> get_paths_from_svg(const std::string& file_path)
{
	std::ifstream svg_file;
	svg_file.open(file_path);
	if (!svg_file.is_open()) {
		throw std::invalid_argument("Unable to open \"" + file_path + "\".");
	}

	std::vector<std::string> paths;
	std::string line;
	std::smatch matches;
	bool in_paths_section = false;
	while (std::getline(svg_file, line)) {
		if (std::regex_match(line, matches, path_regex)) {
			paths.push_back(matches[1]);
			in_paths_section = true;
		} else if (in_paths_section) {
			break;
		}
	}

	return paths;
}

void draw_svg(std::vector<std::uint32_t>& pixels,
	const unsigned int width, const unsigned int height,
	const std::uint32_t color, const std::string& file_path)
{
	std::vector<std::string> paths = get_paths_from_svg(file_path);
	if (paths.empty())
		return;

	for (const std::string& path : paths) {
		std::vector<std::uint32_t> path_pixels(pixels.size());
		draw_path(path_pixels, width, color, path);
		const BoundingRect br = get_bounding_rect(path_pixels, width, height, color);
		scanline_fill_area(path_pixels, width, height, br.x_min, br.y_min, br.x_max, br.y_max, color);
		for (unsigned int y = br.y_min; y <= br.y_max; y++) {
			const unsigned int row = y * width;
			for (unsigned int x = br.x_min; x <= br.x_max; x++) {
				const unsigned int i = row + x;
				if (path_pixels.at(i) == color)
					pixels.at(i) = color;
			}
		}
	}
}
