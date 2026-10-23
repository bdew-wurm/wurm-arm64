# Wurm Unlimited dedicated server on linux ARM64 machines

This repository generates a docker image for Wurm Unlimited on ARM64. Everything runs natively, without emulation.

**This is experimental and not very tested, use at your own risk.**

No image builds will be provided for legal reasons, but everything can be built from this repository.

# Using

First clone this repository and build the docker image using:

```sh
docker build -t wurm-arm64 .
```

After building the image, you can run the server using:

```sh
docker run wurm-arm64 -v /path/to/your/map/data:/opt/wurm/YourMapName -p ... start=YourMapName
```

Ago's mod launcher is included, to install mods place them in `launch/mods` folder before building the image (or mount them as a volume).

# Changes from Vanilla WU

* libSteamServerJni (a native library that bridges between wurm Java code and the Steamworks SDK) is rewritten from scratch to allow running on ARM64
* Bundled Oracle JVM is replaced with Eclipse Temurin OpenJDK 8
* JavaFX and SunJCE libraries are extracted from the bundled JRE and included separately
* sqlite JDBC driver is updated to a version with ARM64 support
* Various steam libraries are taken from SteamWorks SDK and native Steam runtime for ARM64

# Gotchas

* Can only run on kernels with 4kb page size, as steam libraries are built for that page size
