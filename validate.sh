#!/bin/bash
#SBATCH --job-name=pacasp
#SBATCH --output=pacasp.out
#SBATCH --error=pacasp.err
#SBATCH --time=01:00:00
#SBATCH --ntasks=3306
#SBATCH -p standard

TMPDIR=/tmp/lustre_shared/$USER/$SLURM_JOBID
mkdir -p $TMPDIR
cd $TMPDIR
mkdir -p placements
cp $SLURM_SUBMIT_DIR/sp sp
cp $SLURM_SUBMIT_DIR/todo_placements.zip placements.zip
unzip -q placements.zip
cp $SLURM_SUBMIT_DIR/config/run.json run.json
module load mpich
time mpiexec -n 3306 ./sp validate run.json