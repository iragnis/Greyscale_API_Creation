#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>

#include <filesystem>
#include <iostream>

#include <nlohmann/json.hpp>
/*#include "grayscale_conversion.h"*/
#include "gray.h"

void greyconv(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response)    
{
    // Parse image from the request body
    std::string_view image_req = request.body();
    
    if (image_req.empty())
    {
        
        response.send(Pistache::Http::Code::Bad_Request, "Error: Image not received");
      
        return;
    }
    std::vector<uchar> data(image_req.begin(), image_req.end());
    cv::Mat image = cv::imdecode(data, cv::IMREAD_COLOR);

    if (image.empty())
    {
        std::cerr << "Error: Image not received" << std::endl;
        response.send(Pistache::Http::Code::Bad_Request, "Error: Image not received");
        
        return;
    }
    
     nlohmann::json res=conv(image); //greyscale
   
    response.headers().add<Pistache::Http::Header::ContentType>(MIME(Application, Json));


    // Write the JSON object to the response
    response.send(Pistache::Http::Code::Ok, res.dump()); 
    
    return ;
}


int main(int argc, char *argv[])
{
   
    
    Pistache::Rest::Router router; // POST/GET/etc. route handler
    Pistache::Port port(9900);     // port to listen on
    Pistache::Address addr(Pistache::Ipv4::any(), port);
    std::shared_ptr<Pistache::Http::Endpoint> server = std::make_shared<Pistache::Http::Endpoint>(addr);
    auto opts = Pistache::Http::Endpoint::options().threads(8); // how many threads for the server
    opts.maxRequestSize(1024 * 1024 * 100);                     // 100 MB
    server->init(opts);

    Pistache::Rest::Routes::Post(router, "/detect", Pistache::Rest::Routes::bind(&greyconv));
    Pistache::Rest::Routes::Get(router, "/detect", Pistache::Rest::Routes::bind(&greyconv));
    server->setHandler(router.handler());

    server->serve(); 

    return 0; 
    
    
}
