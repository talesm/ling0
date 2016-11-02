/*
 * Parser.cpp
 *
 *  Created on: 2 de nov de 2016
 *      Author: talesm
 */

#include "Parser.h"

#include <boost/spirit/home/x3.hpp>
#include <iterator>
#include <vector>

namespace ling0 {

Parser::Parser(std::istream &in) :
		in(in) {

}

Parser::~Parser() {
}

bool Parser::parseAll() {
	std::string tempBuffer(std::istreambuf_iterator<char>(in),
			std::istreambuf_iterator<char> { });

	using namespace boost::spirit::x3;
	auto first = tempBuffer.begin();
	auto last = tempBuffer.end();
	auto program_id = +(alnum | '_');
	auto string_cte = lexeme['"' >> *(char_ - '"') >> '"'];
	auto command = "log" >> lit('(') >> string_cte >> ')' >> ';';
	auto program = "program" >> program_id >> ':' >> *command >> "end" >> ';';
	return phrase_parse(first, last, program, space) && first == last;
}

} /* namespace ling0 */