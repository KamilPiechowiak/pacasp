#!/bin/bash
#SBATCH --job-name=pac_asp
#SBATCH --output=pacasp.out
#SBATCH --error=pacasp.err
#SBATCH --time=20:00:00
#SBATCH --ntasks=560
#SBATCH -p standard

TMPDIR=/tmp/lustre_shared/$USER/$SLURM_JOBID
mkdir -p $TMPDIR
cd $TMPDIR
mkdir -p instances
mkdir -p tune
cp $SLURM_SUBMIT_DIR/sp sp
cp $SLURM_SUBMIT_DIR/gen gen
cp -r $SLURM_SUBMIT_DIR/used_instances/tuning_instances .
module load mpich
mpiexec -n 560 ./sp tune
cp -r $TMPDIR $SLURM_SUBMIT_DIR/
rm -r $TMPDIR