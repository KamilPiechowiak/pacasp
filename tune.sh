#!/bin/bash
#SBATCH --job-name=pac_asp
#SBATCH --output=pacasp.out
#SBATCH --error=pacasp.err
#SBATCH --time=80:00:00
#SBATCH --ntasks=80
#SBATCH -p standard

TMPDIR=/tmp/lustre_shared/$USER/$SLURM_JOBID
mkdir -p $TMPDIR
cd $TMPDIR
mkdir -p instances
mkdir -p out
cp $SLURM_SUBMIT_DIR/sp sp
cp $SLURM_SUBMIT_DIR/gen gen
module load mpich
mpiexec -n 80 ./sp tune
cp -r $TMPDIR $SLURM_SUBMIT_DIR/
rm -r $TMPDIR