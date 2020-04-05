//
// Created by hugo on 05-04-20.
//

#ifndef PHYSICS_MESHREPO_H
#define PHYSICS_MESHREPO_H


class MeshRepo
    {
    private:
        std::shared_ptr<IMesh> m_cube;
    public:
        MeshRepo(){
          m_cube = std::make_shared<Cube>();
        };
        inline const IMesh* getCube() const { return m_cube.get(); };
};



#endif PHYSICS_MESHREPO_H
