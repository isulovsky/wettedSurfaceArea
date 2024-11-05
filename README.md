This is a simple function object that calculates the wetted surface of a floating object and prints the value in each time step. The wetted surface is simply calculated as the surface area  of cell faces having the value of the phase field fraction 0.5 or less. The threshold of 0.5 can also be adjusted. The wetted surface is a useful metric for tracking overall convergence. Also, it could serve as a benchmark value for comparing different interface tracking methods.

![git](https://github.com/user-attachments/assets/c87b6565-e5f4-4422-992c-208a6c49b6c6)
