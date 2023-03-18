#ifndef SVG_H
#define SVG_H

#include <string>
#include <vector>

std::vector<int> get_path_coords(const std::string& coords_str);
void draw_path(std::vector<std::uint32_t>& pixels, const unsigned int rowlen,
    const std::uint32_t color, const std::string& path);
std::vector<std::string> get_paths_from_svg(const std::string& file_path);
void draw_svg(std::vector<std::uint32_t>& pixels, const unsigned int rowlen,
    const std::uint32_t color, const std::string& file_path);

#endif