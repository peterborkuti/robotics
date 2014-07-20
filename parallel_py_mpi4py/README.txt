apt-get install python3-mpi4py

mpiexec -n 5 python3.4 hello.py

There were some cryptic error message, (syntax error in syte.py) because I tried this:
mpiexec -n 5 python hello.py

but my default python was 2.7