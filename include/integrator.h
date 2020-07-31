#pragma once

#include <memory>

#include <mesh.h>
#include <random.h>

namespace geo{

    template <typename T>
    class Integrator{
    public:

        Integrator(std::unique_ptr<Mesh<T>>&& mesh_ptr,  uint32_t n_steps, T epsilon, T beta)
        : mesh(std::move(mesh_ptr)), n_steps(n_steps), epsilon(epsilon), beta(beta) {
            Pxmu.resize(mesh->Volume());
        }

        void MDUpdate();

        T Hamiltonian();

        void FillPxmuRandom();

        const Mesh<T>* GetMesh() const;

    private:
        std::unique_ptr<Mesh<T>> mesh;

        //Conjugate momenta
        std::vector<Vector<T, 3>> Pxmu;

        //Buffer of old points
        std::vector<Vector<T, 3>> points_old;

        //Number of integration steps
        uint32_t n_steps;

        //Integration step
        T epsilon;

        //Coupling constant
        T beta;

        //Random engine
        RandomEngine rng;
    };

    template <typename T>
    void Integrator<T>::FillPxmuRandom(){
        for(auto& p : Pxmu){
            p = {rng.Normal<T>(), rng.Normal<T>(), rng.Normal<T>()};
        }
    }

    template <typename T>
    const Mesh<T>* Integrator<T>::GetMesh() const{
        return mesh.get();
    }

    template <typename T>
    T Integrator<T>::Hamiltonian(){
        T hamiltonian = mesh->Area();

        for(const auto& p : Pxmu){
            hamiltonian += p.norm() / 2;
        }

        return hamiltonian;
    }

    template <typename T>
    void Integrator<T>::MDUpdate(){
        FillPxmuRandom();

        T H_old = Hamiltonian();

        points_old = mesh->Points();

        for(size_t t = 0; t < n_steps; t++){
            //make half-step in p
            for(size_t id = 0; id < mesh->Volume(); id++){
                Pxmu[id] -= beta * epsilon * mesh->AreaGrad(id) / 2;   
            }
            //make step in points
            for(size_t id = 0; id < mesh->Volume(); id++){
                mesh->Point(id) += epsilon * Pxmu[id];
            }
            //make half-step in p
            for(size_t id = 0; id < mesh->Volume(); id++){
                Pxmu[id] -= beta * epsilon * mesh->AreaGrad(id) / 2;
            }
        }

        T H_new = Hamiltonian();
        T dH = H_new - H_old;

        //In case update is not accepted, discard to previous state
        if(dH > 0 && rng.Uniform<T>() > exp(-dH)){
            mesh->Points() = points_old;
        } 
    }

}