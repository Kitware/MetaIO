#! /usr/bin/env bash

mkdir -p "${ExternalData_OBJECT_STORES}"

echo "Using ITK repository: ${ITK_REPOSITORY_REMOTE:=https://github.com/InsightSoftwareConsortium/ITK.git}"

if [ -n "${APPVEYOR_BUILD_FOLDER+x}" ]
then
    PROJ_SRC=${APPVEYOR_BUILD_FOLDER}
fi


git clone --single-branch ${ITK_REPOSITORY_REMOTE} -b "${ITK_TAG}" "${ITK_SRC}" &&
 ( cd ${ITK_SRC}/Modules/Remote &&
     ln -s ${PROJ_SRC} ${ITK_MODULE_NAME}
 )

( cd ${PROJ_SRC}/test &&
    git clone --single-branch ${ITK_REPOSITORY_REMOTE} -b dashboard dashboard
)

( cd ${ITK_SRC}/Modules/ThirdParty/${ITK_MODULE_NAME}/ &&
    UpdateFromUpstream.sh
)
