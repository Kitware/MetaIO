#!/bin/sh

script_dir="`cd $(dirname $0); pwd`"

docker run \
  --rm \
  -v $script_dir/../..:/usr/src/MetaIO \
    insighttoolkit/metaio-test \
      /usr/src/MetaIO/test/ci/test.sh
