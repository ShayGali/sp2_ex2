#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"
using namespace doctest;

const int MIN_TESTS = 20;

int return_code = -1;

struct ReporterCounter : public ConsoleReporter {
    ReporterCounter(const ContextOptions &input_options) : ConsoleReporter(input_options) {}

    void test_run_end(const TestRunStats &run_stats) override {
        if (run_stats.numAsserts >= MIN_TESTS) {
            return_code = 0;
        } else {
            std::cout << "Please write at least " << MIN_TESTS << " tests! " << std::endl;
            return_code = 1;
        }
        // Print summary
        std::cout << "\nTest run summary:\n";
        std::cout << "Number of assertions: " << run_stats.numAsserts << "\n";
        std::cout << "Number of failed assertions: " << run_stats.numAssertsFailed << "\n";
        std::cout << "Number of test cases passed: " << run_stats.numTestCases - run_stats.numTestCasesFailed << "\n";
        std::cout << "Number of test cases failed: " << run_stats.numTestCasesFailed << "\n";
    }
};

REGISTER_REPORTER("counter", 1, ReporterCounter);

int main(int argc, char **argv) {
    Context context;
    context.addFilter("reporters", "counter");
    context.run();
    return return_code;
}