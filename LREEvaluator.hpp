#ifndef LREEVALUATOR_HPP
#define LREEVALUATOR_HPP

#include "dlre.cpp"
#include "dlref.cpp"
#include "dlreg.cpp"
#include "dlrep.cpp"

/**
* Wraps LRE functionality into object.
*/
class LREEvaluator {
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
  const int   TYPE_DLRE = 0,
              TYPE_DLREF = 1,
              TYPE_DLREG = 2,
              TYPE_DLREP = 3;

    /** Full constructor. */
    LREEvaluator(int type, double xMin, double xMax, double intSize, double error,
                    double preFirst, double gMin,
                    bool forceRMinusAOk, int maxNrv, int skipInterval) {
            this->init(type, xMin, xMax, intSize, error, preFirst, gMin, forceRMinusAOk, maxNrv,  skipInterval);
    }

    /** Convenience constructor for default values. */
    LREEvaluator(int type, double xMin, double xMax){
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

#endif
