#include <blickfeld/core_processing/services/point_cloud.grpc.pb.h>
#include <blickfeld/hardware/client.h>

void process_frame(const blickfeld::core_processing::data::Frame& frame);

int main() {
    try {
        auto channel = blickfeld::hardware::connect_to_device("qb2-xxxxxxx", "serial-number-xxx", "application-key-xxxxxxx");

        // Fetch one point cloud frame
        auto                                                           service = blickfeld::core_processing::services::PointCloud::NewStub(channel);
        grpc::ClientContext                                            context;
        blickfeld::core_processing::services::PointCloudStreamResponse response;
        auto stream = service->Stream(&context, blickfeld::core_processing::services::PointCloudStreamRequest());

        if(!stream->Read(&response)) {
            auto status = stream->Finish();
            std::cerr << "Stream read failed: " << status.error_message() << std::endl;
            return 1;
        }

        std::cout << "Received a frame with the ID " << response.frame().id() << std::endl;
        process_frame(response.frame());
        context.TryCancel();
    } catch(std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    return 0;
}

void process_frame(const blickfeld::core_processing::data::Frame& frame) {
    // print frame parameters
    printf("frame size: %d\n", frame.binary().length());

    // access cartesian point data (flat array of floats)
    auto xyz = (float*) frame.binary().cartesian().data();
    // access photon count / intensity of the points
    auto photon_counts = (uint16_t*) frame.binary().photon_count().data();
    // access direction_id of the points
    auto direction_ids = (uint32_t*) frame.binary().direction_id().data();

    // read out x,y,z coordinates, photon count, direction_id of a point in the frame
    int      pointIndex   = 10;
    float    x            = xyz[pointIndex + 0];
    float    y            = xyz[pointIndex + 1];
    float    z            = xyz[pointIndex + 2];
    uint16_t photon_count = photon_counts[pointIndex];
    uint32_t direction_id = direction_ids[pointIndex];

    printf("point: %d, x: %.3f m, y: %.3f m, z: %.3f m, photon count: %u, direction_id: %u\n", pointIndex, x, y, z, photon_count, direction_id);
}
