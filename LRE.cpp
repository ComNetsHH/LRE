#include <Python.h>
#include <boost/python.hpp>
#include "stateval.cpp"
#include "dlre.cpp"
#include "dlref.cpp"
#include "dlreg.cpp"
#include "dlrep.cpp"
#include <boost/shared_ptr.hpp>
#include "boost/program_options.hpp"

const int   TYPE_DLRE = 0,
            TYPE_DLREF = 1,
            TYPE_DLREG = 2,
            TYPE_DLREP = 3;

/**
* Wraps LRE functionality so it can be passed to Python or used natively.
*/
struct Evaluator {
private:
    /** DLRE inside a smart pointer. Needs to be a copyable pointer. */
    boost::shared_ptr<DLRE> evaluator;

    /** Common initialization for all constructors. */
    void init(int type, double xMin, double xMax, double intSize, double error,
                    double preFirst, double gMin,
                    bool forceRMinusAOk, int maxNrv, int skipInterval) {
        std::string name, description;
        wns::evaluation::statistics::StatEval::formatType format;
        switch (type) {
            case 0:
                name = "DLREF";
                description = "";
                evaluator.reset(new DLREF(
                                xMin,
                                xMax,
                                intSize,
                                error,
                                preFirst,
                                name,
                                description,
                                forceRMinusAOk,
                                gMin,
                                maxNrv,
                                skipInterval,
                                format
                ));
                break;
            case 1:
                name = "DLREG";
                description = "Equidistant";
                evaluator.reset(new DLREG(
                                xMin,
                                xMax,
                                intSize,
                                error,
                                preFirst,
                                name,
                                description,
                                forceRMinusAOk,
                                gMin,
                                maxNrv,
                                skipInterval,
                                format
                ));
                break;
            case 2:
                name = "DLREP";
                description = "Probability Function";
                evaluator.reset(new DLREP(
                                xMin,
                                xMax,
                                intSize,
                                error,
                                preFirst,
                                name,
                                description,
                                forceRMinusAOk,
                                // gMin,
                                maxNrv,
                                skipInterval,
                                format
                ));
                break;
            default:
                cerr << "Invalid evaluator type: " << type << endl;
                exit(-1);
        }
    }

public:
    /** Full constructor. */
    Evaluator(int type, double xMin, double xMax, double intSize, double error,
                    double preFirst, double gMin,
                    bool forceRMinusAOk, int maxNrv, int skipInterval) {
            this->init(type, xMin, xMax, intSize, error, preFirst, gMin, forceRMinusAOk, maxNrv,  skipInterval);
    }

    /** Convenience constructor for default values. */
    Evaluator(int type, double xMin, double xMax){
        this->init(type, xMin, xMax, 0.01, 0.05, 0.1, 1E-2, 0, 100000, 0);
    }

    /** Puts new variable to probe. */
    void put(double value) {
        this->evaluator->put(value);
    }

    void printResult() {
        std::ostream &stream = cout;
        this->evaluator->print(stream);
    }
};

/**
    Python wrapper.
*/
BOOST_PYTHON_MODULE(LRE) {
    // init creates a Python constructor.
    boost::python::class_<Evaluator>("Evaluator",
        boost::python::init<int, double, double, double, double, double, double, bool, int, int>())
        .def(boost::python::init<int, double, double>())
        .def("put", &Evaluator::put)
        .def("printResult", &Evaluator::printResult)
    ;
}

boost::program_options::variables_map parseCommandLine(int argc, char *argv[]) {
    namespace po = boost::program_options;
    po::options_description desc("LRE Options");
    desc.add_options()
        ("help", "This help message")
        ("file", po::value<string>(), "Input filename")
        ("xMin", po::value<int>(), "Minimum value on x-axis")
        ("xMax", po::value<int>(), "Maximum value on x-axis")
        ("intSize", po::value<double>(), "Size of each bin on x-axis")
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
        cout << desc << "\n";
        exit(0);
    }

    if (!vm.count("file")) {
        cerr << "No input filename." << endl;
        cout << desc << "\n";
        exit(-1);
    }
    if (!vm.count("xMin")) {
        cerr << "No xMin provided." << endl;
        cout << desc << "\n";
        exit(-1);
    }
    if (!vm.count("xMax")) {
        cerr << "No xMax provided." << endl;
        cout << desc << "\n";
        exit(-1);
    }
    if (vm["type"].as<int>() < 0 || vm["type"].as<int>() > 2) {
        cerr << "Invalid type: " << vm["type"].as<int>() << endl;
        cout << desc << "\n";
        exit(-1);
    }

    return vm;
}

int main(int argc, char *argv[]) {
    boost::program_options::variables_map vm = parseCommandLine(argc, argv);
    Evaluator evaluator(vm["type"].as<int>(), vm["xMin"].as<int>(), vm["xMax"].as<int>(), vm["intSize"].as<double>(),
        vm["error"].as<double>(), vm["preFirst"].as<double>(), vm["gMin"].as<double>(), vm["forceRMinusAOk"].as<int>(), vm["maxNrv"].as<int>(), vm["skipInterval"].as<int>());
    FILE* file = fopen(vm["file"].as<string>().c_str(), "r");
    if (file != NULL) {
         char str[256];
         double num;
         while(fgets(str, 256, file) != NULL){
            num = std::stod(str);
            evaluator.put(num);
        }
        fclose(file);
    } else {
        cerr << "File couldn't be found." << endl;
        return -1;
    }
    evaluator.printResult();
    return 0;
}
