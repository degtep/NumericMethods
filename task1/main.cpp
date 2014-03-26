#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "timer.h"
#include "quicksort.h"
#include "combinedrand.h"
#include "kmerge.h"
#include "bigfloat.h"

const size_t NUMBERS_COUNT = 1e9;
const size_t NUMBERS_PART_SIZE = 1e8;
const size_t NUMBERS_PARTS_COUNT = NUMBERS_COUNT / NUMBERS_PART_SIZE;
const size_t NUMBERS_SKIP_COUNT = 1e7;
const size_t PRECISION = 50;
const std::string FILENAME_PREFIX = "datapart.";
const std::string OUTPUT_FILENAME = "output.txt";

template <typename T>
std::string numberToString(T Number)
{
    std::ostringstream ss;
    ss << Number;
    return ss.str();
}

void randAndSortParts()
{
    std::vector<int32_t> *numbers = new std::vector<int32_t>();
    CombinedRand rand;

    for(size_t i = 0; i < NUMBERS_PARTS_COUNT; ++i)
    {
        numbers->clear();
        numbers->reserve(NUMBERS_PART_SIZE);
        std::cerr << "Part " << numberToString(i) << " - " << NUMBERS_PART_SIZE << " numbers.\n";
        std::cerr << "Generating random numbers...";
        startTimer();
        for (size_t j = 0; j < NUMBERS_PART_SIZE; ++j)
        {
            numbers->push_back(rand.Next());
        }
        stopTimer();
        std::cerr << "done. It took " << getLastTimerDuration() << " seconds.\n";

        std::cerr << "QuickSorting random numbers...";
        startTimer();
        quickSort(*numbers);
        stopTimer();
        std::cerr << "done. It took " << getLastTimerDuration() << " seconds.\n";

        std::string fileName = FILENAME_PREFIX + numberToString(i);
        std::cerr << "Saving numbers to \"" << fileName << "\"...";
        std::ofstream os (fileName.c_str());
        os.write((const char*)&(*numbers)[0], NUMBERS_PART_SIZE * sizeof(int32_t));
        os.close();
        std::cerr << "done.\n";
    }
    delete numbers;
}

void mergeAllPartsAndSample(std::vector<int32_t> &sortedSamples)
{
    std::vector<std::ifstream*> ifstreams;
    for(size_t i = 0; i < NUMBERS_PARTS_COUNT; ++i)
    {
        std::string fileName = FILENAME_PREFIX + numberToString(i);
        std::ifstream *ifstream = new std::ifstream(fileName.c_str());
        ifstreams.push_back(ifstream);
    }

    std::cerr << "MegreSorting all numbers and sample...";
    startTimer();
    kmergeAndSample(ifstreams, 1, NUMBERS_SKIP_COUNT, sortedSamples);
    stopTimer();
    std::cerr << "done. It took " << getLastTimerDuration() << " seconds.\n";

    for(size_t i = 0; i < ifstreams.size(); ++i)
    {
        ifstreams[i]->close();
        delete ifstreams[i];
        std::string fileName = FILENAME_PREFIX + numberToString(i);
        remove(fileName.c_str());
    }
}

void inverseAndPrintSamples(std::vector<int32_t> &sortedSamples)
{
    std::ofstream ostream(OUTPUT_FILENAME.c_str());
    std::cerr << "Inverse sample numbers and print to file \"" << OUTPUT_FILENAME  << "\"...";
    for(int i = sortedSamples.size() - 1; i >=0 ; --i)
    {
        BigFloat<PRECISION> number(1);
        number /= BigFloat<PRECISION>(sortedSamples[i]);
        number.print(ostream);
        ostream << std::endl;
    }
    std::cerr << "done.\n";
}

int main()
{
    randAndSortParts();
    std::vector<int32_t> sortedSamples;
    mergeAllPartsAndSample(sortedSamples);
    inverseAndPrintSamples(sortedSamples);
    return 0;
}
