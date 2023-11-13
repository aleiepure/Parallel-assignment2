# Introduction to Parallel Computing (A.Y. 2023-2024) - Assignment 2

This repo contains the solution of the [second assignment](homework2.pdf) of the course "Introduction to Parallel 
Computing - prof. Vella" from the University of Trento.

## Build and run
> [!NOTE]
> The instruction bellow are to be executed on the University of Trento's HPC cluster. 

Clone this repository, enter the directory, and submit to the queue the job file with the following 
commands:

```shell
git clone https://github.com/aleiepure/Parallel-assignment2
cd Parallel-assignment2
qsub job.pbs
```

The status of the execution can be checked with the command `qstat -u $USER`. An empty output means that no jobs are 
currently running for your user.

At the end of the job, the results of the computations are going to be available in `.CSV` files in the `out/` folder 
(created automatically). \
Errors abort the execution of the job immediately and the `stderr` is dumped into `out/stderr.e<jobid>`.

## Results
The report analyzing the results is available [here](report/build/report.pdf).
