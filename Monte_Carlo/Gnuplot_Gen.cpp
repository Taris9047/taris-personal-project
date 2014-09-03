#ifndef GNUPLOT_GEN_CPP
#define GNUPLOT_GEN_CPP

#include "Gnuplot_Gen.hpp"

///////////////////////////////////
// Methods and stuff             //
///////////////////////////////////

// Setting up dimension.
bool GnuplotGen::set_dimension_rect(
	double left, double right, \
	double top, double bottom)
{
	if (left > right) {
		this->x_range_left = right;
		this->x_range_right = left;
		this->y_range_top = top;
		this->y_range_bottom = bottom;
	}
	else if (top < bottom) {
		this->x_range_left = left;
		this->x_range_right = right;
		this->y_range_top = bottom;
		this->y_range_bottom = top;
	}
	else {
		this->x_range_left = left;
		this->x_range_right = right;
		this->y_range_top = top;
		this->y_range_bottom = bottom;
	}

	return true;
}

// Writing the gnuplot deck.
bool GnuplotGen::WriteDeck()
{
	std::ofstream gnuplot_input_deck;
	gnuplot_input_deck.open(this->deck_filename.c_str());

	gnuplot_input_deck 
		<< "#!/usr/bin/env gnuplot --persist" << std::endl;
	
	gnuplot_input_deck << "set terminal " << \
		this->terminal_type.c_str() << " " << \
		this->terminal_option.c_str() << " " << \
		this->font_option.c_str() << std::endl;
	
	gnuplot_input_deck << "set datafile separator "
		<< this->datafile_separator.c_str() << std::endl;

	gnuplot_input_deck << "" << std::endl;

	gnuplot_input_deck << "IN_FILENAME = " << \
		"\"" << this->input_filename.c_str() << "\"" << std::endl;

	gnuplot_input_deck << "OUT_FILENAME = " << \
		"\"" << this->output_filename.c_str() << "\"" << std::endl;
	gnuplot_input_deck << std::endl;

	gnuplot_input_deck << "set output OUT_FILENAME" << std::endl;

	gnuplot_input_deck << "set title " << "\"" << \
		this->figure_title << "\"" << std::endl;

	gnuplot_input_deck << "set xrange " << "[" << \
		this->dtostdstr(this->x_range_left).c_str() << ":" << \
		this->dtostdstr(this->x_range_right).c_str() << "]" << std::endl;
	gnuplot_input_deck << "set yrange " << "[" << \
		this->dtostdstr(this->y_range_bottom).c_str() << ":" << \
		this->dtostdstr(this->y_range_top).c_str() << "]" << std::endl;
	gnuplot_input_deck << "set xlabel " << "\"X Dimension\"" << std::endl;
	gnuplot_input_deck << "set ylabel " << "\"Y Dimension\"" << std::endl;
	gnuplot_input_deck << "set y2label " << "\"Time (sec.)\"" << std::endl;
	gnuplot_input_deck << "show margin" << std::endl;
	gnuplot_input_deck << "set key top left" << std::endl;
	gnuplot_input_deck << std::endl;
	gnuplot_input_deck << "set samples " << \
		this->dtostdstr(std::abs(this->x_range_left)).c_str() \
		<< "+" << \
		this->dtostdstr(std::abs(this->x_range_right)).c_str() \
		<< "+1" << std::endl;
	gnuplot_input_deck << "set isosamples " << \
		this->dtostdstr(std::abs(this->y_range_top)).c_str() \
		<< "+" << \
		this->dtostdstr(std::abs(this->y_range_bottom)).c_str() \
		<< "+1" \
		<< std::endl;

	gnuplot_input_deck << "plot IN_FILENAME using " << \
		"($1):($2):($3):($4):($7) " << \
		"w vectors head empty size 0.030,15,135 " <<
		"lc palette ";

	gnuplot_input_deck << "notitle ";

	/*
	if (this->figure_title != "")
		gnuplot_input_deck << "title " << "\"" << this->figure_title \
			<< "\"";
	else
		gnuplot_input_deck << "notitle ";
	*/

	gnuplot_input_deck << std::endl;
	gnuplot_input_deck << std::endl;

	gnuplot_input_deck.close();
	return true;
}

// double to std::string converter.
std::string GnuplotGen::dtostdstr(double innum)
{
	std::ostringstream out_sstream;
	if (!(out_sstream << innum)) {
		throw std::invalid_argument(
			"Bad Double to String stream Conversion!!");
		return "";
	}
	return out_sstream.str();
}



///////////////////////////////////
// Constructors and Destructors  //
///////////////////////////////////

GnuplotGen::GnuplotGen()
	: deck_filename("plot.gp")
	, input_filename("trace.csv")
	, output_filename("trace.eps")
	, terminal_type("postscript eps")
	, terminal_option("enhanced color")
	, font_option("\"Helvetica\" 24")
	, datafile_separator("\",\"")
	, figure_title("")
	, x_range_left(-1000), x_range_right(1000)
	, y_range_top(1000), y_range_bottom(-1000)
{
	this->set_dimension_rect(-1000,1000,1000,-1000);
}

GnuplotGen::GnuplotGen(
	std::string input_deck_filename, \
	std::string data_filename, \
	std::string fig_filename)
	: deck_filename(input_deck_filename)
	, input_filename(data_filename)
	, output_filename(fig_filename)
	, terminal_type("postscript eps")
	, terminal_option("enhanced color")
	, font_option("\"Helvetica\" 24")
	, datafile_separator("\",\"")
	, figure_title("")
	, x_range_left(-1000), x_range_right(1000)
	, y_range_top(1000), y_range_bottom(-1000)
{
}

GnuplotGen::GnuplotGen(std::string input_deck_filename, \
	std::string data_filename, \
	std::string fig_filename, \
	double left, double right, double top, double bottom)
	: deck_filename(input_deck_filename)
	, input_filename(data_filename)
	, output_filename(fig_filename)
	, terminal_type("postscript eps")
	, terminal_option("enhanced color")
	, font_option("\"Helvetica\" 24")
	, datafile_separator("\",\"")
	, figure_title("")
	, x_range_left(left), x_range_right(right)
	, y_range_top(top), y_range_bottom(bottom)
{
}

GnuplotGen::GnuplotGen(std::string input_deck_filename, \
	std::string data_filename, \
	std::string fig_filename, \
	std::string fig_title, \
	double left, double right, double top, double bottom)
	: deck_filename(input_deck_filename)
	, input_filename(data_filename)
	, output_filename(fig_filename)
	, terminal_type("postscript eps")
	, terminal_option("enhanced color")
	, font_option("\"Helvetica\" 24")
	, datafile_separator("\",\"")
	, figure_title(fig_title)
	, x_range_left(left), x_range_right(right)
	, y_range_top(top), y_range_bottom(bottom)
{
}

GnuplotGen::~GnuplotGen()
{
}

#endif