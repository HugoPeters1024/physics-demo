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


#endif PHYSICS_SCENEOBJECT_H
