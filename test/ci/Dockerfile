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
  libeigen3-dev \
  python \
  ninja-build \
  python-pip \
  zlib1g-dev \
  && apt-get clean

RUN pip install cmake

RUN mkdir -p /usr/src/MetaIO-build
WORKDIR /usr/src

# ITKv5.0RC2+ 2019-05-21
ENV ITK_GIT_TAG dd2640de27f3bd30b464c0989cefbdcd67faa381
RUN git clone https://github.com/InsightSoftwareConsortium/ITK.git && \
  cd ITK && \
  git checkout ${ITK_GIT_TAG} && \
  cd ../ && \
  mkdir ITK-build

WORKDIR /usr/src/ITK-build
RUN cmake \
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
    -DITK_USE_EIGEN_MPL2_ONLY:BOOL=ON \
  ../ITK
RUN ninja && \
  find . -name '*.o' -delete
