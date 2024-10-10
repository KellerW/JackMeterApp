#include "AppContext.hpp"
#include "SimplePeakProcessor.hpp"
#include <ctime>
#include <cstdio>

void AppContext::log_status_plain()
{
    time_t rawtime;
    struct tm *ltime;
    time(&rawtime);
    ltime = localtime(&rawtime);

    printf("# jackmeter %04u-%02u-%02ut%02u:%02u:%02u\n", 1900 + ltime->tm_year, 1 + ltime->tm_mon,
           ltime->tm_mday, ltime->tm_hour, ltime->tm_min, ltime->tm_sec);

    for (const auto &processor : processors)
    {
        if (auto peakProcessor =
                std::dynamic_pointer_cast<jackmeter::SimplePeakProcessor>(processor))
        {
            printf("%s : %6.1f db  (min:%6.1f, max:%6.1f)\n", peakProcessor->GetName().data(),
                   peakProcessor->GetLatestPeakDb(), peakProcessor->GetMinPeakDb(),
                   peakProcessor->GetMaxPeakDb());
        }
        else
        {
            printf("%s : (Processor type not supported for detailed logging)\n",
                   processor->GetName().data());
        }
    }
    fflush(stdout);
}
