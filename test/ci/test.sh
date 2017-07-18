#!/bin/bash

# This is a script to build the modules and run the test suite in the base
# Docker container.

set -x
set -o

cd /usr/src/MetaIO
branch=$(git rev-parse --abbrev-ref HEAD)
date=$(date +%F_%H_%M_%S)
sha=$(git rev-parse --short HEAD)

cd /usr/src/ITK
# Dummy values required to make git commits in the update script
git config user.name ci-testing
git config user.email ci@testing
./Modules/ThirdParty/MetaIO/UpdateFromUpstream.sh /usr/src/MetaIO $branch
cd /usr/src/ITK-build
cmake \
  -DBUILDNAME:STRING=External-MetaIO-${branch}-${date}-${sha} \
    .
ninja
ctest -VV -D Experimental -L ITKIOMeta
