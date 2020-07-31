#include <mesh.h>
#include <mesh_initializers.h>
#include <integrator.h>
#include <writer.h>

#include <filesystem>

void TestTetrahedron(){
    std::vector<geo::INode> nodes{
        {1, 2, 3},
        {0, 2, 3},
        {0, 1, 3}, 
        {0, 1, 2}
    };

    std::vector<geo::Vector<float, 3>> points{
        {0, 0, 0},
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    };

    geo::Mesh tetrahedron(std::move(nodes), std::move(points));

    std::cout << tetrahedron.Area() << "\n";
    std::cout << tetrahedron.AreaGrad(0) << "\n";}

void TestHexaedron(){
    std::vector<geo::INode> nodes{
        {1, 2, 3, 4},
        {0, 2, 5, 3},
        {0, 1, 5, 4},
        {0, 2, 5, 3},
        {0, 1, 5, 4},
        {1, 2, 3, 4}
    };

    std::vector<geo::Vector<float, 3>> points{
        {0, 0, -1},
        {0, 0, 0},
        {0, 1, 0},
        {1, 0, 0},
        {1, 1, 0},
        {0, 0, 1},
    };

    geo::Mesh cube(std::move(nodes), std::move(points));

    std::cout << cube.Area() << "\n";
    std::cout << cube.AreaGrad(0) << "\n";
    std::cout << cube.AreaGrad(1) << "\n";
}

void TestSphere(){
    geo::Sphere<float> sphere{40, 40};

    geo::Mesh<float> sphere_mesh{
        std::move(sphere.Vertices()), 
        std::move(sphere.Points())
    };

    std::cout << sphere_mesh.Area() << "\n";
    std::cout << sphere_mesh.AreaGrad(0) << "\n";
}

void TestIntegrator(){
    std::string save_dir = std::filesystem::current_path().generic_string() + "/configurations";

    geo::Sphere<float> sphere{20, 20};
    uint32_t n_steps = 10;
    float epsilon = 0.001;
    float beta = 1.0;

    uint32_t n_trajectories = 1000;
    uint32_t save_on_each = 5;

    geo::Writer writer(save_dir);

    std::unique_ptr<geo::Mesh<float>> sphere_mesh = std::make_unique<geo::Mesh<float>>(
        std::move(sphere.Vertices()), 
        std::move(sphere.Points())
    );

    writer.WriteMesh("conf_start.conf", *sphere_mesh);

    std::cout << "Initial area: " << sphere_mesh->Area() << "\n";

    geo::Integrator integrator{
        std::move(sphere_mesh), n_steps, epsilon, beta
    };

    for(uint32_t t = 0; t < n_trajectories; t++){
        integrator.MDUpdate();
        //std::cout << "Area on step " << t << ": " << integrator.GetMesh()->Area() << "\n";
        if(t % save_on_each == 0){
            writer.WriteMesh("conf" + std::to_string(t) + ".conf", *integrator.GetMesh());
        }
    }
    
}

int main(){
    //TestTetrahedron();
    //TestHexaedron();
    //TestSphere();
    TestIntegrator();
}