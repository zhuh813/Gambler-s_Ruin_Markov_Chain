#include <random>
#include <pybind11/pybind11.h>

namespace py = pybind11;

py::tuple monte_carlo_sim(int i, int m, double p, int rounds) {
    std::mt19937 rng(1234); // 固定隨機種子以便比較
    std::uniform_real_distribution<> dist(0.0, 1.0);

    int ruin_count = 0;
    long total_steps = 0;
    double total_rtp = 0.0;

    for (int r = 0; r < rounds; ++r) {
        int state = i;
        int steps = 0;

        while (state > 0 && state < m) {
            if (dist(rng) < p)
                state++;
            else
                state--;
            steps++;
        }

        if (state == 0)
            ruin_count++;

        total_steps += steps;
        total_rtp += static_cast<double>(state) / m;
    }

    double ruin_rate = static_cast<double>(ruin_count) / rounds;
    double avg_steps = static_cast<double>(total_steps) / rounds;
    double avg_rtp = total_rtp / rounds;

    return py::make_tuple(ruin_rate, avg_steps, avg_rtp);
}

PYBIND11_MODULE(c_monte, m) {
    m.def("monte_carlo_sim", &monte_carlo_sim, "C++ Monte Carlo simulator with p");;
}
