#pragma once

#include <cstdio>
#include <vector>

class GnuplotHandler
{
  public:
    GnuplotHandler();
    ~GnuplotHandler();

    void plot_with_gnuplot_ppm(double peakValue);            
    void plot_fft(const std::vector<double> &frequencyBins); 
    void plot_discontinuities(const std::vector<int>& discontinuityIndices, int signalLength);

  private:
    FILE *gnuplotPipe;
};
