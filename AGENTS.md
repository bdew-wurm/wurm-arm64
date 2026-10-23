# Build Instructions

Run Gradle tasks inside the Nix development shell so the required Java environment is available:

```sh
nix develop -c ./gradlew <task> --no-daemon
```
