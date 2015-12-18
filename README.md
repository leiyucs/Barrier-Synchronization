# Barrier-Synchronization
Implementation and Evaluation of Barrier Synchronization in OpenMP and MPI

## Barrier Algorithms
Sense-reversing Barrier
Sense-reversing barrier is a centralized barrier, where each thread spins on a
globally shared variable. In this algorithm, two shared variables are used: a
count variable and a boolean sense variable. The count variable is initialized
to the number of threads. The sense variable indicates the current phase of
computation and is reversed between consecutive barriers. when a thread arrives
at the barrier, it decrements count by 1 and wait until sense is reversed. The
last arriving thread resets count to the number of threads and reverses sense.
Because multiple threads modify the count, the change to the count should use fetch-and-sub atomic operation.

Tournament Barrier
The tournament barrier is a tree style barrier. The processors are paired and
begin at the leaves of a binary tree.

MCS Tree Barrier
In MCS tree barrier, each processor is assigned to a unique tree node. The tree
nodes are linked by a parent link into an arrival tree that is a 4-ary tree. Also,
these nodes are linked by child links into a wakeup tree that is a binary tree.
Each parent node spins on a set of ready flags of child nodes. Once a child
arrives at the barrier, it updates its ready flag at its parent through parent link
and spins on its local sense. The parent quits the spin if all four children have
set their ready flags for indicating they have arrived, and further moves up the
tree and set its own flag at its parent as ready. When the procedure arrives
at the root, the root initiates the wakup procedure through the wakeup tree.
The root toggles each child’s local sense to release them from the barrier. At
each level of the wakeup tree, the newly released processors further release their
children before leaving the barrier.

OpenMP-MPI Combined Barrier
Our combined barrier combines the Sense-reversing Barrier and the MCS Tree
Barrier to synchronize multiple threads across multiple processes on different
nodes. The sense-reversing barrier implemented by OpenMP is used to syn-
chronize the threads in a MPI process, and the MCS tree barrier implemented
by MPI is used to synchronize the MPI processes.

## Programming Platform
OpenMP is an API that is defined by a group of major computer hard-
ware and software vendors and supports multi-platform shared memory multi-
processing programming in C/C++/Fortran.

Open-MPI
The Message-Passing Interface or MPI is API for parallel programming in dis-
tributed memory systems.
