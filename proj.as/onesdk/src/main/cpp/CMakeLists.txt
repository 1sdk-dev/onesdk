#
# Copyright 2023 xin.yang
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# 有关将 CMake 与 Android Studio 结合使用的更多信息，请阅读
# 文档：https://d.android.com/studio/projects/add-native-code.html。
# 有关如何使用 CMake 的更多示例，请参阅 https://github.com/android/ndk-samples。

# 设置此项目所需的最低 CMake 版本。
cmake_minimum_required(VERSION 3.22.1)

# 声明项目名称。 项目名称可以通过 ${PROJECT_NAME} 访问，
# 由于这是顶级 CMakeLists.txt，因此项目名称也是可访问的
# 与 ${CMAKE_PROJECT_NAME} （两个 CMake 变量在顶层同步构建脚本范围）。
project("onesdk")

# 创建并命名一个库，将其设置为 STATIC 或 SHARED，并提供其源代码的相对路径。
# 您可以定义多个库，CMake 会为您构建它们。
# Gradle 会自动将共享库与您的 APK 打包。
#
# 在这个顶级 CMakeLists.txt 中，${CMAKE_PROJECT_NAME} 用于定义
# 目标库名称； 在子模块的 CMakeLists.txt 中，${PROJECT_NAME}
# 出于相同目的而优选。

include_directories(
        include,
        protocol
        )
#
# 为了从 Java/Kotlin 将库加载到您的应用程序中，您必须调用
# System.loadLibrary() 并传递此处定义的库的名称；
# 对于 GameActivity/NativeActivity 派生的应用程序，必须使用相同的库名称
# 在 AndroidManifest.xml 文件中使用。
add_library(${CMAKE_PROJECT_NAME} STATIC
        # 列出 C/C++ 源文件以及此 CMakeLists.txt 的相对路径。
        protocol/JniHelper.cpp
        protocol/SDKManager.cpp
        protocol/PluginFactory.cpp
        protocol/PluginManager.cpp
        protocol/PluginUtils.cpp
        protocol/IPlugin.cpp
        protocol/WrapperCustom.cpp
        protocol/WrapperUser.cpp
        protocol/WrapperFee.cpp
        protocol/WrapperAds.cpp
        )

# 指定 CMake 应链接到目标库的库。 你
# 可以链接来自不同来源的库，例如在此定义的库
# 构建脚本、预构建的第三方库或 Android 系统库。
target_link_libraries(${CMAKE_PROJECT_NAME}
        # 列出目标库的链接
        android
        log)