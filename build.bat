@echo off

echo "Checking env..."

IF NOT DEFINED VSCMD_VER (
  echo "Setting devenv..."
  @call "%vs160comntools%VsDevCmd.bat"
)

echo "Building zoom2..."

devenv ./zoom2.sln /build

echo "Build complete!"
