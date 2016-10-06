#! /usr/bin/env bash

echo ${ExternalData_OBJECT_STORES}
mkdir -p "${ExternalData_OBJECT_STORES}"

echo "Using ITK repository: ${ITK_REPOSITORY_REMOTE:=https://github.com/InsightSoftwareConsortium/ITK.git}"

if [ -n "${APPVEYOR_BUILD_FOLDER+x}" ]
then
    PROJ_SRC=${APPVEYOR_BUILD_FOLDER}
fi

snapshot_author_name='${ITK_MODULE_NAME} Maintainers'
snapshot_author_email="${ITK_MODULE_NAME,,}@itk.org"

snapshot_redact_cmd=''
snapshot_relative_path='src/${ITK_MODULE_NAME}'
snapshot_paths='
  src
  License.txt
  '

thirdparty_module_name="${ITK_MODULE_NAME}"
upstream_git_url=${PROJ_SRC}
upstream_git_branch='local_branch'

( cd ${PROJ_SRC} &&
     git checkout -b ${upstream_git_branch};
     [ -f $(git rev-parse --git-dir)/shallow ] && git fetch --unshallow
 )

 # Hacking/overriding dirname to return the directory name
 # expected by UpdateThirdPartyFromUpstream.sh
 dirname()
 {
   echo ${ITK_SRC}/Modules/ThirdParty/${ITK_MODULE_NAME}
 }
 
 egrep()
 {
   grep -E $1
 }
 
git clone --single-branch ${ITK_REPOSITORY_REMOTE} -b "${ITK_TAG}" "${ITK_SRC}" &&
 (  cd ${ITK_SRC}/Modules/ThirdParty/${ITK_MODULE_NAME} &&
    source "$(pwd)/../../../Utilities/Maintenance/UpdateThirdPartyFromUpstream.sh" &&
    update_from_upstream &&
    cd ${ITK_SRC} &&
    git merge -X subtree=${ITK_SRC}/Modules/ThirdParty/${ITK_MODULE_NAME}/src/${ITK_MODULE_NAME} upstream-${ITK_MODULE_NAME,,} --strategy-option ours -m "Merge branch 'upstream-${ITK_MODULE_NAME,,}'"
 )

( cd ${PROJ_SRC}/test &&
    git clone --single-branch ${ITK_REPOSITORY_REMOTE} -b dashboard dashboard
)
