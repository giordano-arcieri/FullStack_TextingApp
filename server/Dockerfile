# Use an official GCC image as a parent image
FROM ubuntu:latest

# Set the working directory in the container
WORKDIR /usr/src/app

# Install necessary packages including git, CMake, SQLite, and pkg-config
RUN apt-get update && apt-get install -y \
    g++ \
    pkg-config \
    cmake \
    sqlite3 \
    libsqlite3-dev \
    git \
    libssl-dev \
    libmicrohttpd-dev \
    libcurl4-openssl-dev \
    wget \
    unzip\
    meson \
    ninja-build

# Set the working directory in the container
WORKDIR /usr/src/app

# Install nlohmann/json
RUN git clone https://github.com/nlohmann/json.git && \
    cd json && \
    mkdir build && cd build && \
    cmake .. && \
    make && make install

# Clone, build, and install Pistache
RUN git clone https://github.com/pistacheio/pistache.git && \
    cd pistache && \
    git checkout ${PISTACHE_COMMIT} && \
    meson setup build \
      --buildtype=release \
      -DPISTACHE_USE_SSL=true \
      -DPISTACHE_BUILD_EXAMPLES=false \
      -DPISTACHE_BUILD_TESTS=false \
      -DPISTACHE_BUILD_DOCS=false \
      --prefix=/usr && \
    meson compile -C build && \
    meson install -C build

# Copy the current directory contents into the container at /usr/src/app
COPY data ./data
COPY src ./src
COPY include ./include
COPY CMakeLists.txt .


# Create a build directory
RUN mkdir build
WORKDIR /usr/src/app/build

# Run CMake to configure the project and then build it
RUN cmake ..
RUN cmake --build .

# Make port 8080 available to the world outside this container
EXPOSE 9080

# Run the main binary
CMD ["./server"]
