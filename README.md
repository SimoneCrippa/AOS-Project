# Floating Point Predictability

## Repository structure:

* ### benchmarks_results folder:
  * We have a folder for each platform/processor. The naming convention is: `platform_name_architecture`. Benchmarks done on `armv7l` and `armv8a` also specify if the benchmark is done on the big cluster or on the LITTLE cluster.  
  Benchmarks done on `freescale` and `odriod` boards also specify if the benchmarks are done with [`stress-ng`](https://kernel.ubuntu.com/~cking/stress-ng/) running on a specific cluster.  
  The command used to stress the CPU is `taskset -c cpu_list stress-ng --cache N` where `N` is the number of worker threads (we used a thread for each core) and the benchmark is executed with the command `taskset -c cpu_list` where `cpu_list` is the interval of CPU selected to run the benchmark.
  * Every folder contain 12 `.txt` files, 2 for each benchmark (one is fixed point the other one is floating).  
    The naming convention is: `name_of_the_benchmark_[floating|fixed]_results.txt`.
  * Each `.txt` file contains 100.000 measurements.
* ### benchmarks folder:
  * We have a folder for each benchmark, each one contains 4 or 6 `.c` files. If the benchmark uses multiplications or divisions we have 6 files, otherwise we have 4 files.
  * `name_of_the_benchmark_source.c` is the source version of the benchmark available at: [mrtc.mdh.se](http://www.mrtc.mdh.se/projects/wcet/benchmarks.html).
  * `name_of_the_benchmark_fixed.c` is the fixed point version of the benchmark.
  * `name_of_the_benchmark_bench_floating.c` is the source benchmark modified with a main and a support function to obtain the measurements.
  * `name_of_the_benchmark_bench_fixed.c` is the fixed point version of the benchmark modified with a main and a support function to obtain the measurements.
  * Additionally we can have `fixed_op_64bit.c` and `fixed_op_64bit.h` only for benchmarks with divisions or multiplications.
* ### report folder:
  * It contains the LaTeX report of the project and its related files.
* ### statistics folder:
  * `metrics.py`, given the benchmarks results folder it computes 8 metrics for each `.txt` file:
    * Minimum
    * Maximum
    * Average
    * Variance
    * Standard deviation
    * KPSS
    * BDS
    * Hurst exponent (with `kind=change`)
  * `statistics_tests.py`, script to compute test statistics and check Hurst Exponent values:
    * KPSS upper-tailed test
    * BDS double-tailed test
    * Hurst exponent time series relation (auto-correlated, etc.)
  * `statistics_results.txt`, `tests_results.txt`
    * output files, the first generated by `metrics.py` and required for `statistics_tests.py`, the other one generated by `statistics_tests.py` 
* ### `makefile`:
  * The Makefile is useful to automatically compile and run the benchmarks, we have 4 available commands:
    * `make init` to generate the folders to contain the results and the executables.
    * `make` to automatically compile all the benchmarks.
    * `make run` to automatically execute all the benchmarks.
    * `make clean` to delete builds and results folders and restore the initial situation.
* ### `fixed_op_64bit.h` and `fixed_op_64bit.c`:
  * Header file and support library for multiplications and divisions in fixed point. Based on architecture, we use a 128-bit or a 64-bit multiplication and division, in both cases we have the same precision.
* ### `fixed_point_example.c`:
  * A simple example which shows how the operations in fixed point are done.