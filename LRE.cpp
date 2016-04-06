#include <Python.h>
#include <boost/python.hpp>
#include "stateval.cpp"
#include "dlre.cpp"
#include "dlref.cpp"
#include "dlreg.cpp"
#include "dlrep.cpp"
#include <boost/shared_ptr.hpp>
#include "boost/program_options.hpp"

const int   MODE_DLREF = 0,
            MODE_DLREG = 1,
            MODE_DLREP = 2;

/**
* Wraps LRE functionality so it can be passed to Python or used natively.
*/
struct Evaluator {
private:
    /** DLRE inside a smart pointer. Needs to be a copyable pointer. */
    boost::shared_ptr<DLRE> evaluator;

    /** Common initialization for all constructors. */
    void init(int mode, double xMin, double xMax, double intSize, double error,
                    double preFirst, double gMin,
                    bool forceRMinusAOk, int maxNrv, int skipInterval) {
        std::string name, description;
        wns::evaluation::statistics::StatEval::formatType format;
        switch (mode) {
            case MODE_DLREF:
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
            case MODE_DLREG:
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
            case MODE_DLREP:
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
                cerr << "Invalid evaluator mode: " << mode << endl;
                exit(-1);
        }
    }

public:
    /** Full constructor. */
    Evaluator(int mode, double xMin, double xMax, double intSize, double error,
                    double preFirst, double gMin,
                    bool forceRMinusAOk, int maxNrv, int skipInterval) {
            this->init(mode, xMin, xMax, intSize, error, preFirst, gMin, forceRMinusAOk, maxNrv,  skipInterval);
    }

    /** Convenience constructor for default values. */
    Evaluator(int mode, double xMin, double xMax){
        this->init(mode, xMin, xMax, 0.01, 0.05, 0.1, 1E-2, 0, 100000, 0);
    }

    /** Puts new variable to probe. */
    void put(double value) {
        this->evaluator->put(value);
    }

    void print() {
        std::ostream &stream = cout;
        this->evaluator->print(stream);
    }
};

/**
    Python wrapper.
*/
BOOST_PYTHON_MODULE(LRE) {
    boost::python::class_<Evaluator>("Evaluator",
        // Full constructor.
        boost::python::init<int, double, double, double, double, double, double, bool, int, int>())
        // Convenience constructor.
        .def(boost::python::init<int, double, double>())
        // Put value to proble.
        .def("put", &Evaluator::put)
        // Print results.
        .def("print", &Evaluator::print)
    ;
}

/**
* Parses command line parameters using boost::program_options.
*/
boost::program_options::variables_map parseCommandLine(int argc, char *argv[]) {
    namespace po = boost::program_options;
    po::options_description desc("LRE Options");
    desc.add_options()
        ("help", "This help message")
        ("file", po::value<string>(), "Input filename")
        ("xMin", po::value<int>(), "Minimum x value")
        ("xMax", po::value<int>(), "Maximum x value")
        ("intSize", po::value<double>()->default_value(0.01), "Interval size")
        ("error", po::value<double>()->default_value(0.05), "Maximum relative error")
        ("preFirst", po::value<double>()->default_value(0.1), "")
        ("forceRMinusAOk", po::value<int>()->default_value(0), "")
        ("gMin", po::value<double>()->default_value(1E-2), "")
        ("maxNrv", po::value<int>()->default_value(100000), "Maximum #samples")
        ("skipInterval", po::value<int>()->default_value(0), "")
        ("mode", po::value<int>()->default_value(2), "0 = DLREF\n1 = DLREG (Equidistant)\n2 = DLREP (Probability Function)")
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
        exit(-1);
    }
    if (!vm.count("xMin")) {
        cerr << "No xMin provided." << endl;
        exit(-1);
    }
    if (!vm.count("xMax")) {
        cerr << "No xMax provided." << endl;
        exit(-1);
    }
    if (vm["mode"].as<int>() < 0 || vm["mode"].as<int>() > 2) {
        cerr << "Invalid mode: " << vm["mode"].as<int>() << endl;
        exit(-1);
    }

    return vm;
}

int main(int argc, char *argv[]) {
    boost::program_options::variables_map vm = parseCommandLine(argc, argv);
    Evaluator evaluator(vm["mode"].as<int>(), vm["xMin"].as<int>(), vm["xMax"].as<int>(), vm["intSize"].as<double>(),
        vm["error"].as<double>(), vm["preFirst"].as<double>(), vm["gMin"].as<double>(), vm["forceRMinusAOk"].as<int>(),
        vm["maxNrv"].as<int>(), vm["skipInterval"].as<int>());
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
        cerr << "Error reading file: " << vm["file"].as<string>() << endl;
        return -1;
    }
    evaluator.print();
    return 0;
}
