FROM debian:9
MAINTAINER Insight Software Consortium <community@itk.org>

RUN REPO=http://cdn-fastly.deb.debian.org && \
  echo "deb $REPO/debian stretch main\ndeb $REPO/debian stretch-updates main\ndeb $REPO/debian-security stretch/updates main" > /etc/apt/sources.list

RUN apt-get update && apt-get install -y \
  build-essential \
  cmake \
  git \
  libexpat1-dev \
  libhdf5-dev \
  libjpeg-dev \
  libpng-dev \
  libtiff5-dev \
  python \
  ninja-build \
  zlib1g-dev \
  && apt-get clean

RUN mkdir -p /usr/src/MetaIO-build
WORKDIR /usr/src

# ITK master 2018-01-09
ENV ITK_GIT_TAG 57ccc63e4a42c464e26ed1554c5ae7fc14d7d14d
RUN git clone https://github.com/InsightSoftwareConsortium/ITK.git && \
  cd ITK && \
  git checkout ${ITK_GIT_TAG} && \
  cd ../ && \
  mkdir ITK-build && \
  cd ITK-build && \
  cmake \
    -G Ninja \
    -DCMAKE_INSTALL_PREFIX:PATH=/usr \
    -DCMAKE_BUILD_TYPE:STRING=MinSizeRel \
    -DBUILD_EXAMPLES:BOOL=OFF \
    -DBUILD_TESTING:BOOL=ON \
    -DBUILD_SHARED_LIBS:BOOL=ON \
    -DITK_LEGACY_REMOVE:BOOL=ON \
    -DITK_BUILD_DEFAULT_MODULES:BOOL=OFF \
    -DITKGroup_Core:BOOL=OFF \
    -DITK_USE_SYSTEM_LIBRARIES:BOOL=ON \
    -DModule_ITKIOMeta:BOOL=ON \
  ../ITK && \
  ninja && \
  find . -name '*.o' -delete
