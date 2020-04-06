class CubeObject : public ISceneObject {
private:
    const CubeMesh* m_mesh;
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

