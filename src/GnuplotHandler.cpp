#include "GnuplotHandler.hpp"
#include <iostream>
GnuplotHandler::GnuplotHandler()
{
    gnuplotPipe = popen("gnuplot -persistent", "w");
    if (!gnuplotPipe)
    {
        std::cerr << "Error: Could not open pipe to Gnuplot.\n";
    }
    else
    {
         
    fprintf(gnuplotPipe, "set terminal wxt\n");
    fflush(gnuplotPipe);

    }
}

GnuplotHandler::~GnuplotHandler()
{
    if (gnuplotPipe)
    {
        pclose(gnuplotPipe);
    }
}

void GnuplotHandler::plot_with_gnuplot_ppm(double peakValue)
{
    if (!gnuplotPipe)
    {
        std::cerr << "Error: Gnuplot pipe is not open.\n";
        return;
    }

    const char *barColor = "green";
    if (peakValue >= 5)
    {
        barColor = "red";
    }
    else if (peakValue >= -10)
    {
        barColor = "orange";
    }
    fprintf(gnuplotPipe, "set terminal wxt\n");
    fprintf(gnuplotPipe, "set style fill solid 1.0 border -1\n");
    fprintf(gnuplotPipe, "set xrange [-0.5:0.5]\n");
    fprintf(gnuplotPipe, "set yrange [-60:10]\n");
    fprintf(gnuplotPipe, "set ylabel 'dB'\n");
    fprintf(gnuplotPipe, "set border 3\n");
    fprintf(gnuplotPipe, "set noxtics\n");
    fprintf(gnuplotPipe, "set ytics ('+5' 5, '0' 0, '-10' -10, '-20' -20, '-30' -30, '-40' -40, "
                         "'-50' -50) font ',10'\n");

    fprintf(gnuplotPipe, "plot '-' using 1:2 with boxes lc rgb '%s' notitle\n", barColor);
    fprintf(gnuplotPipe, "0 %f\n", peakValue);
    fprintf(gnuplotPipe, "e\n");
    fflush(gnuplotPipe);
}

void GnuplotHandler::plot_fft(const std::vector<double> &frequencyBins)
{
    if (!gnuplotPipe)
        return;
    fprintf(gnuplotPipe, "set terminal wxt\n");
    fprintf(gnuplotPipe, "set style fill solid 1.0 border -1\n");
    fprintf(gnuplotPipe, "set xrange [0:%lu]\n", frequencyBins.size());
    fprintf(gnuplotPipe, "set yrange [0:*]\n");
    fprintf(gnuplotPipe, "set title 'FFT Frequency Spectrum'\n");
    fprintf(gnuplotPipe, "plot '-' with lines title 'FFT Magnitude'\n");

    for (size_t i = 0; i < frequencyBins.size(); ++i)
    {
        fprintf(gnuplotPipe, "%lu %f\n", i, frequencyBins[i]);
    }
    fprintf(gnuplotPipe, "e\n"); // End of data
    fflush(gnuplotPipe);         // Ensure the data is sent to Gnuplot
}

void GnuplotHandler::plot_discontinuities(const std::vector<int>& discontinuityIndices, int signalLength)
{
    if (!gnuplotPipe)
    {
        std::cerr << "Error: Gnuplot pipe is not open.\n";
        return;
    }
    fprintf(gnuplotPipe, "set terminal wxt\n");
    fprintf(gnuplotPipe, "set style line 1 lc rgb 'red' lw 2\n");
    fprintf(gnuplotPipe, "set xrange [0:%d]\n", signalLength);
    fprintf(gnuplotPipe, "set yrange [0:1]\n"); // Arbitrary range for visualizing discontinuities
    fprintf(gnuplotPipe, "set title 'Discontinuity Detection'\n");
    fprintf(gnuplotPipe, "plot '-' using 1:2 with linespoints ls 1 notitle\n");

    // Plot vertical lines or markers at each discontinuity index
    for (const auto& index : discontinuityIndices)
    {
        fprintf(gnuplotPipe, "%d 1\n", index);  // Mark the position of the discontinuity
        fprintf(gnuplotPipe, "%d 0\n", index);  // Draw a vertical line for visualization
    }

    fprintf(gnuplotPipe, "e\n");  // End of data
    fflush(gnuplotPipe);          // Ensure the data is sent to Gnuplot
}