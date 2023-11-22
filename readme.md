# Introduction to Parallel Computing (A.Y. 2023-2024) - Assignment 2

This repo contains the solution of the [second assignment](homework2.pdf) of the course "Introduction to Parallel 
Computing - prof. Vella" from the University of Trento.

## Build and run
### On the HPC cluster (preferred)

Clone this repository on the University HPC cluster, and submit to the queue the job file with the following 
commands:

```shell
git clone https://github.com/aleiepure/Parallel-assignment2 ~/assignment2
cd ~/assignment2
sed -i "s/alessandro.iepure/$(whoami)/g" job.pbs  # change my username to the current user's
qsub job.pbs
```

The status of the execution can be checked with the command `qstat -u $USER`. An empty output means that no jobs are 
currently running for your user.

At the end of the job, the results of the computations will be available in `.CSV` files in the `out/` folder 
(created automatically). \
Errors abort the execution of the job immediately and the `stderr` is dumped into `out/stderr.e`.

### Locally
The `job.pbs` script can be used as is from in a shell. It depends on GCC (tested with 4.8 and 13.2 only) 
and includes the logic to not go above 8 CPUs cores (my machine's max).

If you prefer a complete manual approach compile both source files with `gcc -std=c99 -fopenmp </path/to/source.c>` and 
pass the required arguments:
- multiplication: `OMP_NUM_THREADS=<number of threads> /path/to/executable <dense|sparse> <A rows> <A columns> <B rows> <B columns> <block rows> <block columns>`
- transposition: `OMP_NUM_THREADS=<number of threads> /path/to/executable <Row size> <Column size> <Block row size> <Block column size>`

The outputs are in the following formats:
- multiplication: `serial_time, openMP_time`
- transposition: `serial_time, serial_blocks_time, openMP_time, openMP_blocks_time`

## Results
The report analyzing the results is available [here](report/build/report.pdf).
