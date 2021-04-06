#!/bin/bash
#SBATCH --job-name=pacasp
#SBATCH --output=pacasp.out
#SBATCH --error=pacasp.err
#SBATCH --time=00:30:00
#SBATCH --ntasks=1
#SBATCH -p standard

zip -r /tmp/lustre_shared/kamilp/13957703/instances.zip /tmp/lustre_shared/kamilp/13957703/instances
cp /tmp/lustre_shared/kamilp/13957703/instances.zip tune_instances.zip