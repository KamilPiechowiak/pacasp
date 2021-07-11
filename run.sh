#!/bin/bash
#SBATCH --job-name=pacasp
#SBATCH --output=pacasp.out
#SBATCH --error=pacasp.err
#SBATCH --time=10:00:00
#SBATCH --ntasks=3306
#SBATCH -p standard

TMPDIR=/tmp/lustre_shared/$USER/$SLURM_JOBID
mkdir -p $TMPDIR
cd $TMPDIR
mkdir -p instances
mkdir -p out
mkdir -p placements
cp $SLURM_SUBMIT_DIR/sp sp
cp $SLURM_SUBMIT_DIR/gen gen
cp $SLURM_SUBMIT_DIR/instances/* instances/
cp $SLURM_SUBMIT_DIR/config/run.json run.json
module load mpich
time mpiexec -n 3306 ./sp run run.json
zip -r -q $SLURM_JOBID.zip out
cp -r $SLURM_JOBID.zip $SLURM_SUBMIT_DIR/
zip -r -q ${SLURM_JOBID}_placements.zip placements
cp -r ${SLURM_JOBID}_placements.zip $SLURM_SUBMIT_DIR/