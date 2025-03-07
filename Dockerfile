# Use Ubuntu as base image.
FROM ubuntu:22.04

# Install prerequisities.
RUN apt-get update && apt-get install -y \
    cmake \
    build-essential \
    libboost-all-dev \
    libcgal-dev \
    git \
    ninja-build \
    && rm -rf /var/lib/apt/lists/*

# Set working directory.
WORKDIR /app

# Clone the latest branch from GitHub.
# ARG CACHEBUST=1 # If need to update branch.
ARG BRANCH=main
RUN git clone --depth 1 --branch ${BRANCH} https://github.com/kerrambit/vrmlxpy.git .

# Clone pybind11 from repository into vendor folder.
RUN git clone --depth 1 https://github.com/pybind/pybind11.git ./vendor/pybind11

# Build the project.
RUN cmake --preset Release
# Try 3 times if compilation fails because of 'c++: fatal error: Killed signal terminated program cc1plus'.
RUN for i in 1 2 3; do cmake --build out/build/Release && break || sleep 3; done

# Copy script to build directory.
RUN cp ./scripts/run_vrmlxpy_from_docker.py ./out/build/Release/

CMD ["python3", "/app/out/build/Release/run_vrmlxpy_from_docker.py"]