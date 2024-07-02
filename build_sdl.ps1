# Might need to add clang-cl to PATH like that:
# C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\Llvm\x64\bin\
# or that
# C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.40.33807\bin\Hostx64\x64\
# for cl.exe
$COMPILER = "clang-cl.exe"
$WORKING_DIRECTORY = Get-Location

New-Item -Force -ItemType Directory -Path "$WORKING_DIRECTORY/src/SDL/build"
Set-Location "$WORKING_DIRECTORY/src/SDL/build"

cmake .. -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_C_COMPILER=$COMPILER \
    -DCMAKE_SYSTEM_NAME=Windows \
    -DCMAKE_SYSTEM_PROCESSOR=x86_64 \
    -DCMAKE_INSTALL_PREFIX=$WORKING_DIRECTORY
    
cmake --build . --config Release
cmake --build . --target install