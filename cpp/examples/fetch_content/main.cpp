#include <blickfeld/core_processing/services/point_cloud.grpc.pb.h>
#include <blickfeld/hardware/client.h>

int main() {
    auto channel = blickfeld::hardware::connect_to_device("qb2-xxxxxxx");

    // Fetch one point cloud frame
    auto                                                           service = blickfeld::core_processing::services::PointCloud::NewStub(channel);
    grpc::ClientContext                                            context;
    blickfeld::core_processing::services::PointCloudStreamResponse response;
    auto stream = service->Stream(&context, blickfeld::core_processing::services::PointCloudStreamRequest());
    stream->Read(&response);
    std::cout << "Received a frame with the ID " << response.frame().id() << std::endl;
    context.TryCancel();

    return 0;
}