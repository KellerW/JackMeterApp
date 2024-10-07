#include <iostream>
#include <iomanip>
#include <cmath>  // For std::isinf()
#include <thread>
#include <chrono>

// Map dB value to meter scale (0 to MAX_LEVEL)
int map_db_to_level(float dB, int MAX_LEVEL) {
    float max_dB = 5.0f;    // Max dB value corresponding to top of the meter
    float min_dB = -50.0f;  // Min dB value corresponding to bottom of the meter

    // If the value is -inf (silent), return 0 to indicate no bar
    if (std::isinf(dB)) return 0;

    // Clamp dB values within the range of -50 dB to +5 dB
    if (dB > max_dB) dB = max_dB;
    if (dB < min_dB) dB = min_dB;

    // Scale dB values to match the meter's height (MAX_LEVEL)
    return (int)((dB - min_dB) / (max_dB - min_dB) * MAX_LEVEL);
}

// Function to print a vertical meter in the terminal
void print_meter(int level, int max_level, const std::string& label) {
    std::cout << label << " ";

    // Print the vertical bar based on the level
    for (int i = 0; i < max_level; i++) {
        if (i < level) {
            std::cout << "|";  // Bar for each level
        } else {
            std::cout << " ";  // Space for empty parts of the bar
        }
    }

    std::cout << "\n";
}

// Function to print the PPM meter horizontally for both channels
void draw_ppm_meter(float left_ppm, float right_ppm, float left_peak, float right_peak, int MAX_LEVEL) {
    int left_level = map_db_to_level(left_ppm, MAX_LEVEL);
    int right_level = map_db_to_level(right_ppm, MAX_LEVEL);
    int left_peak_level = map_db_to_level(left_peak, MAX_LEVEL);
    int right_peak_level = map_db_to_level(right_peak, MAX_LEVEL);

    // Clear screen
    std::cout << "\033[2J\033[1;1H";  // Clear terminal and move cursor to top-left

    std::cout << "PPM Meter:\n";
    std::cout << "+5 dB |----------------------|\n";
    print_meter(left_level, MAX_LEVEL, "Left ");
    print_meter(right_level, MAX_LEVEL, "Right");
    std::cout << "-50 dB|----------------------|\n";

    std::cout << "Peak Hold:\n";
    std::cout << "Left Peak  : " << std::setw(6) << left_peak << " dB\n";
    std::cout << "Right Peak : " << std::setw(6) << right_peak << " dB\n";
}

int main() {
    int MAX_LEVEL = 25;  // Define the height of the PPM meter

    float left_ppm = -5.0f;
    float right_ppm = -3.0f;
    float left_peak = -3.0f;
    float right_peak = -2.5f;

    while (true) {
        // Simulate some variation in PPM levels (you can replace this with actual audio data)
        left_ppm -= 0.5f;
        right_ppm -= 0.3f;

        if (left_ppm < -50.0f) left_ppm = -5.0f;
        if (right_ppm < -50.0f) right_ppm = -3.0f;

        // Keep the peak levels updated
        if (left_ppm > left_peak) left_peak = left_ppm;
        if (right_ppm > right_peak) right_peak = right_ppm;

        // Draw the PPM meter
        draw_ppm_meter(left_ppm, right_ppm, left_peak, right_peak, MAX_LEVEL);

        // Sleep for 500ms to simulate real-time update
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;
}
