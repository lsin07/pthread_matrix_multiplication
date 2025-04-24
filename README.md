# pthread_matrix_multiplication

## build
```bash
make
```

## usage
```bash
./main <matrix size> <max_threads>
```

It will repeat multiplying `<matrix size>` sized square matrix using 1 to `<max_threads>` threads,
and save each processing time at `result.csv`.

Use [`plot.py`](plot.py) to plot the result. Install `numpy`, `pandas`, and `matplotlib` packages if not installed.