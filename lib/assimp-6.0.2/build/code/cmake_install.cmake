# Install script for directory: D:/GV/lib/assimp-6.0.2/code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/Assimp")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/msys64/mingw64/bin/objdump.exe")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "libassimp6.0.2-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/GV/lib/assimp-6.0.2/build/lib/libassimp.dll.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "libassimp6.0.2" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/GV/lib/assimp-6.0.2/build/bin/libassimp-6.dll")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libassimp-6.dll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libassimp-6.dll")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/msys64/mingw64/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libassimp-6.dll")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/anim.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/aabb.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/ai_assert.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/camera.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/color4.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/color4.inl"
    "D:/GV/lib/assimp-6.0.2/build/code/../include/assimp/config.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/ColladaMetaData.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/commonMetaData.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/defs.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/cfileio.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/light.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/material.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/material.inl"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/matrix3x3.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/matrix3x3.inl"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/matrix4x4.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/matrix4x4.inl"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/mesh.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/ObjMaterial.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/pbrmaterial.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/GltfMaterial.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/postprocess.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/quaternion.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/quaternion.inl"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/scene.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/metadata.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/texture.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/types.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/vector2.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/vector2.inl"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/vector3.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/vector3.inl"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/version.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/cimport.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/AssertHandler.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/importerdesc.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/Importer.hpp"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/DefaultLogger.hpp"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/ProgressHandler.hpp"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/IOStream.hpp"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/IOSystem.hpp"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/Logger.hpp"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/LogStream.hpp"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/NullLogger.hpp"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/cexport.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/Exporter.hpp"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/DefaultIOStream.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/DefaultIOSystem.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/ZipArchiveIOSystem.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/SceneCombiner.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/fast_atof.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/qnan.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/BaseImporter.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/Hash.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/MemoryIOWrapper.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/ParsingUtils.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/StreamReader.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/StreamWriter.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/StringComparison.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/StringUtils.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/SGSpatialSort.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/GenericProperty.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/SpatialSort.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/SkeletonMeshBuilder.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/SmallVector.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/SmoothingGroups.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/SmoothingGroups.inl"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/StandardShapes.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/RemoveComments.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/Subdivision.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/Vertex.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/LineSplitter.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/TinyFormatter.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/Profiler.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/LogAux.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/Bitmap.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/XMLTools.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/IOStreamBuffer.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/CreateAnimMesh.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/XmlParser.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/BlobIOSystem.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/MathFunctions.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/Exceptional.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/ByteSwapper.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/Base64.hpp"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/Compiler/pushpack1.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/Compiler/poppack1.h"
    "D:/GV/lib/assimp-6.0.2/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "D:/GV/lib/assimp-6.0.2/build/code/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
