# syntax=docker/dockerfile:1

FROM eclipse-temurin:8-jdk AS build

ENV DEBIAN_FRONTEND=noninteractive

RUN --mount=target=/var/lib/apt/lists,type=cache,sharing=locked \
    --mount=target=/var/cache/apt,type=cache,sharing=locked \
    rm -f /etc/apt/apt.conf.d/docker-clean && \
    apt-get update && \
    apt-get install --no-install-recommends -y build-essential git ca-certificates

RUN groupadd --gid 2200 wurm && \
    useradd -ms /bin/bash -u 2200 -g 2200 wurm

USER wurm

ENV JAVA_HOME=/opt/java/openjdk
ENV PATH="${JAVA_HOME}/bin:${PATH}"

WORKDIR /opt/wurmsteam

COPY --chown=wurm build.gradle gradlew ./
COPY --chown=wurm src ./src/
COPY --chown=wurm gradle ./gradle/

ENV GRADLE_USER_HOME=/home/wurm/.gradle
RUN --mount=type=cache,target=/home/wurm/.gradle,sharing=locked,uid=2200,gid=2200 \
    ./gradlew build --no-daemon

# ========= Prepare server files =========
FROM supersunho/steamcmd-arm64 AS download

ENV MODLOADER_VERSION=0.47

USER root

RUN --mount=target=/var/lib/apt/lists,type=cache,sharing=locked \
    --mount=target=/var/cache/apt,type=cache,sharing=locked \
    rm -f /etc/apt/apt.conf.d/docker-clean && \
    apt-get update && \
    apt-get install --no-install-recommends -y wget unzip fastjar

USER steam
WORKDIR /opt/wurm    

RUN FEXBash -c "/home/steam/steamcmd/steamcmd.sh +@ShutdownOnFailedCommand 1 +quit"

RUN FEXBash -c "/home/steam/steamcmd/steamcmd.sh +@ShutdownOnFailedCommand 1 +force_install_dir /opt/wurm +login anonymous +app_update 402370 validate +quit"

RUN wget -q https://github.com/ago1024/WurmServerModLauncher/releases/download/v${MODLOADER_VERSION}/server-modlauncher-${MODLOADER_VERSION}.zip -O modlauncher.zip \
    && unzip modlauncher.zip \
    && rm -rf *.bat *.sh modlauncher.zip mods/* patcher.jar

# Download updated sqlite JDBC driver with arm64 support and merge into server.jar
ADD --chown=steam https://github.com/xerial/sqlite-jdbc/releases/download/3.53.4.0/sqlite-jdbc-3.53.4.0.jar /tmp/sqlite-jdbc.jar
RUN mkdir /tmp/sqlite && \
    cd /tmp/sqlite && \
    fastjar xf /tmp/sqlite-jdbc.jar && \
    rm -f META-INF/MANIFEST.MF && \
    cd /opt/wurm && \
    fastjar uf server.jar -C /tmp/sqlite . && \
    rm -rf /tmp/sqlite /tmp/sqlite-jdbc.jar

# Copy JavaFX and SunJCE from bundled JRE, and clean up all the junk we won't need
RUN mkdir -p ext && \
    mv runtime/jre1.8.0_172/lib/ext/jfxrt.jar runtime/jre1.8.0_172/lib/ext/sunjce_provider.jar ext/ && \
    rm -rf runtime WurmServerLauncher lib linux64 nativelibs *.so steamapps

COPY launcher/ ./

# ========= Steam client download =========
FROM debian:13.7-slim AS steamclient

RUN --mount=target=/var/lib/apt/lists,type=cache,sharing=locked \
    --mount=target=/var/cache/apt,type=cache,sharing=locked \
    rm -f /etc/apt/apt.conf.d/docker-clean && \
    apt-get update && \
    apt-get install --no-install-recommends -y curl unzip ca-certificates

WORKDIR /opt/steam-arm64

RUN set -eu; \
    manifest_url=https://client-update.fastly.steamstatic.com/steam_client_publicbeta_linuxarm64 && \
    file=$(curl -fsSL "$manifest_url" | sed -n 's/^[[:space:]]*"file"[[:space:]]*"\(bins_linuxarm64_linuxarm64\.zip\.[0-9a-f]*\)".*/\1/p' | head -n1) && \
    test -n "$file" && \
    curl -fsSL -o /tmp/steam-arm64.zip "https://client-update.steamstatic.com/$file" && \
    unzip -q /tmp/steam-arm64.zip -d . && \
    rm /tmp/steam-arm64.zip

FROM eclipse-temurin:8-jre AS jre

# ========= Final image =========
FROM debian:13.7-slim

ENV DEBIAN_FRONTEND=noninteractive

RUN --mount=target=/var/lib/apt/lists,type=cache,sharing=locked \
    --mount=target=/var/cache/apt,type=cache,sharing=locked \
    rm -f /etc/apt/apt.conf.d/docker-clean && \
    apt-get update && \
    apt-get install --no-install-recommends -y ca-certificates libxi6 libxrandr2 libgl1 libxtst6 libpipewire-0.3-0 libopenal1 libpulse0 libnm0

COPY --from=jre /opt/java/openjdk /opt/java/openjdk
ENV JAVA_HOME=/opt/java/openjdk
ENV PATH="${JAVA_HOME}/bin:${PATH}"

RUN groupadd --gid 2200 wurm && \
    useradd -ms /bin/bash -u 2200 -g 2200 wurm

USER wurm

WORKDIR /opt/wurm
ENV HOME=/opt/wurm

COPY --from=download --chown=wurm:wurm /opt/wurm/ ./

COPY --from=build \
    /opt/wurmsteam/build/lib/main/release/stripped/libSteamServerJni.so \
    /opt/wurmsteam/build/steamworks-sdk/redistributable_bin/linuxarm64/libsteam_api.so \
    /opt/wurm/nativelibs/

COPY --from=steamclient --chmod=644 \
    /opt/steam-arm64/steamrtarm64/steamclient.so \
    /opt/steam-arm64/steamrtarm64/libtier0_s.so \
    /opt/steam-arm64/steamrtarm64/libvstdlib_s.so \
    /opt/wurm/nativelibs/

ENTRYPOINT ["./modlauncher.sh"]