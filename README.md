# Trabecula
Bone Trabecula

# 骨小梁分析程序

## 操作系统

编译，运行都需要在64位windows操作系统上进行，建议是windows 10 x64或windows server 2019。

## 编译工具

1. 安装Visual Studio 2019 Community或生成工具，不必带有ATL和MFC。
1. 下载解压或安装最新cmake版本。

## 生成

新建两个生成目录，例如叫做trabe_debug和trabe_build，然后打开VS2019的x64命令行工具，
使用如下的命令生成程序。

Debug版本：

```
cd trabe_debug
<cmake目录>\cmake <源代码目录>\Trabecula -G "Visual Studio 16 2019" -A x64
MSBuild ALL_BUILD.vcxproj /t:build /p:Configuration=Debug
```

Release版本：

```
cd trabe_build
<cmake目录>\cmake <源代码目录>\Trabecula -G "Visual Studio 16 2019" -A x64
MSBuild ALL_BUILD.vcxproj /t:build /p:Configuration=Release
```

## 打包

这里只针对Release版本打包。执行如下命令：

```
cd trabe_build
<cmake目录>\cpack -D CPACK_GENERATOR="ZIP"
```

将生成Zip文件。

将此Zip文件复制到目标机器上，解压后即可运行。
