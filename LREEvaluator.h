#ifndef LRE_GIT_LREEVALUATOR_H_
#define LRE_GIT_LREEVALUATOR_H_

#include "dlre.h"
#include "dlref.h"
#include "dlreg.h"
#include "dlrep.h"
#include "stateval.h"
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <fstream>
#include <float.h>

/**
* Wraps LRE functionality into object.
*/
class LREEvaluator {
private:
    /** DLRE inside a smart pointer. Needs to be a copyable pointer. */
    boost::shared_ptr<wns::evaluation::statistics::DLRE> evaluator;
    double last_x_level = -DBL_MAX;

    /** Common initialization for all constructors. */
    void init(int type, double xMin, double xMax, double intSize, double error,
                    double preFirst, double gMin,
                    bool forceRMinusAOk, int maxNrv, int skipInterval) {
        std::string name, description;
        wns::evaluation::statistics::StatEval::formatType format;
        switch (type) {
            case 1:
                name = "DLREF";
                description = "";
                evaluator.reset(new wns::evaluation::statistics::DLREF(
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
                name = "DLREG";
                description = "Equidistant";
                evaluator.reset(new wns::evaluation::statistics::DLREG(
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
            case 3:
                name = "DLREP";
                description = "Probability Function";
                evaluator.reset(new wns::evaluation::statistics::DLREP(
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
                std::cerr << "Invalid evaluator type: " << type << std::endl;
                exit(-1);
        }        
    }

public:
  static const int   TYPE_DLRE = 0,
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

    ~LREEvaluator() {
        printResult();
    }

    /** Puts new variable to probe. */
    void put(double value) {
        this->evaluator->put(value);
    }

    void printResult(std::string filename = "lre_output.txt") {
        std::ostream &stream = std::cout;
        this->evaluator->print(stream);

        std::ofstream file_stream;
        file_stream.open(filename);
        this->evaluator->print(file_stream);
        file_stream.close();
    }

    void printSnapshot(bool print_to_file, std::string filename = "lre_progress.txt") {
        std::ostream &stream = std::cout;
        this->evaluator->print(stream);

        if (print_to_file) {
            std::ofstream file_stream;
            file_stream.open(filename.c_str());
            this->evaluator->print(file_stream);
            file_stream.close();
        }
    }
};

#endif /* LRE_GIT_LREEVALUATOR_H_ */
