/*******************************************************************************
 * This file is part of ComNets LRE (Limited Reletive Error) Algorithm
 * _____________________________________________________________________________
 *
 * Copyright (C) 2017
 * Institute of Communication Networks (ComNets)
 * Am Schwarzenberg-Campus 3, D-21073 Hamburg, Germany
 * phone: ++49-40-42878-3249,
 * fax: ++49-40-42878-2941
 * email: ComNets@tuhh.de
 * www: http://www.tuhh.de/comnets
 *
_____________________________________________________________________________
 *
 * ComNets LRE is free software; you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License version 2.1 as published by the
 * Free Software Foundation;
 *
 * ComNets LRE is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************/


#include <Python.h>
#include <boost/python.hpp>
#include "boost/program_options.hpp"
#include "../LREEvaluator.h"

/**
    Python wrapper.
*/
BOOST_PYTHON_MODULE(LRE) {
    // init creates a Python constructor.
    boost::python::class_<LREEvaluator>("LREEvaluator",
        boost::python::init<int, double, double, double, double, double, double, bool, int, int>())
        .def(boost::python::init<int, double, double>())
        .def("put", &LREEvaluator::put)
        .def("printResult", &LREEvaluator::printResult)
    ;
}

boost::program_options::variables_map parseCommandLine(int argc, char *argv[]) {
    namespace po = boost::program_options;
    po::options_description desc("LRE Options");
    desc.add_options()
        ("help", "This help message")
        ("file", po::value<std::string>(), "Input filename")
        ("xMin", po::value<double>(), "Minimum value on x-axis")
        ("xMax", po::value<double>(), "Maximum value on x-axis")
        ("intSize", po::value<double>()->default_value(1.0), "Size of each bin on x-axis")
        ("error", po::value<double>()->default_value(0.05), "Maximum allowed relative error")
        ("preFirst", po::value<double>()->default_value(0.0), "Initial state-value on x-axis (usually has no influence)")
        ("forceRMinusAOk", po::value<int>()->default_value(0), "Enforce Large Sample Condition. If disabled still 10 transitions per bin will be required for confident result.")
        ("gMin", po::value<double>()->default_value(1E-5), "Minimum value on y-axis determined with less than the maximum allowed relative error (intended reliability)")
        ("maxNrv", po::value<int>()->default_value(100000000), "Maximum number of samples to be considered (after this evaluation will stop and each bin will be evaluated if result is confident)")
        ("skipInterval", po::value<int>()->default_value(0), "Evaluating if result is confident for each bin is expensive, if >0 evaluation will only be done every n samples")
        ("type", po::value<int>()->default_value(2), "0 = DLREF (CDF)\n1 = DLREG (CCDF)\n2 = DLREP (PDF)")
    ;


    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        exit(0);
    }

    if (!vm.count("file")) {
        std::cerr << "No input filename." << std::endl;
        std::cout << desc << "\n";
        exit(-1);
    }
    if (!vm.count("xMin")) {
        std::cerr << "No xMin provided." << std::endl;
        std::cout << desc << "\n";
        exit(-1);
    }
    if (!vm.count("xMax")) {
        std::cerr << "No xMax provided." << std::endl;
        std::cout << desc << "\n";
        exit(-1);
    }
    if (vm["type"].as<int>() < 0 || vm["type"].as<int>() > 2) {
        std::cerr << "Invalid type: " << vm["type"].as<int>() << std::endl;
        std::cout << desc << "\n";
        exit(-1);
    }

    return vm;
}

int main(int argc, char *argv[]) {
    boost::program_options::variables_map vm = parseCommandLine(argc, argv);
    LREEvaluator evaluator(vm["type"].as<int>(), vm["xMin"].as<double>(), vm["xMax"].as<double>(), vm["intSize"].as<double>(),
        vm["error"].as<double>(), vm["preFirst"].as<double>(), vm["gMin"].as<double>(), vm["forceRMinusAOk"].as<int>(), vm["maxNrv"].as<int>(), vm["skipInterval"].as<int>());
    FILE* file = fopen(vm["file"].as<std::string>().c_str(), "r");
    if (file != NULL) {
         char str[256];
         double num;
         while(fgets(str, 256, file) != NULL){
            num = std::stod(str);
            evaluator.put(num);
        }
        fclose(file);
    } else {
        std::cerr << "File couldn't be found." << std::endl;
        return -1;
    }
    evaluator.printResult();
    return 0;
}

