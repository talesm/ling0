/*
 * Context.cpp
 *
 *  Created on: 25 de out de 2016
 *      Author: Tales
 */

#include "VirtualMachine.h"

#include <boost/spirit/home/x3/char/char_class.hpp>
#include <boost/spirit/home/x3/numeric/real.hpp>
#include <boost/spirit/home/x3/operator/list.hpp>
#include <boost/variant/detail/apply_visitor_binary.hpp>
#include <boost/variant/detail/apply_visitor_delayed.hpp>
#include <boost/variant/detail/apply_visitor_unary.hpp>
#include <stdexcept>

#include "ast/Expression.h"

namespace ling0 {
using namespace ast;
using namespace std;
using namespace boost;

VirtualMachine::VirtualMachine(std::ostream &out) :
		out(out) {
}

VirtualMachine::~VirtualMachine() {
}

void VirtualMachine::runProgram(const ast::Program& program) {
	for (auto const &logStm : program.statements) {
		string::size_type first = 0;
		string::size_type last;
		auto &content = logStm.content;
		auto currentValue = logStm.parameters.begin();
		while((last = content.find("{}", first)) != string::npos){
			out << content.substr(first, last) << solve(*currentValue++);
			first = last+2;
		}
		out << content.substr(first) << endl;
	}
}

double VirtualMachine::solve(const ast::Expression& expression) {
	return apply_visitor(*this, expression);
}

double VirtualMachine::operator ()(const double& value) {
	return value;
}

double VirtualMachine::operator ()(const ast::BinExpression& value) {
	switch (value.operation) {
		case BinOperation::ADD:
			return solve(value.left) + solve(value.right);
		case BinOperation::SUB:
			return solve(value.left) - solve(value.right);
	}
	throw std::runtime_error("Invalid Opcode");
}

} /* namespace ling0 */

