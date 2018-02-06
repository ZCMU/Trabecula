# Trabecula
Bone Trabecula

# 骨小梁分析程序

## 编译工具

1. 安装Visual Studio 2017 Community，带有ATL和MFC。
1. 下载解压或安装最新cmake版本。

## 生成

新建两个生成目录，例如叫做trabe_debug和trabe_build，然后打开VS2017的x64命令行工具，
使用如下的命令生成程序。

Debug版本：

```
cd trabe_debug
<cmake目录>\cmake <源代码目录>\Trabecula -G "Visual Studio 15 Win64"
MSBuild ALL_BUILD.vcxproj /t:build /p:Configuration=Debug
```

Release版本：

```
cd trabe_build
<cmake目录>\cmake <源代码目录>\Trabecula -G "Visual Studio 15 Win64"
MSBuild ALL_BUILD.vcxproj /t:build /p:Configuration=Release
```
