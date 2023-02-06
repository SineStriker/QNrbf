# QNrbfFormat

C++ library to read/write MS-NRBF.

## Requirements

+ Qt Core

## Supported Formats

+ Json
  + Extract MS-NRBF data structure to json file

+ Svip
  + Deserialize X Studio Singer (not 3) project file

## How to Use

### Use as sub-project

+ Copy this folder to where contains the `CMakeLists.txt` which bring in QNrbf
+ Set `QNRBF_BUILD_NRBF_ONLY` to `on`
+ Use `add_subdirectory` to add this repository as your subproject 
  ```cmake
  set(QNRBF_BUILD_NRBF_ONLY on)
  add_subdirectory(QNrbf)
  ````

+ Use `target_link_libraries` to link QNrbf to your project.
  ```cmake
  target_link_libraries(<target> <scope> QNrbfFormat)
  ````

### Use as pre-built library

+ Set `QNRBF_BUILD_NRBF_ONLY` to `on`
+ Set `QNRBF_INSTALL` to `on`
+ Run `CMake Install`
+ Find library add link
  ```cmake
  find_library(QNrbf REQUIRED)
  target_link_libraries(<target> <scope> QNrbfFormat)
  ````


## References

+ [BinaryFormatDataStructure](https://github.com/bbowyersmyth/BinaryFormatDataStructure)
  + MIT License

+ [pypdn](https://github.com/addisonElliott/pypdn)
  + MIT License

## Related Pages

+ [OpenSVIP](https://github.com/yqzhishen/opensvip)
  + `qsvip` is to convert XStudio project files into OpenSVIP json files

## License

+ `QNrbfFormat` is released under GNU LGPL 2.1 License.