This directory has files required for creating an API using the pistache framework

## DESCRIPTION

* It is an API that generates a grayscale output of the image/video frame received as a request and sends back a json output
* It is built using the OpenCV library, the Pistache HTTP library, and the nlohmann JSON library.
* The API listens on port 9900 for incoming requests and responds with a JSON object containing information about the detected faces.

## API Generic Usage:

* Send a POST request to the API endpoint with an image in the body of the request.
* The API will parse the image and generate grayscale output of the image.
* The API will return a JSON object.

## API Endpoints with IO:

### Endpoint: POST /detect

### Request Parameters:

#### image: The image in the form of bytes to be processed. The image must be encoded in JPEG format.

### Request:

```shell
POST /detect HTTP/1.1
Host: localhost:9900
Content-Type: image/jpeg

<image bytes>
```
### Response:

```shell
HTTP/1.1 200 OK
Content-Type: application/json
{
  "num": [
    {
      "n": {
        "x": 1
      }
    },
    {
      "n": {
        "x": 2
      }
    },
    {
      "n": {
        "x": 3
      }
    },
    {
      "n": {
        "x": 4
      }
    },
    {
      "n": {
        "x": 5
      }
    }
  ]
}
```
## Prerequisites:

Before starting to create APIs, make sure to have the following prerequisites:

* Basic knowledge of programming languages C++
* Basic framework of Pistache library to create server
* Understanding of RESTful APIs

## OpenAPI input

### format:

* Binary image/jpeg

### Request Parameters

Request body should contain an image encoded in binary format in the following string format
| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| image | binary | Yes | The image to be processed |

## OpenAPI output

### format:

* json

### Response Codes:

* 200 OK: The face detection was successful and the response body will contain the coordinates of the detected faces.

### Response

The response is a JSON object containing co-ordinates of features and score about the detected faces.

### Example

```
{
  "num": [
    {
      "n": {
        "x": 1
      }
    },
    {
      "n": {
        "x": 2
      }
    },
    {
      "n": {
        "x": 3
      }
    },
    {
      "n": {
        "x": 4
      }
    },
    {
      "n": {
        "x": 5
      }
    }
  ]
}
```
### Response Fields
| Field      | Type       | 
|------------|------------|
| nums       | array      | 
| n          | object     | 
| x          | integer    | 

* 400 Bad Request: the image was not provided in the correct format.

### Example

```json
{
    "error": "Image not received"
}
```

* 500 Internal Server Error: An internal server error occurred while processing the request.

### Example

```json
{
    "error": "Internal server error"
}
```

## Provide examples of how to use the API

### sample requests if car.jpg exist in current directory
```
curl -X POST -H "Content-Type: image/jpeg" -H "Accept: application/json" --data-binary "@car.jpg" http://localhost:9900/detect

```
sample responses
```
{"num":[{"n":{"x":1}},{"n":{"x":2}},{"n":{"x":3}},{"n":{"x":4}},{"n":{"x":5}}]}

```

## Installation of required libraries

* Install gcc, g++, pip, cv2

```shell
sudo apt-get install gcc  g++ python3-pip
pip install cv2
```

* Install OpenCV using the package manager by running the following command:

```shell
sudo apt-get install libopencv-dev
```

* find location .cv using the following:

```shell
dpkg -L libopencv-dev
```

Add in the package, if /usr/lib/x86_64-linux-gnu/pkgconfig/opencv4.pc is present

```shell
export PKG_CONFIG_PATH=/usr/lib/x86_64-linux-gnu/pkgconfig/opencv4.pc
```

* Install OpenCV's DNN module:

```shell
sudo apt-get install libopencv-dnn-dev
```

* Install nlohmann library for json support in c++

```shell
sudo apt -y install nlohmann-json3-dev
```

* Install [Pistache](https://github.com/pistacheio/pistache)

```shell
sudo add-apt-repository ppa:pistache+team/stable
sudo apt update
sudo apt install libpistache-dev
```

```
* if not possible, use unstable version.
```

```shell
sudo add-apt-repository ppa:pistache+team/unstable
```

* Remove errors if ocuur

Ign:11 https://ppa.launchpadcontent.net/pistache+team/stable/ubuntu jammy InRelease Err:12 https://ppa.launchpadcontent.net/pistache+team/stable/ubuntu jammy Release 404 Not Found \[IP: 2620:2d:4000:1::3e 443\] Reading package lists... Done E: The repository 'https://ppa.launchpadcontent.net/pistache+team/stable/ubuntu jammy Release' does not have a Release file. N: Updating from such a repository can't be done securely, and is therefore disabled by default. N: See apt-secure(8) manpage for repository creation and user configuration details.

* uncheck the stable version from software & updates 
<img src="https://gitlab.iotiot.in/shunya/products/apis-creation/uploads/d3d9665320780b0be37f1c942b824941/image.png" width=550 height=450/>

## Creation of API in c++

The only need to know to create API in c++ is following:

* To create a server using Pistache library

```c
    Pistache::Rest::Router router; // POST/GET/etc. route handler
    Pistache::Port port(9900);     // port to listen on
    Pistache::Address addr(Pistache::Ipv4::any(), port);
    std::shared_ptr<Pistache::Http::Endpoint> server = std::make_shared<Pistache::Http::Endpoint>(addr);
    auto opts = Pistache::Http::Endpoint::options().threads(8); // how many threads for the server
    opts.maxRequestSize(1024 * 1024 * 100);                     // 100 MB
    server->init(opts);
```

* To handle incoming requests and produce the desired response.

```c
Pistache::Rest::Routes::Post(router, "/detect", Pistache::Rest::Routes::bind(&greyconv));
```

```
* to handle requests:
```

```c
  std::string_view image_req = request. Body();
    if (image_req.empty())
    {
       response. Send(Pistache::Http::Code::Bad_Request, "Error: Image not received");
       return;
    }

```

```
* To generate json output
```

```c
    nlohmann::json res;
    res["num"] = nlohmann::json::array();
    for (int i = 1; i <= 5; i++)
    {
        nlohmann::json obj;
        // using initializer list
 
        obj["n"] = {{"x", i}};
        res["num"].push_back(obj);
    }

```

```
* To handle response:
```

```c
    // Set response headers
    response.headers().add<Pistache::Http::Header::ContentType>(MIME(Application, Json));
    // Write the JSON object to the response
    response.send(Pistache::Http::Code::Ok, res.dump()); //res is json output
```

## Compile

* Assuming that the source file is named as `server.cpp`
* Using cmake
* create a new file as CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.0)
project(apl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpistache")

find_package(PkgConfig)
pkg_check_modules(Pistache REQUIRED IMPORTED_TARGET libpistache)


find_package(OpenCV REQUIRED)
if (${OpenCV_VERSION} VERSION_LESS 4.5.2)
  message(FATAL_ERROR "OpenCV version must be >= 4.5.2")
endif()
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(nlohmann_json REQUIRED)

add_executable(${PROJECT_NAME} server.cpp)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${OpenCL_LIBRARIES} PkgConfig::Pistache nlohmann_json)

```

```
* Run in terminal with CMakeLists.txt file directory:

```
```
cmake .
make
* An executable file, " apl " is formed now. To run it:
./apl
```
```
* Now run the command to test the API

```
```
curl -X POST -H "Content-Type: image/jpeg" -H "Accept: application/json" --data-binary "@car.jpg" http://localhost:9900/detect

```
* A server started with host url: `http://127.0.0.1:9900`, receive POST request at `http://127.0.0.1:9900/detect`.
* Verify by opening latest log file in logs directory. There should be log of server starting, or simply go to url: http://127.0.0.1:9900/detect, `Image not received` response will be received.

# Output
![Screenshot_from_2023-03-11_17-50-04](/uploads/244b198139443d944fcb37f401aa2599/Screenshot_from_2023-03-11_17-50-04.png)

![Screenshot_from_2023-03-11_17-51-15](/uploads/06d1945fb5c2c9c47facb37020bd697f/Screenshot_from_2023-03-11_17-51-15.png)

# solutions to common errors

1. ```No such file or directory```  

Ensure all files are in the same folder and execute commands in that directory

2. ```std::filesystem::__cxx11::filesystem_error ```

ensure you have the model and inputs in the directory

3.you may encounter ```No module named 'cv2' ```

* sudo apt update
* sudo apt install python3-pip
* pip install cv2
* pip install opencv-python

4. ``` connection refused ```
 ![Screenshot_from_2023-03-09_19-14-53](/uploads/b3de24f191da69290c8bc92b283f34e4/Screenshot_from_2023-03-09_19-14-53.png)
```
solution : try the following command

curl -X 'POST'   'http://127.0.0.1:9900/detect'   -H 'accept: application/json'   -H 'Content-Type: image/jpeg'   --data-binary '@car.jpg'
or ensure that the server is running on one terminal window and try running the curl command on another terminal window
```
