//
// Created by hugo on 05-04-20.
//

#ifndef PHYSICS_MESHREPOSITORY_H
#define PHYSICS_MESHREPOSITORY_H


class MeshRepo
    {
    private:
        std::shared_ptr<CubeMesh> m_cube;
    public:
        MeshRepo(){
          m_cube = std::make_shared<CubeMesh>();
        };
        inline const CubeMesh* getCube() const { return m_cube.get(); };
};



#endif PHYSICS_MESHREPOSITORY_H