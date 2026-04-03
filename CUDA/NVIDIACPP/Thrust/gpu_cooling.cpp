#include <algorithm>   // for std::transform
#include <cstdio>      // for printf
#include <vector>      // for std::vector

int main() {

    // k = rate constant (how fast temperature moves toward ambient)
    float k = 0.5;

    // Ambient/environment temperature
    float ambient_temp = 20;

    // Initial temperatures of 3 objects
    std::vector<float> temp{ 42, 24, 50 };

    // Lambda function defining the transformation rule
    // [=] → capture all external variables (k, ambient_temp) by value
    // (float temp) → input is a single element from the vector
    // return → new temperature after applying cooling formula
    auto transformation = [=](float temp) {
        // Formula:
        // new_temp = current_temp + k * (ambient_temp - current_temp)
        // Moves temperature toward ambient temperature
        return temp + k * (ambient_temp - temp);
    };

    // Print table header
    std::printf("step  temp[0]  temp[1]  temp[2]\n");

    // Perform multiple simulation steps
    for (int step = 0; step < 3; step++) {

        // Core operation:
        // std::transform applies the lambda to each element of the vector
        //
        // Arguments:
        // temp.begin() → start of input range
        // temp.end()   → end of input range
        // temp.begin() → where to store output (same vector → in-place update)
        // transformation → function applied to each element
        //
        // Effect:
        // temp[i] = transformation(temp[i]) for all i
        std::transform(temp.begin(), temp.end(), temp.begin(), transformation);

        // Print current step and updated temperatures
        std::printf("%d     %.2f    %.2f    %.2f\n",
                    step, temp[0], temp[1], temp[2]);
    }

    return 0;
}