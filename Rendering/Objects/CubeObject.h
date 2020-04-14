class CubeObject : public ISceneObject {
private:
    const GenericMesh* m_mesh;
    const rp3d::ProxyShape* m_shape;
    const rp3d::BoxShape* m_box;
    GLuint m_texture;
public:
    CubeObject(const ResourceRepo* repo, const rp3d::ProxyShape* shape, const rp3d::BoxShape* box)
    {
      m_mesh = repo->getCubeMesh();
      m_shape = shape;
      m_box = box;
      m_texture = repo->getBlueTexture();
    }

    void draw(const Camera::Camera* camera) const override {
      m_mesh->draw(camera, getMvp(m_shape) * Matrix4::FromScale(Vector3(m_box->getExtent())*2), m_texture);
    }
};

