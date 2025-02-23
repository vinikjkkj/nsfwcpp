# <div align="center">nsfwcpp</div>

### <div align="center">A powerful/faster C++ model for NSFW detection, based on [nsfwjs](https://github.com/infinitered/nsfwjs)</div>

### User Implementation
If you only need the NSFW detection code, copy-paste **nsfwcpp.h** (header only) and **model.onnx** into your project, and install the requirements.

### API
**Here is a simple API to run the model:**
- **Requirements**: Conan 2.x, CMake 3.x
- **How to Install**:
    1. Run `conan install . --build=missing`
    2. Then `cmake --preset conan-default`
    3. Then `cmake --build build --config Release`

To start, run `./path/to/nsfwcpp`. The API will be available on **http://0.0.0.0:8080**. You can change this in `drogon_config.json` if you want.

#### Request Example

- **Method**: POST
- **Content-Type**: multipart/form-data
- **Field**: file

```
curl -X POST http://0.0.0.0:8080 \
  -F "file=@/file.txt"
```