#ifndef SVG_H
#define SVG_H

#include <SDL.h>
#include <string>
#include <vector>

std::vector<int> get_path_coords(const std::string& coords_str);
void draw_path(SDL_Renderer* r, const std::string& path);
std::vector<std::string> get_paths_from_svg(const std::string& file_path);
void draw_svg(SDL_Renderer* r, const std::string& file_path);

#endif