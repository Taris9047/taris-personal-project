#ifndef GNUPLOT_GEN_HPP
#define GNUPLOT_GEN_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cmath>

class GnuplotGen
{
public:
	std::string deck_filename;
	std::string input_filename;
	std::string output_filename;

	std::string terminal_type;
	std::string terminal_option;
	std::string font_option;
	std::string datafile_separator;

	std::string figure_title;

	double x_range_left;
	double x_range_right;
	double y_range_top;
	double y_range_bottom;

	bool set_dimension_rect(
		double left, double right, \
		double top, double bottom);
	bool WriteDeck();

	std::string dtostdstr(double innum);

	GnuplotGen();
	GnuplotGen(
		std::string input_deck_filename, \
		std::string data_filename, \
		std::string fig_filename);
	GnuplotGen(
		std::string input_deck_filename, \
		std::string data_filename, \
		std::string fig_filename, \
		double left, double right, \
		double top, double bottom);
	GnuplotGen(
		std::string input_deck_filename, \
		std::string data_filename, \
		std::string fig_filename, \
		std::string fig_title, \
		double left, double right, \
		double top, double bottom);
	~GnuplotGen();
};

#endif