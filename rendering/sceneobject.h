//
// Created by hugo on 05-04-20.
//

#ifndef PHYSICS_SCENEOBJECT_H
#define PHYSICS_SCENEOBJECT_H


class ISceneObject {
protected:
    inline virtual Matrix4 getMvp(const rp3d::ProxyShape* shape) const
    {
      float mvp[16];
      shape->getLocalToWorldTransform().getOpenGLMatrix(mvp);
      return Matrix4::FromArray(mvp);
    }
public:
    virtual void draw() const = 0;
};

class CubeObject : public ISceneObject {
private:
    const IMesh* m_mesh;
    const rp3d::ProxyShape* m_shape;
    const rp3d::BoxShape* m_box;
public:
    CubeObject(const MeshRepo* repo, const rp3d::ProxyShape* shape, const rp3d::BoxShape* box)
    {
      m_mesh = repo->getCube();
      m_shape = shape;
      m_box = box;
    }

    void draw() const override {
      m_mesh->draw(getMvp(m_shape) * Matrix4::FromScale(Vector3(m_box->getExtent())*2));
    }
};

class HeightFieldObject : public ISceneObject {
    std::unique_ptr<FloorMesh> m_mesh;
    const rp3d::ProxyShape* m_shape;
    const rp3d::HeightFieldShape* m_map;
public:
    HeightFieldObject(const rp3d::ProxyShape* shape, const rp3d::HeightFieldShape* map, float* data)
    {
      m_shape = shape;
      m_map = map;
      rp3d::Vector3 min, max;
      map->getLocalBounds(min, max);
      m_mesh = std::make_unique<FloorMesh>(data, map->getNbRows(), map->getNbColumns(), max.y - min.y);
    }

    void draw() const override {
      m_mesh->draw(getMvp(m_shape) * Matrix4::FromScale(Vector3(m_map->getScaling())));
    }
};


#endif PHYSICS_SCENEOBJECT_H
