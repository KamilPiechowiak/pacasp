#!/bin/bash
#SBATCH --job-name=pacasp
#SBATCH --output=pac_asp.out
#SBATCH --error=pac_asp.err
#SBATCH --time=00:10:00
#SBATCH --ntasks=2207
#SBATCH -p standard

TMPDIR=/tmp/lustre_shared/$USER/$SLURM_JOBID
mkdir -p $TMPDIR
cd $TMPDIR
mkdir -p instances
mkdir -p out
cp $SLURM_SUBMIT_DIR/sp sp
cp $SLURM_SUBMIT_DIR/gen gen
cp $SLURM_SUBMIT_DIR/instances/* instances/
module load mpich
mpiexec -n 2207 ./sp run
cp -r $TMPDIR $SLURM_SUBMIT_DIR/
rm -r $TMPDIR