#include <iostream>
#include <nsfwcpp.h>
#include <drogon/HttpController.h>

using namespace drogon;

class NSFWController : public HttpController<NSFWController> {
    public:
        METHOD_LIST_BEGIN
        ADD_METHOD_TO(NSFWController::nsfw_route, "/nsfw", Post);
        METHOD_LIST_END
    
        void nsfw_route(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback) {
            try {
                MultiPartParser fileUpload;
                if (fileUpload.parse(req) != 0 || fileUpload.getFiles().size() != 1) {
                    std::cout << "Invalid file upload: " << fileUpload.getFiles().size() << " files" << std::endl;
                    Json::Value jsonResponse;
                    jsonResponse["status"] = k400BadRequest;
                    jsonResponse["title"] = "must be only 1 file";
                
                    auto errorResponse = drogon::HttpResponse::newHttpJsonResponse(jsonResponse);
                    errorResponse->setStatusCode(k400BadRequest);
                    callback(errorResponse);
                    return;
                }

                const auto& file = fileUpload.getFiles()[0];
                const std::string_view& fileData = file.fileContent();

                std::vector<uchar> image_data(fileData.begin(), fileData.end());
                auto result = nsfw(image_data);

                Json::Value responseJson;
                for (auto param : result) {
                    responseJson[param.first] = param.second;
                }

                auto response = drogon::HttpResponse::newHttpJsonResponse(responseJson);
                callback(response);
            } catch(const std::exception& e) {
                std::cout << "exception: " << e.what() << std::endl;
                Json::Value jsonResponse;
                jsonResponse["status"] = k500InternalServerError;
                jsonResponse["title"] = e.what();
            
                auto errorResponse = drogon::HttpResponse::newHttpJsonResponse(jsonResponse);
                errorResponse->setStatusCode(k500InternalServerError);
                callback(errorResponse);
            }
        };
    
        NSFWController() {
            LOG_DEBUG << "NSFW route /nsfw";
        }
    };

int main() {
    std::cout << "started nsfw detector server" << std::endl;
    drogon::app()
        .loadConfigFile("drogon_config.json")
        .run();

    return 0;
}